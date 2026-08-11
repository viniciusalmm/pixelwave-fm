#include "repository.hpp"

#include "json_serialization.hpp"
#include "radio_station.hpp"

#include <stdexcept>
#include <utility>

class json_repository : public repository
{
private:
    std::string path_;

public:
    explicit json_repository(std::string path)
        : path_(std::move(path))
    {
    }

    void save(const radio_station& station) override
    {
        save_json_file(station, path_);
    }

    radio_station load() override
    {
        return load_json_file(path_);
    }
};

class memory_repository : public repository
{
private:
    json data_;
    bool has_data_ = false;

public:
    void save(const radio_station& station) override
    {
        data_ = serialize_station(station);
        has_data_ = true;
    }

    radio_station load() override
    {
        if (!has_data_)
        {
            throw std::runtime_error("Memory repository is empty.");
        }

        return deserialize_station(data_);
    }
};

std::unique_ptr<repository> make_json_repository(const std::string& path)
{
    return std::make_unique<json_repository>(path);
}

std::unique_ptr<repository> make_memory_repository()
{
    return std::make_unique<memory_repository>();
}
