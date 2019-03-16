#include "dron.h"
#include "../structures/heap_monitor.h"


Dron::Dron(int cislo, int typ, Datum* zaradenie)
{
	zarDoEvidencie_ = new Datum(zaradenie->dajDen(), zaradenie->dajHodinu());
	serioveCislo_ = cislo;
	typ_ = typ;
	nalietHodiny_ = 0;
	prepravZasielky_ = 0;
}

Dron::Dron(fstream* inSubor)
{
	fromSubor(inSubor);
}


Dron::~Dron()
{
	delete zarDoEvidencie_;
	zarDoEvidencie_ = nullptr;
}

string Dron::toString()
{
	return "Dron: " + to_string(serioveCislo_) + "\t\t\tTyp: " + to_string(typ_) + "\nCelkovo nalietanych hodin: " + to_string(nalietHodiny_) +
		"\t\t\tCelkovo prepravenych zasielok: " + to_string(prepravZasielky_) + "\n\t\t\tZaradenie do evidencie: " +
		zarDoEvidencie_->toString();
}

void Dron::toSubor(fstream* outSubor)
{
	zarDoEvidencie_->toSubor(outSubor);
	*outSubor << "\n" << serioveCislo_ << "\n" << typ_ << "\n" << nalietHodiny_ << "\n" << prepravZasielky_ << "\n";
}

void Dron::fromSubor(fstream* inSubor)
{
	zarDoEvidencie_ = new Datum(inSubor);
	*inSubor >> serioveCislo_;
	*inSubor >> typ_;
	*inSubor >> nalietHodiny_;
	*inSubor >> prepravZasielky_;
}