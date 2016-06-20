#include <iostream>
#include <numeric>
#include <chrono>

#include "graph.hpp"
#include "dna_sequence.hpp"
#include "dna_oligonucleotides.hpp"
#include "genetic_isbh.hpp"

const int K_MER_LENGTH = 5;
const int DNA_LENGTH = 400;
const int DNA_TEMPERATURE = 30;
const double NEGATIVE_ERRORS_RATIO = 0.01;

int levenshtein_distance(const std::string& s1, const std::string& s2)
{
    // To change the type this function manipulates and returns, change
    // the return type and the types of the two variables below.
    int s1len = s1.size();
    int s2len = s2.size();

    auto column_start = (decltype(s1len))1;

    auto column = new decltype(s1len)[s1len + 1];
    std::iota(column + column_start, column + s1len + 1, column_start);

    for (auto x = column_start; x <= s2len; x++)
    {
        column[0] = x;
        auto last_diagonal = x - column_start;
        for (auto y = column_start; y <= s1len; y++)
        {
            auto old_diagonal = column[y];
            auto possibilities = {
                column[y] + 1,
                column[y - 1] + 1,
                last_diagonal + (s1[y - 1] == s2[x - 1] ? 0 : 1)
            };
            column[y] = std::min(possibilities);
            last_diagonal = old_diagonal;
        }
    }
    auto result = column[s1len];
    delete[] column;
    return result;
}

int main(int argc, const char* argv[])
{
    std::vector<double> errors_ratio = {0.00, 0.05, 0.1, 0.15, 0.20, 0.30};
    std::vector<int> dna_length = {100, 200, 400, 600, 800, 1200, 1600};
    std::vector<int> dna_temp = {20, 30, 40, 50};
    std::ofstream outfile;
    outfile.open("test.csv", std::ios::app);
    for (auto& length : dna_length)
    {
        for(auto& temp: dna_temp)
        {
            for(auto& error : errors_ratio)
            {
                
            }
        }
    }
    auto dna_loader = new DnaSequence("dna-data.txt");
    auto dna_scattered = new DnaOligonucleotides(dna_loader->getDna(DNA_LENGTH), DNA_TEMPERATURE);
    auto oligo = dna_scattered->getFirstOligo();
    std::cout << "The first oligo is: " << oligo << std::endl;
    auto oligo_map = dna_scattered->getOligoMap();
    for (auto& t : oligo_map)
        std::cout << t.first << " " << t.second << std::endl;
    auto degen_oligo_map = dna_scattered->getOligoMapStructuredWithNegativeErrors(NEGATIVE_ERRORS_RATIO);
    auto genetic_isbh = new GeneticISBH();
    genetic_isbh->loadOligoMap(degen_oligo_map, DNA_LENGTH);
    genetic_isbh->loadFirstOligo(oligo);

    auto start = std::chrono::system_clock::now();
    auto solution = genetic_isbh->computeSolution();
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();;
    std::cout << "Elapsed time of computing solution: " << elapsed << std::endl;

    auto leven = levenshtein_distance(dna_loader->getDna(DNA_LENGTH), solution);
    //std::cout << dna_loader->getDna(DNA_LENGTH) << std::endl;
    //std::cout << solution << std::endl;
    std::cout << "Levensthein distance: " << leven << std::endl;
    std::cout << "Levensthein distance in %: " << (2 * leven / double(DNA_LENGTH + solution.size())) * 100.0 << std::endl;

    Graph* graph = new Graph(*dna_scattered, false);
    auto path = graph->getPath();
    for (auto item : path)
        printf("%s,", item.c_str());
    return 0;
}
