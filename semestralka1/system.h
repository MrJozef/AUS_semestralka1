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
	///<summary>Kon�truktor pri vytv�ran� v programe</summary>
	System();
	///<summary>Kon�truktor pre na��tavanie zo s�boru</summary>
	System(fstream* inSubor);
	~System();
	///<summary>Umo��uje ulo�enie objektu do s�boru</summary>
	void toSubor(fstream* outSubor);

	///<summary>Do stringu ulo�� aktu�lny �as</summary>
	string casToString();
	///<summary>Spust� met�dy dalsiaHodina v prekladisk�ch a vozidl�ch, popr. aj dalsiaNoc</summary>
	void dalsiaHodina();
	void vypisVsetkyVozidla();
	///<summary>Vyp�e drony zo zvolen�ho prekladiska</summary>
	void vypisDrony(int cisPrekladiska);
	///<summary>Vyp�e cel� list z�sielok</summary>
	void vypisVsetkyZasielky();

	///<summary>Prid� z�sielku do syst�mu</summary>
	void vytvorZasielku(double hmotnost, int regZac, int regKon, int regZacVzdialenost, int regKonVzdialenost);
	///<summary>Prid� vozidlo do syst�mu</summary>
	void pridajVozidlo(string spz, int nosnost, double naklady, structures::Array<bool>* trasaVoz);
	///<summary>M� nejak� vozidlo u� t�to �pz?</summary>
	bool overSPZ(string spz);
	///<summary>M� nejak� dron u� toto ��slo v danom prekladisku?</summary>
	bool overSerioveCislo(int serioveCislo, int region);
	///<summary>Prid� z�sielku do dan�ho regi�nu</summary>
	void pridajDron(int cisOkresu, int cis, int typ);

	///<summary>Vr�ti prepraven� z�sielky  - �tatistika �. 5</summary>
	int spocitajPrepravZas();
	///<summary>Vr�ti prepraven� z�sielky - �tatistika �. 6</summary>
	void vypisNalietaneHodiny();
	///<summary>�tatistika �. 4</summary>
	void vypisZrusZasVsetReg(structures::Array<int>* casObd);
	///<summary>�tatistika �. 3</summary>
	void vypisZrusZasVDanomReg(structures::Array<int>* casObd, int region);
	///<summary>�tatistika �. 2</summary>
	void vypisRegionSNajPoslanymiZas(structures::Array<int>* casObd);
	///<summary>�tatistika �. 1</summary>
	void vypisRegionSNajDorucZas(structures::Array<int>* casObd);

private:
	///<summary>Met�du pou��va kon�truktor pri na��tavan� zo s�boru</summary>
	void fromSubor(fstream* inSubor);
	bool nacitajAnoNie();
};