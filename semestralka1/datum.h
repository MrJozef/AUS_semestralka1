#pragma once

#include "../structures/heap_monitor.h"
#include "const.h"

using namespace std;

class Datum
{
private:
	int den_;
	int hodina_;
	bool menitelny_;

public:
	Datum();
	Datum(int den, int hodina);
	Datum(int den, int hodina, bool moznostMenit);
	~Datum();

	string toString();
	string toSubor();
	bool dalsiaHodina();
};

///<summary>Tento kostruktor sa pouziva pri spusteni projektu pre aktualny cas, kt. bude dalej "plynut"</summary>
inline Datum::Datum()
{
	den_ = 1;
	hodina_ = ZACIATOCNA_HOD_DNA;
	menitelny_ = true;
}

///<summary>Tento konstruktor pouzivame pre vsetky ostatne objekty, ktore si maju uchovavat staticky cas</summary>
inline Datum::Datum(int den, int hodina)
{
	den_ = den;
	hodina_ = hodina;
	menitelny_ = false;
}

///<summary>Konstruktor pouzivany pri nacitavani so suboru</summary>
inline Datum::Datum(int den, int hodina, bool moznostMenit)
{
	den_ = den;
	hodina_ = hodina;
	menitelny_ = moznostMenit;
}

inline Datum::~Datum()
{
}

inline string Datum::toString()
{
	return to_string(den_) + ". den " + to_string(hodina_) + ":00";
}

inline string Datum::toSubor()
{
	return to_string(den_) + "\n" + to_string(hodina_) + "\n" + to_string(menitelny_) + "\n";
}

//todo vo vypise napisat ze tento datum sa menit nemoze ak to bude potrebne
inline bool Datum::dalsiaHodina()
{
	if (!menitelny_)
		return false;

	if (hodina_ == 21)
	{
		den_++;
		hodina_ = ZACIATOCNA_HOD_DNA;
	}
	else
		hodina_++;

	return true;
}