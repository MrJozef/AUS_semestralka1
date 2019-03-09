#include <iostream>
#include "../structures/heap_monitor.h" 
#include "const.h"

using namespace std;

int nacitajInt(int poCislo);
void Zadavam();

int main()
{
	initHeapMonitor();

	bool pokracovat = true;
	while (pokracovat)
	{
		cout << "1. semestralna praca - Jozef Kubik\n\n";
		cout << "Menu:\n  0) Koniec\n\n" << endl;

		switch(nacitajInt(POCET_POLOZIEK_MENU))
		{
		case 1:
			//todo prva moznost
			break;
		case 0:
		default:
			cout << "\n\n\nKoniec programu - stlacte <Enter> pre ukoncenie" << endl;
			pokracovat = false;
		}

	}
	
	cin.get();
	return 0;
}


int nacitajInt(int poCislo)
{
	char cislo;
	cout << "Zadajte cislo od 0 po " + to_string(poCislo) + ":" << endl;
	Zadavam();
	cin >> cislo;
	cin.ignore(1000, '\n');

	while (!((cislo - ASCII_CISLO_CISLICA) <= poCislo && (cislo - ASCII_CISLO_CISLICA) >= 0))
	{
		cout << "Zly vstup! Zadajte cislo od 0 po " + to_string(poCislo) + ":" << endl;
		Zadavam();
		cin.get(cislo);
		cin.ignore(1000, '\n');
	}

	return cislo - ASCII_CISLO_CISLICA;
}

void Zadavam() { cout << ">> "; }