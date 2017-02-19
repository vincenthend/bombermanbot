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

void FindSafe(GameState& G, Point P, bool& IsSafe, int& path)
{
	int i, j;
	bool tengah;
	if ((G.GB[P.x][P.y].Entity == "Bomb"))
	{
		tengah = true;
	}
	else
	{
		tengah = false;
	}
	// cek kanan 
	bool kanan, kekanan; // kanan true jika ada bom di kanan
	i = 1;
	kekanan = true;
	if ((G.GB[P.x + 1][P.y].Entity == "IW") || (G.GB[P.x + 1][P.y].Entity == "DW"))
	{
		kanan = false;
		kekanan = false;
	}
	else
	{
		while (G.GB[P.x + i][P.y].Entity == "Null") i++; // skip null, berenti saat entitynya bukan null
		if (G.GB[P.x + i][P.y].Entity == "Bomb")
		{
			//cari id bom
			j = 0;
			while (G.B[j].LocX != (P.x + i)) j++;
			// j adalah id bom
			if (P.x >= (G.B[j].LocX - G.B[j].BombRadius))
			{
				kanan = true;
				kekanan = false;
			}
			else
			{
				kanan = false;
				kekanan = true;
			}
		}
		else
		{
			kanan = false;
			kekanan = true;
		}
	}

	// cek kiri
	bool kiri, kekiri; // kiri true jika ada bom di kanan
	i = 1;
	kekiri = true;
	if ((G.GB[P.x - 1][P.y].Entity == "IW") || (G.GB[P.x - 1][P.y].Entity == "DW"))
	{
		kiri = false;
		kekiri = false;
	}
	else
	{
		while (G.GB[P.x - i][P.y].Entity == "Null") i++; // skip null, berenti saat entitynya bukan null
		if (G.GB[P.x - i][P.y].Entity == "Bomb")
		{
			//cari id bom
			j = 0;
			while (G.B[j].LocX != (P.x - i)) j++;
			// j adalah id bom
			if (P.x <= (G.B[j].LocX + G.B[j].BombRadius))
			{
				kiri = true;
				kekiri = false;
			}
			else
			{
				kiri = false;
				kekiri = true;
			}
		}
		else
		{
			kiri = false;
			kekiri = true;
		}
	}

	// cek atas
	bool atas, keatas; // kiri true jika ada bom di kanan
	i = 1;
	keatas = true;
	if ((G.GB[P.x][P.y - 1].Entity == "IW") || (G.GB[P.x][P.y - 1].Entity == "DW"))
	{
		atas = false;
		keatas = false;
	}
	else
	{
		while (G.GB[P.x][P.y - i].Entity == "Null") i++; // skip null, berenti saat entitynya bukan null
		if (G.GB[P.x][P.y - i].Entity == "Bomb")
		{
			//cari id bom
			j = 0;
			while (G.B[j].LocY != (P.y - i)) j++;
			// j adalah id bom
			if (P.y <= (G.B[j].LocY + G.B[j].BombRadius))
			{
				atas = true;
				keatas = false;
			}
			else
			{
				atas = false;
				keatas = true;
			}
		}
		else
		{
			atas = false;
			keatas = true;
		}
	}

	// cek bawah
	bool bawah, kebawah; // kiri true jika ada bom di kanan
	i = 1;
	kebawah = true;
	if ((G.GB[P.x][P.y + 1].Entity == "IW") || (G.GB[P.x][P.y + 1].Entity == "DW"))
	{
		bawah = false;
		kebawah = false;
	}
	else
	{
		while (G.GB[P.x][P.y + i].Entity == "Null") i++; // skip null, berenti saat entitynya bukan null
		if (G.GB[P.x][P.y + i].Entity == "Bomb")
		{
			//cari id bom
			j = 0;
			while (G.B[j].LocY != (P.y + i)) j++;
			// j adalah id bom
			if (P.y >= (G.B[j].LocY - G.B[j].BombRadius))
			{
				bawah = true;
				kebawah = false;
			}
			else
			{
				bawah = false;
				kebawah = true;
			}
		}
		else
		{
			bawah = false;
			kebawah = true;
		}
	}
	//cek
	{
		IsSafe = tengah || kanan || atas || kiri || bawah;
		IsSafe = !(IsSafe);
		if (!IsSafe)
		{
			if (kekiri) path = 2;
			else if (kekanan) path = 3;
			else if (keatas) path = 1;
			else if (kebawah) path = 4;
			else
				path = 7;
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
	
	//Init Location
	PowerUpLoc = new Point[G.MapWidth*G.MapHeight];
	WallLoc = new Point[G.MapWidth*G.MapHeight];
	
	//Get Player CurPos
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
