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

void FindGameObj(Point* PowerUpLoc, Point* WallLoc, GameState& G, int& PUCount, int& WallCount){
//Mencatat lokasi semua powerup dan wall
	int i,j;

	for(i=0;i<=G.MapWidth;i++){
		for(j=0;j<=G.MapHeight;j++){
			if(G.GB[i][j].Entity.find("PU.",0) != -1){
				PUCount++;
				PowerUpLoc[PUCount].x = i;
				PowerUpLoc[PUCount].y = j;
				cout << "PU "<<PUCount<< " x:" << PowerUpLoc[PUCount].x << " y:" << PowerUpLoc[PUCount].y << endl;
			}
			else if(G.GB[i][j].Entity == "DW"){
				WallCount++;
				WallLoc[WallCount].x = i;
				WallLoc[WallCount].y = j;
				cout << "Wall "<<WallCount<<" x:" << WallLoc[WallCount].x << " y:" << WallLoc[WallCount].y << endl;
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Inisialisasi Variabel
	GameState G;
	string filePath = argv[2];
	Point* PowerUpLoc;
	Point* WallLoc;
	Point CurPos;
	int PlayerID = int(argv[1][0]) - 65;
	int PUCount = -1;
	int WallCount = -1;
	int i;

	Point targetLoc;
	bool targetEnemy;

	//Start Parse
	Parse(filePath, G);

	
	PowerUpLoc = new Point[G.MapWidth*G.MapHeight];
	WallLoc = new Point[G.MapWidth*G.MapHeight];
	

	CurPos.x = G.RPE[PlayerID].LocX;
	CurPos.y = G.RPE[PlayerID].LocY;

	// AI Thinking Process
		// Move Up :1
		// Move Left : 2
		// Move Right : 3
		// Move Down : 4
		// Place Bomb : 5
		// Do Nothing : 7

	// FindGameObj
	FindGameObj(PowerUpLoc, WallLoc, G, PUCount, WallCount);
	//FindClosestPowerUp
	int temp;
	int minPU = (abs(PowerUpLoc[0].x-CurPos.x) + abs(PowerUpLoc[0].y-CurPos.y));
	int minPUid = 0;
	for(i = 1; i<=PUCount; i++){
		temp = (abs(PowerUpLoc[i].x-CurPos.x) + abs(PowerUpLoc[i].y-CurPos.y));
		if(temp<minPU){
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

	//cout << minEnid;

	//cout << G.RPE[minEnid].LocY;

	//Closest target
	if(minEn + distOffset < minPU){
		targetEnemy = true;
		targetLoc.x = G.RPE[minEnid].LocX;
		targetLoc.y = G.RPE[minEnid].LocY;
	}
	else{
		targetEnemy = false;
		targetLoc.x = PowerUpLoc[minPUid].x;
		targetLoc.y = PowerUpLoc[minPUid].y;
	}


	// OUTPUT
	cout << "Writing move file " << filePath + "/" + "move.txt" << endl;
	ofstream outfile(filePath + "/" + "move.txt");

	outfile << "1"<<endl;
	cout << "x: " << targetLoc.x << "y: " << targetLoc.y << endl;
	//outfile.close();
	
	cout << "a";
	return 0;
}
