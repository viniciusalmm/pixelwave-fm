#include "news_program.hpp"

#include <iostream>

NewsProgram::NewsProgram(
    const std::string& title,
    const std::string& schedule,
    int newsCount
)
    : BroadcastContent(title, schedule),
      newsCount_(newsCount)
{
}

int NewsProgram::calculateAudience() const
{
    return newsCount_ * 250;
}

std::string NewsProgram::type() const
{
    return "News Program";
}

void NewsProgram::display() const
{
    BroadcastContent::display();

    std::cout
        << "Type: "
        << type()
        << '\n';

    std::cout
        << "News: "
        << newsCount_
        << '\n';

    std::cout
        << "Estimated audience: "
        << calculateAudience()
        << '\n';
}

void NewsProgram::transmit() const
{
    std::cout
        << "[ON AIR] News program: "
        << title_
        << '\n';
}