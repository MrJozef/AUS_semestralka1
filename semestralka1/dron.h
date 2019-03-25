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

	int nabitie_;		//v %
	StavDronu stav_;
	structures::ExplicitQueue<RamecRozvrhu*>* rozvrh_;
	int minutyZaneprazd_;

public:
	Dron(int cislo, int typ, Datum* zaradenie);
	Dron(fstream* inSubor);
	~Dron();

	string toString();
	void toSubor(fstream* outSubor);

	int dajTyp();
	StavDronu dajStav();
	int dajNabitie();
	bool doletis(int vzdialenost);

	int dajMinutyZaneprazd();
	int overCasVyzdvihnutia(int vzdialenost);//vrati cas vyzdvihnutia
	int transportujZasielku(int vzdialenost);//vrati celkovy cas vyzdvihnutia zasielky + vratenie sa do prekladiska
	void dalsiaHodina();

private:
	int casNabijania(int vzdialenost);
	int dobaTrvaniaLetu(int vzdialenost);
	void fromSubor(fstream* inSubor);
};