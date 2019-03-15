#include "../structures/heap_monitor.h"
#include "vozidlo.h"

Vozidlo::Vozidlo(string spz, int nosnost, double naklady, Datum * zaradenie)
{
	zarDoEvidencie_ = new Datum(zaradenie->dajDen(), zaradenie->dajHodinu());
	SPZ_ = spz;
	nosnost_ = nosnost;
	nakladyNaReg_ = naklady;
	celkNaklady_ = 0;
}

Vozidlo::Vozidlo(fstream* inSubor)
{
	fromSubor(inSubor);
}

Vozidlo::~Vozidlo()
{
	delete zarDoEvidencie_;
	zarDoEvidencie_ = nullptr;
}

string Vozidlo::toString()
{		//todo zaokruhlit celkNaklady_
	return "Vozidlo: " + SPZ_ + "\n  Nosnost: " + to_string(nosnost_) + " t\t\tCelkove prevadzkove naklady: " + to_string(celkNaklady_)
		+ " Eur\n\t\t\tZaradenie do evidencie: " + zarDoEvidencie_->toString();
}

void Vozidlo::toSubor(fstream* outSubor)
{
	*outSubor << zarDoEvidencie_->dajDen() << "\n" << zarDoEvidencie_->dajHodinu() << "\n" << SPZ_ << "\n" << nosnost_ << "\n";
	*outSubor << nakladyNaReg_ << "\n" << celkNaklady_ << "\n";
}

void Vozidlo::fromSubor(fstream* inSubor)
{
	int den;
	int hodina;

	*inSubor >> den;
	*inSubor >> hodina;

	zarDoEvidencie_ = new Datum(den, hodina);
	*inSubor >> SPZ_;
	*inSubor >> nosnost_;
	*inSubor >> nakladyNaReg_;
	*inSubor >> celkNaklady_;
}