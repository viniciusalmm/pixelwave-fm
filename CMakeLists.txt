cmake_minimum_required(VERSION 3.20)

project(pixelwave_fm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(enable_thread_sanitizer "Ativa o ThreadSanitizer nos alvos do projeto" OFF)
option(enable_address_sanitizer "Ativa AddressSanitizer e UndefinedBehaviorSanitizer nos alvos do projeto" OFF)
option(enable_qt_gui "Compila a interface grafica Qt Widgets (extra Q6)" OFF)

if(enable_thread_sanitizer AND enable_address_sanitizer)
    message(FATAL_ERROR "Ative apenas um sanitizador por build: ThreadSanitizer ou AddressSanitizer/UBSan.")
endif()

include(FetchContent)
find_package(Threads REQUIRED)

# Dependência de serialização exigida no TP3.
find_package(nlohmann_json 3.11 QUIET)
if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

set(pixelwave_sources
    src/music.cpp
    src/playlist.cpp
    src/listener.cpp
    src/radio_station.cpp
    src/broadcast_content.cpp
    src/music_program.cpp
    src/news_program.cpp
    src/json_serialization.cpp
    src/repository.cpp
)

add_executable(pixelwave_fm
    src/main.cpp
    ${pixelwave_sources}
)

target_include_directories(pixelwave_fm PRIVATE src)
target_link_libraries(pixelwave_fm PRIVATE nlohmann_json::nlohmann_json Threads::Threads)
target_compile_options(pixelwave_fm PRIVATE -Wall -Wextra -Wpedantic)

if(enable_thread_sanitizer)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(pixelwave_fm PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
        target_link_options(pixelwave_fm PRIVATE -fsanitize=thread)
    else()
        message(FATAL_ERROR "ThreadSanitizer configurado apenas para compiladores GNU/Clang neste projeto.")
    endif()
endif()

if(enable_address_sanitizer)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(pixelwave_fm PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(pixelwave_fm PRIVATE -fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "AddressSanitizer/UBSan configurado apenas para compiladores GNU/Clang neste projeto.")
    endif()
endif()

if(enable_qt_gui)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    qt_add_executable(pixelwave_gui
        src/gui_main.cpp
        src/main_window.cpp
        src/demo_data.cpp
        ${pixelwave_sources}
    )

    target_include_directories(pixelwave_gui PRIVATE src)
    target_link_libraries(pixelwave_gui PRIVATE
        Qt6::Widgets
        nlohmann_json::nlohmann_json
        Threads::Threads
    )
    target_compile_options(pixelwave_gui PRIVATE -Wall -Wextra -Wpedantic)

    if(enable_thread_sanitizer)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(pixelwave_gui PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
            target_link_options(pixelwave_gui PRIVATE -fsanitize=thread)
        endif()
    endif()

    if(enable_address_sanitizer)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(pixelwave_gui PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(pixelwave_gui PRIVATE -fsanitize=address,undefined)
        endif()
    endif()
endif()

# Alvo propositalmente excluído do build normal para demonstrar a violação do concept.
add_executable(concept_violation EXCLUDE_FROM_ALL tests/concept_violation.cpp)
target_include_directories(concept_violation PRIVATE src)
target_compile_options(concept_violation PRIVATE -Wall -Wextra -Wpedantic)

include(CTest)
if(BUILD_TESTING)
    # Extra Q5: Catch2 v3 via FetchContent.
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(tests
        tests/test_tp3.cpp
        ${pixelwave_sources}
    )

    target_include_directories(tests PRIVATE src)
    target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json Catch2::Catch2WithMain Threads::Threads)
    target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)

    if(enable_thread_sanitizer)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(tests PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
            target_link_options(tests PRIVATE -fsanitize=thread)
        endif()
    endif()

    if(enable_address_sanitizer)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(tests PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(tests PRIVATE -fsanitize=address,undefined)
        endif()
    endif()

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(tests)
endif()
