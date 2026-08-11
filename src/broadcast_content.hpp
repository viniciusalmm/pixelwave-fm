#ifndef broadcast_content_hpp
#define broadcast_content_hpp

#include <string>

class broadcast_content
{
protected:
    std::string title_;
    std::string schedule_;

public:
    broadcast_content(
        const std::string& title,
        const std::string& schedule
    );

    virtual ~broadcast_content();

    virtual int calculate_audience() const = 0;
    virtual std::string type() const = 0;

    const std::string& get_title() const;
    const std::string& get_schedule() const;

    virtual void display() const;
};

#endif
