#pragma once

#include <vector>
#include <map>
#include "oligo_numbers.hpp"

class GeneticISBH
{
private:
    int dnaSize;
    std::vector<std::string> oligoSpectrum;
    std::vector<std::vector<int>> population;
    const static double populationRatio;
    static int castOligoNumbersToInt(OligoNumbers x, bool withExcess = true);
public:
    void loadOligoMap(std::map<std::string, OligoNumbers> oligoMap, int size);
    static int getOverlap(std::string s, std::string s2);
    void computeSolution();
    GeneticISBH();
    ~GeneticISBH();
};

