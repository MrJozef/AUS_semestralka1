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

	int nabitie_;

public:
	Dron(int cislo, int typ, Datum* zaradenie);
	Dron(fstream* inSubor);
	~Dron();

	string toString();
	void toSubor(fstream* outSubor);
	int dajTyp();

private:
	void fromSubor(fstream* inSubor);
};