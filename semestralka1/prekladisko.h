#pragma once
#include <iostream>
#include "../structures/heap_monitor.h"
#include "../structures/list/linked_list.h"
#include "dron.h"

class Prekladisko
{
private:
	structures::LinkedList<Dron*>* listDronov_;

public:
	Prekladisko();
	Prekladisko(fstream* inSubor);//todo toto zatial nikde nepouzivam
	~Prekladisko();

	void toSubor(fstream* outSubor);
	void pridajDron(int cislo, int typ, Datum* zaradenie);
	void vypisDrony();
	bool overSerioveCislo(int serioveCislo);
	void fromSubor(fstream* inSubor);
	void dalsiaHodina();
	void dalsiaNoc();

	DovodZamietnutia overPrevzatieZasielky(double hmotnostZasielky, int vzdialenost);
	Dron* vyberDron(double hmotnostZasielky, int vzdialenost, Transport trans);
};

inline Prekladisko::Prekladisko()
{
	listDronov_ = new structures::LinkedList<Dron*>();
}

inline Prekladisko::Prekladisko(fstream * inSubor)
{
	fromSubor(inSubor);
}

inline Prekladisko::~Prekladisko()
{
	for (Dron* dron : *listDronov_)
	{
		delete dron;
	}
	delete listDronov_;
	listDronov_ = nullptr;
}

inline void Prekladisko::toSubor(fstream* outSubor)
{
	*outSubor << listDronov_->size() << "\n";
	for (Dron* dron : *listDronov_)
	{
		dron->toSubor(outSubor);
	}
}

inline void Prekladisko::fromSubor(fstream* inSubor)
{
	int pocetDronov;
	*inSubor >> pocetDronov;

	listDronov_ = new structures::LinkedList<Dron*>();
	for (int i = 0; i < pocetDronov; i++)
	{
		listDronov_->add(new Dron(inSubor));
	}
}

inline void Prekladisko::dalsiaHodina()
{
	for(Dron* dron : *listDronov_)
	{
		dron->dalsiaHodina();
	}
}

inline void Prekladisko::dalsiaNoc()
{
	for (Dron* dron : *listDronov_)
	{
		dron->dalsiaNoc();
	}
}

inline void Prekladisko::pridajDron(int cislo, int typ, Datum* zaradenie)
{
	listDronov_->add(new Dron(cislo, typ, zaradenie));
}

inline void Prekladisko::vypisDrony()
{
	cout << "Zoznam vsetkych dronov v danom prekladisku:\n" << endl;

	if (listDronov_->size() == 0)
	{
		cout << "  -  V tomto prekladisku sa aktualne nenachadza ziaden dron  -\n" << endl;
	}
	else
	{
		for (Dron* dron : *listDronov_)
		{
			cout << dron->toString();
		}
	}
}

inline bool Prekladisko::overSerioveCislo(int serioveCislo)
{
	for (Dron* dron : *listDronov_)
	{
		if (dron->dajSerioveCislo() == serioveCislo)
		{
			return false;
		}
	}
	return true;
}

inline DovodZamietnutia Prekladisko::overPrevzatieZasielky(double hmotnostZasielky, int vzdialenost)
{
	DovodZamietnutia pom = mimoRadius;
	int typ;

	for (Dron* dron : *listDronov_)
	{
		typ = dron->dajTyp();

		if (dajNosnostDronu(typ) >= hmotnostZasielky)
		{
			if (((static_cast<double>(dajDobuLetuDronu(typ)) / 60) * (static_cast<double>(dajRychlostDronu(typ)) / 2)) >= vzdialenost)
			{
				pom = nezamietnuta;
				break;
			}
			else { pom = mimoRadius; }
		}
		else { pom = velkaHmotnost; }
	}
	return pom;
}

inline Dron* Prekladisko::vyberDron(double hmotnostZasielky, int vzdialenost, Transport trans)
{
	Dron* vybranyDron = nullptr;
	int aktNosnost = 100000;
	int aktNabitie = 0;

	for (Dron* dron : *listDronov_)
	{
		if (trans == odZakaznika)
		{
			if (dron->dajStav() == volny)
			{
				if ((dajNosnostDronu(dron->dajTyp()) >= hmotnostZasielky) && (dajNosnostDronu(dron->dajTyp()) < aktNosnost) && (dron->dajNabitie() > aktNabitie) && (dron->doletis(vzdialenost)))
				{
					vybranyDron = dron;
					aktNosnost = dajNosnostDronu(dron->dajTyp());
					aktNabitie = dron->dajNabitie();
				}
			}
		}
		else
		{
			if (dron->dajBuduceMinutyZaneprazd() == 0)
			{
				if ((dajNosnostDronu(dron->dajTyp()) >= hmotnostZasielky) && (dajNosnostDronu(dron->dajTyp()) < aktNosnost) && (dron->dajBuduceNabitie() > aktNabitie) && (dron->doletis(vzdialenost)))
				{
					vybranyDron = dron;
					aktNosnost = dajNosnostDronu(dron->dajTyp());
					aktNabitie = dron->dajBuduceNabitie();
				}
			}
		}
	}

	if (vybranyDron == nullptr)
	{
		int minutyDoVyzdvihnutia = 10000000;

		for (Dron* dron : *listDronov_)
		{
			if (trans == odZakaznika)
			{
				if (dron->doletis(vzdialenost) && dajNosnostDronu(dron->dajTyp()) >= hmotnostZasielky)
				{
					if (dron->dajMinutyZaneprazd() < minutyDoVyzdvihnutia)
					{
						minutyDoVyzdvihnutia = dron->dajMinutyZaneprazd();
						vybranyDron = dron;
					}
				}
			}
			else
			{
				if (dron->doletis(vzdialenost) && dajNosnostDronu(dron->dajTyp()) >= hmotnostZasielky)
				{
					if (dron->dajBuduceMinutyZaneprazd() < minutyDoVyzdvihnutia)
					{
						minutyDoVyzdvihnutia = dron->dajBuduceMinutyZaneprazd();
						vybranyDron = dron;
					}
				}
			}
			
		}
	}

	return vybranyDron;
}