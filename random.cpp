#include "random.h"

Random::Random()
    : gen(rd())
{

}


int Random::getRandomInt(int lowerBound, int highBound)
{
    std::uniform_int_distribution<int32_t> rd(lowerBound, highBound);
    return rd(gen);
}
