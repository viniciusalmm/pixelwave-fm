#include "broadcast_content.hpp"

#include <iostream>

BroadcastContent::BroadcastContent(
    const std::string& title,
    const std::string& schedule
)
    : title_(title),
      schedule_(schedule)
{
}

BroadcastContent::~BroadcastContent() = default;

void BroadcastContent::display() const
{
    std::cout
        << "Title: "
        << title_
        << '\n';

    std::cout
        << "Schedule: "
        << schedule_
        << '\n';
}