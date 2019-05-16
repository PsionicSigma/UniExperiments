//#pragma once
#ifndef MH
#define MH
#include <iostream>
#include <fstream>
#include <bitset>

class Virtual_Machine
{
private:
	int ProgramCounter, Register[16], RegX, RegY, Memory[256];
	bool ZFlag, EFlag;
public:
	Virtual_Machine();
	void Run();
};


#endif