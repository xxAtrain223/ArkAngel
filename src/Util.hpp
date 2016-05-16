//
// Created by Anthony on 5/15/2016.
//

#ifndef ARKANGEL_UTIL_HPP
#define ARKANGEL_UTIL_HPP

/**
 * Clamps a value within a range
 *
 * Clamps Val to the range [Min,Max].
 *
 * @param Val Value
 * @param Min Beginning of range
 * @param Max End of range
 *
 * @return Clamped value
 */
template<typename T>
T Clamp(const T& Val, const T& Min, const T& Max)
{
    return Val < Min ? Min : (Max < Val ? Max : Val);
}

/**
 * Wraps a value within a range
 *
 * Wraps Val to the range [Min,Max).
 *
 * @param Val Value
 * @param Min Beginning of range
 * @param Max End of range
 *
 * @return Wrapped value
 */
template<typename T>
T Wrap(T Val, const T& Min, const T& Max)
{
    auto dist = Max - Min;
    while (Val < Min)
    {
        Val = Val + dist;
    }
    while (!(Val < Max))
    {
        Val = Val - dist;
    }
    return Val;
}

/**
 * Gets the sign of a value
 *
 * If the value is less than zero, returns -1.
 * If the value is greater than zero, returns 1.
 * Otherwise, returns 0.
 *
 * @param Val Value
 *
 * @return Sign of Value
 */
template<typename T>
T Sign(const T& Val)
{
    return Val < 0 ? -1 : (0 < Val ? 1 : 0);
}

#endif //ARKANGEL_UTIL_HPP
