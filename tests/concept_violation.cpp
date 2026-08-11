#include "generic.hpp"

#include <vector>

int main()
{
    const std::vector<int> values{1, 2, 3};

    // Este arquivo existe para demonstrar o diagnóstico claro do concept quando compilado de propósito.
    return total_audience(values);
}
