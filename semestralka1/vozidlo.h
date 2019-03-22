#pragma once
#include "../structures/heap_monitor.h"
#include "../structures/array/array.h"
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
	structures::Array<bool>* trasa_;

public:
	Vozidlo(string spz, int nosnost, double naklady, Datum* zaradenie, structures::Array<bool>* trasa);
	Vozidlo(fstream* inSubor);
	~Vozidlo();

	string toString();
	void toSubor(fstream* outSubor);

	string getSPZ();
	//void priradRegion(int cisReg);

private:
	void fromSubor(fstream* inSubor);
};