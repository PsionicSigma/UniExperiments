#include "pch.h"
#include "VirtualMachine.h"

Virtual_Machine::Virtual_Machine()
{
	ProgramCounter = 0;
	ZFlag = false;
	EFlag = false;
	for (int i = 0; i < 16; i++) Register[i] = 0;
	for (int i = 0; i < 256; i++) Memory[i] = 0;
	std::ifstream PF("decryptor.bin", std::ios::binary);
	if (!PF.good())	std::cout << "Bad or no Program file D:\n";
	else for (int i = 0; !PF.eof(); i++) Memory[i] = PF.get();
	PF.close();
	Run();
}

void Virtual_Machine::Run()
{
	bool run = true;
	std::cout << "\nVM started\n\n";
	std::ifstream DF("q1_encr.txt");
	if (!DF.good())
	{
		std::cout << "Bad or no Material file\n";
		run = false;
	}
	while (run)
	{
		RegX = Memory[ProgramCounter + 1] & 0x0f;
		RegY = (Memory[ProgramCounter + 1] & 0xf0)>>4;
		switch (Memory[ProgramCounter])
		{
		case 0x01:Register[RegX]++; break;
		case 0x02:Register[RegX]--; break;
		case 0x03:Register[RegX] = Register[RegY]; break;
		case 0x04:Register[0] = Memory[ProgramCounter+1]; break;
		case 0x05:Register[RegX] <<= 1; break;
		case 0x06:Register[RegX] >>= 1; break;
		case 0x07:if(Memory[ProgramCounter+1] < 127)ProgramCounter = (ProgramCounter + Memory[ProgramCounter + 1]) - 2; else ProgramCounter = (ProgramCounter + Memory[ProgramCounter + 1]) - 258; break;
		case 0x08:if (ZFlag) if (Memory[ProgramCounter + 1] < 127)ProgramCounter = (ProgramCounter + Memory[ProgramCounter + 1]) - 2; else ProgramCounter = (ProgramCounter + Memory[ProgramCounter + 1]) - 258; break;
		case 0x09:if (!ZFlag) if (Memory[ProgramCounter + 1] < 127)ProgramCounter = (ProgramCounter + Memory[ProgramCounter + 1]) - 2; else ProgramCounter = (ProgramCounter + Memory[ProgramCounter + 1]) - 258; break;
		case 0x0a:if(EFlag) if (Memory[ProgramCounter + 1] < 127)ProgramCounter = (ProgramCounter + Memory[ProgramCounter + 1]) - 2; else ProgramCounter = (ProgramCounter + Memory[ProgramCounter + 1]) - 258; break;
		case 0x0b:run = false; break;
		case 0x0c:Register[RegX] += Register[RegY]; break;
		case 0x0d:Register[RegX] -= Register[RegY]; break;
		case 0x0e:Register[RegX] ^= Register[RegY]; break;
		case 0x0f:Register[RegX] |= Register[RegY]; break;
		case 0x10:if (!DF.eof()) { Register[RegX] = DF.get();} else EFlag = true; break;
		case 0x11:std::cout << (char)Register[RegX]; break;
		}
		ProgramCounter += 2;
	}
	std::cout << "\n\nVM is done";
}


