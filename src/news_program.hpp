#ifndef news_program_hpp
#define news_program_hpp

#include "broadcast_content.hpp"
#include "generic.hpp"
#include "transmissible.hpp"

class news_program final : public broadcast_content,
                           public transmissible,
                           public counted<news_program>
{
private:
    int news_count_;

public:
    news_program(
        const std::string& title,
        const std::string& schedule,
        int news_count
    );

    int calculate_audience() const override;
    std::string type() const override;
    void display() const override;
    void transmit() const override;
    int get_news_count() const;
};

#endif
