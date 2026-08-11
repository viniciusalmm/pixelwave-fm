#include "listener.hpp"

#include <iostream>

listener::listener()
    : nickname_("Unknown"),
      country_("Unknown"),
      favorite_genre_("Unknown"),
      online_(true)
{
}

listener::listener(
    const std::string& nickname,
    const std::string& country,
    const std::string& favorite_genre
)
    : nickname_(nickname),
      country_(country),
      favorite_genre_(favorite_genre),
      online_(true)
{
}

std::string listener::get_nickname() const
{
    return nickname_;
}

std::string listener::get_country() const
{
    return country_;
}

std::string listener::get_favorite_genre() const
{
    return favorite_genre_;
}

bool listener::is_online() const
{
    return online_;
}

void listener::set_nickname(const std::string& nickname)
{
    nickname_ = nickname;
}

void listener::set_country(const std::string& country)
{
    country_ = country;
}

void listener::set_favorite_genre(const std::string& genre)
{
    favorite_genre_ = genre;
}

void listener::set_online(bool online)
{
    online_ = online;
}

void listener::show_profile() const
{
    std::cout
        << nickname_
        << " | "
        << country_
        << " | Favorite: "
        << favorite_genre_
        << " | "
        << (online_ ? "Online" : "Offline");
}

bool listener::operator==(const listener& other) const
{
    return nickname_ == other.nickname_
        && country_ == other.country_
        && favorite_genre_ == other.favorite_genre_
        && online_ == other.online_;
}
