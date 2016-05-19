#pragma once

#include <iostream>
#include <string>

class DnaOligonucleotides
{
private:
    const static int shift{ 2 };
    std::string dna;
    std::map<std::string, int> oligoMap;
    int temp;
    int temp2;
public:
    DnaOligonucleotides(std::string dna, int temp);
    virtual ~DnaOligonucleotides();
};

