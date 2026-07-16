#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <string>


class Music
{
private:


    std::string title_;

    std::string artist_;

    int duration_;

public:

    Music();

    Music(
        const std::string& title,
        const std::string& artist,
        int duration
    );

    std::string getTitle() const;
    std::string getArtist() const;
    int getDuration() const;

    void setTitle(const std::string& title);
    void setArtist(const std::string& artist);
    void setDuration(int duration);
    
    void showInfo() const;
};

#endif