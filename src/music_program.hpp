#ifndef MUSIC_PROGRAM_HPP
#define MUSIC_PROGRAM_HPP

#include "broadcast_content.hpp"
#include "transmissible.hpp"

class MusicProgram : public BroadcastContent,
                     public Transmissible
{
private:

    int songs_;

public:

    MusicProgram(
        const std::string& title,
        const std::string& schedule,
        int songs
    );

    int calculateAudience() const override;

    std::string type() const override;

    void display() const override;

    void transmit() const override;
};

#endif