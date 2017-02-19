bool WallInRange(const Point& CurLoc, const Point& PosStart, const Point& PosEnd, const GameState& G){
	int i;
	bool inrange = false;
	if (PosStart.x == PosEnd.x){
		for(i=min(PosStart.y,PosEnd.y);i<=max(PosStart.y,PosEnd.y);i++){
			if (G.GB[PosStart.x][i].Entity == "DW"){
				inRange = true;
			}
		}
	}
	else{
		for(i=min(PosStart.x,PosEnd.x);i<=max(PosStart.x,PosEnd.x);i++){
			if (G.GB[i][PosStart.y].Entity == "DW"){
				inRange = true;
			}
		}
	}
	return inRange;
}