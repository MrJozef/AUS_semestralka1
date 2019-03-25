#include <cmath>
#include "../structures/heap_monitor.h"
#include "dron.h"


Dron::Dron(int cislo, int typ, Datum* zaradenie)
{
	zarDoEvidencie_ = new Datum(zaradenie->dajDen(), zaradenie->dajHodinu());
	serioveCislo_ = cislo;
	typ_ = static_cast<short>(typ);
	nalietMinuty_ = 0;
	prepravZasielky_ = 0;

	nabitie_ = 100;
	stav_ = volny;
	rozvrh_ = new structures::ExplicitQueue<RamecRozvrhu*>();
	minutyZaneprazd_ = 0;
}

Dron::Dron(fstream* inSubor)
{
	fromSubor(inSubor);
}


Dron::~Dron()
{
	delete zarDoEvidencie_;
	zarDoEvidencie_ = nullptr;

	while(rozvrh_->size() > 0)
	{
		delete rozvrh_->pop();
	}
	delete rozvrh_;
}

string Dron::toString()
{
	return "Dron: " + to_string(serioveCislo_) + "\t\t\t\t\tTyp: " + to_string(typ_) + "\n  Celkovo nalietanych hodin: " + to_string(nalietMinuty_) +
		"\t\tCelkovo prepravenych zasielok: " + to_string(prepravZasielky_) + "\n\t\t\t\t\t\tZaradenie do evidencie: " +
		zarDoEvidencie_->toString() + "  Aktualne: " + toStringStavDronu(stav_) + "\n";
}

void Dron::toSubor(fstream* outSubor)
{

	zarDoEvidencie_->toSubor(outSubor);
	*outSubor << serioveCislo_ << "\n" << typ_ << "\n" << nalietMinuty_ << "\n" << prepravZasielky_ << "\n" << nabitie_ << "\n" << stav_ << "\n";

	*outSubor << minutyZaneprazd_ << "\n" << rozvrh_->size() << "\n";
	int pocetRamcov = rozvrh_->size();
	RamecRozvrhu* pomRamec;

	while(pocetRamcov > 0)
	{
		pomRamec = rozvrh_->pop();
		pomRamec->toSubor(outSubor);		//zapis do suboru
		rozvrh_->push(pomRamec);			//pretoze nezatvarame program, musime vratit to co sme vybrali -> front nema prehliadku :(
		pocetRamcov--;
	}
}

int Dron::dajTyp()
{
	return typ_;
}

StavDronu Dron::dajStav()
{
	return stav_;
}

int Dron::dajNabitie()
{
	return nabitie_;
}

bool Dron::doletis(int vzdialenost)
{
	return static_cast<double>(dajRychlostDronu(typ_)) * (static_cast<double>(dajDobuLetuDronu(typ_)) / 60) >= 2 * vzdialenost;		//vzdialenost 2x lebo sa musi aj vratit
}

int Dron::dajMinutyZaneprazd()
{
	return minutyZaneprazd_;
}

int Dron::overCasVyzdvihnutia(int vzdialenost)
{
	return minutyZaneprazd_ + casNabijania(vzdialenost) + (dobaTrvaniaLetu(vzdialenost) / 2);		//deleno 2 pretoze chceme vediet cas za ktory sa zasielka vyzdvihne
}

int Dron::transportujZasielku(int vzdialenost)
{
	int nabijanie = casNabijania(vzdialenost);
	if (nabijanie > 0)
	{
		rozvrh_->push(new RamecRozvrhu(nabija, nabijanie));
	}

	int dobaLetu = dobaTrvaniaLetu(vzdialenost);
	rozvrh_->push(new RamecRozvrhu(pracuje, dobaTrvaniaLetu(vzdialenost)));

	if (stav_ == volny)				//dron zacne rozvazat okamzite po prijati zasielky
	{
		stav_ = rozvrh_->peek()->dajCinnost();
	}

	minutyZaneprazd_ += nabijanie;
	minutyZaneprazd_ += dobaLetu;
	nabitie_ -= (dobaLetu / dajDobuLetuDronu(typ_)) * 100;		//o kolko sa vybije dron pri lete

	return nabijanie + dobaLetu;
}


void Dron::dalsiaHodina()
{
	// nalietMinuty_ prepravZasielky_ nabitie_ stav_ rozvrh_ minutyZaneprazd_;;;zostavajuceMinuty
	int zostavajuceMinuty = 60;
	int trvanie;

	while (rozvrh_->size() > 0 && zostavajuceMinuty > 0)
	{
		trvanie = rozvrh_->peek()->dajDobuTrvania();

		if (trvanie > zostavajuceMinuty)
		{
			rozvrh_->peek()->aktualizuj(zostavajuceMinuty);
			stav_ = rozvrh_->peek()->dajCinnost();
			minutyZaneprazd_ -= zostavajuceMinuty;

			if (rozvrh_->peek()->dajCinnost() == pracuje)
			{
				nalietMinuty_ += zostavajuceMinuty;
				nabitie_ = nabitie_ - (zostavajuceMinuty / dajDobuNabijaniaDronu(typ_)) * 10;
			}
			else
			{
				nabitie_ = nabitie_ + (zostavajuceMinuty / dajDobuNabijaniaDronu(typ_)) * 10;
			}
			zostavajuceMinuty = 0;
		}
		else
		{

			zostavajuceMinuty -= trvanie;
			minutyZaneprazd_ -= trvanie;
			
			if (rozvrh_->peek()->dajCinnost() == pracuje)
			{
				nalietMinuty_ += trvanie;
				prepravZasielky_++;
				nabitie_ = nabitie_ - (trvanie / dajDobuNabijaniaDronu(typ_)) * 10;
			}
			else
			{
				nabitie_ = nabitie_ + (trvanie / dajDobuNabijaniaDronu(typ_)) * 10;
			}

			delete rozvrh_->pop();		//casovy ramec zanika, pretoze uz objednavku odniesol/nabil sa
		}
	}

	if (zostavajuceMinuty > 0)
	{
		if (nabitie_ >= 100)
		{
			stav_ = volny;
		}
		else
		{
			nabitie_ = nabitie_ + (zostavajuceMinuty / dajDobuNabijaniaDronu(typ_)) * 10;
			if (nabitie_ > 100)			//vyuzijeme vsetok cas na nabijanie, moze sa vsak stat, ze cas je prilis dlhy a baterku by dobil na viac ako 100%
			{
				nabitie_ = 100;
				stav_ = volny;
			}
			else
			{
				stav_ = nabija;
			}
		}
	}
}


int Dron::casNabijania(int vzdialenost)
{
	double akeNabitiePotrebujem = static_cast<double>(vzdialenost * 2) / dajRychlostDronu(typ_) * 100;
	double dobaNabijania = (akeNabitiePotrebujem - nabitie_) / 10 * dajDobuNabijaniaDronu(typ_);

	if(dobaNabijania > 0.05)		//ak vyjde zaporna doba nabijania -> dron je nabity viac nez je potrebne na danu cestu
	{
		int minutyNabijania = static_cast<int>(ceil(dobaNabijania));
		return minutyNabijania;
	}
	return 0;
}

int Dron::dobaTrvaniaLetu(int vzdialenost)
{
	double dlzkaCelejCesty = static_cast<double>(vzdialenost * 2);
	double trvanieCesty = dlzkaCelejCesty / dajRychlostDronu(typ_) * 60;		//dlzka je v km, rychlost v km/hod, chceme vsak minuty

	return static_cast<int>(ceil(trvanieCesty));
}

void Dron::fromSubor(fstream* inSubor)
{
	int pom;
	zarDoEvidencie_ = new Datum(inSubor);
	rozvrh_ = new structures::ExplicitQueue<RamecRozvrhu*>();

	*inSubor >> serioveCislo_;
	*inSubor >> typ_;
	*inSubor >> nalietMinuty_;
	*inSubor >> prepravZasielky_;
	*inSubor >> nabitie_;
	*inSubor >> pom;

	stav_ = static_cast<StavDronu>(pom);

	*inSubor >> minutyZaneprazd_;
	*inSubor >> pom;				//pocet cinnosti, ktore mal dron v rozvrhu -> size of rozvrh_
	for (int i = 0; i < pom; i++)
	{
		rozvrh_->push(new RamecRozvrhu(inSubor));
	}
}
