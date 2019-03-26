#include <iostream>
#include "../structures/heap_monitor.h"
#include "system.h"


System::System()
{
	aktualnyCas_ = new Datum();
	listVozidiel_ = new structures::ArrayList<Vozidlo*>();
	listPrekladisk_ = new structures::Array<Prekladisko*>(POCET_REGIONOV);
	for (int i = 0; i < POCET_REGIONOV; i++)
	{
		(*listPrekladisk_)[i] = new Prekladisko();
	}

	listZasielok_ = new structures::LinkedList<Zasielka*>();
}

System::System(fstream* inSubor)
{
	fromSubor(inSubor);
}



System::~System()
{
	delete aktualnyCas_;

	for (Vozidlo* voz : *listVozidiel_)
	{
		delete voz;
	}
	delete listVozidiel_;

	for (int i = 0; i < POCET_REGIONOV; i++)
	{
		delete (*listPrekladisk_)[i];
	}
	delete listPrekladisk_;

	for (Zasielka* zas : *listZasielok_)
	{
		delete zas;
	}
	delete listZasielok_;
}

void System::toSubor(fstream* outSubor)
{
	aktualnyCas_->toSubor(outSubor);
	*outSubor << listVozidiel_->size() << "\n";
	for (Vozidlo* voz : *listVozidiel_)
	{
		voz->toSubor(outSubor);
	}

	for (int i = 0; i < POCET_REGIONOV; i++)
	{
		(*listPrekladisk_)[i]->toSubor(outSubor);
	}

	*outSubor << listZasielok_->size() << "\n";
	for (Zasielka* zas : *listZasielok_)
	{
		zas->toSubor(outSubor);
	}
}

void System::fromSubor(fstream* inSubor)
{
	int pocet;

	aktualnyCas_ = new Datum(inSubor);

	listVozidiel_ = new structures::ArrayList<Vozidlo*>();
	*inSubor >> pocet;					//pocet vozidiel
	for (int i = 0; i < pocet; i++)
	{
		listVozidiel_->add(new Vozidlo(inSubor));
	}

	listPrekladisk_ = new structures::Array<Prekladisko*>(POCET_REGIONOV);
	for (int i = 0; i < POCET_REGIONOV; i++)
	{
		(*listPrekladisk_)[i] = new Prekladisko(inSubor);
	}

	*inSubor >> pocet;					//pocet zasielok
	listZasielok_ = new structures::LinkedList<Zasielka*>();
	for (int i = 0; i < pocet; i++)
	{
		listZasielok_->add(new Zasielka(inSubor));
	}
}


string System::casToString()
{
	return aktualnyCas_->toString();
}

void System::dalsiaHodina()
{
	aktualnyCas_->dalsiaHodina();
	if(aktualnyCas_->dajHodinu() == 7)
	{
		for (int i = 0; i < POCET_REGIONOV; i++)
		{
			(*listPrekladisk_)[i]->dalsiaNoc();
		}

		for (Vozidlo* voz : *listVozidiel_)
		{
			voz->dalsiaNoc();
		}
	}
	else
	{
		for (int i = 0; i < POCET_REGIONOV; i++)
		{
			(*listPrekladisk_)[i]->dalsiaHodina();
		}
	}
}

void System::vypisVsetkyVozidla()
{
	cout << "Zoznam vsetkych vozidiel firmy:\n" << endl;

	if (listVozidiel_->size() == 0)
	{
		cout << "  -  Aktualne firma nema ziadne vozidla  -\n" << endl;
	}
	else
	{
		for (Vozidlo* voz : *listVozidiel_)
		{
			cout << voz->toString();
		}
	}
}

void System::vypisDrony(int cisPrekladiska)
{
	(*listPrekladisk_)[cisPrekladiska]->vypisDrony();
}

void System::vypisVsetkyZasielky()
{
	for (Zasielka* zas : *listZasielok_)
	{
		cout << zas->toString() << "\n";
	}
}

void System::vytvorZasielku(double hmotnost, int regZac, int regKon, int regZacVzdialenost, int regKonVzdialenost)
{
	Zasielka* pomZasielka = new Zasielka(hmotnost, static_cast<short>(regZac), static_cast<short>(regKon), regZacVzdialenost, regKonVzdialenost, aktualnyCas_);

	listZasielok_->add(pomZasielka);
	DovodZamietnutia stavObjednavky = nezamietnuta;

	Vozidlo* vozDoCentralSkladu = nullptr;
	Vozidlo* vozDoPrekladiska = nullptr;
	Dron* dronDoSkladu = nullptr;

	if (aktualnyCas_->dajHodinu() < POSLEDNA_HOD_NA_VYZD_DRONOM)
	{
		stavObjednavky = (*listPrekladisk_)[regZac]->overPrevzatieZasielky(hmotnost, regZacVzdialenost);
		if (stavObjednavky == nezamietnuta)						//ci drony nedokazu objednavku vyzdvihnut u odosielatela
		{
			stavObjednavky = (*listPrekladisk_)[regKon]->overPrevzatieZasielky(hmotnost, regKonVzdialenost);
			if (stavObjednavky == nezamietnuta)					//ci drony nedokazu dorucit objednavku
			{

				if (regZac != REGION_ZILINA)			//ak sa objednavka urobi v ziline, nemusim kontrolovat, ci ju vozidlo dovezie do centralneho skladu
				{
					stavObjednavky = vozNeuvezie;
					//test odvoz vozidlom do centralneho skladu
					for (Vozidlo* voz : *listVozidiel_)
					{
						if (voz->overPrechodRegion(regZac))
						{
							if (voz->overDoSkladu(hmotnost))
							{
								vozDoCentralSkladu = voz;
								stavObjednavky = nezamietnuta;
								break;
							}
						}
					}
				}
				
				if (regKon != REGION_ZILINA)
				{
					if (stavObjednavky == nezamietnuta)
					{
						stavObjednavky = vozNeuvezie;
						//test odvoz vozidlom do prekladiska
						for (Vozidlo* voz : *listVozidiel_)
						{
							if (voz->overPrechodRegion(regKon))
							{
								if (voz->overDoPrekladiska(hmotnost))
								{
									vozDoPrekladiska = voz;
									stavObjednavky = nezamietnuta;
									break;
								}
							}
						}
					}
				}


				//ci je zasielka naozaj prevzata do 20:00
				if (stavObjednavky == nezamietnuta)
				{
					dronDoSkladu = (*listPrekladisk_)[regZac]->vyberDron(hmotnost, regZacVzdialenost);
					int minutyDoVyzdvihnutia = dronDoSkladu->overCasVyzdvihnutia(regZacVzdialenost);


					if (aktualnyCas_->dajHodinu() + (minutyDoVyzdvihnutia / 60) >= POSLEDNA_HOD_NA_VYZD_DRONOM)
					{
						stavObjednavky = neskorVyzdvihnutie;		//zasielku nemozeme prijat lebo by sme ju museli vyzdvihnut po 20:00
					}
					else
					{
						if (minutyDoVyzdvihnutia > 60)
						{
							cout << "Vyzdvihnutie zasielky moze trvat viac ako 1 hodinu, napriek tomu chcete odoslat objednavku? [A/N]" << endl;

							if (!nacitajAnoNie())
							{
								stavObjednavky = zrusZakaznikom;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		stavObjednavky = neskorVyzdvihnutie;
	}

	if (stavObjednavky == nezamietnuta)
	{
		if (regZac != REGION_ZILINA)
		{
			vozDoCentralSkladu->nalozZasDoSkladu(hmotnost);
		}

		if (regKon != REGION_ZILINA)
		{
			vozDoPrekladiska->nalozZasDoPrekladiska(hmotnost);
		}
		
		int celkCasTransportuDoSkladu = dronDoSkladu->transportujZasielku(regZacVzdialenost);		//prikaz na transport => na zaradenie zasielky do rozvrhu
		cout << "celkCasTransportuDoSkladu: " + to_string(celkCasTransportuDoSkladu);
	}

	pomZasielka->zamietni(stavObjednavky);
	cout << toStringDovodZamietnutia(stavObjednavky) << endl;
	pomZasielka = nullptr;
}

void System::pridajVozidlo(string spz, int nosnost, double naklady, structures::Array<bool>* trasaVoz)
{
	listVozidiel_->add(new Vozidlo(spz, nosnost, naklady, aktualnyCas_, trasaVoz));
}

bool System::overSPZ(string spz)
{
	for (Vozidlo* voz : *listVozidiel_)
	{
		if (voz->getSPZ() == spz)
		{
			return true;
		}
	}
	return false;
}

void System::pridajDron(int cisOkresu, int cis, int typ)
{
	(*listPrekladisk_)[cisOkresu]->pridajDron(cis, typ, aktualnyCas_);
}

bool System::nacitajAnoNie()
{
	char pismeno;
	while (true)
	{
		cout << ">> ";
		cin >> pismeno;
		if (pismeno == 'A' || pismeno == 'a')
		{
			return true;
		}
		else
		{
			if (pismeno == 'N' || pismeno == 'n')
			{
				return false;
			}
			else
			{
				cout << "Zly vstup! Stlacte klavesu A alebo N\n" << endl;
			}
		}
	}
}