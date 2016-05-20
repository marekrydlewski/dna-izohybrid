#pragma once
#include <iostream>

enum class OligoNumbers : int { BEGIN_OF_LIST = -1, ZERO = 0, ONE, TWO_OR_THREE, FOUR_OR_FIVE, MANY, END_OF_LIST };

OligoNumbers& operator++(OligoNumbers &c) {
    using IntType = typename std::underlying_type<OligoNumbers>::type;
    c = static_cast<OligoNumbers>(static_cast<IntType>(c) + 1);
    if (c == OligoNumbers::END_OF_LIST)
        c = static_cast<OligoNumbers>(0);
    return c;
}

OligoNumbers operator++(OligoNumbers &c, int) {
    OligoNumbers result = c;
    ++c;
    return result;
}

OligoNumbers& operator--(OligoNumbers &c) {
    using IntType = typename std::underlying_type<OligoNumbers>::type;
    c = static_cast<OligoNumbers>(static_cast<IntType>(c) - 1);
    if (c == OligoNumbers::BEGIN_OF_LIST)
        c = static_cast<OligoNumbers>(0);
    return c;
}

OligoNumbers operator--(OligoNumbers &c, int) {
    OligoNumbers result = c;
    --c;
    return result;
}