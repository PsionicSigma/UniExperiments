// PsudoGraphicsTest.cpp
#include "pch.h"
#include <iostream>
#include "PGTest.h"

using namespace std;

int main()
{
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD SpcWindowCords;
	SpcWindowCords.X = 60;
	SpcWindowCords.Y = 30;
	SetConsoleScreenBufferSize(Handle, SpcWindowCords);
	SetConsoleTitle(L"Psudo Graphics demo?");

	My_Window::Window Main_Window;
	My_Window::Update(Main_Window);
}

