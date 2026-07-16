#ifndef RADIO_STATION_HPP
#define RADIO_STATION_HPP

#include "listener.hpp"
#include "playlist.hpp"

#include <string>
#include <vector>
#include "broadcast_content.hpp"

#include <memory>

class RadioStation
{
private:

    std::string name_;
    Playlist playlist_;
    std::vector<Listener> listeners_;
    std::vector<std::unique_ptr<BroadcastContent>> programs_;

public:

    explicit RadioStation(const std::string& name);

    Playlist& getPlaylist();

    void addListener(const Listener& listener);

    bool removeListener(std::size_t index);

    void showListeners() const;

    void showStationInfo() const;

    void playMusic() const;

    void addProgram(std::unique_ptr<BroadcastContent> program);

    void showPrograms() const;

    const std::vector<std::unique_ptr<BroadcastContent>>&
    getPrograms() const;
};

#endif