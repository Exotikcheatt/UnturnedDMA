#include "Unturned.h"

#pragma warning(disable: 28251)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Unturned* skill = new Unturned();
	skill->MakeMeBetter();
	return 0;
}