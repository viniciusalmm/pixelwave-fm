#ifndef generic_hpp
#define generic_hpp

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>
#include <vector>

// Template reutilizável para armazenar objetos do domínio.
template <typename item_type>
class registry
{
private:
    std::vector<item_type> items_;

public:
    void add(item_type item)
    {
        items_.push_back(std::move(item));
    }

    const item_type& at(std::size_t index) const
    {
        return items_.at(index);
    }

    std::size_t size() const
    {
        return items_.size();
    }

    template <typename predicate_type>
    std::optional<std::reference_wrapper<const item_type>> find_first(
        predicate_type predicate) const
    {
        const auto iterator = std::find_if(items_.begin(), items_.end(), predicate);

        if (iterator == items_.end())
        {
            return std::nullopt;
        }

        return std::cref(*iterator);
    }
};

template <typename item_type>
concept calculable = requires(const item_type& value)
{
    { value.calculate_audience() } -> std::convertible_to<int>;
};

template <calculable item_type>
int total_audience(const std::vector<item_type>& items)
{
    int total = 0;

    for (const auto& item : items)
    {
        total += item.calculate_audience();
    }

    return total;
}

// CRTP para contar instâncias sem criar uma vtable para esse comportamento.
template <typename derived_type>
class counted
{
private:
    static inline int count_ = 0;

protected:
    counted()
    {
        ++count_;
    }

    counted(const counted&)
    {
        ++count_;
    }

    ~counted()
    {
        --count_;
    }

public:
    static int alive()
    {
        return count_;
    }
};

#endif
