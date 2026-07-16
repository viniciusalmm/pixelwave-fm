cmake_minimum_required(VERSION 3.15)

project(PixelWaveFM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    pixelwave-fm

    src/main.cpp

    src/music.cpp
    src/playlist.cpp
    src/listener.cpp
    src/radio_station.cpp

    src/broadcast_content.cpp

    src/music_program.cpp
    src/news_program.cpp
)