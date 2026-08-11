#include "music_program.hpp"

#include <iostream>

music_program::music_program(
    const std::string& title,
    const std::string& schedule,
    int songs
)
    : broadcast_content(title, schedule),
      songs_(songs)
{
}

int music_program::calculate_audience() const
{
    return songs_ * 120;
}

std::string music_program::type() const
{
    return "Music Program";
}

void music_program::display() const
{
    broadcast_content::display();
    std::cout << "Type: " << type() << '\n';
    std::cout << "Songs: " << songs_ << '\n';
    std::cout << "Estimated audience: " << calculate_audience() << '\n';
}

void music_program::transmit() const
{
    std::cout << "[ON AIR] Music program: " << title_ << '\n';
}

int music_program::get_songs() const
{
    return songs_;
}
