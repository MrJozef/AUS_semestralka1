#include <iostream>
#include <fstream>
#include "../structures/heap_monitor.h" 
#include "../structures/list/array_list.h"
#include "../structures/array/array.h"
#include "datum.h"
#include "vozidlo.h"
#include "prekladisko.h"

using namespace std;

int nacitajInt(int poCislo);
void zadavam();
void zadavamEnter();
void vypisPrekladiska();

int main()
{
	initHeapMonitor();

	Datum* aktualny_cas = new Datum();
	fstream* subor = new fstream;

	structures::ArrayList<Vozidlo*>* listVozidiel = new structures::ArrayList<Vozidlo*>();

	structures::Array<Prekladisko*>* prekladiska = new structures::Array<Prekladisko*>(POCET_REGIONOV);
	for(int i = 0; i < POCET_REGIONOV; i++)
	{
		(*prekladiska)[i] = new Prekladisko();
	}

	string pom = "";
	bool pokracovat = true;
	while (pokracovat)
	{
		system("cls");				//todo  toto odstranit?
		cout << "1. semestralna praca - Jozef Kubik\n\n";
		cout << "Aktualny cas: " + aktualny_cas->toString() + "\n\n";
		cout << "Menu:\n  1) Posun o hodinu vpred\n  2) Nacitaj data zo suboru\n  3) Uloz aktualny stav do suboru" << endl;
		cout << "  4) Pridaj vozidlo\n  5) Vypis vsetky vozidla\n  6) Pridaj dron\n  7) Vypis drony z prekladiska" << endl;
		cout << "  8) Vytvor objednavku" << endl;
		cout << "  0) Koniec\n\n" << endl;

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

				int pocVoz;
				*subor >> pocVoz;
				for (int i = 0; i < pocVoz; i++)
				{
					listVozidiel->add(new Vozidlo(subor));
				}

				for (int i = 0; i < POCET_REGIONOV; i++)
				{
					(*prekladiska)[i]->fromSubor(subor);
				}

				(*subor).close();
				cout << "Data boli uspesne nacitane!" << endl;
			}
			else
			{
				cout << "Nastala chyba, nie je mozne otvorit subor s ulozenymi datami!" << endl;
			}
			zadavamEnter();
			break;
			
		case 3:
			(*subor).open(CESTA_K_SUBORU, ios::out);

			if ((*subor).is_open())
			{
				aktualny_cas->toSubor(subor);
				*subor << listVozidiel->size() << "\n";
				for (Vozidlo* voz : *listVozidiel)
				{
					voz->toSubor(subor);
				}

				for (int i = 0; i < POCET_REGIONOV; i++)
				{
					(*prekladiska)[i]->toSubor(subor);
				}

				(*subor).close();
				cout << "Data boli uspesne ulozene do suboru!" << endl;
			}
			else
			{
				cout << "Nastala chyba, nie je mozne pripravit subor na zapis dat!" << endl;
			}
			zadavamEnter();
			break;

		case 4://todo osetrit nacitavanie udajov pre vozidla
			int nosnost;
			double nakladyNaRegion;
			bool zhodnaSPZ;
			zhodnaSPZ = false;
			Vozidlo* pomVoz;
			int cisReg;

			cout << "Zadajte SPZ noveho vozidla:" << endl;
			zadavam();
			cin >> pom;

			for (Vozidlo* voz : *listVozidiel)
			{
				if (voz->getSPZ() == pom)
				{
					zhodnaSPZ = true;
					break;
				}
			}

			if (zhodnaSPZ)
			{
				cout << "Chyba - Zadana SPZ uz existuje!" << endl;
				cin.ignore();
			}
			else
			{
				cout << "Zadajte nosnost vozidla (v tonach):" << endl;
				zadavam();
				cin >> nosnost;
				cout << "Zadajte naklady na prevadzku (v Eurach na region):" << endl;
				zadavam();
				cin >> nakladyNaRegion;

				pomVoz = new Vozidlo(pom, nosnost, nakladyNaRegion, aktualny_cas);
				listVozidiel->add(pomVoz);

				cout << "Vlozte trasu vozidla zadanim cisiel prislusnych regionov:\n(Zadavanie ukoncite prekladiskom v ZA)\n" << endl;
				vypisPrekladiska();

				zadavam();
				cin >> cisReg;
				
				while(cisReg != 8)
				{
					cisReg--;
					pomVoz->priradRegion(cisReg);
					zadavam();
					cin >> cisReg;
				}

			}

			zadavamEnter();
			break;

		case 5:
			cout << "Zoznam vsetkych vozidiel firmy:\n" << endl;

			if (listVozidiel->size() == 0)
			{
				cout << "  -  Aktualne firma nema ziadne vozidla  -\n" << endl;
			}
			else
			{
				for (Vozidlo* voz : *listVozidiel)
				{
					cout << voz->toString();
				}
			}
			zadavamEnter();
			break;

		case 6://todo osetrit vstupy
			int cisOkresu;
			int cis;
			int typ;

			cout << "Zvolte prekladisko, do ktoreho chcete priradit novy dron zadanim prislusneho cisla:" << endl;
			vypisPrekladiska();
			zadavam();
			cin >> cisOkresu;

			cout << "Zadajte seriove cislo dronu:" << endl;
			zadavam();
			cin >> cis;
			cout << "Zadajte typ dronu:" << endl;
			zadavam();
			cin >> typ;

			(*prekladiska)[cisOkresu - 1]->pridajDron(cis, typ, aktualny_cas); //nakolko cislovanie zacina od 1, ale pole od indexu 0
			zadavamEnter();
			break;

		case 7:
			int cisloOkresu;

			cout << "Zvolte prekladisko zadanim zodpovedajuceho cisla:\n" << endl;
			vypisPrekladiska();
			zadavam();
			cin >> cisloOkresu;//todo osetrit

			(*prekladiska)[cisloOkresu - 1]->vypisDrony(); //nakolko cislovanie zacina od 1, ale pole od indexu 0
			cin.ignore();
			zadavamEnter();
			break;

		case 8://todo aj toto osetrit
			double hmotnostZas;
			int regZac;
			int regKon;
			int regZacVzdialenost;
			int regKonVzdialenost;

			cout << "Zadajte hmotnost zasielky:" << endl;
			zadavam();
			cin >> hmotnostZas;

			cout << "Zadajte region, v ktorom ma dron vyzdvihnut zasielku, zadanim prislusneho cisla:" << endl;
			vypisPrekladiska();
			zadavam();
			cin >> regZac;
			cout << "Zadajte vzdialenost zasielky od prekladiska:" << endl;
			zadavam();
			cin >> regZacVzdialenost;

			cout << "Zadajte region, kam ma byt zasielka dorucena, zadanim prislusneho cisla:" << endl;
			vypisPrekladiska();
			zadavam();
			cin >> regKon;
			cout << "Zadajte vzdialenost miesta dorucenia od prekladiska:" << endl;
			zadavam();
			cin >> regKonVzdialenost;


			break;

		case 0:
		default:
			pokracovat = false;
			cout << "\n\n\nKoniec programu - stlacte <Enter> pre ukoncenie" << endl;
		}
	}

	delete aktualny_cas;
	delete subor;

	for(Vozidlo* voz : *listVozidiel)
	{
		delete voz;
	}
	delete listVozidiel;

	for(int i = 0; i < POCET_REGIONOV; i++)
	{
		 delete (*prekladiska)[i];
	}
	delete prekladiska;

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

void zadavamEnter() { cout << "Pre pokracovanie stlacte <Enter>\n"; cin.ignore(); zadavam(); }

void vypisPrekladiska()
{
	cout << "MA - 1\t\tBA - 2\t\tTT - 3\t\tTN - 4\nNR - 5\t\tKN - 6\t\tCA - 7\t\tZA - 8\nMT - 9\t\tPD - 10\t\tLV - 11\t\t"
	<< "NO - 12\nLM - 13\t\tBB - 14\t\tZV - 15\t\tKA - 16\nLC - 17\t\tPP - 18\t\tRA - 19\t\tSL - 20\nSN - 21\t\tPO - 22\t\t"
	<< "KE - 23\t\tHE - 24\nMI - 25" << endl;
}