#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


struct RegisteredPlayerEntities
{
	string Name;
	string Key;
	int Points;
	bool Killed;
	int BombBag;
	int BombRadius;
	int LocX;
	int LocY;
};

struct GameBlocks
{
	string Entity;
	bool Exploding;
};


struct Bomb
{
	int LocX;
	int LocY;
	char PlayerKey;
	int BombRadius;
	int Timer;
};
struct GameState
{
	RegisteredPlayerEntities RPE[4];
	int NeffRPE;
	GameBlocks **GB;
	int MapHeight;
	int MapWidth;
	Bomb B[20];
	int NeffBomb;
};

void ParseBlocks(string str, GameState& G, int x, int y)
// contoh format : 
// untuk IW : {"Entity":{"$type":"Domain.Entities.IndestructibleWallEntity, Domain","Location":{"X":1,"Y":2}},"Bomb":null,"PowerUp":null,"Exploding":false,"Location":{"X":1,"Y":2}}
{

	int i, j, idx, start;
	string tempstr;
	for (i = 0; str[i] != ':'; i++);
	i++;
	if (str[i] == '{')
	{
		if (str.find("IndestructibleWallEntity", i) != string::npos)
		{
			G.GB[x][y].Entity = "IW";
			G.GB[x][y].Exploding = false;
		}
		else if (str.find("DestructibleWallEntity", i) != string::npos)
		{
			G.GB[x][y].Entity = "DW";
			G.GB[x][y].Exploding = false;
		}
		else if (str.find("PlayerEntity", i) != string::npos)
		{
			G.GB[x][y].Entity = "Player";
			G.GB[x][y].Exploding = false;
		}
		else
		{
			G.GB[x][y].Entity = "Null";
			G.GB[x][y].Exploding = false;
		}
	}
	i = str.find("Bomb", i);
	for (; str[i] != ':'; i++);
	i++;
	if (str.find("Owner", i) != string::npos)
	{
		G.GB[x][y].Entity = "Bomb";
		G.GB[x][y].Exploding = false;
		//"Bomb":{"Owner":{"Name":"John","Key":"B","Points":22,"Killed":false,"BombBag":2,"BombRadius":1,"Location":{"X":20,"Y":18}},"BombRadius":1,"BombTimer":7,"IsExploding":false,"Location":{"X":20,"Y":18}}
		idx = G.NeffBomb;
		G.B[idx].LocX = x;
		G.B[idx].LocY = y;
		i = str.find("Key", i);
		for (; str[i] != ':'; i++);
		i = i + 2;
		G.B[idx].PlayerKey = str[i];
		// skip smp owner kelar
		for (; str[i] != '}'; i++);
		i = str.find("Radius", i);
		for (; str[i] != ':'; i++);
		i++;
		start = i;
		for (; str[i] != ','; i++);
		G.B[idx].BombRadius = stoi(str.substr(start, i - start));
		i = str.find("Timer", i);
		for (; str[i] != ':'; i++);
		i++;
		start = i;
		for (; str[i] != ','; i++);
		G.B[idx].Timer = stoi(str.substr(start, i - start));
		G.NeffBomb++;
	}
	i = str.find("PowerUp");
	for (; str[i] != ':'; i++);
	i++;
	if (str[i] == '{')
	{
		//"PowerUp":{"$type":"Domain.Entities.PowerUps.SuperPowerUp, Domain","Location":{"X":11,"Y":11}}
		//"PowerUp":{"$type":"Domain.Entities.PowerUps.BombBagPowerUpEntity, Domain","Location":{"X":6,"Y":2}}
		//"PowerUp":{"$type":"Domain.Entities.PowerUps.BombRaduisPowerUpEntity, Domain","Location":{"X":14,"Y":14}}
		if (str.find("SuperPowerUp", i) != string::npos)
		{
			G.GB[x][y].Entity = "PU.Super";
			G.GB[x][y].Exploding = false;
		}
		else if (str.find("BombBagPowerUpEntity", i) != string::npos)
		{
			G.GB[x][y].Entity = "PU.BombBag";
			G.GB[x][y].Exploding = false;
		}
		else if (str.find("BombRaduisPowerUpEntity", i) != string::npos)
		{
			G.GB[x][y].Entity = "PU.Radius";
			G.GB[x][y].Exploding = false;
		}
	}
}
void ParseEntities2(string str, GameState& G, int IDX, int mode)
{
	if (mode == 1)
	{
		int i;
		for(i = 1; str[i] != '"'; i++);
		G.RPE[IDX].Name = str.substr(1,i-1);
	}
	else if (mode == 2)
	{
		G.RPE[IDX].Key = str[1];
	}
	else if (mode == 3)
	{
		G.RPE[IDX].Points = std::stoi(str);
	}
	else if (mode == 4)
	{
		if (str[0] = 'f') G.RPE[IDX].Killed = false;
		else G.RPE[IDX].Killed = true;
	}
	else if (mode == 5)
	{
		G.RPE[IDX].BombBag = stoi(str);
	}
	else if (mode == 6)
	{
		G.RPE[IDX].BombRadius = stoi(str);
	}
	else if (mode == 7)
	{
		int i, idx;
		for(i = 0; str[i] != ':'; i++);
		i++;
		idx = i;
		for(; str[i] != ','; i++);
		string tempstr;
		tempstr = str.substr(idx, i-idx) + '\0';

		G.RPE[IDX].LocX = stoi(tempstr);
		for(; str[i] != ':'; i++);
		i++;
		idx = i;
		for(; str[i] != '}'; i++);
		tempstr = str.substr(idx, i-idx) + '\0';

		G.RPE[IDX].LocY = stoi(tempstr);		
	}
} 

void ParseEntities(string str, GameState& G, int IDX)
// Json yang masuk ke dalam prosedur ini adalah isi dari salah satu RegisteredPlayerEntities
// F.S. Gamestate G bagian RPE terisi di index yang tepat
// Format input : {"Name":"Player 1","Key":"A","Points":0,"Killed":false,"BombBag":1,"BombRadius":1,"Location":{"X":2,"Y":2}}
{
	int i = 1; // skip { pertama
	int bracket = 1;
	int mode = 0;
	int idx;
	string tempstr;
	while (str[i] != '\0')
	{
		if (str[i] == '{')
		{
			bracket++;
		}
		else if (str[i] == '}')
		{
			bracket--;
		}
		else if (str[i] == ':')
		{
			i++;
			idx = i;
			if (mode == 7)
			{
				for(;str[i] != '}'; i++);
				{
				tempstr = str.substr(idx,i-idx+1) + '\0';
						
				ParseEntities2(tempstr,G,IDX,mode);
				mode = 0;
				}
			}
			else
			{
				for(;str[i] != ',';i++);
				{
					// parsing
					tempstr = str.substr(idx,i-idx) + '\0';
								
					ParseEntities2(tempstr,G,IDX,mode);
					mode = 0;
				}
			}
		}
		else if (str[i] == '"')
		{
			i++;
			idx = i;
			for(;str[i] != '"';i++);
			tempstr = str.substr(idx,i-idx);

			if (tempstr == "Name") mode = 1;
			else if (tempstr == "Key") mode = 2;
			else if (tempstr == "Points") mode = 3;
			else if (tempstr == "Killed") mode = 4;
			else if (tempstr == "BombBag") mode = 5;
			else if (tempstr == "BombRadius") mode = 6;
			else if (tempstr == "Location") mode = 7;
		}
		i++;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	string fileContent;
	string line;
	string J;
	string filePath = argv[2];
	ifstream myfile(filePath + "/" + "state.json");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			fileContent += line += "\n";
		}
		myfile.close();
	}
	J = fileContent;
	GameState G;
	size_t i = 0;
	int length, idx;
	int bracket = 0;
	//cout << J;
	while (J[i] != '[') i++;
	i++; idx = i; // index pertama
	int numberPlayer = 0;
	while (J[i] != ']')
	{
		if (J[i] == '{')
		{
			if (bracket == 0)
			{
				idx = i;
			}
			bracket++;
		}
		else if (J[i] == '}')
		{
			if (bracket == 1)
			{
				length = i - idx + 1;
				string temp = J.substr(idx,length) + '\0';
				//cout << temp << endl;
				//cout << Pointer("/Name").Get(d) << endl;
				ParseEntities(temp,G,numberPlayer);
				numberPlayer++; 
			}
			bracket--;
		}
		i++;
	}
	G.NeffRPE = numberPlayer-1;
	
	int x, y;
	string tempstr;
	G.GB = new GameBlocks*[23];
	for (int itera = 0; itera < 23; itera++)
	{
		G.GB[itera] = new GameBlocks [23];
	}
	G.NeffBomb = 0;
	x = 1;
	while (J[i] != '[') i++;
	while (J[i] != ']')
	{
		
		for(;J[i] != '['; i++);
		for(;J[i] != '['; i++);
		bracket = 0;
		y = 1;
		while(J[i] != ']')
		{
			i++;
			if (J[i] == '{')
			{
				if (bracket == 0)
				{
					idx = i;
				}
				bracket++;
			}
			else if (J[i] == '}')
			{
				bracket--;
				if (bracket == 0)
				{
					
					tempstr = J.substr(idx,i-idx) + '\0';
					ParseBlocks(tempstr,G, x,y);
					if (G.GB[x][y].Entity == "IW") cout << "#";
					else if (G.GB[x][y].Entity == "DW") cout << "+";
					else cout << " ";
					y++;
				}
			}
		}
		cout << endl;
		x++;
		i++;
	}


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
	
	/*
		if (G.B[i].LocX == G.RPE[KEY].LocX)
		{
			
			if (G.GB[i + 1][G.RPE[KEY].LocY].Entity == "DW")
			{
				outfile << 2 << std::endl;
			}
			else
			{
				outfile << 3 << std::endl;
			}
		}
		else if (G.GB[G.B[i].LocX][G.RPE[KEY].LocY - 1].Entity != "IW")
		{
			outfile << 4 << std::endl;
		}
		else if (G.GB[G.B[i].LocX][G.RPE[KEY].LocY - 1].Entity != "IW")
		{
			outfile << 1 << std::endl;
		}
		else if (G.B[i].LocX < G.RPE[KEY].LocX)
		{
			outfile << 3 << std::endl;
		}
		else
		{
			outfile << 2 << std::endl;
		}*/
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
