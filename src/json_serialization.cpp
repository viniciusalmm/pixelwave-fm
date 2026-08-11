#include "json_serialization.hpp"

#include "music_program.hpp"
#include "news_program.hpp"

#include <fstream>
#include <memory>
#include <stdexcept>
#include <utility>

void to_json(json& document, const music& music_item)
{
    document = json::object();
    document["title"] = music_item.get_title();
    document["artist"] = music_item.get_artist();
    document["duration"] = music_item.get_duration();
}

void from_json(const json& document, music& music_item)
{
    music_item = music{
        document.at("title").get<std::string>(),
        document.at("artist").get<std::string>(),
        document.at("duration").get<int>()
    };
}

void to_json(json& document, const listener& listener_item)
{
    document = json::object();
    document["nickname"] = listener_item.get_nickname();
    document["country"] = listener_item.get_country();
    document["favorite_genre"] = listener_item.get_favorite_genre();
    document["online"] = listener_item.is_online();
}

void from_json(const json& document, listener& listener_item)
{
    listener_item = listener{
        document.at("nickname").get<std::string>(),
        document.at("country").get<std::string>(),
        document.at("favorite_genre").get<std::string>()
    };
    listener_item.set_online(document.at("online").get<bool>());
}

void to_json(json& document, const radio_station& station)
{
    document = json::object();
    document["version"] = 1;
    document["name"] = station.get_name();
    document["musics"] = station.get_playlist().get_musics();
    document["listeners"] = station.get_listeners();
    document["programs"] = json::array();

    for (const auto& program : station.get_programs())
    {
        json item = json::object();
        item["type"] = program->type();
        item["title"] = program->get_title();
        item["schedule"] = program->get_schedule();

        if (const auto* music_item = dynamic_cast<const music_program*>(program.get()))
        {
            item["songs"] = music_item->get_songs();
        }
        else if (const auto* news_item = dynamic_cast<const news_program*>(program.get()))
        {
            item["news_count"] = news_item->get_news_count();
        }
        else
        {
            throw std::runtime_error("Unsupported program type during serialization.");
        }

        document["programs"].push_back(std::move(item));
    }
}

void from_json(const json& document, radio_station& station)
{
    const int version = document.at("version").get<int>();
    if (version != 1)
    {
        throw std::runtime_error("Unsupported JSON version: " + std::to_string(version));
    }

    radio_station restored{document.at("name").get<std::string>()};

    for (const auto& item : document.at("musics"))
    {
        restored.get_playlist().add_music(item.get<music>());
    }

    for (const auto& item : document.at("listeners"))
    {
        restored.add_listener(item.get<listener>());
    }

    for (const auto& item : document.at("programs"))
    {
        const std::string program_type = item.at("type").get<std::string>();

        if (program_type == "Music Program")
        {
            restored.add_program(std::make_unique<music_program>(
                item.at("title").get<std::string>(),
                item.at("schedule").get<std::string>(),
                item.at("songs").get<int>()));
        }
        else if (program_type == "News Program")
        {
            restored.add_program(std::make_unique<news_program>(
                item.at("title").get<std::string>(),
                item.at("schedule").get<std::string>(),
                item.at("news_count").get<int>()));
        }
        else
        {
            throw std::runtime_error("Unknown program type: " + program_type);
        }
    }

    station = std::move(restored);
}

json serialize_station(const radio_station& station)
{
    json document;
    to_json(document, station);
    return document;
}

radio_station deserialize_station(const json& document)
{
    radio_station station{""};
    from_json(document, station);
    return station;
}

void save_json_file(const radio_station& station, const std::string& path)
{
    std::ofstream output(path);
    if (!output)
    {
        throw std::runtime_error("Could not open JSON file for writing: " + path);
    }

    output << serialize_station(station).dump(2);
}

radio_station load_json_file(const std::string& path)
{
    std::ifstream input(path);
    if (!input)
    {
        throw std::runtime_error("Could not open JSON file for reading: " + path);
    }

    json document;
    input >> document;
    return deserialize_station(document);
}
