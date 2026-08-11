#include "broadcast_content.hpp"

#include <iostream>

broadcast_content::broadcast_content(
    const std::string& title,
    const std::string& schedule
)
    : title_(title),
      schedule_(schedule)
{
}

broadcast_content::~broadcast_content() = default;

void broadcast_content::display() const
{
    std::cout << "Title: " << title_ << '\n';
    std::cout << "Schedule: " << schedule_ << '\n';
}

const std::string& broadcast_content::get_title() const
{
    return title_;
}

const std::string& broadcast_content::get_schedule() const
{
    return schedule_;
}
