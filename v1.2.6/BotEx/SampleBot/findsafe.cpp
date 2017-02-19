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
		while (G.GB[P.x + i][P.y].Entity == "Null")
		{
			i++; // skip Null, berenti saat entitynya bukan Null
		}
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
		while (G.GB[P.x - i][P.y].Entity == "Null") i++; // skip Null, berenti saat entitynya bukan Null
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
		while (G.GB[P.x][P.y - i].Entity == "Null") i++; // skip Null, berenti saat entitynya bukan Null
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
		while (G.GB[P.x][P.y + i].Entity == "Null") i++; // skip Null, berenti saat entitynya bukan Null
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
