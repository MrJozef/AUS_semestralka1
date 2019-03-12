#include <iostream>
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

	bool pokracovat = true;
	while (pokracovat)
	{
		cout << "1. semestralna praca - Jozef Kubik\n\n";
		cout << "Aktualny cas: " + aktualny_cas->toString() + "\n\n";
		cout << "Menu:\n  1) Posun o hodinu vpred\n  2) Pridaj nove vozidlo\n  3) Vypis zoznam vozidiel\n  0) Koniec\n\n" << endl;

		switch(nacitajInt(POCET_POLOZIEK_MENU))
		{
		case 1:
			aktualny_cas->dalsiaHodina();
			break;
		case 2:
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