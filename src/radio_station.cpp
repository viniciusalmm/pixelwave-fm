#include "radio_station.hpp"

#include "errors.hpp"

#include <algorithm>
#include <iostream>
#include <memory>

radio_station::radio_station(const std::string& name)
    : name_(name)
{
}

playlist& radio_station::get_playlist()
{
    return playlist_;
}

const playlist& radio_station::get_playlist() const
{
    return playlist_;
}

void radio_station::add_listener(const listener& listener_item)
{
    if (listener_item.get_nickname().empty())
    {
        throw domain_error("Listener nickname cannot be empty.");
    }

    listeners_.push_back(listener_item);
}

bool radio_station::remove_listener(std::size_t index)
{
    if (index >= listeners_.size())
    {
        return false;
    }

    listeners_.erase(listeners_.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

void radio_station::show_station_info() const
{
    std::cout << "\n=====================================\n";
    std::cout << "         " << name_ << '\n';
    std::cout << "=====================================\n";
    std::cout << "Songs available: " << playlist_.size() << '\n';
    std::cout << "Listeners online: " << listeners_.size() << "\n\n";
}

void radio_station::show_listeners() const
{
    std::cout << "\n===== LISTENERS =====\n\n";

    if (listeners_.empty())
    {
        std::cout << "No listeners connected.\n";
        return;
    }

    for (std::size_t index = 0; index < listeners_.size(); ++index)
    {
        std::cout << index + 1 << ". ";
        listeners_[index].show_profile();
        std::cout << '\n';
    }
}

void radio_station::play_music() const
{
    if (playlist_.size() == 0)
    {
        std::cout << "Playlist is empty.\n";
        return;
    }

    std::cout << "\n===== NOW PLAYING =====\n\n";
    playlist_.get_music(0).show_info();
}

void radio_station::add_program(std::unique_ptr<broadcast_content> program)
{
    if (!program)
    {
        throw invalid_program("program cannot be null");
    }

    programs_.push_back(std::move(program));
}

void radio_station::show_programs() const
{
    std::cout << "\n===== PROGRAMS =====\n\n";

    if (programs_.empty())
    {
        std::cout << "No programs registered.\n";
        return;
    }

    for (const auto& program : programs_)
    {
        program->display();
        std::cout << '\n';
    }
}

const std::vector<std::unique_ptr<broadcast_content>>& radio_station::get_programs() const
{
    return programs_;
}

const std::string& radio_station::get_name() const
{
    return name_;
}

const std::vector<listener>& radio_station::get_listeners() const
{
    return listeners_;
}

std::optional<listener> radio_station::find_listener(const std::string& nickname) const
{
    const auto iterator = std::find_if(
        listeners_.begin(),
        listeners_.end(),
        [&nickname](const listener& listener_item)
        {
            return listener_item.get_nickname() == nickname;
        });

    if (iterator == listeners_.end())
    {
        return std::nullopt;
    }

    return *iterator;
}
