#ifndef TRANSMISSIBLE_HPP
#define TRANSMISSIBLE_HPP

class Transmissible
{
public:

    virtual void transmit() const = 0;

    virtual ~Transmissible() = default;
};

#endif