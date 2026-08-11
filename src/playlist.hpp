#ifndef playlist_hpp
#define playlist_hpp

#include "music.hpp"

#include <cstddef>
#include <vector>

class playlist
{
private:
    std::vector<music> musics_;

public:
    void add_music(const music& music_item);
    bool remove_music(std::size_t index);
    void show_playlist() const;
    std::size_t size() const;
    const music& get_music(std::size_t index) const;
    void clear();
    const std::vector<music>& get_musics() const;
};

#endif
