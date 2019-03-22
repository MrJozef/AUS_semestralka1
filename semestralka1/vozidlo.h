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

	double nalozDoSkladu_;
	double nalozDoPrekladiska_;

public:
	Vozidlo(string spz, int nosnost, double naklady, Datum* zaradenie, structures::Array<bool>* trasa);
	Vozidlo(fstream* inSubor);
	~Vozidlo();

	string toString();
	void toSubor(fstream* outSubor);

	string getSPZ();

	bool overDoSkladu(double hmotnostZas);
	bool overDoPrekladiska(double hmotnostZas);
	bool overPrechodRegion(int region);

private:
	void fromSubor(fstream* inSubor);
};