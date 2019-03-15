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
	Datum(fstream* inSubor);
	Datum(int den, int hodina);
	~Datum();

	string toString();
	void toSubor(fstream* outSubor);
	bool dalsiaHodina();
	void fromSubor(fstream* inSubor);

	int dajDen();
	int dajHodinu();
};

///<summary>Tento kostruktor sa pouziva pri spusteni projektu pre aktualny cas, kt. bude dalej "plynut"</summary>
inline Datum::Datum()
{
	den_ = 1;
	hodina_ = ZACIATOCNA_HOD_DNA;
	menitelny_ = true;
}

///<summary>Konstruktor pouzivany pri nacitavani zo suboru</summary>
inline Datum::Datum(fstream * inSubor)
{
	fromSubor(inSubor);
}

///<summary>Tento konstruktor pouzivame pre vsetky ostatne objekty, ktore si maju uchovavat staticky cas</summary>
inline Datum::Datum(int den, int hodina)
{
	den_ = den;
	hodina_ = hodina;
	menitelny_ = false;
}

inline Datum::~Datum()
{
}

inline string Datum::toString()
{
	return to_string(den_) + ". den " + to_string(hodina_) + ":00\n";
}

inline void Datum::toSubor(fstream* outSubor)
{
	*outSubor << den_ << "\n" << hodina_ << "\n" << menitelny_ << "\n";
}

///<summary>Metoda pouzivana konstruktorom pri nacitavani zo suboru a pri nacitavani aktualny_cas (vtedy je volana priamo)</summary>
inline void Datum::fromSubor(fstream* inSubor)
{
	*inSubor >> den_;
	*inSubor >> hodina_;
	*inSubor >> menitelny_;
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

inline int Datum::dajDen()
{
	return den_;
}

inline int Datum::dajHodinu()
{
	return hodina_;
}