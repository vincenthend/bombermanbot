// TIPE BENTUKAN BARU
struct BT
{
	bool bolret;
	int step;
	int path;
};

void FindSafe(GameState& G, Point P, bool& IsSafe, int& timerbomb)
{
	int i, j;
	bool tengah;
	timerbomb = 0;
	if ((G.GB[P.x][P.y].Entity == "Bomb"))
	{
		tengah = true;
		j = 0;
		while ((G.B[j].LocX != P.x) && (G.B[j].LocY != P.y)) j++;
		timerbomb = G.B[j].Timer;
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
				timerbomb = G.B[j].Timer;
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
				timerbomb = G.B[j].Timer;
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
				timerbomb = G.B[j].Timer;
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
				timerbomb = G.B[j].Timer;
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
	IsSafe = tengah || kanan || atas || kiri || bawah;
	IsSafe = !(IsSafe);
}

BT BomTengah(GameState& G, Point P, int timerbomb)
{
	BT ret;
	if (timerbomb == 0)
	{
		ret.bolret = false;
		ret.step = 99;
		ret.path = 7;
	}
	else if ((G.GB[P.x][P.y].Entity == "IW") || (G.GB[P.x][P.y].Entity == "DW"))
	{
		ret.bolret = false;
		ret.step = 99;
		ret.path = 7;
	}
	else
	{
		bool issafe;
		int sampah;
		FindSafe(G, P, issafe, sampah);
		if (issafe)
		{
			cout << "ISSAFE" << endl;
			ret.bolret = true;
			ret.step = 1;
			ret.path = 7;
		}
		else
		{
			int * arr;
			BT u, d, l, r;
			arr = new int[5];
			cout << P.x << P.y << endl;
			// cek atas
			P.y = P.y - 1;
			u = BomTengah(G, P, timerbomb - 1);
			arr[1] = u.step;
			P.y = P.y + 1;
			// cek bawah
			P.y = P.y + 1;
			d = BomTengah(G, P, timerbomb - 1);
			arr[4] = d.step;
			P.y = P.y - 1;
			// cek kiri
			P.x = P.x - 1;
			l = BomTengah(G, P, timerbomb - 1);
			arr[2] = l.step;
			P.x = P.x + 1;
			// cek kanan
			P.x = P.x + 1;
			r = BomTengah(G, P, timerbomb - 1);
			arr[3] = r.step;
			P.x = P.x - 1;

			//hasilnya masukin array -> cari minnya -> idnya siapa
			ret.bolret = u.bolret || d.bolret || l.bolret || r.bolret;
			if (ret.bolret)
			{
				ret.path = 1;
				for (int i = 2; i < 5; i++)
				{
					if (arr[ret.path] > arr[i])
					{
						ret.path = i;
					}
				}
				ret.step = arr[ret.path] + 1;
			}
			else
			{
				ret.path = 7;
				ret.step = 99;
			}
		}
	}
	cout << P.x << P.y << ret.bolret << ret.path << timerbomb << endl;
	return (ret);
}


/* untuk aplikasi, bagian cek save
int timbomb;
FindSafe (G,P,save,timbomb);
if (save) {
  //kode vh
  }
  else{
  BT a = BomTengah(G,P, timbomb);
  outfile << a.path << endl;
  }
 */
    
