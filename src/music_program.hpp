#ifndef music_program_hpp
#define music_program_hpp

#include "broadcast_content.hpp"
#include "generic.hpp"
#include "transmissible.hpp"

class music_program : public broadcast_content,
                      public transmissible,
                      public counted<music_program>
{
private:
    int songs_;

public:
    music_program(
        const std::string& title,
        const std::string& schedule,
        int songs
    );

    int calculate_audience() const override;
    std::string type() const override;
    void display() const override;
    void transmit() const override;
    int get_songs() const;
};

#endif
