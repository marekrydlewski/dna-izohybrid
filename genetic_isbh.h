#pragma once

#include <vector>
#include <map>
#include "oligo_numbers.hpp"
#include "individual.hpp"

class GeneticISBH
{
private:
    int dnaSize;
    int firstOligoIndex;
    std::vector<std::string> oligoSpectrum;
    std::vector<Individual> population;
    std::string firstOligo;
    const static double parentsRatio;
    const static double populationRatio;
    static int castOligoNumbersToInt(OligoNumbers x, bool withExcess = true);
public:
    void loadOligoMap(std::map<std::string, OligoNumbers> oligoMap, int size);
    void loadFirstOligo(const std::string& cs);
    static int getOverlap(std::string s, std::string s2);
    int getPredecessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed);
    int getSuccessorOverlap(int index, Individual& individual, std::vector<bool>& alreadyUsed);
    std::pair<int, std::vector<int>::iterator> findBestPredecessorOverlap(int index, Individual & individual, std::vector<bool>& alreadyUsed);
    std::pair<int, std::vector<int>::iterator> findBestSuccessorOverlap(int index, Individual & individual, std::vector<bool> alreadyUsed);
    void mutatePopulation();
    int computeFitness(Individual& child);
    std::string createStringSolution(const Individual& individual);
    std::string computeSolution();
    GeneticISBH();
    ~GeneticISBH();
};