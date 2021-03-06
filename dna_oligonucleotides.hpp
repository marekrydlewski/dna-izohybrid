#pragma once

#include <map>
#include <random>
#include "oligo_numbers.hpp"

class DnaOligonucleotides
{
private:
    const static int shift{ 2 };
    const static int atTemp{ 2 };
    const static int cgTemp{ 4 };
    std::string dna;
    std::map<std::string, int> oligoMap;
    std::map<std::string, OligoNumbers> oligoMapStructured;
    int temp;
    int temp2;
    void generateScatteredMap();
    static int getNucleotideTemp(char);
public:
    DnaOligonucleotides(std::string dna, int temp);
    std::map<std::string, int> getOligoMap() { return this->oligoMap; }

    std::string getFirstOligo();;
    std::map<std::string, OligoNumbers> getOligoMapStructuredWithNegativeErrors(double = 0.0);
    virtual ~DnaOligonucleotides();
};

