#ifndef UTILS_HPP
#define UTILS_HPP

#include "broadcast_content.hpp"

#include <memory>
#include <vector>

inline const BroadcastContent* highestAudience(
    const std::vector<std::unique_ptr<BroadcastContent>>& programs)
{
    if (programs.empty())
    {
        return nullptr;
    }

    const BroadcastContent* best = programs.front().get();

    for (const auto& program : programs)
    {
        if (program->calculateAudience() >
            best->calculateAudience())
        {
            best = program.get();
        }
    }

    return best;
}

#endif