#ifndef listener_hpp
#define listener_hpp

#include <string>

class listener
{
private:
    std::string nickname_;
    std::string country_;
    std::string favorite_genre_;
    bool online_;

public:
    listener();

    listener(
        const std::string& nickname,
        const std::string& country,
        const std::string& favorite_genre
    );

    std::string get_nickname() const;
    std::string get_country() const;
    std::string get_favorite_genre() const;
    bool is_online() const;

    void set_nickname(const std::string& nickname);
    void set_country(const std::string& country);
    void set_favorite_genre(const std::string& genre);
    void set_online(bool online);

    void show_profile() const;

    bool operator==(const listener& other) const;
};

#endif
