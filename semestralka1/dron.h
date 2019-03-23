#pragma once
#include "../structures/heap_monitor.h"
#include "datum.h"

class Dron
{
private:
	Datum* zarDoEvidencie_;
	int serioveCislo_;
	short typ_;
	double nalietHodiny_;
	int prepravZasielky_;

	int nabitie_;		//v %
	StavDronu stav_;

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

private:
	void fromSubor(fstream* inSubor);
};