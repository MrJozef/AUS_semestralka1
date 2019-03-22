#include "../structures/heap_monitor.h"
#include "vozidlo.h"

Vozidlo::Vozidlo(string spz, int nosnost, double naklady, Datum * zaradenie, structures::Array<bool>* trasa)
{
	zarDoEvidencie_ = new Datum(zaradenie->dajDen(), zaradenie->dajHodinu());
	SPZ_ = spz;
	nosnost_ = nosnost;
	nakladyNaReg_ = naklady;
	celkNaklady_ = 0;
	trasa_ = trasa;
	nalozDoSkladu_ = 0;
	nalozDoPrekladiska_ = 0;
}

Vozidlo::Vozidlo(fstream* inSubor)
{
	trasa_ = new structures::Array<bool>(POCET_REGIONOV);
	fromSubor(inSubor);
}

Vozidlo::~Vozidlo()
{
	delete zarDoEvidencie_;
	zarDoEvidencie_ = nullptr;
	delete trasa_;
	trasa_ = nullptr;
}

string Vozidlo::toString()
{
	return "Vozidlo: " + SPZ_ + "\n  Nosnost: " + to_string(nosnost_) + " kg\t\t\tCelkove prevadzkove naklady: " + to_string(celkNaklady_)
		+ " Eur\n  Zaradenie do evidencie: " + zarDoEvidencie_->toString();
}

void Vozidlo::toSubor(fstream* outSubor)
{
	zarDoEvidencie_->toSubor(outSubor);
	*outSubor << SPZ_ << "\n" << nosnost_ << "\n" << nakladyNaReg_ << "\n" << celkNaklady_ << "\n" << nalozDoSkladu_ << "\n" << nalozDoPrekladiska_ << "\n";

	for (int i = 0; i < POCET_REGIONOV; i++)
	{
		*outSubor << (*trasa_)[i] << "\n";
	}
}

string Vozidlo::getSPZ()
{
	return SPZ_;
}

bool Vozidlo::overDoSkladu(double hmotnostZas)
{		//todo pridat aj zasobnik (teoreticky nie je treba lebo viem ze vsetky tieto zasielky doveziem do centr. skladu)
	return ((hmotnostZas + nalozDoSkladu_) < nosnost_);
}

bool Vozidlo::overDoPrekladiska(double hmotnostZas)
{		//todo pridat aj zasobnik
	return ((hmotnostZas + nalozDoPrekladiska_) < nosnost_);
}

bool Vozidlo::overPrechodRegion(int region)
{
	return (*trasa_)[region];
}

void Vozidlo::fromSubor(fstream* inSubor)
{
	zarDoEvidencie_ = new Datum(inSubor);
	*inSubor >> SPZ_;
	*inSubor >> nosnost_;
	*inSubor >> nakladyNaReg_;
	*inSubor >> celkNaklady_;
	*inSubor >> nalozDoSkladu_;
	*inSubor >> nalozDoPrekladiska_;

	for (int i = 0; i < POCET_REGIONOV; i++)
	{
		*inSubor >> (*trasa_)[i];
	}
}
