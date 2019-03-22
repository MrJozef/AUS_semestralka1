#include <iostream>
#include <fstream>
#include "../structures/heap_monitor.h" 
#include "system.h"

using namespace std;

void zadavam();
void zadavamEnter();
void vypisPrekladiska();
int nacitajCeleKladneCislo();
double nacitajKladneDouble();
bool nacitajAN();
int nacitajCeleKladneCisMensieRovne(int max, bool sNulou);

int main()
{
	initHeapMonitor();

	fstream* subor = new fstream;
	System* AoE = nullptr;

	string pom = "";
	bool pokracovat = true;

	cout << "  ~~  1. semestralna praca - Jozef Kubik  ~~\n\nPrajete si nacitat ulozeny projekt?  [A/N]\n" << endl;
	cout << "(Ak nie, bude otvoreny novy projekt)" << endl;

	if (nacitajAN())
	{
		(*subor).open(CESTA_K_SUBORU, ios::in);

		if ((*subor).is_open())
		{
			AoE = new System(subor);
			(*subor).close();
			cout << "Data boli uspesne nacitane!" << endl;
		}
		else
		{
			cout << "Nastala chyba, nie je mozne otvorit subor s ulozenymi datami!" << endl;
		}
		zadavamEnter();
	}
	else
	{
		AoE = new System();
	}

	while (pokracovat)
	{
		system("cls");				//todo  toto odstranit?
		cout << "  ~~  1. semestralna praca - Jozef Kubik  ~~\n\n";
		cout << "Aktualny cas v systeme: " + AoE->casToString() + "\n\n";
		cout << "Menu:\n  1) Posun o hodinu vpred\n  2) Uloz aktualny stav" << endl;
		cout << "  3) Pridaj vozidlo\n  4) Vypis vsetky vozidla\n  5) Pridaj dron\n  6) Vypis drony z prekladiska" << endl;
		cout << "  7) Vytvor objednavku\n  8) Vypis vsetky zasielky" << endl;
		cout << "  0) Koniec\n\n" << endl;

		switch(nacitajCeleKladneCisMensieRovne(POCET_POLOZIEK_MENU, true))
		{
		case 1:
			AoE->dalsiaHodina();
			break;
			
		case 2:
			(*subor).open(CESTA_K_SUBORU, ios::out);

			if ((*subor).is_open())
			{
				AoE->toSubor(subor);

				(*subor).close();
				cout << "Data boli uspesne ulozene do suboru!" << endl;
			}
			else
			{
				cout << "Nastala chyba, nie je mozne pripravit subor na zapis dat!" << endl;
			}
			cin.ignore();
			zadavamEnter();
			break;

		case 3:
			structures::Array<bool>* trasaVoz;

			cout << "Zadajte SPZ noveho vozidla:" << endl;
			zadavam();
			cin >> pom;

			if (AoE->overSPZ(pom))
			{
				cout << "Chyba - Zadana SPZ uz existuje!" << endl;
				cin.ignore();
			}
			else
			{
				int nosnost;
				double nakladyNaRegion;
				int cisReg;

				cout << "Zadajte nosnost vozidla (v kg):" << endl;
				nosnost = nacitajCeleKladneCislo();

				cout << "Zadajte naklady na prevadzku (v Eurach na region):" << endl;
				nakladyNaRegion = nacitajKladneDouble();

				cout << "Vlozte trasu vozidla zadanim cisiel prislusnych regionov:\n(Zadavanie ukoncite prekladiskom v ZA)\n" << endl;
				vypisPrekladiska();
				trasaVoz = new structures::Array<bool>(POCET_REGIONOV);
				(*trasaVoz)[7] = true;			//ZA je vzdy True
				cisReg = nacitajCeleKladneCisMensieRovne(POCET_REGIONOV, false);

				while(cisReg != 8)
				{
					cisReg--;
					(*trasaVoz)[cisReg] = true;
					cisReg = nacitajCeleKladneCisMensieRovne(POCET_REGIONOV, false);
				}

				AoE->pridajVozidlo(pom, nosnost, nakladyNaRegion, trasaVoz);
			}

			trasaVoz = nullptr;		//smernik si bude pametat novovytvorene vozidlo
			zadavamEnter();
			break;

		case 4:
			AoE->vypisVsetkyVozidla();
			cin.ignore();
			zadavamEnter();
			break;

		case 5:
			int cis;
			int typ;
			int cisOkresu;

			cout << "Zvolte prekladisko, do ktoreho chcete priradit novy dron zadanim prislusneho cisla:" << endl;
			vypisPrekladiska();
			cisOkresu = nacitajCeleKladneCisMensieRovne(POCET_REGIONOV, false) - 1;//nakolko cislovanie zacina od 1, ale pole od indexu 0			

			cout << "Zadajte seriove cislo dronu:" << endl;
			cis = nacitajCeleKladneCislo();
			cout << "Zadajte typ dronu:" << endl;
			typ = nacitajCeleKladneCisMensieRovne(POCET_TYPOV_DRONOV, false);


			AoE->pridajDron(cisOkresu, cis, typ);
			zadavamEnter();
			break;

		case 6:
			cout << "Zvolte prekladisko zadanim zodpovedajuceho cisla:\n" << endl;
			vypisPrekladiska();

			AoE->vypisDrony(nacitajCeleKladneCisMensieRovne(POCET_REGIONOV, false) - 1);		//nakolko cislovanie zacina od 1, ale pole od indexu 0
			cin.ignore();
			zadavamEnter();
			break;

		case 7:
			double hmotnostZas;
			int regZac;
			int regZacVzdialenost;
			int regKon;
			int regKonVzdialenost;

			cout << "Zadajte hmotnost zasielky (v kg):" << endl;
			hmotnostZas = nacitajKladneDouble();

			cout << "Zadajte region, v ktorom ma dron vyzdvihnut zasielku, zadanim prislusneho cisla:" << endl;
			vypisPrekladiska();
			regZac = nacitajCeleKladneCisMensieRovne(POCET_REGIONOV, false) - 1;

			cout << "Zadajte vzdialenost zasielky od prekladiska (v km):" << endl;
			regZacVzdialenost = nacitajCeleKladneCislo();

			cout << "Zadajte region, kam ma byt zasielka dorucena, zadanim prislusneho cisla:" << endl;
			vypisPrekladiska();
			regKon = nacitajCeleKladneCisMensieRovne(POCET_REGIONOV, false) - 1;

			cout << "Zadajte vzdialenost miesta dorucenia od prekladiska (v km):" << endl;
			regKonVzdialenost = nacitajCeleKladneCislo();

			AoE->vytvorZasielku(hmotnostZas, regZac, regKon, regZacVzdialenost, regKonVzdialenost);
			break;

		case 8:
			AoE->vypisVsetkyZasielky();
			cin.ignore();
			zadavamEnter();
			break;
		case 0:
		default:
			pokracovat = false;
			cout << "\n\n\nKoniec programu - stlacte <Enter> pre ukoncenie" << endl;
		}
	}

	delete AoE;
	delete subor;

	cin.get();
	return 0;
}


void zadavam() { cout << ">> "; }

void zadavamEnter() { cout << "Pre pokracovanie stlacte <Enter>\n"; cin.ignore(); zadavam(); }

void vypisPrekladiska()
{
	cout << "MA - 1\t\tBA - 2\t\tTT - 3\t\tTN - 4\nNR - 5\t\tKN - 6\t\tCA - 7\t\tZA - 8\nMT - 9\t\tPD - 10\t\tLV - 11\t\t"
	<< "NO - 12\nLM - 13\t\tBB - 14\t\tZV - 15\t\tKA - 16\nLC - 17\t\tPP - 18\t\tRA - 19\t\tSL - 20\nSN - 21\t\tPO - 22\t\t"
	<< "KE - 23\t\tHE - 24\nMI - 25" << endl;
}


int nacitajCeleKladneCisMensieRovne(int max, bool sNulou)
{
	int cislo = nacitajCeleKladneCislo();

	while ((cislo > max) || (!sNulou && cislo <= 0))
	{

		cout << "Zadane cislo nie je mensie, rovne " << to_string(max) << " a sucasne vacsie";
		if(sNulou)
		{
			cout << ", rovne 0!" << endl;
		}
		else
		{
			cout << " ako 0!" << endl;
		}
		
		cislo = nacitajCeleKladneCislo();
	}

	return cislo;
}

int nacitajCeleKladneCislo()
{
	bool zlyVstup = false;
	string pom;
	cout << "Zadajte cele, kladne cislo:" << endl;
	zadavam();
	cin >> pom;

	for (unsigned int i = 0; i < pom.size(); i++)
	{
		if (!(pom[i] >= '0' && pom[i] <= '9'))
		{
			zlyVstup = true;
			break;
		}
	}

	while (zlyVstup)
	{
		cout << "Zly vstup! Zadajte cele, kladne cislo:" << endl;
		zadavam();
		cin >> pom;
		zlyVstup = false;

		for (unsigned int i = 0; i < pom.size(); i++)
		{
			if (!(pom[i] >= '0' && pom[i] <= '9'))
			{
				zlyVstup = true;
				break;
			}
		}
	}

	return stoi(pom);
}

double nacitajKladneDouble()
{
	bool zlyVstup = false;
	string pom;
	cout << "Zadajte desatinne cislo (pouzite des. bodku):" << endl;
	zadavam();
	cin >> pom;

	for (unsigned int i = 0; i < pom.size(); i++)
	{
		if (!((pom[i] >= '0' && pom[i] <= '9') || pom[i] == '.'))
		{
			zlyVstup = true;
			break;
		}
	}

	while (zlyVstup)
	{
		cout << "Zly vstup! Zadajte desatinne cislo (pouzite des. bodku):" << endl;
		zadavam();
		cin >> pom;
		zlyVstup = false;

		for (unsigned int i = 0; i < pom.size(); i++)
		{
			if (!((pom[i] >= '0' && pom[i] <= '9') || pom[i] == '.'))
			{
				zlyVstup = true;
				break;
			}
		}
	}

	return stod(pom);
}

bool nacitajAN()
{
	char pismeno;
	while (true)
	{
		zadavam();
		cin >> pismeno;
		if (pismeno == 'A' || pismeno == 'a')
		{
			return true;
		}
		else
		{
			if (pismeno == 'N' || pismeno == 'n')
			{
				return false;
			}
			else
			{
				cout << "Zly vstup! Stlacte klavesu A alebo N\n" << endl;
			}
		}
	}
}