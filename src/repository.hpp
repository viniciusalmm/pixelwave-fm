#ifndef repository_hpp
#define repository_hpp

#include <memory>
#include <string>

class radio_station;

class repository
{
public:
    virtual ~repository() = default;
    virtual void save(const radio_station& station) = 0;
    virtual radio_station load() = 0;
};

std::unique_ptr<repository> make_json_repository(const std::string& path);
std::unique_ptr<repository> make_memory_repository();

#endif
