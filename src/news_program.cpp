#include "news_program.hpp"

#include <iostream>

news_program::news_program(
    const std::string& title,
    const std::string& schedule,
    int news_count
)
    : broadcast_content(title, schedule),
      news_count_(news_count)
{
}

int news_program::calculate_audience() const
{
    return news_count_ * 250;
}

std::string news_program::type() const
{
    return "News Program";
}

void news_program::display() const
{
    broadcast_content::display();
    std::cout << "Type: " << type() << '\n';
    std::cout << "News: " << news_count_ << '\n';
    std::cout << "Estimated audience: " << calculate_audience() << '\n';
}

void news_program::transmit() const
{
    std::cout << "[ON AIR] News program: " << title_ << '\n';
}

int news_program::get_news_count() const
{
    return news_count_;
}
