#pragma once

#include <iomanip>

using namespace std;

class MemoryProcessor {
private:
	FILE *i_fp;
	FILE *o_fp;
	long length;
	uint8_t *buffer;
public:
	MemoryProcessor();
	~MemoryProcessor();
	void readFile(string);
	void writeFile(string);
};
