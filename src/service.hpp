#ifndef service_hpp
#define service_hpp

#include "radio_station.hpp"
#include "repository.hpp"

class station_service
{
private:
    repository& repository_;

public:
    explicit station_service(repository& repository_instance)
        : repository_(repository_instance)
    {
    }

    void save(const radio_station& station)
    {
        repository_.save(station);
    }

    radio_station load()
    {
        return repository_.load();
    }
};

#endif
