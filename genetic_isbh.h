#pragma once

#include <vector>
#include <map>
#include "oligo_numbers.hpp"

class GeneticISBH
{
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
private:
    int dnaSize;
    std::vector<std::string> oligoSpectrum;
    std::vector<Individual> population;
    const static double parentsRatio;
    const static double populationRatio;
    static int castOligoNumbersToInt(OligoNumbers x, bool withExcess = true);
public:
    void loadOligoMap(std::map<std::string, OligoNumbers> oligoMap, int size);
    static int getOverlap(std::string s, std::string s2);
    int getPredecessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed);
    int getSuccessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed);
    std::pair<int, std::vector<int>::iterator> findBestPredecessorOverlap(int index, Individual & individual, std::vector<bool>& alreadyUsed);
    std::pair<int, std::vector<int>::iterator> findBestSuccessorOverlap(int index, Individual & individual, std::vector<bool>& alreadyUsed);
    void computeSolution();
    GeneticISBH();
    ~GeneticISBH();
};

