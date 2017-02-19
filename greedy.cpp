#include <algorithm>

const int nodeOffset = 1; //Max Node Offset = 10

int findPath{const Point& Target, const Point& CurLoc, const GameState& G){
//F.S. menemukan node dengan wall paling banyak dan jarak yang berkurang
	Point* Node; //Candidate Set
	int* P;
	int NodeIdx = -1;
	int i,j,k;
	int wallcount;
	int dTarget, move;
	
	////////////////////
	// Initialization //
	////////////////////
	
	//Init Node with Location
	NodeIdx++;
	Node[NodeIdx].x = CurLoc.x;
	Node[NodeIdx].y = CurLoc.y;
	
	Node = new Point [(((G.MapHeight)-1)/2)*(((G.MapWidth)-1)/2)+1];
	for(i=2;i<=G.MapHeight;i+2){
		for(j=2;j<=G.MapWidth;j+2){
			NodeIdx++;
			Node[NodeIdx].x = j;
			Node[NodeIdx].y = i;
		}
	}
	
	NodeIdx++;
	Node[NodeIdx].x = Target.x;
	Node[NodeIdx].y = Target.y;
	
	//Init PathData with number of walls between starting node and node within distance
	P = new int [NodeIdx+1];
	for(i=1; i<=NodeIdx; i++){
		//Count number of walls from a node within a distance of nodeoffset
		wallcount = 0;
		if(Node[0].x == Node[i].x && abs(Node[0].y-Node[i].y)<=2*nodeOffset){
			for(k=min(Node[0].y,Node[i].y);k<=max(Node[0].y,Node[i].y);k++){
				if(G.GB[Node[i].x][k].Entity == "DW"){
					wallcount++;
				}
			}
		}
		else if(Node[0].y == Node[i].y && abs(Node[0].x-Node[i].x)<=2*nodeOffset){
			for(k=min(Node[0].x,Node[i].x);k<=max(Node[0].x,Node[i].x);k++){
				if(G.GB[k][Node[i].y].Entity == "DW"){
					wallcount++;
				}
			}
		}
		P[i] = wallcount;
	}
	
	//////////////////////
	// Greedy Algorithm //
	//////////////////////
	
	dTarget = abs(CurLoc.x - Target.x) + abs(CurLoc.y - Target.y);
	
	int max;
	
	max = P[1];
	j = 1;
	
	//Find the most wall, with the most block, and reduced distance
	for(i = 2; i<=NodeIdx; i++){
		if (P[i] >= max && dTarget >= abs(Node.x - Target.x) + abs(Node.y - Target.y))//Selection function
		{
			dTarget = abs(Node.x - Target.x) + abs(Node.y - Target.y);
			max = P[i];
			j = i;
		}
	}
	
	//Returned selected node j
	
	///////////////////
	// Move Decision //
	///////////////////
	
	if(Node[j].x == CurLoc.x){
		if (Node[j].y > CurLoc.y){
			if(G.GB[CurLoc.x][CurLoc.y+1].Entity = "DW"){
				move = 5;
			}
			else{
				move = 1;
			}
		}
		else{
			if(G.GB[CurLoc.x][CurLoc.y-1].Entity = "DW"){
				move = 5;
			}
			else{
				move = 4;
			}
		}
	}
	else if (Node[j].y == CurLoc.y){
		if (Node[j].x > CurLoc.x){
			if(G.GB[CurLoc.x+1][CurLoc.y].Entity = "DW"){
				move = 5;
			}
			else{
				move = 3;
			}
		}
		else{
			if(G.GB[CurLoc.x+1][CurLoc.y].Entity = "DW"){
				move = 5;
			}
			else{
				move = 2;
			}
		}
	}
	
	
	return move;
}