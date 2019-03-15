#pragma once
#include "../structures/heap_monitor.h"
#include "datum.h"


using namespace std;

class Vozidlo
{
private:
	string SPZ_;
	int nosnost_;
	double nakladyNaReg_;
	double celkNaklady_;
	Datum* zarDoEvidencie_;

public:
	Vozidlo(string spz, int nosnost, double naklady, Datum* zaradenie);
	Vozidlo(fstream* inSubor);
	~Vozidlo();

	string toString();
	void toSubor(fstream* outSubor);

private:
	void fromSubor(fstream* inSubor);
};