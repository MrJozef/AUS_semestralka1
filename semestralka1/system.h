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
	System();
	System(fstream* inSubor);
	~System();
	void toSubor(fstream* outSubor);

	string casToString();
	void dalsiaHodina();
	void vypisVsetkyVozidla();
	void vypisDrony(int cisPrekladiska);
	void vypisVsetkyZasielky();

	void vytvorZasielku(double hmotnost, int regZac, int regKon, int regZacVzdialenost, int regKonVzdialenost);
	void pridajVozidlo(string spz, int nosnost, double naklady, structures::Array<bool>* trasaVoz);
	bool overSPZ(string spz);
	bool overSerioveCislo(int serioveCislo, int region);
	void pridajDron(int cisOkresu, int cis, int typ);

	int spocitajPrepravZas();
	void vypisNalietaneHodiny();
	void vypisZrusZasVsetReg(structures::Array<int>* casObd);
	void vypisZrusZasVDanomReg(structures::Array<int>* casObd, int region);
	void vypisRegionSNajPoslanymiZas(structures::Array<int>* casObd);
	void vypisRegionSNajDorucZas(structures::Array<int>* casObd);

private:
	void fromSubor(fstream* inSubor);
	bool nacitajAnoNie();
};