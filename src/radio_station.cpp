#include "radio_station.hpp"

#include <iostream>
#include <memory>

RadioStation::RadioStation(const std::string& name)
    : name_(name)
{
}

Playlist& RadioStation::getPlaylist()
{
    return playlist_;
}

void RadioStation::addListener(const Listener& listener)
{
    listeners_.push_back(listener);
}

bool RadioStation::removeListener(std::size_t index)
{
    if (index >= listeners_.size())
    {
        return false;
    }

    listeners_.erase(listeners_.begin() + index);

    return true;
}

void RadioStation::showStationInfo() const
{
    std::cout << "\n=====================================\n";
    std::cout << "         " << name_ << '\n';
    std::cout << "=====================================\n";

    std::cout << "Songs available: "
              << playlist_.size()
              << '\n';

    std::cout << "Listeners online: "
              << listeners_.size()
              << "\n\n";
}

void RadioStation::showListeners() const
{
    std::cout << "\n===== LISTENERS =====\n\n";

    if (listeners_.empty())
    {
        std::cout << "No listeners connected.\n";
        return;
    }

    for (std::size_t i = 0; i < listeners_.size(); ++i)
    {
        std::cout << i + 1 << ". ";
        listeners_[i].showProfile();
        std::cout << '\n';
    }
}

void RadioStation::playMusic() const
{
    if (playlist_.size() == 0)
    {
        std::cout << "Playlist is empty.\n";
        return;
    }

    std::cout << "\n===== NOW PLAYING =====\n\n";

    playlist_.getMusic(0).showInfo();
}

void RadioStation::addProgram(
    std::unique_ptr<BroadcastContent> program)
{
    programs_.push_back(std::move(program));
}

void RadioStation::showPrograms() const
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

const std::vector<std::unique_ptr<BroadcastContent>>&
RadioStation::getPrograms() const
{
    return programs_;
}