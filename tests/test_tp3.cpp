#include "errors.hpp"
#include "generic.hpp"
#include "json_serialization.hpp"
#include "music_program.hpp"
#include "news_program.hpp"
#include "repository.hpp"
#include "service.hpp"
#include "utils.hpp"

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <vector>

TEST_CASE("template e concept")
{
    registry<music_program> music_registry;
    registry<news_program> news_registry;

    music_registry.add(music_program("Morning", "08:00", 10));
    news_registry.add(news_program("News", "12:00", 4));

    REQUIRE(music_registry.size() == 1);
    REQUIRE(news_registry.size() == 1);

    const auto found = music_registry.find_first(
        [](const music_program& program)
        {
            return program.get_title() == "Morning";
        });
    REQUIRE(found.has_value());
    REQUIRE(found->get().get_title() == "Morning");

    const std::vector<music_program> programs{
        music_program("Morning", "08:00", 10),
        music_program("Night", "20:00", 5)
    };

    REQUIRE(total_audience(programs) == 1800);
}

TEST_CASE("excecao e optional")
{
    radio_station station("Test FM");
    station.get_playlist().add_music(music("Song", "Artist", 100));
    station.add_listener(listener("Ana", "Brazil", "Pop"));

    REQUIRE_THROWS_AS(
        station.get_playlist().add_music(music("", "Artist", 0)),
        domain_error);
    REQUIRE_THROWS_AS(
        station.add_program(std::unique_ptr<broadcast_content>{}),
        domain_error);

    REQUIRE(station.find_listener("Ana").has_value());
    REQUIRE_FALSE(station.find_listener("Nobody").has_value());
}

TEST_CASE("serializacao round trip")
{
    radio_station original("Test FM");
    original.get_playlist().add_music(music("Song", "Artist", 100));
    original.add_listener(listener("Ana", "Brazil", "Pop"));
    original.add_program(std::make_unique<music_program>("Show", "10:00", 4));

    const json document = serialize_station(original);
    REQUIRE(document.at("version").get<int>() == 1);
    REQUIRE(document.at("programs").front().at("type").get<std::string>() == "Music Program");

    const radio_station restored = deserialize_station(document);

    REQUIRE(restored.get_name() == original.get_name());
    REQUIRE(restored.get_playlist().get_musics() == original.get_playlist().get_musics());
    REQUIRE(restored.get_listeners() == original.get_listeners());
    REQUIRE(restored.get_programs().size() == original.get_programs().size());
    REQUIRE(restored.get_programs().front()->type() == original.get_programs().front()->type());
    REQUIRE(restored.get_programs().front()->get_title() == original.get_programs().front()->get_title());
}

TEST_CASE("DIP com memory repository")
{
    radio_station original("Test FM");
    original.get_playlist().add_music(music("Song", "Artist", 100));

    auto repository_instance = make_memory_repository();
    station_service service(*repository_instance);
    service.save(original);

    const radio_station restored = service.load();
    REQUIRE(restored.get_name() == "Test FM");
    REQUIRE(restored.get_playlist().size() == 1);
}

TEST_CASE("concorrencia produz o mesmo resultado serial")
{
    radio_station station("Concurrent FM");
    station.add_program(std::make_unique<music_program>("Morning", "08:00", 12));
    station.add_program(std::make_unique<news_program>("News", "12:00", 6));

    int serial_total = 0;
    for (const auto& program : station.get_programs())
    {
        serial_total += program->calculate_audience();
    }

    REQUIRE(parallel_audience(station.get_programs()) == serial_total);
}
