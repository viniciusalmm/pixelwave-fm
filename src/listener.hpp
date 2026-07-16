#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <string>


class Listener
{
private:

    std::string nickname_;

    std::string country_;

    std::string favoriteGenre_;

    bool online_;

public:

    Listener();

    Listener(
        const std::string& nickname,
        const std::string& country,
        const std::string& favoriteGenre
    );

    std::string getNickname() const;
    std::string getCountry() const;
    std::string getFavoriteGenre() const;
    bool isOnline() const;

    void setNickname(const std::string& nickname);
    void setCountry(const std::string& country);
    void setFavoriteGenre(const std::string& genre);
    void setOnline(bool online);

    void showProfile() const;
};

#endif