//#pragma once	//or
#ifndef Mano_h
#define Mano_h
#include <iostream>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <string>

namespace My_Window
{
	//void init(int St1, int St2, int St3, int St4);
	
	// style def: 0-TL 1-TR 2-BL 3-BR 4-H1 5-H2 6-H3 7-V1 8-V2 9-V3 10-BC 11-FC 12-BH 13-FH
	//Standart style
	const int Standart[14] = { 218,192,191,217,205,205,205,186,186,186,8,0,7,0 };
	//XCOM_EU style
	const int XCOM_EU[14] = { 201,92,92,188,205,205,196,186,179,179,1,11,3,11 };
	//XCOM_EW style
	const int XCOM_EW[14] = { 201,92,92,188,205,205,196,186,179,179,6,0,14,0 };
	//XCOM_2 style
	const int XCOM_2[14] = { 195,192,191,180,196,196,196,179,179,179,1,14,9,14};
	//other option - 1,14
	////Standart style
	//const Window_Style Standart = { 9556,9562,9559,9565,9472,9472,9472,9474,9474,9474,8,0 };
	////XCOM_EU style
	//const Window_Style XCOM_EU = { 9487,9584,9582,9496,9473,9598,9472,9599,9474,9474,1,11 };
	////XCOM_EW style
	//const Window_Style XCOM_EW = { 9487,9584,9582,9496,9473,9598,9472,9599,9474,9474,6,14 };
	////XCOM_2 style
	//const Window_Style XCOM_2 = { 9484,9584,9582,9496,9472,9472,9472,9474,9474,9474,1,14};
	////other option - 1,14

	class Window
	{
	private:
		int Style[14];
		short x_L, y_T, x_R, y_B;
		short height, length;
		short High_Light_Y;
		std::vector<std::string> Content;
	public:
		Window(int, int, const int[14]);
		Window();
		~Window();
		void Init_Window(int, int, int, int, const int[14], int);
		void Draw_Window();
		friend void Update(Window&);
	};

	void Update(Window&);
}
#endif