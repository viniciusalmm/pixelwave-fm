#include "music_program.hpp"

#include <iostream>

MusicProgram::MusicProgram(
    const std::string& title,
    const std::string& schedule,
    int songs
)
    : BroadcastContent(title, schedule),
      songs_(songs)
{
}

int MusicProgram::calculateAudience() const
{
    return songs_ * 120;
}

std::string MusicProgram::type() const
{
    return "Music Program";
}

void MusicProgram::display() const
{
    BroadcastContent::display();

    std::cout
        << "Type: "
        << type()
        << '\n';

    std::cout
        << "Songs: "
        << songs_
        << '\n';

    std::cout
        << "Estimated audience: "
        << calculateAudience()
        << '\n';
}

void MusicProgram::transmit() const
{
    std::cout
        << "[ON AIR] Music program: "
        << title_
        << '\n';
}