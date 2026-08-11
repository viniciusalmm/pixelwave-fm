#ifndef radio_station_hpp
#define radio_station_hpp

#include "broadcast_content.hpp"
#include "listener.hpp"
#include "playlist.hpp"

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class radio_station
{
private:
    std::string name_;
    playlist playlist_;
    std::vector<listener> listeners_;
    std::vector<std::unique_ptr<broadcast_content>> programs_;

public:
    explicit radio_station(const std::string& name);

    radio_station(const radio_station&) = delete;
    radio_station& operator=(const radio_station&) = delete;
    radio_station(radio_station&&) noexcept = default;
    radio_station& operator=(radio_station&&) noexcept = default;

    playlist& get_playlist();
    const playlist& get_playlist() const;

    void add_listener(const listener& listener_item);
    bool remove_listener(std::size_t index);
    void show_listeners() const;
    void show_station_info() const;
    void play_music() const;

    void add_program(std::unique_ptr<broadcast_content> program);
    void show_programs() const;

    const std::vector<std::unique_ptr<broadcast_content>>& get_programs() const;
    const std::string& get_name() const;
    const std::vector<listener>& get_listeners() const;
    std::optional<listener> find_listener(const std::string& nickname) const;
};

#endif
