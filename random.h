#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random
{
public:
    Random();
    int getRandomInt(int lowerBound, int highBound);
private:
    std::random_device rd;
    std::mt19937_64 gen;
};

#endif // RANDOM_H
