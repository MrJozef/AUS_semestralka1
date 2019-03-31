#pragma once
#include "../structures/heap_monitor.h"
#include "../structures/list/array_list.h"
#include "../structures/array/array.h"
#include "datum.h"
#include "vozidlo.h"
#include "prekladisko.h"
#include "zasielka.h"


class System
{
private:
	Datum* aktualnyCas_;
	structures::ArrayList<Vozidlo*>* listVozidiel_;
	structures::Array<Prekladisko*>* listPrekladisk_;
	structures::LinkedList<Zasielka*>* listZasielok_;


public:
	///<summary>Konštruktor pri vytváraní v programe</summary>
	System();
	///<summary>Konštruktor pre naèítavanie zo súboru</summary>
	System(fstream* inSubor);
	~System();
	///<summary>Umožòuje uloženie objektu do súboru</summary>
	void toSubor(fstream* outSubor);

	///<summary>Do stringu uloží aktuálny èas</summary>
	string casToString();
	///<summary>Spustí metódy dalsiaHodina v prekladiskách a vozidlách, popr. aj dalsiaNoc</summary>
	void dalsiaHodina();
	void vypisVsetkyVozidla();
	///<summary>Vypíše drony zo zvoleného prekladiska</summary>
	void vypisDrony(int cisPrekladiska);
	///<summary>Vypíše celý list zásielok</summary>
	void vypisVsetkyZasielky();

	///<summary>Pridá zásielku do systému</summary>
	void vytvorZasielku(double hmotnost, int regZac, int regKon, int regZacVzdialenost, int regKonVzdialenost);
	///<summary>Pridá vozidlo do systému</summary>
	void pridajVozidlo(string spz, int nosnost, double naklady, structures::Array<bool>* trasaVoz);
	///<summary>Má nejaké vozidlo už túto špz?</summary>
	bool overSPZ(string spz);
	///<summary>Má nejaký dron už toto èíslo v danom prekladisku?</summary>
	bool overSerioveCislo(int serioveCislo, int region);
	///<summary>Pridá zásielku do daného regiónu</summary>
	void pridajDron(int cisOkresu, int cis, int typ);

	///<summary>Vráti prepravené zásielky  - Štatistika è. 5</summary>
	int spocitajPrepravZas();
	///<summary>Vráti prepravené zásielky - Štatistika è. 6</summary>
	void vypisNalietaneHodiny();
	///<summary>Štatistika è. 4</summary>
	void vypisZrusZasVsetReg(structures::Array<int>* casObd);
	///<summary>Štatistika è. 3</summary>
	void vypisZrusZasVDanomReg(structures::Array<int>* casObd, int region);
	///<summary>Štatistika è. 2</summary>
	void vypisRegionSNajPoslanymiZas(structures::Array<int>* casObd);
	///<summary>Štatistika è. 1</summary>
	void vypisRegionSNajDorucZas(structures::Array<int>* casObd);

private:
	///<summary>Metódu používa konštruktor pri naèítavaní zo súboru</summary>
	void fromSubor(fstream* inSubor);
	bool nacitajAnoNie();
};