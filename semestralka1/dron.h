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

	structures::ExplicitQueue<RamecRozvrhu*>* buduciRozvrh_;
	int buduceNabitie_;
	int buduceMinutyZaneprazd_;

public:
	Dron(int cislo, int typ, Datum* zaradenie);
	Dron(fstream* inSubor);
	~Dron();

	string toString();
	void toSubor(fstream* outSubor);

	int dajTyp();
	StavDronu dajStav();
	int dajNabitie();
	int dajSerioveCislo();
	bool doletis(int vzdialenost);

	int dajMinutyZaneprazd();
	int overCasVyzdvihnutia(int vzdialenost, Transport trans);//vrati cas vyzdvihnutia
	int transportujZasielku(int vzdialenost);//vrati celkovy cas vyzdvihnutia zasielky + vratenie sa do prekladiska
	void dalsiaHodina();
	void dalsiaNoc();

	int dajBuduceMinutyZaneprazd();
	int dajBuduceNabitie();
	int pridajZasielkuNaPrepravu(int vzdialenost);

private:
	int casNabijania(int vzdialenost, int nabitie);
	int dobaTrvaniaLetu(int vzdialenost);
	void fromSubor(fstream* inSubor);
};