#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <string>
#include "stdafx.h"

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
	RegisteredPlayerEntities RPE[12];
	int NeffRPE;
	GameBlocks **GB;
	int MapHeight;
	int MapWidth;
	Bomb B[20];
	int NeffBomb;
};


void ParseBlocks(string str, GameState& G, int x, int y);
void ParseEntities2(string str, GameState& G, int IDX, int mode);
void ParseEntities(string str, GameState& G, int IDX);
void Parse(string filePath, GameState& G);


#endif
