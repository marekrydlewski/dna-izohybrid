#pragma once

#include <string>>
#include <fstream>
#include <iostream>

class DnaSequence
{
private:
	std::string dna;
public:
	void loadDnaFromFile(const std::string& name);
	DnaSequence();
	virtual ~DnaSequence();
};

