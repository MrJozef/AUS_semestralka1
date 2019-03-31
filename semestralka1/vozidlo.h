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
	///<summary>ktorymi vozidlo prechadza; nezapisujem do suboru</summary>
	short pocetReg_;

	///<summary>Mnostvo (váha) zásielok na ceste do skladu</summary>
	double nalozDoSkladu_;
	///<summary>Mnostvo (váha) zásielok na ceste do prekladísk</summary>
	double nalozDoPrekladiska_;
	
public:
	///<summary>Konštruktor pri vytváraní v programe</summary>
	Vozidlo(string spz, int nosnost, double naklady, Datum* zaradenie, structures::Array<bool>* trasa);
	///<summary>Konštruktor pre naèítavanie zo súboru</summary>
	Vozidlo(fstream* inSubor);
	~Vozidlo();

	string toString();
	///<summary>Umoòuje uloenie objektu do súboru</summary>
	void toSubor(fstream* outSubor);

	string getSPZ();

	///<summary>Prechadza toto vozidlo zadanym regionom?</summary>
	bool overPrechodRegion(int region);

	///<summary>Odveziem aj túto zásielku do skladu este?</summary>
	bool overDoSkladu(double hmotnostZas);
	///<summary>Odveziem aj túto zásielku pri ceste do prekladísk?</summary>
	bool overDoPrekladiska(double hmotnostZas);
	///<summary>Nalo túto hmotnos na cestu do skladu</summary>
	void nalozZasDoSkladu(double hmotnostZas);
	///<summary>Nalo túto hmotnos na cestu do prekladísk</summary>
	void nalozZasDoPrekladiska(double hmotnostZas);

	///<summary>Aktualizácia nákladov</summary>
	void dalsiaNoc();

private:
	void fromSubor(fstream* inSubor);
};