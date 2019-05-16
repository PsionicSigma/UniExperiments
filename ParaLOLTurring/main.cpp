//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>

using namespace std;

const int running = 1;
const int stopped = 0;

struct action
{
	int currentphase;
	char currentsymbol;
	char nextsymbol;
	char nextposition;
	int nextphase;
};

class turma
{
private:
	string line, stock;
	vector<action> machine;
	int position, start, phase, index, status;
public:
	//konstruktorius
	turma(string file)
	{
		ifstream df(file);
		action process;
		df >> position;
		position--;
		start = position;
		df >> line;
		stock = line;
		string tempnextphase;
		while (!df.eof())
		{
			df >> process.currentphase >> process.currentsymbol >> process.nextsymbol >> process.nextposition >> tempnextphase;
			if (tempnextphase[0] == 'X') process.nextphase = -1;
			else process.nextphase = stoi(tempnextphase);
			machine.push_back(process);
		}
		status = 1;
		df.close();
	}
	//update'as
	void updatemachine()
	{
		string changesymbol;
		while((machine[index].currentphase != phase) || (machine[index].currentsymbol != line[position])) index++;
		changesymbol = machine[index].nextsymbol;
    line.replace(position, 1, changesymbol);
    if (machine[index].nextposition == 'R') position++;
    else if (machine[index].nextposition == 'L') position--;
    phase = machine[index].nextphase;
    index = 0;

		/*while(index<machine.size())
		{
		  if ((machine[index].currentphase == phase) && (machine[index].currentsymbol == line[position]))
      {
        changesymbol = machine[index].nextsymbol;
        line.replace(position, 1, changesymbol);
        if (machine[index].nextposition == 'R') position++;
        else if (machine[index].nextposition == 'L') position--;
        phase = machine[index].nextphase;
        index = 0;
      }
      index++;
		}*/
		if ((position<0) || (position >= line.size())|| index>=machine.size()) status = 0;
	}
	//reset
	void reset()
	{
		position = start;
		line = stock;
		index = 0;
		status = 0;
	}
	//print'inam
	void PrintLine()
	{
		for (int j = 0; j<line.size(); j++) cout << line[j];
		cout << endl;
		for (int j = 0; j<line.size(); j++)
		{
			if (j != position) cout << " ";
			else cout << "^";
		};
		cout << endl;
	}
	//get status
	int getstatus() const
	{
		return status;
	}
	//set status
	void setstatus(int s)
	{
		status = s;
	}
};

void ClearScreen()
{
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellcount;
	COORD homeCoords = { 0,0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
	cellcount = csbi.dwSize.X*csbi.dwSize.Y;
	FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellcount, homeCoords, &count);
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

int main()
{
	turma Turring1("1.txt");
	turma Turring2("2.txt");
	turma Turring3("3.txt");
	turma Turring4("4.txt");
	cout << "Controls:" << endl;
	cout << "q/w/e/r to togle run/reset of 1/2/3/4 turrings" << endl;
  cout << "SPACE to stop entire program" << endl << endl;
	Turring1.PrintLine();
	if (Turring1.getstatus() == 1) cout << "Turring running" << endl;
	else cout << "Turring not running" << endl;
	Turring2.PrintLine();
	if (Turring2.getstatus() == 1) cout << "Turring running" << endl;
	else cout << "Turring not running" << endl;
	Turring3.PrintLine();
	if (Turring3.getstatus() == 1) cout << "Turring running" << endl;
	else cout << "Turring not running" << endl;
	Turring4.PrintLine();
	if (Turring4.getstatus() == 1) cout << "Turring running" << endl;
	else cout << "Turring not running" << endl;
	while (true)
	{
		ClearScreen();
		//update
		if (Turring1.getstatus() == running) Turring1.updatemachine();
		if (Turring2.getstatus() == running) Turring2.updatemachine();
		if (Turring3.getstatus() == running) Turring3.updatemachine();
		if (Turring4.getstatus() == running) Turring4.updatemachine();
		//toggle run/restart/stop
		if (GetKeyState('Q')) { if (Turring1.getstatus() == running) Turring1.reset(); else if (Turring1.getstatus() == stopped) Turring1.setstatus(running); };
		//113
		if (GetKeyState('W')) { if (Turring2.getstatus() == running) Turring2.reset(); else if (Turring2.getstatus() == stopped) Turring2.setstatus(running); };
		//119
		if (GetKeyState('E')) { if (Turring3.getstatus() == running) Turring3.reset(); else if (Turring3.getstatus() == stopped) Turring3.setstatus(running); };
		//101
		if (GetKeyState('R')) { if (Turring4.getstatus() == running) Turring4.reset(); else if (Turring4.getstatus() == stopped) Turring4.setstatus(running); };
		//114
		cout << "Controls:" << endl;
		cout << "q/w/e/r to togle run/reset of 1/2/3/4 turrings" << endl;
		cout << "SPACE to stop entire program" << endl << endl;
		Turring1.PrintLine();
		if (Turring1.getstatus() == 1) cout << "Turring1 running" << endl << endl;
		else cout << "Turring1 not running" << endl << endl;
		Turring2.PrintLine();
		if (Turring2.getstatus() == 1) cout << "Turring2 running" << endl << endl;
		else cout << "Turring2 not running" << endl << endl;
		Turring3.PrintLine();
		if (Turring3.getstatus() == 1) cout << "Turring3 running" << endl << endl;
		else cout << "Turring3 not running" << endl << endl;
		Turring4.PrintLine();
		if (Turring4.getstatus() == 1) cout << "Turring4 running" << endl << endl;
		else cout << "Turring4 not running" << endl << endl;
		if (GetAsyncKeyState(VK_SPACE)) break;
		Sleep(50);
	}
	cout << "Programa issijunge" << endl;
	Sleep(3000);
	return 0;
}



