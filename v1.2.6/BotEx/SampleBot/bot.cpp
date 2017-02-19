#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <cmath>
using namespace std;

const int distOffset = 3; //Offset for targetfinding before switching to the least priority
const int nodeOffset = 2; //Offset for pathfinding; Max Node Offset = 10

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
				//cout << "PU "<<PUCount<< " x:" << PowerUpLoc[PUCount].x << " y:" << PowerUpLoc[PUCount].y << endl;
			}
			else if(G.GB[i][j].Entity == "DW"){
				WallCount++;
				WallLoc[WallCount].x = i;
				WallLoc[WallCount].y = j;
				//cout << "Wall "<<WallCount<<" x:" << WallLoc[WallCount].x << " y:" << WallLoc[WallCount].y << endl;
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

int FindPath (const Point& Target, const Point& CurLoc, const GameState& G){
//F.S. menemukan node dengan wall paling banyak dan jarak yang berkurang
	Point* Node; //Candidate Set
	int* P;
	int NodeIdx = -1;
	int i,j,k;
	int wallcount;
	int dNode, dCurLoc, move;

	////////////////////
	// Initialization //
	////////////////////

	//Init Node with Location
	Node = new Point[(((G.MapHeight) - 1) / 2)*(((G.MapWidth) - 1) / 2) + 3];

	NodeIdx++;
	Node[NodeIdx].x = CurLoc.x;
	Node[NodeIdx].y = CurLoc.y;	
	
	for (i = 2;i <= G.MapHeight;i=i+ 2) {
		for (j = 2;j <= G.MapWidth;j=j+ 2) {
			NodeIdx++;
			Node[NodeIdx].x = j;
			Node[NodeIdx].y = i;
		}
	}

	//Init PathData with number of walls between starting node and node within distance
	P = new int[NodeIdx + 1];
	for (i = 1; i <= NodeIdx; i++) {
		//Count number of walls from a node within a distance of nodeoffset
		wallcount = 0;
		if (Node[0].x == Node[i].x && abs(Node[0].y - Node[i].y) <= 2 * nodeOffset) {
			cout << "Node " << i << "x " << Node[i].x << " y " << Node[i].y<<endl;
			for (k = min(Node[0].y,Node[i].y);k <= max(Node[0].y,Node[i].y);k++) {
				if (G.GB[Node[i].x][k].Entity == "DW") {
					wallcount++;
				}
				cout << "x " << Node[i].x << " y " << k << " e:" << G.GB[Node[i].x][k].Entity<<endl;
			}
		}
		else if (Node[0].y == Node[i].y && abs(Node[0].x - Node[i].x) <= 2 * nodeOffset) {
			cout << "Node " << i << "x " << Node[i].x << " y " << Node[i].y <<endl;
			for (k = min(Node[0].x,Node[i].x);k <= max(Node[0].x,Node[i].x);k++) {
				if (G.GB[k][Node[i].y].Entity == "DW") {
					wallcount++;
				}				
				cout << "x " << k << " y " << Node[i].y << " e:" << G.GB[k][Node[i].y].Entity<<endl;
			}
		}
		else {
			wallcount = -1;
		}
		P[i] = wallcount;
		cout << "Node no " << i << " " << P[i] << " walls"<<endl;
	}
	
	//////////////////////
	// Greedy Algorithm //
	//////////////////////
	
	dNode = abs(CurLoc.x - Target.x) + abs(CurLoc.y - Target.y);
	dCurLoc = dNode;
	int max;
	
	max = P[1];
	j = 1;
	
	//Find the most wall, with the most block, and reduced distance
	for (i = 2; i <= NodeIdx; i++) {
		dNode = abs(Node[i].x - Target.x) + abs(Node[i].y - Target.y);
		if (P[i] >= max && dNode >= abs(Node[i].x - Target.x) + abs(Node[i].y - Target.y) && dNode - dCurLoc <= 2 * nodeOffset) //Selection function
		{
			dNode = abs(Node[i].x - Target.x) + abs(Node[i].y - Target.y);
			max = P[i];
			j = i;
		}
	}
	
	//Returned selected node j
	cout << "CurLoc X: " << CurLoc.x << " Y: " << CurLoc.y<<endl;
	cout << "Selected node number :" << j << ", X: " << Node[j].x << " Y:" << Node[j].y<<endl;
	
	///////////////////
	// Move Decision //
	///////////////////
	move = 7;
	if (Node[j].x == CurLoc.x) {
		if (Node[j].y > CurLoc.y) {
			if (G.GB[CurLoc.x][CurLoc.y + 1].Entity == "DW") {
				move = 5;
			}
			else {
				move = 1;
			}
		}
		else {
			if (G.GB[CurLoc.x][CurLoc.y - 1].Entity == "DW") {
				move = 5;
			}
			else {
				move = 4;
			}
		}	
	}
	else if (Node[j].y == CurLoc.y) {
		if (Node[j].x > CurLoc.x) {
			if (G.GB[CurLoc.x + 1][CurLoc.y].Entity == "DW") {
				move = 5;
			}
			else {
				move = 3;
			}
		}
		else {
			if (G.GB[CurLoc.x - 1][CurLoc.y].Entity == "DW") {
				move = 5;
			}
			else {
				move = 2;
			}
		}
	}
	cout << "Selected move : " << move<<endl;
	return move;
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

	////////////////////
	// Initialization //
	////////////////////

	//Start Parse
	Parse(filePath, G);
	
	//Init Location
	PowerUpLoc = new Point[G.MapWidth*G.MapHeight];
	WallLoc = new Point[G.MapWidth*G.MapHeight];
	
	//Get Player CurPos
	CurPos.x = G.RPE[PlayerID].LocX;
	CurPos.y = G.RPE[PlayerID].LocY;

	////////////////////
	// Finding Target //
	////////////////////

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

	/////////////////////
	// Decision Making //
	/////////////////////

	// AI Thinking Process
		// Move Up :1
		// Move Left : 2
		// Move Right : 3
		// Move Down : 4
		// Place Bomb : 5
		// Trigger : 6
		// Do Nothing : 7

	int move;
	bool safe,safenext;
	Point nextLoc;

	FindSafe(G, CurPos, safe, move);
	if (safe) {
		move = FindPath(targetLoc, CurPos, G);
		if (move == 1) {
			nextLoc.x = CurPos.x;
			nextLoc.y = CurPos.y + 1;
			FindSafe(G, nextLoc, safenext, temp);
		}
		else if (move == 2) {
			nextLoc.x = CurPos.x-1;
			nextLoc.y = CurPos.y;
			FindSafe(G, nextLoc, safenext, temp);
		}
		else if (move == 3) {
			nextLoc.x = CurPos.x+1;
			nextLoc.y = CurPos.y;
			FindSafe(G, nextLoc, safenext, temp);
		}
		else if (move == 4) {
			nextLoc.x = CurPos.x;
			nextLoc.y = CurPos.y - 1;
			FindSafe(G, nextLoc, safenext, temp);
		}

		if (!safenext) {
			move = 7;
		}
	}

	///////////////////
	//  FileWriting  //
	///////////////////

	// OUTPUT
	cout << "Writing move file " << filePath + "/" + "move.txt" << endl;
	ofstream outfile(filePath + "/" + "move.txt");

	outfile << move <<endl;
	outfile.close();
	return 0;
}
