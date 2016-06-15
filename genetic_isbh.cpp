#include "genetic_isbh.h"
#include <algorithm>
#include <functional>
#include <random>


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
    for (auto& x : oligoMap)
    {
        for (auto i = 0; i < castOligoNumbersToInt(x.second); ++i)
        {
            oligoSpectrum.push_back(x.first);
        }
    }
}

void GeneticISBH::loadFirstOligo(const std::string& cs)
{
    firstOligo = cs;
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

int GeneticISBH::getPredecessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed)
{
    if (index == 0 || alreadyUsed[index - 1])
        return -1;
    else return getOverlap(oligoSpectrum[individual.oligos[index - 1]], oligoSpectrum[individual.oligos[index]]);
}

int GeneticISBH::getSuccessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed)
{
    if (index + 1 == individual.oligos.size() || alreadyUsed[index + 1])
        return -1;
    else return getOverlap(oligoSpectrum[individual.oligos[index]], oligoSpectrum[individual.oligos[index + 1]]);
}

std::pair<int, std::vector<int>::iterator> GeneticISBH::findBestPredecessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed)
{
    int overlap = 0, currentOverlap = 0;
    std::vector<int>::iterator iter;
    for (auto i = 0; i < individual.oligos.size(); ++i)
    {
        if (!alreadyUsed[i])
        {
            currentOverlap = getOverlap(oligoSpectrum[individual.oligos[i]], oligoSpectrum[individual.oligos[index]]);
            if (currentOverlap > overlap)
            {
                overlap = currentOverlap;
                iter = individual.oligos.begin() + i;
            }
        }
    }
    return std::make_pair(overlap, iter);
}

std::pair<int, std::vector<int>::iterator> GeneticISBH::findBestSuccessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed)
{
    int overlap = 0, currentOverlap = 0;
    std::vector<int>::iterator iter;
    for (auto i = 0; i < individual.oligos.size(); ++i)
    {
        if (!alreadyUsed[i])
        {
            currentOverlap = getOverlap(oligoSpectrum[individual.oligos[index]], oligoSpectrum[individual.oligos[i]]);
            if (currentOverlap > overlap)
            {
                overlap = currentOverlap;
                iter = individual.oligos.begin() + i;
            }
        }
    }
    return std::make_pair(overlap, iter);
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
    }
    // sort descending
    std::sort(population.begin(), population.end(), std::greater<Individual>());
    // select best c = 0.9 * n parents
    std::vector<Individual> newPopulation;
    int numberOfBestParents = populationRatio * population.size();
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> parentsDist(0, numberOfBestParents);
    std::uniform_int_distribution<int> oligosDist(0, oligoSpectrum.size() - 1);
    // crossover 3.1 && 3.2
    for (auto i = 0; i <= numberOfBestParents; ++i)
    {
        auto firstParent = population[parentsDist(rng)];
        auto secondParent = population[parentsDist(rng)];
        int firstOligoIndex = oligosDist(rng);
        auto ofIndex = firstOligoIndex, oiIndex = firstOligoIndex;

        // keep info if already in child's solution
        std::vector<bool> alreadyUsed(firstParent.oligos.size(), false);
        Individual child;
        child.oligos.push_back(firstOligoIndex);
        alreadyUsed[firstOligoIndex] = true;
        while (child.oligos.size() != firstParent.oligos.size())
        {
            auto firstParentOligoOfIter = std::find(firstParent.oligos.begin(), firstParent.oligos.end(), ofIndex);
            auto secondParentOligoOfIter = std::find(secondParent.oligos.begin(), secondParent.oligos.end(), ofIndex);
            auto firstParentOligoOiIter = std::find(firstParent.oligos.begin(), firstParent.oligos.end(), oiIndex);
            auto secondParentOligoOiIter = std::find(secondParent.oligos.begin(), secondParent.oligos.end(), oiIndex);
            // predecessor
            auto overlapPreFirstParent = getPredecessorOverlap(std::distance(firstParent.oligos.begin(), firstParentOligoOfIter), firstParent, alreadyUsed);
            auto overlapPreSecondParent = getPredecessorOverlap(std::distance(secondParent.oligos.begin(), secondParentOligoOfIter), secondParent, alreadyUsed);
            // successor
            auto overlapSuccFirstParent = getSuccessorOverlap(std::distance(firstParent.oligos.begin(), firstParentOligoOiIter), firstParent, alreadyUsed);
            auto overlapSuccSecondParent = getSuccessorOverlap(std::distance(secondParent.oligos.begin(), secondParentOligoOiIter), secondParent, alreadyUsed);

            // if no predecessor available take best one in all range
            if (overlapPreFirstParent == -1 && overlapPreSecondParent == -1)
            {
                auto firstParentPairPre = findBestPredecessorOverlap(std::distance(firstParent.oligos.begin(), firstParentOligoOfIter), firstParent, alreadyUsed);
                auto secondParentPairPre = findBestPredecessorOverlap(std::distance(secondParent.oligos.begin(), secondParentOligoOfIter), secondParent, alreadyUsed);
                std::pair<int, std::vector<int>::iterator> finalPair;
                if (firstParentPairPre.first > secondParentPairPre.first)
                    finalPair = firstParentPairPre;
                else finalPair = secondParentPairPre;
                child.oligos.insert(child.oligos.begin(), *finalPair.second);
                alreadyUsed[*finalPair.second] = true;
                ofIndex = *finalPair.second;
            }
            else if (overlapPreFirstParent > overlapPreSecondParent)
            {
                child.oligos.insert(child.oligos.begin(), *(firstParentOligoOfIter - 1));
                alreadyUsed[*(firstParentOligoOfIter - 1)] = true;
                ofIndex = *(firstParentOligoOfIter - 1);
            }
            else
            {
                child.oligos.insert(child.oligos.begin(), *(secondParentOligoOfIter - 1));
                alreadyUsed[*(secondParentOligoOfIter - 1)] = true;
                ofIndex = *(secondParentOligoOfIter - 1);
            }
            // if all are used:
            if (child.oligos.size() != firstParent.oligos.size()) break;
            // if no succcessor available take best one in all range
            if (overlapSuccFirstParent == -1 && overlapSuccSecondParent == -1)
            {
                auto firstParentPairPre = findBestSuccessorOverlap(std::distance(firstParent.oligos.begin(), firstParentOligoOiIter), firstParent, alreadyUsed);
                auto secondParentPairPre = findBestPredecessorOverlap(std::distance(secondParent.oligos.begin(), secondParentOligoOiIter), secondParent, alreadyUsed);
                std::pair<int, std::vector<int>::iterator> finalPair;
                if (firstParentPairPre.first > secondParentPairPre.first)
                    finalPair = firstParentPairPre;
                else finalPair = secondParentPairPre;
                child.oligos.push_back(*finalPair.second);
                alreadyUsed[*finalPair.second] = true;
                oiIndex = *finalPair.second;
            }
            else if (overlapSuccFirstParent > overlapSuccSecondParent)
            {
                child.oligos.push_back(*(firstParentOligoOiIter + 1));
                alreadyUsed[*(firstParentOligoOiIter + 1)] = true;
                oiIndex = *(firstParentOligoOiIter + 1);
            }
            else
            {
                child.oligos.push_back(*(secondParentOligoOiIter - 1));
                alreadyUsed[*(secondParentOligoOiIter + 1)] = true;
                oiIndex = *(secondParentOligoOiIter + 1);
            }
        }
        //add child to temporary population
        newPopulation.push_back(child);
    }
    //take best parents (already sorted)
    for (auto i = 0; i <= population.size() * 0.1; ++i)
    {
        newPopulation.push_back(this->population[i]);
    }
    this->population = newPopulation;
}

GeneticISBH::GeneticISBH()
{
}

GeneticISBH::~GeneticISBH()
{
}
