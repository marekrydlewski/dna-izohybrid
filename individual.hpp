#pragma once
#include <vector>

struct Individual
{
    std::vector<int> oligos;
    int fitness;
    bool operator < (const Individual& str) const
    {
        return (this->fitness < str.fitness);
    }
    bool operator > (const Individual& str) const
    {
        return (this->fitness > str.fitness);
    }
    Individual() { this->fitness = 0; }
};
