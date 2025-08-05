#include <iostream>
#include <string>
#include <ctime>

using namespace std;
char jelek[6] = { 'A','B','C','D','E', '_'};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

struct pile {
	std::string nev = "pile";
	int suly = 0;
	int tartalom[5] = { 0,0,0,0,0 };
	bool kezdo = false;
};

struct player {
	std::string nev = "player";

	pile padlo;
	pile mintasorok[5];
	pile kez;
	pile fal;

	int mozaik[5][5];
	int pontszam = 0;
	bool gyoztes = false;
};

struct game {
	pile zsak;
	pile dobott;
	pile kozos;

	pile* korongok = NULL;
	player* jatekosok = NULL;

	int jatekosszam = 0;
	int korongdb = 0;
	int suly = 0;
};

/*--------------------------------------------------------------------Interaction---------------------------------------------------------------------*/

void Quit(game &jatek) {
	cout << "kilepes....\n";
	delete[] jatek.korongok;
	delete[] jatek.jatekosok;
	exit(EXIT_SUCCESS);
}

char Request(std::string uzenet, char mettol, char meddig) {
	char ans = 'n';
	do {
		cout << uzenet;
		cin >> ans;

		if (ans == 'n') { return ans; }

	} while (ans > meddig || ans < mettol);
	return ans;
};

/*--------------------------------------------------------------------Initialize---------------------------------------------------------------------*/

void PileInit(pile& pile, int kupac, const std::string& nev, bool kezdo) {
	pile.suly = 0;

	for (int i = 0; i < 5; i++) {
		pile.tartalom[i] = kupac;
		pile.suly += kupac;
	}
	pile.nev = nev;
	pile.kezdo = kezdo;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

pile* DiscInit(int korongdb) {

	pile* korongok = new pile[korongdb];

	for (int i = 0; i < korongdb; i++) {
		PileInit(korongok[i], 0, "korong " + std::to_string(i + 1), false);
	};

	return korongok;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

player* PlayerInit(int jatekosszam) {
	player* jatekosok = new player[jatekosszam+1];

	jatekosok[0].nev = "csere";

	for (int s = 0; s < 5; s++) {
		PileInit(jatekosok[0].mintasorok[s], 0, std::to_string(s + 1), false);

		for (int o = 0; o < 5; o++) {
			jatekosok[0].mozaik[s][o] = 5;
		}
	}

	PileInit(jatekosok[0].padlo, 0, "padlo", false);
	PileInit(jatekosok[0].kez, 0, "kez", false);
	jatekosok[0].pontszam = 0;
	jatekosok[0].gyoztes = false;



	for (int p = 1; p < jatekosszam+1; p++) {
		cout << "jatekos neve?	";
		cin >> jatekosok[p].nev;

		for (int s = 0; s < 5; s++) {
			PileInit(jatekosok[p].mintasorok[s], 0, std::to_string(s + 1), false);

			for (int o = 0; o < 5; o++) {
				jatekosok[p].mozaik[s][o] = 5;
			}
		}

		PileInit(jatekosok[p].padlo, 0, "padlo", false);
		PileInit(jatekosok[p].kez, 0, "kez", false);
		jatekosok[p].pontszam = 0;
		jatekosok[p].gyoztes = false;
	}
	return jatekosok;
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void GameInit(game& jatek) {

	char ans;
	do {
		ans = Request("Jatekosok szama?:	", '2', '4');
	} while (ans == 'n');


	jatek.jatekosszam = ans - '0';
	jatek.suly = 0;

	jatek.jatekosok = PlayerInit(jatek.jatekosszam);

	jatek.korongdb = jatek.jatekosszam * 2 + 1;
	jatek.korongok = DiscInit(jatek.korongdb);

	/*------------------------------------*/

	PileInit(jatek.zsak, 20, "zsak", false);
	PileInit(jatek.kozos, 0, "kozos", true);
	PileInit(jatek.dobott, 0, "dobott", false);
}

/*-------------------------------------------------------------------Support functions---------------------------------------------------------------*/

bool CheckPlace(player jatekos, int s, int o, int ID) {
	bool correct = true;
	int sor;
	int oszlop;

	correct = jatekos.mozaik[s][o] == 5;

	sor = 0;
	oszlop = 0;

	while (correct && sor < 5) {
		correct = jatekos.mozaik[sor][o] != ID;
		sor++;
	};

	while (correct && oszlop < 5) {
		correct = jatekos.mozaik[s][oszlop] != ID;
		oszlop++;
	};

	return correct;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

int ColorToID(char szin) {
	int ID = -1;
	do {
		ID++;
	} while (szin != jelek[ID] && ID < 5);
	return ID;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

int FindContent(pile pile) {
	int ID = -1;

	do {
		ID++;
	} while (pile.tartalom[ID] == 0);
	return ID;
};


/*-------------------------------------------------------------------Display Functions--------------------------------------------------------------*/

void Fill(pile pile, int meddig) {
	int n = meddig - pile.suly;
	if (pile.kezdo) { n--; }
	for (int i = 0; i < n; i++) {
		cout << jelek[5] << ' ';
	}
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void PileDisplay(pile pile) {

	cout << pile.nev << ": ";
	if (pile.kezdo) { cout << "K "; }
	for (int ID = 0; ID < 5; ID++) {
		for (int i = 1; i <= pile.tartalom[ID]; i++) {
			cout << jelek[ID] << ' ';
		}
	}
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void BoardDisplay(game jatek) {
	cout << "\n\n";
	for (int i = 0; i < jatek.korongdb; i++) {
		PileDisplay(jatek.korongok[i]);
		cout << "\n";
	}
	PileDisplay(jatek.kozos);
	cout << "\n";

	cout << "\n\n";
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void PlayerDisplay(player jatekos) {

	cout << jatekos.nev << "	";
	PileDisplay(jatekos.kez);
	cout << '\n';

	cout << "1 2 3 4 5   Pontok: " << jatekos.pontszam << '\n';

	for (int s = 0; s < 5; s++) {
		for (int o = 0; o < 5; o++) {
			cout << jelek[jatekos.mozaik[s][o]] << ' ';
		}
		PileDisplay(jatekos.mintasorok[s]);
		Fill(jatekos.mintasorok[s], s + 1);
		cout << '\n';
	}

	cout << "\n  ";
	PileDisplay(jatekos.padlo);
	Fill(jatekos.padlo, 7);
	cout << '\n';
	cout << "levonas: 1 1 2 2 2 3 3 \n\n\n";
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void GameDisplay(game jatek) {
	for (int i = 0; i < 10; i++) {
		cout << "\n\n";
	}
	BoardDisplay(jatek);
	for (int p = 1; p < jatek.jatekosszam + 1; p++) {
		PlayerDisplay(jatek.jatekosok[p]);
	}
};

/*------------------------------------------------------------------Scoring Functions----------------------------------------------------------------*/

int Score(player jatekos, int sor, int oszlop) {
	int pont = 0;
	int opont = 0;
	int spont = 0;
	int i;

	i = 1;
	while (oszlop + i < 5 && jatekos.mozaik[sor][oszlop + i] != 5) {
		spont++;
		i++;
	}

	i = 1;
	while (oszlop - i > -1 && jatekos.mozaik[sor][oszlop - i] != 5) {
		spont++;
		i++;
	}

	if (spont != 0) { spont++; }


	i = 1;
	while (sor + i < 5 && jatekos.mozaik[sor + i][oszlop] != 5) {
		opont++;
		i++;
	}

	i = 1;
	while (sor - i > -1 && jatekos.mozaik[sor - i][oszlop] != 5) {
		opont++;
		i++;
	}

	if (opont != 0) { opont++; }

	pont = spont + opont;
	if (pont == 0) { pont = 1; }

	return pont;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

int FullRows(player jatekos) {
	int sorok = 0;
	for (int s = 0; s < 5; s++) {
		int o = -1;
		do {
			o++;
		} while (jatekos.mozaik[s][o] != 5 && o != 5);

		if (o == 5) {
			sorok++;
		}
	}
	return sorok;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

int FullColumns(player jatekos) {
	int oszlopok = 0;
	for (int o = 0; o < 5; o++) {
		int s = -1;
		do {
			s++;
		} while (jatekos.mozaik[s][o] != 5 && s != 5);

		if (s == 5) {
			oszlopok++;
		}
	}
	return oszlopok;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void PenaltyPoints(player& jatekos) {
	int n = jatekos.padlo.suly;
	if (jatekos.padlo.kezdo) {
		n++;
	}

	for (int i = 1; i < n + 1; i++) {
		jatekos.pontszam = jatekos.pontszam - (i / 3 + 1);
	}
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void BonusPoints(player& jatekos) {
	int pont = FullRows(jatekos) * 5 + FullColumns(jatekos) * 7;
	for (int ID = 0; ID < 5; ID++) {
		pont += (jatekos.fal.tartalom[ID] / 5) * 10;
	}
	jatekos.pontszam += pont;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void Winner(game jatek) {
	int n = jatek.jatekosszam + 1;
	int maxpont = 0;

	for (int p = 1; p < n; p++) {
		if (jatek.jatekosok[p].pontszam * 10 + FullRows(jatek.jatekosok[p]) > maxpont) {
			maxpont = jatek.jatekosok[p].pontszam * 10 + FullRows(jatek.jatekosok[p]);
		}
	}

	for (int p = 1; p < n; p++) {
		if ((jatek.jatekosok[p].pontszam * 10 + FullRows(jatek.jatekosok[p])) == maxpont) {
			jatek.jatekosok[p].gyoztes = true;
		}
		else {
			jatek.jatekosok[p].gyoztes = false;
		}
	}

	cout << "nyert:	";
	for (int p = 1; p < n; p++) {
		if (jatek.jatekosok[p].gyoztes) {
			cout << jatek.jatekosok[p].nev;
		}
	}
	cout << "\n\n";
};

/*------------------------------------------------------------------Placement Functions---------------------------------------------------------------*/

void Transfer(pile& pilefrom, pile& pileto) {
	pileto.suly += pilefrom.suly;
	pilefrom.suly = 0;

	for (int ID = 0; ID < 5; ID++) {
		pileto.tartalom[ID] += pilefrom.tartalom[ID];
		pilefrom.tartalom[ID] = 0;
	}

	if (pilefrom.kezdo) {
		pilefrom.kezdo = false;
		pileto.kezdo = true;
	}
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void DrawAll(pile& pilefrom, pile& pileto, int ID) {
	pilefrom.suly -= pilefrom.tartalom[ID];
	pileto.suly += pilefrom.tartalom[ID];

	pileto.tartalom[ID] = pilefrom.tartalom[ID];
	pilefrom.tartalom[ID] = 0;

	if (pilefrom.kezdo) {
		pilefrom.kezdo = false;
		pileto.kezdo = true;
	}
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

int Draw(pile &pilefrom,pile &pileto) {
	int ID = -1;

	if (pilefrom.suly != 0) {

		int r = rand() % pilefrom.suly + 1;
		int osszeg = 0;

		do {
			ID++;
			osszeg = osszeg + pilefrom.tartalom[ID];
		} while (osszeg < r);

		pilefrom.tartalom[ID]--;
		pilefrom.suly--;

		pileto.tartalom[ID]++;
		pileto.suly++;
	}
	return ID;
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void Choose(game& jatek, int p) {
	char hely = '0';
	char szin = '0';
	int ID;
	bool correct = false;

	cout << '\n';

	do {
		do {
			cout << jatek.jatekosok[p].nev;
			hely = Request(" honnan veszel?	", '0', jatek.korongdb + '0');

			cout << jatek.jatekosok[p].nev;
			szin = Request(" mit veszel?	", jelek[0], jelek[4]);

		} while (hely == 'n' || szin == 'n');

		hely = hely - '0';

		ID = ColorToID(szin);

		if (hely == 0) {
			correct = jatek.kozos.tartalom[ID] > 0;
		}
		else {
			correct = jatek.korongok[hely - 1].tartalom[ID] > 0;
		}

	} while (!correct);

	if (hely == 0) {
		DrawAll(jatek.kozos, jatek.jatekosok[p].kez, ID);
	}
	else {
		DrawAll(jatek.korongok[hely - 1], jatek.jatekosok[p].kez, ID);
		Transfer(jatek.korongok[hely - 1], jatek.kozos);
	}
	jatek.suly -= jatek.jatekosok[p].kez.suly;

	cout << '\n';
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void PlaceOnFloor(game& jatek, int p) {
	if (jatek.jatekosok[p].kez.kezdo) {
		jatek.jatekosok[p].padlo.kezdo = true;
		jatek.jatekosok[p].kez.kezdo = false;
	}

	int n = 7 - jatek.jatekosok[p].padlo.suly;
	if (jatek.jatekosok[p].padlo.kezdo) { n--; }

	for (int i = 0; i < n; i++) {
		Draw(jatek.jatekosok[p].kez, jatek.jatekosok[p].padlo);
	}
	Transfer(jatek.jatekosok[p].kez, jatek.dobott);
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void PlaceInRow(game& jatek, int p) {
	char ans = 'n';
	int hely = 1;
	int sor = 0;
	bool correct = false;
	int ID;
	int n;

	do {

		do {
			cout << jatek.jatekosok[p].nev;
			ans = Request(" hova teszed?	", '0', '5');

		} while (ans == 'n');

		hely = ans - '0';
		sor = hely - 1;

		n = jatek.jatekosok[p].mintasorok[sor].suly;
		ID = FindContent(jatek.jatekosok[p].kez);

		if (n == 0) {

			int o = -1;
			do {
				o++;
				correct = CheckPlace(jatek.jatekosok[p], sor, o, ID);
			} while (!correct && o < 5);
		}
		else {
			correct = FindContent(jatek.jatekosok[p].mintasorok[sor]) == ID;
		}
	} while (!correct && hely != 0);

	if (hely == 0) {
		PlaceOnFloor(jatek, p);
	}
	else {

		for (int s = 0; s < sor - n + 1; s++) {
			Draw(jatek.jatekosok[p].kez, jatek.jatekosok[p].mintasorok[sor]);
		}
		PlaceOnFloor(jatek, p);
	}
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void PlaceOnWall(game& jatek, int s, int p) {
	bool jooszlopok[5] = { false,false,false,false,false };
	int ID;
	int oszlop;
	char ans;

	if (jatek.jatekosok[p].mintasorok[s].suly == s + 1) {

		ID = FindContent(jatek.jatekosok[p].mintasorok[s]);

		for (int o = 0; o < 5; o++) {
			jooszlopok[o] = CheckPlace(jatek.jatekosok[p], s, o, ID);
		}

		do {
			do {
				cout << jatek.jatekosok[p].nev << " " << s + 1;
				ans = Request(".sorbol lerakhatod... hova teszed?	", '1', '5');
			} while (ans == 'n');

			oszlop = ans - '1';
		} while (!jooszlopok[oszlop]);

		jatek.jatekosok[p].mozaik[s][oszlop] = Draw(jatek.jatekosok[p].mintasorok[s], jatek.jatekosok[p].fal);

		jatek.jatekosok[p].pontszam += Score(jatek.jatekosok[p], s, oszlop);

		Transfer(jatek.jatekosok[p].mintasorok[s], jatek.dobott);
		GameDisplay(jatek);
	}
};

/*-----------------------------------------------------------------------New Round--------------------------------------------------------------------*/

void ChangePlayerOrder(game& jatek) {
	int p = 1;

	while (!jatek.jatekosok[p].padlo.kezdo) {
		p++;
	}

	int n = jatek.jatekosszam - p + 1;

	for (int i = 0; i < n; i++) {
		jatek.jatekosok[0] = jatek.jatekosok[jatek.jatekosszam];
		for (int j = jatek.jatekosszam; j > 0; j--) {
			jatek.jatekosok[j] = jatek.jatekosok[j - 1];
		}
	}
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void NewRound(game& jatek) {

	jatek.dobott.kezdo = false;
	jatek.kozos.kezdo = true;

	for (int i = 0; i < jatek.korongdb; i++) {
		for (int j = 0; j < 4; j++) {

			if (jatek.zsak.suly == 0) {
				Transfer(jatek.dobott, jatek.zsak);
			}

			Draw(jatek.zsak, jatek.korongok[i]);
		}
	}
	jatek.suly = jatek.korongdb * 4;
};

/*--------------------------------------------------------------------Main----------------------------------------------------------------------------*/

int main(){

	srand((unsigned)time(NULL));

	char valasz = 'i';
	int p;
	int sorok = 0;

	game jatek;
	GameInit(jatek);

	/*forduló----------------------------------------------------------------------------------------------------------------------------------------------------*/
	do {
		NewRound(jatek);
		GameDisplay(jatek);
		int p = 1;

		do {
			if (p == jatek.jatekosszam+1) {
				p = 1;
			}
			Choose(jatek, p);
			GameDisplay(jatek);
			PlaceInRow(jatek, p);
			GameDisplay(jatek);
			p++;
		} while (jatek.suly != 0);

		for (p = 1; p < jatek.jatekosszam+1; p++) {
			for (int i = 0; i < 5; i++) {
				PlaceOnWall(jatek,i,p);
			}	
		}

		ChangePlayerOrder(jatek);
		for (p = 1; p < jatek.jatekosszam+1; p++) {
			PenaltyPoints(jatek.jatekosok[p]);
			Transfer(jatek.jatekosok[p].padlo, jatek.dobott);
		}

		p = 0;
		sorok = 0;
		do {
			p++;
			sorok = FullRows(jatek.jatekosok[p]);
		} while (sorok == 0 && p != jatek.jatekosszam);

		if (sorok == 0) {
			cout << "Uj fordulo? i/n:	";
			cin >> valasz;
		}

	} while (valasz == 'i' && sorok == 0);

	for (p = 1; p < jatek.jatekosszam+1; p++) {
		BonusPoints(jatek.jatekosok[p]);
	}

	GameDisplay(jatek);
	Winner(jatek);
	/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/

	Quit(jatek);
	return 0;
};
