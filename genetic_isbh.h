#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "oligo_numbers.hpp"


class GeneticISBH
{
private:
    std::vector<std::string> oligoSpectrum;
    std::vector<std::vector<int>> population;
    const static double populationRatio;
    int castOligoNumbersToInt(OligoNumbers x, bool withExcess = true);
public:
    void loadOligoMap(std::map<std::string, OligoNumbers> oligoMap);
    void computeSolution();
    GeneticISBH();
    ~GeneticISBH();
};

