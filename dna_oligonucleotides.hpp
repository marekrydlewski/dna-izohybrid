#pragma once

#include <iostream>
#include <string>
#include <map>
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
    int getNucleotideTemp(char);
public:
    DnaOligonucleotides(std::string dna, int temp);
    std::map<std::string, int> getOligoMap() { return this->oligoMap; };
    virtual ~DnaOligonucleotides();
};

