#ifndef json_serialization_hpp
#define json_serialization_hpp

#include "listener.hpp"
#include "music.hpp"
#include "radio_station.hpp"

#include <nlohmann/json.hpp>

#include <string>

using json = nlohmann::json;

void to_json(json& document, const music& music_item);
void from_json(const json& document, music& music_item);
void to_json(json& document, const listener& listener_item);
void from_json(const json& document, listener& listener_item);
void to_json(json& document, const radio_station& station);
void from_json(const json& document, radio_station& station);

json serialize_station(const radio_station& station);
radio_station deserialize_station(const json& document);
void save_json_file(const radio_station& station, const std::string& path);
radio_station load_json_file(const std::string& path);

#endif
