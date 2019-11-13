#include "GameEngine.h"
#include "Game.h"

using namespace std;

int main()
{
	Game test;
	
	test.console(80, 100, 8, 8);
	test.start();
	Sleep(3000);
	return 0;
}