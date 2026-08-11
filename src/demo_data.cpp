#include "demo_data.hpp"

#include "listener.hpp"
#include "music.hpp"
#include "music_program.hpp"
#include "news_program.hpp"

#include <memory>

radio_station create_demo_station()
{
    radio_station station{"PixelWave FM"};

    station.get_playlist().add_music(music{"Snow Storm", "Habbo", 180});
    station.get_playlist().add_music(music{"Club Mammoth", "Habbo", 205});
    station.get_playlist().add_music(music{"Battle Ball", "Habbo", 194});

    station.add_listener(listener{"Vinicius", "Brazil", "Retro"});
    station.add_listener(listener{"Carlos", "Portugal", "Dance"});

    station.add_program(std::make_unique<music_program>(
        "Morning Hits", "08:00", 12));
    station.add_program(std::make_unique<news_program>(
        "Pixel News", "12:00", 6));

    return station;
}
