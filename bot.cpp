#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{

	Gamestate G;
	string filePath = argv[2];



	//Start Parse
	Parse(filePath, G);



	// BAGIAN AI
		// Move Up :1
		// Move Left : 2
		// Move Right : 3
		// Move Down : 4
		// Place Bomb : 5
		// Do Nothing : 7

	// outputkan ke
	cout << "Writing move file " << filePath + "/" + "move.txt" << std::endl;
	ofstream outfile(filePath + "/" + "move.txt");


	// TESTING BOT
	//liat apa masih ada tembok di sumbu x
	char lalala = argv[1][0];
	int KEY;
	if (lalala == 'A') KEY = 0;
	else if (lalala == 'B') KEY = 1;
	i = 0;
	bool found = false;

	bool adabom = false;
	//cek apakah di area bom blast, jika ia menghindar
	//cek apakah ada bom yg satu sumbu
	while ((!adabom) && (i <= G.NeffBomb))
	{
		if (G.B[i].LocY == G.RPE[KEY].LocY) // masih taru bom satu sumbu Y
		{
			adabom = true;
		}
		else
		{
			i++;
		}
	}
	if (adabom)
	{
		outfile << 2 << std::endl;
	}
	else
	{
		i = 1;
		found = false;
		while ((!found) && (i < 23))
		{
			if (G.GB[i][G.RPE[KEY].LocY].Entity == "DW")
			{
				found = true;
			}
			else
			{
				i++;
			}
		}
		if (found)
		{
			if ((i == (G.RPE[KEY].LocX - 1)) || (i == (G.RPE[KEY].LocX + 1)))
			{
				outfile << 5 << std::endl;
			}
			else if (i < G.RPE[KEY].LocX)
			{
				outfile << 2 << std::endl;
			}
			else
			{
				outfile << 3 << std::endl;
			}
		}
		else if (G.GB[G.RPE[KEY].LocX - 1][G.RPE[KEY].LocY].Entity == "IW")
		{
			outfile << 4 << std::endl;
		}
		else
		{
			outfile << 7 << std::endl;
		}
	}
	return 0;
}
