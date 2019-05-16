#include "pch.h"
#include "PGTest.h"

namespace My_Window
{
	
	std::vector<std::string> Main_Window_Content = { "Window style: Standart", "Window style: XCOM_EU", "Window style: XCOM_EW", "Window style: XCOM_2", "Exit" };
	std::vector<std::string> End_Window_Content = { "Are you sure about that?", "Yes", "No" };

	void Window::Init_Window(int x, int y, int h, int l, const int St[14], int HL)
	{
		HANDLE ConH;
		DWORD Count;
		DWORD CellCount;
		COORD HomeCoords = { 0,0 };
		
		ConH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(ConH, 15);
		CellCount = 100*50;
		FillConsoleOutputCharacter(ConH, (TCHAR) ' ', CellCount, HomeCoords, &Count);
		SetConsoleCursorPosition(ConH, HomeCoords);

		x_L = x;
		y_T = y;
		height = h;
		length = l;
		x_R = x_L + l-1;
		y_B = y_T + h-1;
		for (int i = 0; i < 14; i++)Style[i] = St[i];
		High_Light_Y = HL;
	}

	void Window::Draw_Window()
	{
		HANDLE ConH = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO CSBI;
		COORD Cords;
		COORD HomeCoords = { 0,0 };
		WORD OldAtts;
		GetConsoleScreenBufferInfo(ConH, &CSBI);
		OldAtts = CSBI.wAttributes;
		//set needed color
		SetConsoleTextAttribute(ConH, Style[10] * 16 + Style[11]);
		//draw needed window
		for (short j = x_L; j <= x_R; j++)
		{
			Cords = { j, y_T };
			SetConsoleCursorPosition(ConH, Cords);
			if (j == x_L) std::cout << (char)Style[0];
			else if (j == x_L + 1) std::cout << (char)Style[4];
			else if (j == x_L + 2) std::cout << (char)Style[5];
			else if (j == x_R) std::cout << (char)Style[2];
			else std::cout << (char)Style[6];
		}
		for (short i = y_T + 1; i <= y_B - 1; i++)
		{
			for (short j = x_L; j <= x_R; j++)
			{
				Cords = { j, i };
				SetConsoleCursorPosition(ConH, Cords);
				if ((j == x_L) && (i == y_T + 1)) std::cout << (char)Style[7];
				else if ((j == x_L) || (j == x_R)) std::cout << (char)Style[8];
				else std::cout << ' ';
			}
		}
		for (short j = x_L; j <= x_R; j++)
		{
			Cords = { j, y_B };
			SetConsoleCursorPosition(ConH, Cords);
			if (j == x_L) std::cout << (char)Style[1];
			else if (j == x_R) std::cout << (char)Style[3];
			else std::cout << (char)Style[6];
		}
		//draw needed windows content
		for (short k = 0; k < Content.size(); k++)
		{
			Cords = { x_L + 2, y_T + 2 + k };
			SetConsoleCursorPosition(ConH, Cords);
			if (k == High_Light_Y)
			{
				SetConsoleTextAttribute(ConH, Style[12] * 16 + Style[13]);
				for (short j = x_L + 2; j <= x_R - 2; j++)
				{
					Cords = { j, y_T + 2 + k };
					std::cout << " ";
				}
				Cords = { x_L + 2, y_T + 2 + k };
				SetConsoleCursorPosition(ConH, Cords);
				std::cout << Content[k];
				SetConsoleTextAttribute(ConH, Style[10] * 16 + Style[11]);
			}
			else std::cout << Content[k];
		}

		SetConsoleTextAttribute(ConH, OldAtts);
		SetConsoleCursorPosition(ConH, HomeCoords);
	}

	Window::Window(int x, int y, const int St[14])
	{
		Content = End_Window_Content;
		Init_Window(x, y, Content.size()+4, 28, St, 1);
	}

	Window::Window()
	{
		Content = Main_Window_Content;
		Init_Window(2, 2, Content.size()+4, 30, Standart, 0);
		Draw_Window();
	}

	Window::~Window(){}

	void Update(Window &A_Window)
	{
		bool Main_Run = true, End_Run;
		while (Main_Run)
		{
			if (GetAsyncKeyState(VK_UP))
				if (A_Window.High_Light_Y > 0)
				{ 
					A_Window.High_Light_Y--;
					A_Window.Draw_Window();
				}
			if (GetAsyncKeyState(VK_DOWN)) 
				if (A_Window.High_Light_Y < 4) 
				{ 
					A_Window.High_Light_Y++;
					A_Window.Draw_Window();
				}
			if (GetAsyncKeyState(VK_RETURN))
			{
				switch (A_Window.High_Light_Y)
				{
				case 0:
					for (int i = 0; i < 14; i++)A_Window.Style[i] = Standart[i];
					break;
				case 1:
					for (int i = 0; i < 14; i++)A_Window.Style[i] = XCOM_EU[i];
					break;
				case 2:
					for (int i = 0; i < 14; i++)A_Window.Style[i] = XCOM_EW[i];
					break;
				case 3:
					for (int i = 0; i < 14; i++)A_Window.Style[i] = XCOM_2[i];
					break;
				case 4:
					Window End_Window(A_Window.x_L + 1, A_Window.y_T + 1, A_Window.Style);
					A_Window.Draw_Window();
					End_Window.Draw_Window();
					End_Run = true;
					Sleep(500);
					while (End_Run)
					{
						if (GetAsyncKeyState(VK_UP))
							if (End_Window.High_Light_Y > 1)
							{
								End_Window.High_Light_Y--;
								End_Window.Draw_Window();
							}
						if (GetAsyncKeyState(VK_DOWN))
							if (End_Window.High_Light_Y < 2)
							{
								End_Window.High_Light_Y++;
								End_Window.Draw_Window();
							}
						if (GetAsyncKeyState(VK_RETURN))
						{
							switch (End_Window.High_Light_Y)
							{
							case 1:
								Main_Run = false;
								End_Run = false;
								break;
							case 2:
								End_Run = false;
								break;
							default:
								break;
							}
						}
						Sleep(100);
					}
					End_Window.~Window();
					break;
				}
				A_Window.Draw_Window();
			}
			if (GetAsyncKeyState(VK_ESCAPE)) Main_Run=false;
			Sleep(100);
		}
	}
}