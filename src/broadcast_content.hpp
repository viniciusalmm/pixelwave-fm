#ifndef BROADCAST_CONTENT_HPP
#define BROADCAST_CONTENT_HPP

#include <string>

class BroadcastContent
{
protected:

    std::string title_;
    std::string schedule_;

public:

    BroadcastContent(
        const std::string& title,
        const std::string& schedule
    );

    virtual ~BroadcastContent();

    virtual int calculateAudience() const = 0;

    virtual std::string type() const = 0;

    virtual void display() const;
};

#endif