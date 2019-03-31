#pragma once
#include "../structures/heap_monitor.h"
#include "datum.h"

class Zasielka
{
private:
	double hmotnost_;
	///<summary>Odkia¾ smeruje zásielka</summary>
	short regZac_;
	///<summary>Kam smeruje zásielka</summary>
	short regKon_;
	///<summary>km od poèiatoèného prekladiska</summary>
	int regZacVzdial_;
	///<summary>km od koncového prekladiska</summary>
	int regKonVzdial_;
	Datum* odoslanie_;
	Datum* dorucenie_;
	///<summary>Prepravuje sa zásielka, je už vybavená...?</summary>
	StavZasielky stav_;
	///<summary>Dôvod zamietnutia</summary>
	DovodZamietnutia zamietnutie_;

public:
	///<summary>Konštruktor pri vytváraní v programe</summary>
	Zasielka(double hmotnost, short regZac, short regKon, int regZacVzdialenost, int regKonVzdialenost, Datum* datOdoslania);
	///<summary>Konštruktor pre naèítavanie zo súboru</summary>
	Zasielka(fstream* inSubor);
	~Zasielka();

	///<summary>Umožòuje uloženie objektu do súboru</summary>
	void toSubor(fstream* outSubor);
	string toString();
	///<summary>Zmení dôvod zamietnutia</summary>
	void zamietni(DovodZamietnutia dovod);
	///<summary>Èas doruèenia nastaví na daný èas</summary>
	void pridajCasDorucenia(Datum* casDorucenia);
	Datum* dajCasDorucenia();
	StavZasielky dajStavZasielky();
	///<summary>Zmení stav zásielky</summary>
	void zmenStavZasielky(StavZasielky novyStav);

	int dajZaciatocnyRegion();
	int dajKoncovyRegion();
	Datum* dajDatumOdoslania();

private:
	///<summary>Metódu používa konštruktor pri naèítavaní zo súboru</summary>
	void fromSubor(fstream* inSubor);
};


inline Zasielka::Zasielka(double hmotnost, short regZac, short regKon, int regZacVzdialenost, int regKonVzdialenost, Datum* datOdoslania)
{
	odoslanie_ = new Datum(datOdoslania->dajDen(), datOdoslania->dajHodinu());
	hmotnost_ = hmotnost;
	regZac_ = regZac;
	regKon_ = regKon;
	regZacVzdial_ = regZacVzdialenost;
	regKonVzdial_ = regKonVzdialenost;
	dorucenie_ = nullptr;
	stav_ = zamietnuta;
	zamietnutie_ = nezamietnuta;
}

inline Zasielka::Zasielka(fstream* inSubor)
{
	fromSubor(inSubor);
}

inline Zasielka::~Zasielka()
{
	delete odoslanie_;
	odoslanie_ = nullptr;
	if (dorucenie_ != nullptr)
	{
		delete dorucenie_;
		dorucenie_ = nullptr;
	}
}

inline void Zasielka::toSubor(fstream* outSubor)
{
	odoslanie_->toSubor(outSubor);
	if (dorucenie_ == nullptr)				//ak objednavka este nebola dorucena
	{
		*outSubor << "-1\n-1\n-1\n";
	}
	else { dorucenie_->toSubor(outSubor); }
	*outSubor << hmotnost_ << "\n" << regZac_ << "\n" << regKon_ << "\n" << regZacVzdial_ << "\n" << regKonVzdial_ << "\n";
	*outSubor << stav_ << "\n" << zamietnutie_ << "\n";
}

inline void Zasielka::fromSubor(fstream* inSubor)
{
	int pom;
	int hodina;
	int menitelny;

	odoslanie_ = new Datum(inSubor);

	*inSubor >> pom;
	if(pom == -1)
	{
		*inSubor >> pom;
		*inSubor >> pom;
		dorucenie_ = nullptr;
	}
	else
	{
		*inSubor >> hodina;
		*inSubor >> menitelny;
		dorucenie_ = new Datum(pom, hodina);
	}

	*inSubor >> hmotnost_;
	*inSubor >> regZac_;
	*inSubor >> regKon_;
	*inSubor >> regZacVzdial_;
	*inSubor >> regKonVzdial_;
	*inSubor >> pom;
	stav_ = static_cast<StavZasielky>(pom);
	*inSubor >> pom;
	zamietnutie_ = static_cast<DovodZamietnutia>(pom);
}

inline string Zasielka::toString()
{
	string pom = "Objednavka:\n  Datum odoslania: " + odoslanie_->toString() + "  Hmotnost: " + to_string(hmotnost_) + " kg\n  Region odosielatela: " +
		to_string((regZac_ + 1)) + "\t\t\tRegion odberatela: " +
		to_string((regKon_ + 1)) + "\n  Vzdialenost: " + to_string(regZacVzdial_) + " km\t\t\t\tVzdialenost: " + to_string(regKonVzdial_) + " km\n  ";

	if (stav_ == vybavena)
	{
		pom += "  Datum dorucenia: " + dorucenie_->toString() + "\n";
	}
	else
	{
		if (stav_ == zamietnuta)
		{
			pom += "  " + toStringDovodZamietnutia(zamietnutie_);
		}
	}

	return pom;
}

inline void Zasielka::zamietni(DovodZamietnutia dovod)
{
	zamietnutie_ = dovod;
}

inline void Zasielka::pridajCasDorucenia(Datum* casDorucenia)
{
	dorucenie_ = casDorucenia;
}

inline Datum* Zasielka::dajCasDorucenia()
{
	return dorucenie_;
}

inline StavZasielky Zasielka::dajStavZasielky()
{
	return stav_;
}

inline void Zasielka::zmenStavZasielky(StavZasielky novyStav)
{
	stav_ = novyStav;
}

inline int Zasielka::dajZaciatocnyRegion()
{
	return static_cast<int>(regZac_);
}

inline int Zasielka::dajKoncovyRegion()
{
	return static_cast<int>(regKon_);
}

inline Datum* Zasielka::dajDatumOdoslania()
{
	return odoslanie_;
}
