#pragma once

#include <string>
#include <fstream>
#include <iostream>


class DnaSequence
{
private:
    std::string dna;
public:
    void loadDnaFromFile(const std::string& name);
    std::string getDna(int size) { return this->dna.substr(0, size); };
    DnaSequence();
    DnaSequence(const std::string& name);
    virtual ~DnaSequence();
};

