#include "dna_sequence.hpp"



void DnaSequence::loadDnaFromFile(const std::string & name)
{
	std::ifstream infile;
	infile.open(name);
	if (!infile.good())
	{
		std::cout << "File "<< name << "is not open";
	}
	else
	{	
		std::cout << "Loaded";
		/*
		std::string line;
		while (std::getline(infile, line))
		this = text + line + '\n';
		*/
	}
	
}

DnaSequence::DnaSequence()
{
}


DnaSequence::~DnaSequence()
{
}
