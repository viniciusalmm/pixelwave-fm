#include "listener.hpp"

#include <iostream>

Listener::Listener()
    : nickname_("Unknown"),
      country_("Unknown"),
      favoriteGenre_("Unknown"),
      online_(true)
{
}

Listener::Listener(
    const std::string& nickname,
    const std::string& country,
    const std::string& favoriteGenre
)
    : nickname_(nickname),
      country_(country),
      favoriteGenre_(favoriteGenre),
      online_(true)
{
}

std::string Listener::getNickname() const
{
    return nickname_;
}

std::string Listener::getCountry() const
{
    return country_;
}

std::string Listener::getFavoriteGenre() const
{
    return favoriteGenre_;
}

bool Listener::isOnline() const
{
    return online_;
}

void Listener::setNickname(const std::string& nickname)
{
    nickname_ = nickname;
}

void Listener::setCountry(const std::string& country)
{
    country_ = country;
}

void Listener::setFavoriteGenre(const std::string& genre)
{
    favoriteGenre_ = genre;
}

void Listener::setOnline(bool online)
{
    online_ = online;
}

void Listener::showProfile() const
{
    std::cout
        << nickname_
        << " | "
        << country_
        << " | Favorite: "
        << favoriteGenre_
        << " | "
        << (online_ ? "Online" : "Offline");
}