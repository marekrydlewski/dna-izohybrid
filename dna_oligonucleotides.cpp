
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
    for (auto &oligo : this->oligoMap)
    {
        if (oligo.second == 0) this->oligoMapStructured[oligo.first] = OligoNumbers::ZERO;
        else if (oligo.second == 1) this->oligoMapStructured[oligo.first] = OligoNumbers::ONE;
        else if (oligo.second == 2 || oligo.second == 3) this->oligoMapStructured[oligo.first] = OligoNumbers::TWO_OR_THREE;
        else if (oligo.second == 4 || oligo.second == 5) this->oligoMapStructured[oligo.first] = OligoNumbers::FOUR_OR_FIVE;
        else this->oligoMapStructured[oligo.first] = OligoNumbers::MANY;
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

std::map<std::string, OligoNumbers> DnaOligonucleotides::getOligoMapStructuredWithNegativeErrors(double degenRatio)
{
    int n = this->oligoMapStructured.size();
    std::vector<std::string> arr;
    std::random_device rd; 
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, n - 1);

    for (auto &oligo : this->oligoMapStructured)
        arr.push_back(oligo.first);

    int m = this->oligoMapStructured.size() * degenRatio;
    for (int i = 0; i < n; i++) {
        int j = uni(rng);
        std::swap(arr[i], arr[j]);
    }
    //first m elements in arr;
    std::vector<std::string> subArr(&arr[0], &arr[m]);
    std::map<std::string, OligoNumbers> degenMap;
    degenMap = this->oligoMapStructured;
    for (auto &oligo : subArr)
        degenMap[oligo]--;
    return degenMap;
}

DnaOligonucleotides::~DnaOligonucleotides()
{
}
