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
	void fromSubor(fstream* inSubor);

	DovodZamietnutia overPrevzatieZasielky(double hmotnostZasielky, int vzdialenost);
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

inline DovodZamietnutia Prekladisko::overPrevzatieZasielky(double hmotnostZasielky, int vzdialenost)
{
	DovodZamietnutia pom = mimoRadius;
	int typ;

	for (Dron* dron : *listDronov_)
	{
		typ = dron->dajTyp();

		if (dajNosnostDronu(typ) > hmotnostZasielky)
		{
			if (((static_cast<double>(dajDobuLetuDronu(typ)) / 60) * (dajRychlostDronu(typ) / 2)) > vzdialenost)
			{
				pom = nezamietnuta;
			}
			else { pom = mimoRadius; }
		}
		else { pom = velkaHmotnost; }
	}
	return pom;
}