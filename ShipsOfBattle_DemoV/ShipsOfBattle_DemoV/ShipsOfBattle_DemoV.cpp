//for debug demo, comment viewfinder.DrawBoard() and uncomment PC.DrawBoard()

#include "pch.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <windows.h>
#include <string>

/*
reikia:
1. padaryti smarter_bot+;
2. reikia prideti <windows.h> ir padaryti cls;
*/

using namespace std;

//lentos langeliu busenos
const char water = '~';        //water
const char missed = 'X';       //hit water
const char est_occupado = 'O'; //occupied by ship
const char hit = '8';          //hit ship
const char dead = ':';         //DED ship

//boto smegenys
const int randcord = 0;
const int newhit = 1;
const int linehit = 2;
const int reline = 3;

struct shipposs
{
	int xbegin, ybegin, xend, yend;
};

struct lastshots
{
	int x, y, checklastshot, linedir;
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

class GameBoardOf
{
private:
	char Board[12][12];
	int shipsleft;
	shipposs ships[10];
	lastshots previouscords;
public:
	//darom lenta
	GameBoardOf()
	{
		Board[0][0] = '+';
		char l = 'A';
		for (int j = 1; j < 11; j++) { Board[0][j] = l; l++; }
		for (int i = 1; i < 10; i++) Board[i][0] = i + '0';
		Board[10][0] = '0';
		for (int i = 1; i < 11; i++) for (int j = 1; j < 11; j++) Board[i][j] = water;
		shipsleft = 0;
		previouscords.checklastshot = randcord;
	}
	//piesiam lenta
	void DrawBoard()
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD oldatts;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(h, &csbi);
		oldatts = csbi.wAttributes;
		Board[0][0] = '+';
		char l = 'A';
		for (int j = 1; j < 11; j++) { Board[0][j] = l; l++; }
		for (int i = 1; i < 10; i++) Board[i][0] = i + '0';
		Board[10][0] = '0';
		for (int i = 0; i < 11; i++) for (int j = 0; j < 11; j++)
		{
			if (i == 0)
			{
				if (j % 2 == 0) { SetConsoleTextAttribute(h, 224); cout << " " << Board[i][j] << " "; }
				else { SetConsoleTextAttribute(h, 96); cout << " " << Board[i][j] << " "; }
			}
			else if (j == 0)
			{
				if (i % 2 == 0) { SetConsoleTextAttribute(h, 224); cout << " " << Board[i][j] << " "; }
				else { SetConsoleTextAttribute(h, 96); cout << " " << Board[i][j] << " "; }
			}
			else
			{
				switch (Board[i][j])
				{
				case water: SetConsoleTextAttribute(h, 159); cout << " " << Board[i][j] << " "; break;
				case missed: SetConsoleTextAttribute(h, 159); cout << " " << Board[i][j] << " "; break;
				case est_occupado: SetConsoleTextAttribute(h, 128); cout << " " << Board[i][j] << " "; break;
				case hit: SetConsoleTextAttribute(h, 140); cout << " " << Board[i][j] << " "; break;
				case dead: SetConsoleTextAttribute(h, 12); cout << " " << Board[i][j] << " "; break;
				}
			}
			SetConsoleTextAttribute(h, oldatts);
			//cout << Board[i][j] << " ";
			if (j == 10) cout << endl;
		}
		cout << endl;
	}
	//tikrinam, ar laisvas plotas laivui padeti
	bool IsSpaceFree(int size, char x, int y, char dir)
	{
		bool plot = true;
		int tx1, tx2;
		int ty1, ty2;
		ty1 = y;
		tx1 = x - 64;
		if (dir == 'H' || dir == 'h')
		{
			if (tx1 + size - 1 > 10) { tx2 = tx1; tx1 = tx1 - size + 1; }
			else { tx2 = tx1 + size - 1; }
			ty2 = ty1;
		}
		else if (dir == 'V' || dir == 'v')
		{
			if (ty1 + size - 1 > 10) { ty2 = ty1; ty1 = ty1 - size + 1; }
			else { ty2 = ty1 + size - 1; }
			tx2 = tx1;
		}
		for (int i = ty1 - 1; i <= ty2 + 1; i++) for (int j = tx1 - 1; j <= tx2 + 1; j++) if (Board[i][j] == est_occupado) plot = false;
		if (plot == true)
		{
			for (int i = ty1; i <= ty2; i++) for (int j = tx1; j <= tx2; j++) Board[i][j] = est_occupado;
			ships[shipsleft].xbegin = tx1;
			ships[shipsleft].xend = tx2;
			ships[shipsleft].ybegin = ty1;
			ships[shipsleft].yend = ty2;
		}
		return plot;
	}
	//atsitiktinai dedam laivus
	void PlaceShipAtRandom()
	{
		int y, temp;
		char x;
		char dir;
		for (int i = 4; i >= 1; i--)
		{
			for (int j = i; j <= 4; j++)
			{
				do {
					x = rand() % 10 + 65;
					y = rand() % 10 + 1;
					temp = rand() % 100 + 1;
					if (temp >= 50) dir = 'H';
					else dir = 'V';
				} while (IsSpaceFree(i, x, y, dir) == false);
				shipsleft++;
			};
		};
	}
	//rankiniu budu dedam laivus
	void PlaceShipManually()
	{
		string crd;
		int y;
		char x;
		char dir;
		for (int i = 4; i >= 1; i--)
		{
			for (int j = 1; j <= 5 - i; j++)
			{
				cout << "Choose start coordinates for size " << i << " ship No." << j << " (X,Y): ";
				do {
					cin >> crd;
					if (crd[0] - 48 > 9)
					{
						x = crd[0];
						if (crd[1] == '0') y = 10;
						else y = crd[1] - 48;
					}
					else
					{
						x = crd[1];
						if (crd[0] == '0') y = 10;
						else y = crd[0] - 48;
					}
					cout << "Choose direction of ship placement (H-horisontally/V-vertically):";
					cin >> dir;
				} while (IsSpaceFree(i, x, y, dir) == false);
				shipsleft++;
				ClearScreen();
				cout << "Ye board. Ships left: " << getshipsleft() << endl;
				DrawBoard();
			};
		};
	}
	//friend saudymas ir naikinimas
	friend void shot(GameBoardOf&, GameBoardOf&);
	friend void shot(GameBoardOf&);
	friend void search(GameBoardOf&, GameBoardOf&, int, int);
	friend void search(GameBoardOf&, int, int);
	friend void botisbraining(GameBoardOf&, int&, int&, int&);
	//kiek liko laivu
	int getshipsleft()
	{
		return shipsleft;
	}
};
//zaidejas sauna i kompiuteri
void shot(GameBoardOf &viewfinder, GameBoardOf &pc)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD oldatts;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(h, &csbi);
	oldatts = csbi.wAttributes;
	string crd;
	int cordX, cordY;
	bool correctcoords = false;
	do {
		cout << "Choose shot coordinates [x,y]: ";
		cin >> crd;
		if (crd[0] - 48 > 9)
		{
			cordX = crd[0] - 64;
			if (crd[1] == '0') cordY = 10;
			else cordY = crd[1] - 48;
		}
		else
		{
			cordX = crd[1] - 64;
			if (crd[0] == '0') cordY = 10;
			else cordY = crd[0] - 48;
		}
		if ((cordX > 10) || (cordX < 1)) correctcoords = false;
		else if ((cordY > 10) || (cordY < 1)) correctcoords = false;
		else if ((pc.Board[cordY][cordX] == missed) || (pc.Board[cordY][cordX] == hit) || (pc.Board[cordY][cordX] == dead)) correctcoords = false;
		else correctcoords = true;
	} while (correctcoords == false);
	switch (pc.Board[cordY][cordX])
	{
	case water:
		pc.Board[cordY][cordX] = missed;
		viewfinder.Board[cordY][cordX] = missed;
		if (cordY == 10) { SetConsoleTextAttribute(h, 12); cout << "Ya hit nuthin at " << char(cordX + 64) << "0" << endl; }
		else { SetConsoleTextAttribute(h, 12); cout << "Ya hit nuthin at " << char(cordX + 64) << cordY << endl; }
		SetConsoleTextAttribute(h, oldatts);
		break;
	case est_occupado:
		pc.Board[cordY][cordX] = hit;
		viewfinder.Board[cordY][cordX] = hit;
		if (cordY == 10) { SetConsoleTextAttribute(h, 10); cout << "Ya hit sumthin at " << char(cordX + 64) << "0" << endl; }
		else { SetConsoleTextAttribute(h, 10); cout << "Ya hit sumthin at " << char(cordX + 64) << cordY << endl; }
		SetConsoleTextAttribute(h, oldatts);
		search(pc, viewfinder, cordX, cordY);
		break;
		Sleep(1500);
	}
}
//kompiuteris sauna i zaideja
void shot(GameBoardOf &player)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD oldatts;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(h, &csbi);
	oldatts = csbi.wAttributes;
	char tempcordX;
	int cordX, cordY;
	bool correctcoords = false;
	cout << "PC is choosing shot coordinates" << endl;
	do {
		botisbraining(player, cordX, cordY, player.previouscords.linedir);
		if ((cordX > 10) || (cordX < 1)) correctcoords = false;
		else if ((cordY > 10) || (cordY < 1)) correctcoords = false;
		else if ((player.Board[cordY][cordX] == missed) || (player.Board[cordY][cordX] == hit) || (player.Board[cordY][cordX] == dead)) correctcoords = false;
		else correctcoords = true;
	} while (correctcoords == false);
	switch (player.Board[cordY][cordX])
	{
	case water:
		player.Board[cordY][cordX] = missed;
		if (player.previouscords.checklastshot == linehit) player.previouscords.checklastshot = reline;
		if (cordY == 10) { SetConsoleTextAttribute(h, 10); cout << "PC missed ya at " << char(cordX + 64) << "0" << endl; }
		else { SetConsoleTextAttribute(h, 10); cout << "PC missed ya at " << char(cordX + 64) << cordY << endl; }
		SetConsoleTextAttribute(h, oldatts);
		break;
	case est_occupado:
		player.Board[cordY][cordX] = hit;
		switch (player.previouscords.checklastshot)
		{
		case randcord:player.previouscords.checklastshot = newhit; break;
		case newhit:player.previouscords.checklastshot = linehit; break;
		case reline:player.previouscords.checklastshot = linehit; break;
		}
		player.previouscords.x = cordX; player.previouscords.y = cordY;
		if (cordY == 10) { SetConsoleTextAttribute(h, 12); cout << "PC hit one of ya shipz at " << char(cordX + 64) << "0" << endl; }
		else { SetConsoleTextAttribute(h,12); cout << "PC hit one of ya shipz at " << char(cordX + 64) << cordY << endl; }
		SetConsoleTextAttribute(h, oldatts);
		search(player, cordX, cordY);
		break;
		Sleep(5000);
	}
}
//botas protauja
void botisbraining(GameBoardOf &player, int &cordX, int &cordY, int &linedir)
{
	switch (player.previouscords.checklastshot)
	{
	case randcord:
		cordX = rand() % 10 + 1;
		cordY = rand() % 10 + 1;
		break;
	case newhit:
		linedir = rand() % 4;
		cordX = player.previouscords.x;
		cordY = player.previouscords.y;
		switch (linedir)
		{
		case 0:cordX = player.previouscords.x + 1; break;
		case 1:cordY = player.previouscords.y + 1; break;
		case 2:cordX = player.previouscords.x - 1; break;
		case 3:cordY = player.previouscords.y - 1; break;
		}
		break;
	case linehit:
		cordX = player.previouscords.x;
		cordY = player.previouscords.y;
		switch (linedir)
		{
		case 0:cordX = player.previouscords.x + 1; if (player.Board[cordY][cordX] == missed)player.previouscords.checklastshot = reline; break;
		case 1:cordY = player.previouscords.y + 1; if (player.Board[cordY][cordX] == missed)player.previouscords.checklastshot = reline; break;
		case 2:cordX = player.previouscords.x - 1; if (player.Board[cordY][cordX] == missed)player.previouscords.checklastshot = reline; break;
		case 3:cordY = player.previouscords.y - 1; if (player.Board[cordY][cordX] == missed)player.previouscords.checklastshot = reline; break;
		}
		break;
	case reline:
		cordX = player.previouscords.x;
		cordY = player.previouscords.y;
		switch (linedir)
		{
		case 0:linedir = 2; break;
		case 1:linedir = 3; break;
		case 2:linedir = 0; break;
		case 3:linedir = 1; break;
		}
		while (player.Board[cordY][cordX] == hit) {
			switch (linedir)
			{
			case 0:cordX++; break;
			case 1:cordY++; break;
			case 2:cordX--; break;
			case 3:cordY--; break;
			}
		}
		break;
	}
}
//iesko sunaikintu kompiuterio laivu
void search(GameBoardOf &pc, GameBoardOf &viewfinder, int cordX, int cordY)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD oldatts;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(h, &csbi);
	oldatts = csbi.wAttributes;
	bool shipded = true;
	int ty1, ty2, tx1, tx2;
	for (int k = 0; k < 10; k++)
	{
		tx1 = pc.ships[k].xbegin;
		tx2 = pc.ships[k].xend;
		ty1 = pc.ships[k].ybegin;
		ty2 = pc.ships[k].yend;
		if ((cordX >= tx1) && (cordX <= tx2) && (cordY >= ty1) && (cordY <= ty2))
		{
			for (int i = ty1; i <= ty2; i++) for (int j = tx1; j <= tx2; j++) if (pc.Board[i][j] == est_occupado) shipded = false;
			break;
		}
	}
	if (shipded == true)
	{
		for (int i = ty1 - 1; i <= ty2 + 1; i++) for (int j = tx1 - 1; j <= tx2 + 1; j++) { pc.Board[i][j] = missed; viewfinder.Board[i][j] = missed; }
		for (int i = ty1; i <= ty2; i++) for (int j = tx1; j <= tx2; j++) { pc.Board[i][j] = dead; viewfinder.Board[i][j] = dead; }
		pc.shipsleft--;
		SetConsoleTextAttribute(h,10);
		cout << "Ya destroyed an enemy ship" << endl << endl;
		SetConsoleTextAttribute(h, oldatts);
	}
	Sleep(1500);
}
//iesko sunaikintu zaidejo laivu
void search(GameBoardOf &player, int cordX, int cordY)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD oldatts;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(h, &csbi);
	oldatts = csbi.wAttributes;
	bool shipded = true;
	int tx1, tx2, ty1, ty2;
	for (int k = 0; k < 10; k++)
	{
		tx1 = player.ships[k].xbegin;
		tx2 = player.ships[k].xend;
		ty1 = player.ships[k].ybegin;
		ty2 = player.ships[k].yend;
		if ((cordX >= tx1) && (cordX <= tx2) && (cordY >= ty1) && (cordY <= ty2))
		{
			for (int i = ty1; i <= ty2; i++) for (int j = tx1; j <= tx2; j++) if (player.Board[i][j] == est_occupado) shipded = false;
			break;
		}
	}
	if (shipded == true)
	{
		for (int i = ty1 - 1; i <= ty2 + 1; i++) for (int j = tx1 - 1; j <= tx2 + 1; j++) player.Board[i][j] = missed;
		for (int i = ty1; i <= ty2; i++) for (int j = tx1; j <= tx2; j++) player.Board[i][j] = dead;
		player.shipsleft--;
		player.previouscords.checklastshot = randcord;
		SetConsoleTextAttribute(h,12);
		cout << "Ya ship is dead matey" << endl << endl;
		SetConsoleTextAttribute(h, oldatts);
	}
}

int main()
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD newwindowcords;
	newwindowcords.X = 80;
	newwindowcords.Y = 80;
	SetConsoleScreenBufferSize(hcon, newwindowcords);
	SetConsoleTitle(L"Ships Of Battle");
	srand(time(NULL));
	char choice;
	int turn = rand() % 2;
	GameBoardOf Player;
	GameBoardOf PC;
	GameBoardOf ViewFinder;
	cout << "Ye board." << endl;
	Player.DrawBoard();
	cout << "How do you want to place your ships? (Manually-M/AtRandom-anything else) ";
	cin >> choice;
	switch (choice)
	{
	case 'M':
		Player.PlaceShipManually();
		break;
	case 'm':
		Player.PlaceShipManually();
		break;
	default:
		Player.PlaceShipAtRandom();
		break;
	}
	ClearScreen();
	PC.PlaceShipAtRandom();
	cout << "Ye board. Ships left: " << Player.getshipsleft() << endl;
	Player.DrawBoard();
	cout << endl << "Ye viewfinder board. Enemy ships left: " << PC.getshipsleft() << endl;
	ViewFinder.DrawBoard();
	//PC.DrawBoard();
	//gameloop start
	while ((Player.getshipsleft() > 0) && (PC.getshipsleft() > 0))
	{
		ClearScreen();
		cout << "Ye board. Ships left: " << Player.getshipsleft() << endl;
		Player.DrawBoard();
		cout << endl << "Ye viewfinder board. Enemy ships left: " << PC.getshipsleft() << endl;
		ViewFinder.DrawBoard();
		//PC.DrawBoard();
		switch (turn)
		{
		case 0:
			shot(ViewFinder, PC);
			turn = 1;
			break;
		case 1:
			shot(Player);
			turn = 0;
			break;
		}
		Sleep(1000); //needs <windows.h>
	}
	//gameloop end
	ClearScreen();
	cout << "Ye board. Ships left: " << Player.getshipsleft() << endl;
	Player.DrawBoard();
	cout << endl << "Ye viewfinder board. Enemy ships left: " << PC.getshipsleft() << endl;
	ViewFinder.DrawBoard();
	if (Player.getshipsleft() == 0) cout << "PC won!.. pls nerf" << endl;
	else cout << "Player won!.. Ya got lucky" << endl;
	Sleep(5000);
	return 0;
}