#include "genetic_isbh.h"
#include <algorithm>


const double GeneticISBH::populationRatio = 0.5;

int GeneticISBH::castOligoNumbersToInt(OligoNumbers x, bool withExcess)
{
    if (x == OligoNumbers::ONE) return 1;
    if (x == OligoNumbers::ZERO) return 0;
    if (x == OligoNumbers::TWO_OR_THREE)
    {
        if (withExcess) return 3;
        else return 2;
    }
    if (x == OligoNumbers::FOUR_OR_FIVE)
    {
        if (withExcess) return 5;
        else return 4;
    }
    return 6; //many
}

void GeneticISBH::loadOligoMap(std::map<std::string, OligoNumbers> oligoMap)
{
    for (auto & x : oligoMap)
    {
        for (auto i = 0; i < castOligoNumbersToInt(x.second); ++i)
        {
            oligoSpectrum.push_back(x.first);
        }
    }
}

void GeneticISBH::computeSolution()
{
    //step1 - generate population
    const int numPop = oligoSpectrum.size() * populationRatio;
    const int spectrumSize = oligoSpectrum.size();
    std::vector<int> firstIndividual;
    for (int i = 0; i < spectrumSize; ++i)
    {
        firstIndividual.push_back(i);
    }
    // create 1/2 * size instances of individuals
    for (int i = 0; i < numPop; ++i) 
    {
        std::vector<int> individual = firstIndividual;
        std::random_shuffle(individual.begin(), individual.end()); // uses default random engine, we can change to other one in future
        population.push_back(individual);
    }

    //step 2 - repaet step 3 - parent selection
}

GeneticISBH::GeneticISBH()
{
}

GeneticISBH::~GeneticISBH()
{
}
