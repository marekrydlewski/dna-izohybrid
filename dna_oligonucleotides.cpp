#include "dna_oligonucleotides.hpp"


DnaOligonucleotides::DnaOligonucleotides(std::string dna, int temp)
{
    this->dna = dna;
    this->temp = temp;
    this->temp2 = temp + this->shift;
}

DnaOligonucleotides::~DnaOligonucleotides()
{
}
