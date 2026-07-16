#include "music.hpp"

#include <iostream>

Music::Music()
    : title_("Unknown"),
      artist_("Unknown"),
      duration_(0)
{
}

Music::Music(
    const std::string& title,
    const std::string& artist,
    int duration
)
    : title_(title),
      artist_(artist),
      duration_(duration)
{
}

std::string Music::getTitle() const
{
    return title_;
}

std::string Music::getArtist() const
{
    return artist_;
}

int Music::getDuration() const
{
    return duration_;
}

void Music::setTitle(const std::string& title)
{
    title_ = title;
}

void Music::setArtist(const std::string& artist)
{
    artist_ = artist;
}

void Music::setDuration(int duration)
{
    duration_ = duration;
}

void Music::showInfo() const
{
    std::cout
        << title_
        << " - "
        << artist_
        << " ("
        << duration_
        << " s)"
        << std::endl;
}