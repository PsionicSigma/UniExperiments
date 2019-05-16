#include "pch.h"					//VisualStudio 2017
//#include "stdafx.h"				//VisualStudio 2015?
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <string>
#include <iomanip>
#include <conio.h>
#include <fstream>

using namespace std;
//simbolio struktura
struct acharacter
{
	int intas;
	int desimtainis;
	string UniUTF8;
};
//verciam simbolio int i utf
string inttoutf(int base)
{
	int temp1, temp2;
	string utf, bin, hexutf;
	//int i bin
	temp1 = base;
	if (temp1 == 0)bin == "00000000";
	else
	{
		while (temp1 > 0)
		{
			bin += temp1 % 2 + '0';
			temp1 /= 2;
		}
		while (bin.size() % 8 > 0) bin += "0";
		reverse(bin.begin(), bin.end());
	}
	//bin i utf
	if (base >= 0 && base <= 127)
	{
		utf = "0.......";
		int k = bin.size() - 1;
		for (int i = utf.size() - 1; i >= 0; i--)if (utf[i] == '.') { if (k >= 0) { utf[i] = bin[k]; k--; } else utf[i] = '0'; }
	}
	else if (base >= 128 && base <= 2047)
	{
		utf = "110.....10......";
		int k = bin.size() - 1;
		for (int i = utf.size() - 1; i >= 0; i--)if (utf[i] == '.') { if (k >= 0) { utf[i] = bin[k]; k--; } else utf[i] = '0'; }
	}
	else if (base >= 2048 && base <= 65535)
	{
		utf = "1110....10......10......";
		int k = bin.size() - 1;
		for (int i = utf.size() - 1; i >= 0; i--)if (utf[i] == '.') { if (k >= 0) { utf[i] = bin[k]; k--; } else utf[i] = '0'; }
	}
	else
	{
		utf = "11110...10......10......10......";
		int k = bin.size() - 1;
		for (int i = utf.size() - 1; i >= 0; i--)if (utf[i] == '.') { if (k >= 0) { utf[i] = bin[k]; k--; } else utf[i] = '0'; }
	}
	//utf i hexutf
	for (int i = 0; i <= utf.size() - 4; i += 4)
	{
		temp2 = (utf[i] - 48) * 8 + (utf[i + 1] - 48) * 4 + (utf[i + 2] - 48) * 2 + (utf[i + 3] - 48);
		if (temp2 == 0)hexutf += "0";
		else if (temp2 > 0 && temp2 < 10) hexutf += (temp2 + 48);
		else hexutf += temp2 + 55;
	}
	return hexutf;
}
//verciam simbolio utf i nauja inta
int utftoint(string utf)
{
	int dec = 0;
	for (int i = 0; i < utf.size(); i++)
	{
		dec *= 16;
		if (utf[i] - 48 > 9) dec += utf[i] - 55;
		else dec += utf[i] - 48;
	}
	return dec;
}
//bendras vertimas
void conv(acharacter &achar)
{
	int base;
	base = achar.intas;
	//intas i uniutf
	achar.UniUTF8 = inttoutf(base);
	//uniutf i nauja inta
	achar.desimtainis = utftoint(achar.UniUTF8);
}
//ekrano valymas
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
//vykdomas a skyrius
void runA()
{
	int x;
	acharacter acharA;
	ClearScreen();
	while (true)
	{
		cout << "Iveskite int reiksme: \n";
		cin >> x;
		if (x >= 0) { acharA.intas = x; break; }
	}
	conv(acharA);
	cout << "Int: " << setw(10) << acharA.intas << ", UTF-8: " << setw(16) << acharA.UniUTF8  << " " << acharA.desimtainis << endl;
	cout << "SPACE - iseiti is skyriaus \n";
	while (true)if (_getch() == ' ')break;
}
//vykdomas b skyrius
void runB()
{
	int k, i, oldint[128], newint[128], char1, char2, char3, char4, valueofoldchar;
	unsigned char oldchar;
	string abyte;
	acharacter acharB;
	//nuskaitome dec makeitimo lentele
	ifstream d2f("473_to_Unicode.txt");
	i = 0;
	while (!d2f.eof()) { d2f >> oldint[i] >> newint[i];	i++; }
	d2f.close();
	//pradedam konvertuoti faila
	ifstream d1f("386intel.txt");
	ofstream rf("converted.txt");
	cout << " Konvertuojama\n";
	while (!d1f.eof())
	{
		// paimam simboli
		oldchar = d1f.get();
		valueofoldchar = (int)oldchar;
		//atitinkamai isvedam simboli
		if (valueofoldchar >=0 && valueofoldchar <= 127)rf << oldchar;
		else
		{
			k = 0;
			while (oldint[k] != valueofoldchar) k++;
			acharB.intas = newint[k];
			conv(acharB);
			if (acharB.UniUTF8.size() == 2)
			{
				char1 = utftoint(acharB.UniUTF8);
				rf << (char)char1;
			}
			else if (acharB.UniUTF8.size() == 4)
			{
				abyte = acharB.UniUTF8[0];
				abyte += acharB.UniUTF8[1];
				char1 = utftoint(abyte);
				abyte = acharB.UniUTF8[2];
				abyte += acharB.UniUTF8[3];
				char2 = utftoint(abyte);
				rf << (char)char1 << (char)char2;
			}
			else if (acharB.UniUTF8.size() == 6)
			{
				abyte = acharB.UniUTF8[0];
				abyte += acharB.UniUTF8[1];
				char1 = utftoint(abyte);
				abyte = acharB.UniUTF8[2];
				abyte += acharB.UniUTF8[3];
				char2 = utftoint(abyte);
				abyte = acharB.UniUTF8[4];
				abyte += acharB.UniUTF8[5];
				char3 = utftoint(abyte);
				rf << (char)char1 << (char)char2 << (char)char3;
			}
			else if (acharB.UniUTF8.size() == 8)
			{
				abyte = acharB.UniUTF8[0];
				abyte += acharB.UniUTF8[1];
				char1 = utftoint(abyte);
				abyte = acharB.UniUTF8[2]; 
				abyte += acharB.UniUTF8[3];
				char2 = utftoint(abyte);
				abyte = acharB.UniUTF8[4];
				abyte += acharB.UniUTF8[5];
				char3 = utftoint(abyte);
				abyte = acharB.UniUTF8[6];
				abyte += acharB.UniUTF8[7];
				char4 = utftoint(abyte);
				rf << (char)char1 << (char)char2 << (char)char3 << (char)char4;
			}
		}
	}
	d1f.close();
	rf.close();
	cout << " Baigta konvertuoti\n";
	while (true) if (_getch() == ' ')break;
}

int main()
{
	while (true)
	{
		ClearScreen();
		cout << " Pasirinkite: \n A - versti int i UTF-8 \n B - konvertuoti faila \n SPACE - iseiti is programos \n";
		if (_getch() == 'a' || _getch() == 'A') runA();
		else if (_getch() == 'b' || _getch() == 'B') runB();
		else if (_getch() == ' ') break;
		cout << "???\n";
		Sleep(50);
	}
	ClearScreen();
	cout << " Programa baige darba";
	Sleep(5000);
	return 0;
}

