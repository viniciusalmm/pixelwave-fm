#ifndef utils_hpp
#define utils_hpp

#include "broadcast_content.hpp"
#include "music.hpp"

#include <algorithm>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <variant>
#include <vector>

inline const broadcast_content* highest_audience(
    const std::vector<std::unique_ptr<broadcast_content>>& programs)
{
    if (programs.empty())
    {
        return nullptr;
    }

    const auto iterator = std::max_element(
        programs.begin(),
        programs.end(),
        [](const auto& left, const auto& right)
        {
            return left->calculate_audience() < right->calculate_audience();
        });

    return iterator->get();
}

using music_search_result = std::variant<music, std::string>;

inline music_search_result find_music(
    const std::vector<music>& musics,
    const std::string& title)
{
    const auto iterator = std::find_if(
        musics.begin(),
        musics.end(),
        [&title](const music& music_item)
        {
            return music_item.get_title() == title;
        });

    if (iterator != musics.end())
    {
        return *iterator;
    }

    return std::string{"Music not found: "} + title;
}

inline int parallel_audience(
    const std::vector<std::unique_ptr<broadcast_content>>& programs)
{
    std::mutex total_mutex;
    int total = 0;
    std::vector<std::future<void>> futures;

    futures.reserve(programs.size());

    for (const auto& program : programs)
    {
        const broadcast_content* item = program.get();

        futures.push_back(std::async(
            std::launch::async,
            [item, &total, &total_mutex]()
            {
                // O cálculo de cada programa é independente; apenas a agregação é compartilhada.
                const int partial = item->calculate_audience();
                std::lock_guard<std::mutex> lock{total_mutex};
                total += partial;
            }));
    }

    for (auto& future : futures)
    {
        future.get();
    }

    return total;
}

#endif
