#ifndef NEWS_PROGRAM_HPP
#define NEWS_PROGRAM_HPP

#include "broadcast_content.hpp"
#include "transmissible.hpp"

class NewsProgram final : public BroadcastContent,
                          public Transmissible
{
private:

    int newsCount_;

public:

    NewsProgram(
        const std::string& title,
        const std::string& schedule,
        int newsCount
    );

    int calculateAudience() const override;

    std::string type() const override;

    void display() const override;

    void transmit() const override;
};

#endif