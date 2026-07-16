#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include "music.hpp"

#include <vector>

class Playlist
{
private:

    std::vector<Music> musics_;

public:

    void addMusic(const Music& music);

    bool removeMusic(std::size_t index);

    void showPlaylist() const;

    std::size_t size() const;

    const Music& getMusic(std::size_t index) const;

    void clear();
};

#endif