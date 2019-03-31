#pragma once
#include "../structures/heap_monitor.h"
#include "../structures/queue/explicit_queue.h"
#include "datum.h"
#include "ramecRozvrhu.h"

class Dron
{
private:
	Datum* zarDoEvidencie_;
	int serioveCislo_;
	short typ_;
	int nalietMinuty_;
	int prepravZasielky_;

	///<summary>Nabitie je v %</summary>
	int nabitie_;
	///<summary>Stav urèuje èi dron aktuálne (na prelome hodín) pracuje, je prázdny alebo sa nabíja</summary>
	StavDronu stav_;
	///<summary>Rozvrh dnešnho dòa</summary>
	structures::ExplicitQueue<RamecRozvrhu*>* rozvrh_;
	int minutyZaneprazd_;

	///<summary>Pripravenı rozvrh na zajtrajší deò</summary>
	structures::ExplicitQueue<RamecRozvrhu*>* buduciRozvrh_;
	///<summary>Potrebné pre plánovanie zajtrajška - 5e</summary>
	int buduceNabitie_;
	///<summary>Potrebné pre plánovanie zajtrajška - 5e</summary>
	int buduceMinutyZaneprazd_;

public:
	///<summary>Konštruktor pri vytváraní v programe</summary>
	Dron(int cislo, int typ, Datum* zaradenie);
	///<summary>Konštruktor pre naèítavanie zo súboru</summary>
	Dron(fstream* inSubor);
	~Dron();

	string toString();
	///<summary>Umoòuje uloenie objektu do súboru</summary>
	void toSubor(fstream* outSubor);

	int dajTyp();
	StavDronu dajStav();
	int dajNabitie();
	int dajSerioveCislo();
	///<summary>Vráti true/false podla toho, èi doletí na danú vzdialenos</summary>
	bool doletis(int vzdialenost);

	int dajMinutyZaneprazd();
	///<summary>Vráti èas vyzdvihnutia</summary>
	int overCasVyzdvihnutia(int vzdialenost, Transport trans);
	///<summary>Vrati celkovy cas vyzdvihnutia zasielky + vratenie sa do prekladiska</summary>
	int transportujZasielku(int vzdialenost);
	///<summary>Mení èasovı rozvrh a aktualizuje potrebné attr</summary>
	void dalsiaHodina();
	///<summary>Predovšetkım vymieòa budúci rozvrh za súèasnı a pripraví sa na jeho vykonávanie</summary>
	void dalsiaNoc();

	int dajBuduceMinutyZaneprazd();
	int dajBuduceNabitie();
	///<summary>Zaradí zasielku do svojho rozvrhu a prepravi ju za dany cas</summary>
	int pridajZasielkuNaPrepravu(int vzdialenost);

	int dajNalietaneMin();

private:
	///<summary>Interna funkcia, vypocita cas potrebny na nabijanie pred vyzdvihnutim zasielky</summary>
	int casNabijania(int vzdialenost, int nabitie);
	///<summary>Vrati trvanie letu na danu vzdialenost</summary>
	int dobaTrvaniaLetu(int vzdialenost);
	///<summary>Nacitanie zo suboru, volana konstruktorom</summary>
	void fromSubor(fstream* inSubor);
};