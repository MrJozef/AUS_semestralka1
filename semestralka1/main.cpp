#include <iostream>
#include <fstream>
#include "../structures/heap_monitor.h" 
#include "const.h"
#include "datum.h"

using namespace std;

int nacitajInt(int poCislo);
void zadavam();

int main()
{
	initHeapMonitor();

	Datum* aktualny_cas = new Datum();

	fstream* subor = new fstream;

	bool pokracovat = true;
	while (pokracovat)
	{
		cout << "1. semestralna praca - Jozef Kubik\n\n";
		cout << "Aktualny cas: " + aktualny_cas->toString() + "\n\n";
		cout << "Menu:\n  1) Posun o hodinu vpred\n  2) Nacitaj data zo suboru\n  3) Uloz aktualny stav do suboru\n  0) Koniec\n\n" << endl;

		switch(nacitajInt(POCET_POLOZIEK_MENU))
		{
		case 1:
			aktualny_cas->dalsiaHodina();
			break;

		case 2:
			(*subor).open(CESTA_K_SUBORU, ios::in);

			if ((*subor).is_open())
			{
				aktualny_cas->fromSubor(subor);

				(*subor).close();
				cout << "Data boli uspesne nacitane!" << endl;
			}
			else
			{
				cout << "Nastala chyba, nie je mozne otvorit subor s ulozenymi datami!" << endl;
			}
			break;
			
		case 3:
			(*subor).open(CESTA_K_SUBORU, ios::out);

			if ((*subor).is_open())
			{
				aktualny_cas->toSubor(subor);

				(*subor).close();
				cout << "Data boli uspesne ulozene do suboru!" << endl;
			}
			else
			{
				cout << "Nastala chyba, nie je mozne pripravit subor na zapis dat!" << endl;
			}
			break;

		case 0:
		default:
			pokracovat = false;
			cout << "\n\n\nKoniec programu - stlacte <Enter> pre ukoncenie" << endl;
		}
	}
	
	cin.get();
	return 0;
}


int nacitajInt(int poCislo)
{
	char cislo;
	cout << "Zadajte cislo od 0 po " + to_string(poCislo) + ":" << endl;
	zadavam();
	cin >> cislo;
	cin.ignore(1000, '\n');

	while (!((cislo - ASCII_CISLO_CISLICA) <= poCislo && (cislo - ASCII_CISLO_CISLICA) >= 0))
	{
		cout << "Zly vstup! Zadajte cislo od 0 po " + to_string(poCislo) + ":" << endl;
		zadavam();
		cin.get(cislo);
		cin.ignore(1000, '\n');
	}

	return cislo - ASCII_CISLO_CISLICA;
}

void zadavam() { cout << ">> "; }