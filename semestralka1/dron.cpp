#include "dron.h"
#include "../structures/heap_monitor.h"


Dron::Dron(int cislo, int typ, Datum* zaradenie)
{
	zarDoEvidencie_ = new Datum(zaradenie->dajDen(), zaradenie->dajHodinu());
	serioveCislo_ = cislo;
	typ_ = static_cast<short>(typ);
	nalietHodiny_ = 0;
	prepravZasielky_ = 0;
	nabitie_ = 100;
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
	return "Dron: " + to_string(serioveCislo_) + "\t\t\t\t\tTyp: " + to_string(typ_) + "\n  Celkovo nalietanych hodin: " + to_string(nalietHodiny_) +
		"\t\tCelkovo prepravenych zasielok: " + to_string(prepravZasielky_) + "\n\t\t\t\t\t\tZaradenie do evidencie: " +
		zarDoEvidencie_->toString();
}

void Dron::toSubor(fstream* outSubor)
{
	zarDoEvidencie_->toSubor(outSubor);
	*outSubor << serioveCislo_ << "\n" << typ_ << "\n" << nalietHodiny_ << "\n" << prepravZasielky_ << "\n" << nabitie_ << "\n";
}

int Dron::dajTyp()
{
	return typ_;
}

void Dron::fromSubor(fstream* inSubor)
{
	zarDoEvidencie_ = new Datum(inSubor);
	*inSubor >> serioveCislo_;
	*inSubor >> typ_;
	*inSubor >> nalietHodiny_;
	*inSubor >> prepravZasielky_;
	*inSubor >> nabitie_;
}
