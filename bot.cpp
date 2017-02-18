#include "parse.h"
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

const int distOffset = 3;


using namespace std;

struct Point{
	int x=0;
	int y=0;
};

void FindGameObj(Point* PowerUpLoc, Point* WallLoc, const Gamestate& G, int& PUCount, int& WallCount){
//Mencatat lokasi semua powerup dan wall
	GameBlocks **Block;
	Block = G.GB;
	int i,j;

	for(i=0;i<=G.MapWidth;i++){
		for(j=0;j<=G.MapHeight;j++){
			if(Block[i][j].Entity.find("PU.",0) != -1){
				PUCount++;
				PowerUpLoc[PUCount].x = i;
				PowerUpLoc[PUCount].y = j;
			}
			else if(Block[i][j].Entity. == "DW"){
				WallCount++;
				WallLoc[PUCount].x = i;
				WallLoc[PuCount].y = j;
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Inisialisasi Variabel
	Gamestate G;
	string filePath = argv[2];
	Point * PowerUpLoc;
	Point * WallLoc;
	Point CurPos;
	int PUCount,WallCount;
	int PlayerID = (int)argv[1][0] - 65;
	int PUCount = -1;
	int WallCount = -1;
	int i;

	Point targetLoc;
	bool targetEnemy;

	PowerUpLoc = new Point [G.MapWidth*G.MapHeight];
	WallLoc = new Point [G.MapWidth*G.MapHeight];

	//Start Parse
	Parse(filePath, G);
	CurPos.x = G.RPE[PlayerID].LocX;
	CurPos.y = G.RPE[PlayerID].LocY;

	// AI Thinking Process
		// Move Up :1
		// Move Left : 2
		// Move Right : 3
		// Move Down : 4
		// Place Bomb : 5
		// Do Nothing : 7

	// outputkan ke
	cout << "Writing move file " << filePath + "/" + "move.txt" << std::endl;
	ofstream outfile(filePath + "/" + "move.txt");

	// FindGameObj
	FindGameObj(PowerUpLoc, WallLoc, G, PUCount, WallCount);
	//FindClosestPowerUp
	int temp;
	int minPU = (abs(PowerUpLoc[0].x-CurPos.x) + abs(PowerUpLoc[0].y-CurPos.y));
	int minPUid = 0;
	for(i = 1; i<=PUCount; i++){
		temp = (abs(PowerUpLoc[i].x-CurPos.x) + abs(PowerUpLoc[i].y-CurPos.y));
		if(temp<min){
			minPU = temp;
			minPUid = i;
		}
	}

	//FindClosestEnemy
	int minEn = 999;
	int minEnid;
	for(i = 0;i<4;i++){
		if (i != PlayerID){
			temp = abs(G.RPE[i].LocX-CurPos.x) + abs(G.RPE[i].LocY-CurPos.y);
			if (temp < minEn){
				minEn = temp;
				minEnid = i;
			}
		}
	}

	//Closest target
	if(minEn + distOffset < minPU){
		targetEnemy = true;
		targetLoc.x = G.RPE[minEnid].LocX;
		targetLoc.y = G.RPE[minEnid].LocY;
	}
	else{
		targetEnemy = false;
		targetLoc.x = PowerUpLoc[minPUid].LocX;
		targetLoc.y = PowerUpLoc[minPUid].LocY;
	}


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
