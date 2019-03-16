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
	zarDoEvidencie_->toSubor(outSubor);
	*outSubor  << SPZ_ << "\n" << nosnost_ << "\n" << nakladyNaReg_ << "\n" << celkNaklady_ << "\n";
}

void Vozidlo::fromSubor(fstream* inSubor)
{

	zarDoEvidencie_ = new Datum(inSubor);
	*inSubor >> SPZ_;
	*inSubor >> nosnost_;
	*inSubor >> nakladyNaReg_;
	*inSubor >> celkNaklady_;
}