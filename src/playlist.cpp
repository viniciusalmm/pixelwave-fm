#include "playlist.hpp"

#include "errors.hpp"

#include <iostream>
#include <stdexcept>

void playlist::add_music(const music& music_item)
{
    if (music_item.get_title().empty())
    {
        throw invalid_music("title cannot be empty");
    }

    if (music_item.get_duration() <= 0)
    {
        throw invalid_music("duration must be positive");
    }

    musics_.push_back(music_item);
}

bool playlist::remove_music(std::size_t index)
{
    if (index >= musics_.size())
    {
        return false;
    }

    musics_.erase(musics_.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

void playlist::show_playlist() const
{
    std::cout << "\n===== PLAYLIST =====\n\n";

    if (musics_.empty())
    {
        std::cout << "Playlist is empty.\n";
        return;
    }

    for (std::size_t index = 0; index < musics_.size(); ++index)
    {
        std::cout << index + 1 << ". ";
        musics_[index].show_info();
    }

    std::cout << "\nTotal: " << musics_.size() << " music(s).\n";
}

std::size_t playlist::size() const
{
    return musics_.size();
}

const music& playlist::get_music(std::size_t index) const
{
    if (index >= musics_.size())
    {
        throw std::out_of_range("Invalid music index.");
    }

    return musics_[index];
}

void playlist::clear()
{
    musics_.clear();
}

const std::vector<music>& playlist::get_musics() const
{
    return musics_;
}
