#include "genetic_isbh.hpp"
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
    if (index == 0 || alreadyUsed[individual.oligos[index - 1]])
        return -1;
    else return getOverlap(oligoSpectrum[individual.oligos[index - 1]], oligoSpectrum[individual.oligos[index]]);
}

int GeneticISBH::getSuccessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed)
{
    if (index + 1 == individual.oligos.size() || alreadyUsed[individual.oligos[index + 1]])
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

std::pair<int, std::vector<int>::iterator> GeneticISBH::findBestSuccessorOverlap(int index, Individual& individual, std::vector<bool> alreadyUsed)
{
    int overlap = 0, currentOverlap = 0;
    std::vector<int>::iterator iter;
    for (auto i = 0; i < individual.oligos.size(); ++i)
    {
        if (!alreadyUsed[individual.oligos[i]])
        {
            currentOverlap = getOverlap(oligoSpectrum[individual.oligos[index]], oligoSpectrum[individual.oligos[i]]);
            if (currentOverlap >= overlap)
            {
                overlap = currentOverlap;
                iter = individual.oligos.begin() + i;
            }
        }
    }
    return std::make_pair(overlap, iter);
}

void GeneticISBH::mutatePopulation()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> popDist(0, population.size() - 1);
    for(auto r = 0; r < 0.01 * oligoSpectrum.size() * population.size() * 0.5; ++r)
    {
        auto randomIndex = popDist(rng);
        int overlap = 66666, currentOverlap, index = -1;
        for (auto i = 2; i < population[randomIndex].oligos.size() - 2; ++i)
        {
            currentOverlap = getOverlap(oligoSpectrum[population[randomIndex].oligos[i]], oligoSpectrum[population[randomIndex].oligos[i + 1]]) +
                getOverlap(oligoSpectrum[population[randomIndex].oligos[i -1]], oligoSpectrum[population[randomIndex].oligos[i]]);    
            if (currentOverlap < overlap)
            {
                overlap = currentOverlap;
                index = i;
            }
            int overlapLeftNeighbour = getOverlap(oligoSpectrum[population[randomIndex].oligos[index - 1]], oligoSpectrum[population[randomIndex].oligos[index]]) +
                getOverlap(oligoSpectrum[population[randomIndex].oligos[index - 2]], oligoSpectrum[population[randomIndex].oligos[index - 1]]);
            int overlapRightNeighbour = getOverlap(oligoSpectrum[population[randomIndex].oligos[index + 1]], oligoSpectrum[population[randomIndex].oligos[index + 2]]) +
                getOverlap(oligoSpectrum[population[randomIndex].oligos[index]], oligoSpectrum[population[randomIndex].oligos[index + 1]]);
            if (overlapLeftNeighbour < overlapRightNeighbour)
                std::swap(population[randomIndex].oligos[index - 1], population[randomIndex].oligos[index]);
            else
                std::swap(population[randomIndex].oligos[index], population[randomIndex].oligos[index + 1]);
        }
    }
}

int GeneticISBH::computeFitness(Individual& individual)
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
    return numberOfOligos;
}

std::string GeneticISBH::createStringSolution(const Individual& individual)
{
    std::string solution = oligoSpectrum[individual.oligos[0]]; //first oligo
    for(auto i = 1; i < individual.oligos.size(); ++i)
    {

        auto s1 = oligoSpectrum[individual.oligos[i -1]];
        auto s2 = oligoSpectrum[individual.oligos[i]];
        int overlap = getOverlap(s1, s2);
        //if (solution.size() + s1.substr(overlap).size() > dnaSize) break;
        solution += s1.substr(overlap);
    }
    return solution;
}

std::string GeneticISBH::computeSolution()
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
        individual.fitness = computeFitness(individual);
    }
    // sort descending
    std::sort(population.begin(), population.end(), std::greater<Individual>());
    // select best c = 0.9 * n parents
    int numberOfBestParents = populationRatio * population.size();
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> parentsDist(0, numberOfBestParents);
    std::uniform_int_distribution<int> oligosDist(0, oligoSpectrum.size() - 1);
    // find index of first oligo
    for (auto i = 0; i < oligoSpectrum.size(); ++i)
    {
        if (oligoSpectrum[i] == firstOligo)
        {
            firstOligoIndex = i;
            break;
        }
    }
    //main loop
    for (auto j = 0; j < 50; ++j)
    {
        std::cout << "Genetic Algorithm iter: " << j << " from 50" << std::endl;
        std::vector<Individual> newPopulation;
        // crossover 3.1 && 3.2
        for (auto i = 0; i <= numberOfBestParents; ++i)
        {
            auto firstParent = population[parentsDist(rng)];
            auto secondParent = population[parentsDist(rng)];
            auto oiIndex = firstOligoIndex; // it will be index of current last element, we want to append to him best successor oligo
            // keep info if already in child's solution
            std::vector<bool> alreadyUsed(oligoSpectrum.size(), false);
            Individual child;
            child.oligos.push_back(firstOligoIndex);
            alreadyUsed[firstOligoIndex] = true;
            while (child.oligos.size() != oligoSpectrum.size())
            {
                auto firstParentOligoIter = std::find(firstParent.oligos.begin(), firstParent.oligos.end(), oiIndex);
                auto secondParentOligoIter = std::find(secondParent.oligos.begin(), secondParent.oligos.end(), oiIndex);

                // successor
                //auto firstDis = std::distance(firstParent.oligos.begin(), firstParentOligoIter);
                //auto secondDis = std::distance(secondParent.oligos.begin(), secondParentOligoIter);
                auto overlapSuccFirstParent = getSuccessorOverlap(std::distance(firstParent.oligos.begin(), firstParentOligoIter), firstParent, alreadyUsed);
                auto overlapSuccSecondParent = getSuccessorOverlap(std::distance(secondParent.oligos.begin(), secondParentOligoIter), secondParent, alreadyUsed);

                // if no succcessor available take best one in all range
                if (overlapSuccFirstParent == -1 && overlapSuccSecondParent == -1)
                {
                    auto firstParentPair = findBestSuccessorOverlap(std::distance(firstParent.oligos.begin(), firstParentOligoIter), firstParent, alreadyUsed);
                    auto secondParentPair = findBestSuccessorOverlap(std::distance(secondParent.oligos.begin(), secondParentOligoIter), secondParent, alreadyUsed);
                    std::pair<int, std::vector<int>::iterator> finalPair;
                    if (firstParentPair.first > secondParentPair.first)
                        finalPair = firstParentPair;
                    else finalPair = secondParentPair;
                    //bool flag = false;
                    //if (std::find(child.oligos.begin(), child.oligos.end(), *finalPair.second) != child.oligos.end())
                    //    flag = true;

                    child.oligos.push_back(*finalPair.second);
                    alreadyUsed[*finalPair.second] = true;
                    oiIndex = *finalPair.second;
                }
                else if (overlapSuccFirstParent > overlapSuccSecondParent)
                {

                    //bool flag = false;
                    //if (std::find(child.oligos.begin(), child.oligos.end(), *(firstParentOligoIter + 1)) != child.oligos.end())
                    //    flag = true;
                    oiIndex = *(firstParentOligoIter + 1);
                    child.fitness = computeFitness(child);
                    child.oligos.push_back(oiIndex);

                    alreadyUsed[oiIndex] = true;
                }
                else
                {
                    //bool flag = false;
                    //if (std::find(child.oligos.begin(), child.oligos.end(), *(secondParentOligoIter + 1)) != child.oligos.end())
                    //    flag = true;
                    oiIndex = *(secondParentOligoIter + 1);
                    child.oligos.push_back(oiIndex);
                    alreadyUsed[oiIndex] = true;
                }
            }
            //add child to temporary population
            //child.fitness = computeFitness(child);
            newPopulation.push_back(child);
        }
        //take best parents (already sorted)
        for (auto i = 0; i <= population.size() * (1 - populationRatio); ++i)
        {
            newPopulation.push_back(this->population[i]);
        }
        this->population = newPopulation;
        mutatePopulation(); // mutate current solution
        for(auto& individual: population) //computes fitness for all 
        {
            individual.fitness = computeFitness(individual);
        }
    }
    auto bestSolution = *(std::max_element(population.begin(), population.end(),
        [](const Individual &a, const Individual &b)
        {
            return a.fitness < b.fitness;
        }));
    std::cout << "Solution's fitness: " << bestSolution.fitness << std::endl;
    return createStringSolution(bestSolution);
}

GeneticISBH::GeneticISBH(): dnaSize(-1), firstOligoIndex(-1)
{
}

GeneticISBH::~GeneticISBH()
{
}
