#include "music.hpp"

#include <iostream>

music::music()
    : title_("Unknown"),
      artist_("Unknown"),
      duration_(0)
{
}

music::music(
    const std::string& title,
    const std::string& artist,
    int duration
)
    : title_(title),
      artist_(artist),
      duration_(duration)
{
}

std::string music::get_title() const
{
    return title_;
}

std::string music::get_artist() const
{
    return artist_;
}

int music::get_duration() const
{
    return duration_;
}

void music::set_title(const std::string& title)
{
    title_ = title;
}

void music::set_artist(const std::string& artist)
{
    artist_ = artist;
}

void music::set_duration(int duration)
{
    duration_ = duration;
}

void music::show_info() const
{
    std::cout
        << title_
        << " - "
        << artist_
        << " ("
        << duration_
        << " s)"
        << '\n';
}
