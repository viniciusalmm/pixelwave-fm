#include "playlist.hpp"

#include <iostream>
#include <stdexcept>

void Playlist::addMusic(const Music& music)
{
    musics_.push_back(music);
}

bool Playlist::removeMusic(std::size_t index)
{
    if (index >= musics_.size())
    {
        return false;
    }

    musics_.erase(musics_.begin() + index);

    return true;
}

void Playlist::showPlaylist() const
{
    std::cout << "\n===== PLAYLIST =====\n\n";

    if (musics_.empty())
    {
        std::cout << "Playlist is empty.\n";
        return;
    }

    for (std::size_t i = 0; i < musics_.size(); ++i)
    {
        std::cout << i + 1 << ". ";

        musics_[i].showInfo();
    }

    std::cout
        << "\nTotal: "
        << musics_.size()
        << " music(s).\n";
}

std::size_t Playlist::size() const
{
    return musics_.size();
}

const Music& Playlist::getMusic(std::size_t index) const
{
    if (index >= musics_.size())
    {
        throw std::out_of_range("Invalid music index.");
    }

    return musics_[index];
}

void Playlist::clear()
{
    musics_.clear();
}