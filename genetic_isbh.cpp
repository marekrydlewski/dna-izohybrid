#include "genetic_isbh.h"
#include <algorithm>
#include <functional>


const double GeneticISBH::populationRatio = 0.5;
const double GeneticISBH::parentsRatio = 0.9;

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

void GeneticISBH::loadOligoMap(std::map<std::string, OligoNumbers> oligoMap, int size)
{
    dnaSize = size;
    for (auto & x : oligoMap)
    {
        for (auto i = 0; i < castOligoNumbersToInt(x.second); ++i)
        {
            oligoSpectrum.push_back(x.first);
        }
    }
}

int GeneticISBH::getOverlap(std::string s1, std::string s2)
{
    int max = 0; // max common substring (end s1 ___ begin s2) - overlap
    int size = s1.size() > s2.size() ? s2.size() : s1.size();
    int j = 1;
    for (auto i = size - 1; i >= 0; --i)
    {
        bool isValid = true;
        for (auto jj = 0; jj < j; ++jj)
        {
            if (s1[s1.size() - 1 - j + 1 + jj] != s2[jj])
            {
                isValid = false;
                break;
            }
        }
        if (isValid)
        {
            max = j;
        }
        ++j;
    }
    return max;
}

void GeneticISBH::computeSolution()
{
    //step1 - generate population
    const int numPop = oligoSpectrum.size() * populationRatio;
    const int spectrumSize = oligoSpectrum.size();
    Individual firstIndividual;
    for (auto i = 0; i < spectrumSize; ++i)
    {
        firstIndividual.oligos.push_back(i);
    }
    // create 1/2 * size instances of individuals
    for (auto i = 0; i < numPop; ++i)
    {
        auto individual = firstIndividual;
        // uses default random engine, we can change to other one in future
        std::random_shuffle(individual.oligos.begin(), individual.oligos.end());
        population.push_back(individual);
    }
    // step 2 - repaet step 3 - parent selection
    for (auto& individual : population)
    {
        auto lastButOne = individual.oligos.end();
        --lastButOne;
        auto currentSize = oligoSpectrum[individual.oligos[0]].size();
        auto numberOfOligos = 1;
        for (auto it = individual.oligos.begin(); it != lastButOne; ++it)
        {
            auto overlap = getOverlap(oligoSpectrum[*it], oligoSpectrum[*(it + 1)]);
            auto possibleCurrentSize = currentSize + oligoSpectrum[*(it + 1)].size() - overlap;
            if (possibleCurrentSize > dnaSize) 
                break;
            else
            {
                currentSize = possibleCurrentSize;
                ++numberOfOligos;
            }
        }
        individual.fitness = numberOfOligos;
        //select best c = 0.9 * n parents
    }
    // sort descending
    std::sort(population.begin(), population.end(), std::greater<Individual>());

}

GeneticISBH::GeneticISBH()
{
}

GeneticISBH::~GeneticISBH()
{
}
