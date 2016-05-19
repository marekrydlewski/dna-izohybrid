#include "dna_oligonucleotides.hpp"

void DnaOligonucleotides::generateScatteredMap()
{
    std::string oligo;
    int temporaryTemp = 0;
    for (int i = 0; i < dna.length(); i++)
    {
        oligo += dna[i];
        temporaryTemp += getNucleotideTemp(dna[i]);
        if (temporaryTemp == this->temp || temporaryTemp == this->temp2)
            this->oligoMap[oligo]++; // possible coz int is init with 0 
        if (temporaryTemp >= this->temp2)
        {
            i -= oligo.length() - 1;
            temporaryTemp = 0;
            oligo = "";
        }
    }
}

int DnaOligonucleotides::getNucleotideTemp(char str)
{
    if (str == 'a' || str == 'A' || str == 't' || str == 'T')
        return 2;
    else if (str == 'c' || str == 'C' || str == 'g' || str == 'G')
        return 4;
    else
        return 0;
}

DnaOligonucleotides::DnaOligonucleotides(std::string dna, int temp)
{
    this->dna = dna;
    this->temp = temp;
    this->temp2 = temp + this->shift;
    this->generateScatteredMap();
}

DnaOligonucleotides::~DnaOligonucleotides()
{
}
