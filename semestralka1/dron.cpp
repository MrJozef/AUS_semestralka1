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

	buduciRozvrh_ = new structures::ExplicitQueue<RamecRozvrhu*>();
	buduceNabitie_ = 100;
	buduceMinutyZaneprazd_ = 0;
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

	while(rozvrh_->size() > 0)
	{
		delete buduciRozvrh_->pop();
	}
	delete rozvrh_;
	delete buduciRozvrh_;
}

string Dron::toString()
{
	int nalietaneHodiny = nalietMinuty_ / 60;

	return "Dron: " + to_string(serioveCislo_) + "\t\t\t\t\tTyp: " + to_string(typ_) + "\n  Celkovo nalietanych hodin: " + to_string(nalietaneHodiny) +
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

	*outSubor << buduceNabitie_ << "\n" << buduceMinutyZaneprazd_ << "\n" << buduciRozvrh_->size() << "\n";
	pocetRamcov = buduciRozvrh_->size();
	while (pocetRamcov > 0)
	{
		pomRamec = buduciRozvrh_->pop();
		pomRamec->toSubor(outSubor);		//zapis do suboru
		buduciRozvrh_->push(pomRamec);
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

int Dron::dajSerioveCislo()
{
	return serioveCislo_;
}

bool Dron::doletis(int vzdialenost)
{
	return static_cast<double>(dajRychlostDronu(typ_)) * (static_cast<double>(dajDobuLetuDronu(typ_)) / 60) >= 2 * vzdialenost;		//vzdialenost 2x lebo sa musi aj vratit
}

int Dron::dajMinutyZaneprazd()
{
	return minutyZaneprazd_;
}

int Dron::overCasVyzdvihnutia(int vzdialenost, Transport trans)
{
	if (trans == odZakaznika)
	{
		return minutyZaneprazd_ + casNabijania(vzdialenost, nabitie_) + (dobaTrvaniaLetu(vzdialenost) / 2);		//deleno 2 pretoze chceme vediet cas za ktory sa zasielka vyzdvihne
	}
	return buduceMinutyZaneprazd_ + casNabijania(vzdialenost, buduceNabitie_) + (dobaTrvaniaLetu(vzdialenost) / 2);
}

int Dron::transportujZasielku(int vzdialenost)
{
	int nabijanie = casNabijania(vzdialenost, nabitie_);
	if (nabijanie > 0)
	{
		rozvrh_->push(new RamecRozvrhu(nabija, nabijanie));
		nabitie_ += static_cast<int>(ceil(static_cast<double>(nabijanie) / dajDobuNabijaniaDronu(typ_) * 10));
	}

	int dobaLetu = dobaTrvaniaLetu(vzdialenost);
	rozvrh_->push(new RamecRozvrhu(pracuje, dobaTrvaniaLetu(vzdialenost)));

	if (stav_ == volny)				//dron zacne rozvazat okamzite po prijati zasielky
	{
		stav_ = rozvrh_->peek()->dajCinnost();
	}

	minutyZaneprazd_ += nabijanie;
	minutyZaneprazd_ += dobaLetu;
	nabitie_ -= static_cast<int>(ceil(static_cast<double>(dobaLetu) / dajDobuLetuDronu(typ_) * 100));		//o kolko sa vybije dron pri lete

	return nabijanie + dobaLetu;
}

int Dron::pridajZasielkuNaPrepravu(int vzdialenost)
{
	int nabijanie = casNabijania(vzdialenost, buduceNabitie_);
	if (nabijanie > 0)
	{
		buduciRozvrh_->push(new RamecRozvrhu(nabija, nabijanie));
		buduceNabitie_ += static_cast<int>(ceil(static_cast<double>(nabijanie) / dajDobuNabijaniaDronu(typ_) * 10));
	}

	int dobaLetu = dobaTrvaniaLetu(vzdialenost);
	buduciRozvrh_->push(new RamecRozvrhu(pracuje, dobaTrvaniaLetu(vzdialenost)));

	buduceMinutyZaneprazd_ += nabijanie;
	buduceMinutyZaneprazd_ += dobaLetu;
	buduceNabitie_ -= static_cast<int>(ceil(static_cast<double>(dobaLetu) / dajDobuLetuDronu(typ_) * 100));

	return nabijanie + dobaLetu;
}

int Dron::dajNalietaneMin()
{
	return nalietMinuty_;
}

void Dron::dalsiaHodina()
{
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
			}

			zostavajuceMinuty = -1;
		}
		else
		{

			zostavajuceMinuty -= trvanie;
			minutyZaneprazd_ -= trvanie;
			
			if (rozvrh_->peek()->dajCinnost() == pracuje)
			{
				nalietMinuty_ += trvanie;
				prepravZasielky_++;
			}

			delete rozvrh_->pop();		//casovy ramec zanika, pretoze uz objednavku odniesol/nabil sa
		}
	}

	if (rozvrh_->size() > 0)
	{
		stav_ = rozvrh_->peek()->dajCinnost();
	}

	if (zostavajuceMinuty > 0 && nabitie_ < 100)
	{
		nabitie_ = nabitie_ + static_cast<int>(ceil((static_cast<double>(zostavajuceMinuty) / dajDobuNabijaniaDronu(typ_)) * 10));
		if (nabitie_ > 100)			//vyuzijeme vsetok cas na nabijanie, moze sa vsak stat, ze cas je prilis dlhy a baterku by dobil na viac ako 100%
		{
			stav_ = volny;
		}
		else
		{
			stav_ = nabija;
		}

	}

	if (nabitie_ > 100)			//korekcia kvoli zaokruhlovaniu
	{
		nabitie_ = 100;
	}
}

void Dron::dalsiaNoc()
{
	structures::ExplicitQueue<RamecRozvrhu*>* pomRozvrh = rozvrh_;
	rozvrh_ = buduciRozvrh_;
	buduciRozvrh_ = pomRozvrh;
	pomRozvrh = nullptr;

	if (rozvrh_->size() == 0)			//den nemoze zacat nabijanim
	{
		stav_ = volny;
		nabitie_ = 100;
	}
	else
	{
		stav_ = pracuje;
		nabitie_ = buduceNabitie_;		//uz bolo raz vypocitane kolko % baterky bude mat dron po tom, co roznese poslednu zasielku z predchadzajuceho dna
	}									//a nove objednavky mozem zaradit do frontu az po vybaveni poslednej z predchadzajuceho dna

	minutyZaneprazd_ = buduceMinutyZaneprazd_;
	buduceNabitie_ = 100;
	buduceMinutyZaneprazd_ = 0;
}

int Dron::dajBuduceMinutyZaneprazd()
{
	return buduceMinutyZaneprazd_;
}

int Dron::dajBuduceNabitie()
{
	return buduceNabitie_;
}

int Dron::casNabijania(int vzdialenost, int nabitie)
{
	double akeNabitiePotrebujem = static_cast<double>(vzdialenost * 2) / static_cast<double>(dajRychlostDronu(typ_)) * 100;
	double dobaNabijania = (akeNabitiePotrebujem - nabitie) / 10 * static_cast<double>(dajDobuNabijaniaDronu(typ_));

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
	double trvanieCesty = dlzkaCelejCesty / static_cast<double>(dajRychlostDronu(typ_)) * 60;		//dlzka je v km, rychlost v km/hod, chceme vsak minuty

	return static_cast<int>(ceil(trvanieCesty));
}

void Dron::fromSubor(fstream* inSubor)
{
	int pom;
	zarDoEvidencie_ = new Datum(inSubor);
	rozvrh_ = new structures::ExplicitQueue<RamecRozvrhu*>();
	buduciRozvrh_ = new structures::ExplicitQueue<RamecRozvrhu*>();

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

	*inSubor >> buduceNabitie_;
	*inSubor >> buduceMinutyZaneprazd_;
	*inSubor >> pom;
	for (int i = 0; i < pom; i++)
	{
		buduciRozvrh_->push(new RamecRozvrhu(inSubor));
	}
}