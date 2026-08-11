#ifndef music_hpp
#define music_hpp

#include <string>

class music
{
private:
    std::string title_;
    std::string artist_;
    int duration_;

public:
    music();
    music(const std::string& title, const std::string& artist, int duration);

    std::string get_title() const;
    std::string get_artist() const;
    int get_duration() const;

    void set_title(const std::string& title);
    void set_artist(const std::string& artist);
    void set_duration(int duration);

    void show_info() const;

    bool operator==(const music& other) const = default;
};

#endif
