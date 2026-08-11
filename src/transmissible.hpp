#ifndef transmissible_hpp
#define transmissible_hpp

class transmissible
{
public:
    virtual void transmit() const = 0;
    virtual ~transmissible() = default;
};

#endif
