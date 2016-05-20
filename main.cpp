#include <iostream>

#include "graph.hpp"
#include "dna_sequence.hpp"
#include "dna_oligonucleotides.hpp"

const int K_MER_LENGTH = 5;
const int DNA_LENGTH = 100;
const int DNA_TEMPERATURE = 8;


int main(int argc, const char * argv[]) {

    auto dna_loader = new DnaSequence("dna-data.txt");
    auto dna_scattered = new DnaOligonucleotides(dna_loader->getDna(DNA_LENGTH), DNA_TEMPERATURE);
    auto oligo_map = dna_scattered->getOligoMap();
    for (auto &t : oligo_map)
        std::cout << t.first << " " << t.second << std::endl;

    Graph* graph = new Graph("a_long_long_long_time", K_MER_LENGTH);
    auto path = graph->eulerianPath();
    for (auto &item : path)
        printf("%s,", item->value().c_str());
    // output should be:
    // ['a_lo', '_lon', 'long', 'ong_', 'ng_l', 'g_lo', '_lon', 'long', 'ong_', 'ng_l',
    // 'g_lo', '_lon', 'long', 'ong_', 'ng_t', 'g_ti', '_tim', 'time']
    return 0;
}
