#include "music.hpp"
#include "playlist.hpp"
#include "listener.hpp"
#include "radio_station.hpp"

#include "music_program.hpp"
#include "news_program.hpp"
#include "utils.hpp"

#include <iostream>
#include <memory>

int main()
{
    RadioStation station("PixelWave FM");

    station.getPlaylist().addMusic(
        Music("Snow Storm","Habbo",180));

    station.getPlaylist().addMusic(
        Music("Club Mammoth","Habbo",205));

    station.getPlaylist().addMusic(
        Music("Battle Ball","Habbo",194));

    station.addListener(
        Listener("Vinicius","Brazil","Retro"));

    station.addListener(
        Listener("Carlos","Portugal","Dance"));

    station.addProgram(
        std::make_unique<MusicProgram>(
            "Morning Hits",
            "08:00",
            12));

    station.addProgram(
        std::make_unique<NewsProgram>(
            "Pixel News",
            "12:00",
            6));

    station.showStationInfo();

    station.getPlaylist().showPlaylist();

    station.playMusic();

    station.showListeners();

    station.showPrograms();

    std::vector<std::unique_ptr<BroadcastContent>> demo;

    demo.push_back(
        std::make_unique<MusicProgram>(
            "Afternoon Mix",
            "15:00",
            15));

    demo.push_back(
        std::make_unique<NewsProgram>(
            "Night News",
            "21:00",
            10));

    const BroadcastContent* best =
        highestAudience(demo);

    if(best)
    {
        std::cout
            << "\n===== HIGHEST AUDIENCE =====\n\n";

        best->display();
    }

    return 0;
}