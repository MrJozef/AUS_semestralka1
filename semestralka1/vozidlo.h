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
	short pocetReg_;				//ktorymi vozidlo prechadza; nezapisujem do suboru

	double nalozDoSkladu_;
	double nalozDoPrekladiska_;
	
public:
	Vozidlo(string spz, int nosnost, double naklady, Datum* zaradenie, structures::Array<bool>* trasa);
	Vozidlo(fstream* inSubor);
	~Vozidlo();

	string toString();
	void toSubor(fstream* outSubor);

	string getSPZ();

	bool overPrechodRegion(int region);

	bool overDoSkladu(double hmotnostZas);
	bool overDoPrekladiska(double hmotnostZas);
	void nalozZasDoSkladu(double hmotnostZas);
	void nalozZasDoPrekladiska(double hmotnostZas);

	void dalsiaNoc();

private:
	void fromSubor(fstream* inSubor);
};