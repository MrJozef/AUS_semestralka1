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

	///<summary>Mno�stvo (v�ha) z�sielok na ceste do skladu</summary>
	double nalozDoSkladu_;
	///<summary>Mno�stvo (v�ha) z�sielok na ceste do preklad�sk</summary>
	double nalozDoPrekladiska_;
	
public:
	///<summary>Kon�truktor pri vytv�ran� v programe</summary>
	Vozidlo(string spz, int nosnost, double naklady, Datum* zaradenie, structures::Array<bool>* trasa);
	///<summary>Kon�truktor pre na��tavanie zo s�boru</summary>
	Vozidlo(fstream* inSubor);
	~Vozidlo();

	string toString();
	///<summary>Umo��uje ulo�enie objektu do s�boru</summary>
	void toSubor(fstream* outSubor);

	string getSPZ();

	///<summary>Prechadza toto vozidlo zadanym regionom?</summary>
	bool overPrechodRegion(int region);

	///<summary>Odveziem aj t�to z�sielku do skladu este?</summary>
	bool overDoSkladu(double hmotnostZas);
	///<summary>Odveziem aj t�to z�sielku pri ceste do preklad�sk?</summary>
	bool overDoPrekladiska(double hmotnostZas);
	///<summary>Nalo� t�to hmotnos� na cestu do skladu</summary>
	void nalozZasDoSkladu(double hmotnostZas);
	///<summary>Nalo� t�to hmotnos� na cestu do preklad�sk</summary>
	void nalozZasDoPrekladiska(double hmotnostZas);

	///<summary>Aktualiz�cia n�kladov</summary>
	void dalsiaNoc();

private:
	void fromSubor(fstream* inSubor);
};