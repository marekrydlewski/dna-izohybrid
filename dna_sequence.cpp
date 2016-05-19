#include "dna_sequence.hpp"



void DnaSequence::loadDnaFromFile(const std::string & name)
{
    std::ifstream infile;
    infile.open(name);
    if (!infile.good())
    {
        std::cout << "File " << name << "is not open";
    }
    else
    {
        std::string line;
        while (infile >> line)
            this->dna += line;
    }

}

DnaSequence::DnaSequence()
{
}

DnaSequence::DnaSequence(const std::string & name)
{
    this->loadDnaFromFile(name);
}


DnaSequence::~DnaSequence()
{
}
