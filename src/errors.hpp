#ifndef errors_hpp
#define errors_hpp

#include <stdexcept>
#include <string>

class domain_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class invalid_music : public domain_error
{
public:
    explicit invalid_music(const std::string& message)
        : domain_error("Invalid music: " + message)
    {
    }
};

class listener_not_found : public domain_error
{
public:
    explicit listener_not_found(const std::string& nickname)
        : domain_error("Listener not found: " + nickname)
    {
    }
};

class invalid_program : public domain_error
{
public:
    explicit invalid_program(const std::string& message)
        : domain_error("Invalid program: " + message)
    {
    }
};

#endif
