#include "../structures/heap_monitor.h"
#include "system.h"
#include <iostream>


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

		for (Zasielka* zas : *listZasielok_)
		{
			if(zas->dajStavZasielky() == transportPrvaFaza)
			{
				zas->zmenStavZasielky(transportDruhaFaza);
			}
		}
	}
	else
	{
		for (int i = 0; i < POCET_REGIONOV; i++)
		{
			(*listPrekladisk_)[i]->dalsiaHodina();
		}

		for (Zasielka* zas : *listZasielok_)
		{
			if (zas->dajStavZasielky() == transportDruhaFaza && zas->dajCasDorucenia()->dajHodinu() == aktualnyCas_->dajHodinu())
			{
				zas->zmenStavZasielky(vybavena);
			}
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
			cout << voz->toString() << endl;
		}
	}
}

void System::vypisDrony(int cisPrekladiska)
{
	(*listPrekladisk_)[cisPrekladiska]->vypisDrony();
}

void System::vypisVsetkyZasielky()
{
	if (listZasielok_->size() == 0)
	{
		cout << "  -  Aktualne firma neeviduje ani jednu zasielku  -\n" << endl;
	}
	else
	{
		for (Zasielka* zas : *listZasielok_)
		{
			cout << zas->toString() << "\n";
		}
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
	Dron* dronZPrekladiska = nullptr;

	int casTransZPrekladiska = 0;

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
					dronDoSkladu = (*listPrekladisk_)[regZac]->vyberDron(hmotnost, regZacVzdialenost, odZakaznika);
					int minutyDoVyzdvihnutia = dronDoSkladu->overCasVyzdvihnutia(regZacVzdialenost, odZakaznika);


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

					//ci dokazeme zasielku dorucit do 18:00 nasledujuceho dna
					if (stavObjednavky == nezamietnuta)
					{
						dronZPrekladiska = (*listPrekladisk_)[regKon]->vyberDron(hmotnost, regKonVzdialenost, kOdberatelovi);
						casTransZPrekladiska = dronZPrekladiska->overCasVyzdvihnutia(regKonVzdialenost, kOdberatelovi);

						if (ZACIATOCNA_HOD_DNA + (casTransZPrekladiska / 60) >= POSLEDNA_HOD_NA_DORUCENIE)
						{
							stavObjednavky = plnePrekladisko;
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

		dronDoSkladu->transportujZasielku(regZacVzdialenost);		//prikaz na transport => na zaradenie zasielky do rozvrhu

		dronZPrekladiska->pridajZasielkuNaPrepravu(regKonVzdialenost);		//prikaz na transport z prekladiska k odberatelovi tj. na druhy den
		pomZasielka->pridajCasDorucenia(new Datum(aktualnyCas_, casTransZPrekladiska));
		pomZasielka->zmenStavZasielky(transportPrvaFaza);
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

bool System::overSerioveCislo(int serioveCislo, int region)
{
	return (*listPrekladisk_)[region]->overSerioveCislo(serioveCislo);
}

void System::pridajDron(int cisOkresu, int cis, int typ)
{
	(*listPrekladisk_)[cisOkresu]->pridajDron(cis, typ, aktualnyCas_);
}

int System::spocitajPrepravZas()
{
	int pocet = 0;
	for (Zasielka* zas : *listZasielok_)
	{
		if (zas->dajStavZasielky() == vybavena)
		{
			pocet++;
		}
	}
	return pocet;
}

void System::vypisNalietaneHodiny()
{
	for (int i = 0; i < POCET_REGIONOV; i++)
	{
		cout << "\n  Prekladisko v regione " + to_string(i + 1) + ":" << endl;
		(*listPrekladisk_)[i]->vypisNalietHodiny();
	}
}

void System::vypisZrusZasVsetReg(structures::Array<int>* casObd)
{
	Datum* pomDatum;
	//bud sa pociatocny a koncovy den rovnaju a vtedy hodina zaciatku musi byt < ako hodina konca alebo je pociatocny den < ako den koncovy
	if (((*casObd)[0] == (*casObd)[2] && (*casObd)[1] < (*casObd)[3]) || ((*casObd)[0] < (*casObd)[2]))
	{
		structures::Array<int>* poctyZrusObj = new structures::Array<int>(POCET_REGIONOV);

		for (Zasielka* zas : *listZasielok_)
		{
			//ci bola zasielka zamietnuta po zaciatku obdobia ktore hladame, do konca obdobia, ktore hladame
			if (zas->dajStavZasielky() == zamietnuta)
			{
				pomDatum = zas->dajDatumOdoslania();

				if (pomDatum->dajDen() > (*casObd)[0] || (pomDatum->dajDen() == (*casObd)[0] && pomDatum->dajHodinu() >= (*casObd)[1]))
				{
					if (pomDatum->dajDen() < (*casObd)[2] || (pomDatum->dajDen() == (*casObd)[2] && pomDatum->dajHodinu() <= (*casObd)[3]))
					{
						(*poctyZrusObj)[zas->dajZaciatocnyRegion()] += 1;
					}
				}
			}
		}

		cout << "Pocet zrusenych zasielok v: " << endl;
		for (int i = 0; i < POCET_REGIONOV; i++)
		{
			cout << "  regione " + to_string(i+1) + ":\t" + to_string((*poctyZrusObj)[i]) << endl;
		}

		delete poctyZrusObj;
	}
	else
	{
		cout << "Chyba - Zle zadane casove obdobie!" << endl;
	}
	pomDatum = nullptr;
	delete casObd;
}

void System::vypisZrusZasVDanomReg(structures::Array<int>* casObd, int region)
{
	bool nicNenajdene = true;
	Datum* pomDatum;
	if (((*casObd)[0] == (*casObd)[2] && (*casObd)[1] < (*casObd)[3]) || ((*casObd)[0] < (*casObd)[2]))
	{
		for (Zasielka* zas : *listZasielok_)
		{
			if (zas->dajZaciatocnyRegion() == region && zas->dajStavZasielky() == zamietnuta)
			{
				pomDatum = zas->dajDatumOdoslania();

				if (pomDatum->dajDen() > (*casObd)[0] || (pomDatum->dajDen() == (*casObd)[0] && pomDatum->dajHodinu() >= (*casObd)[1]))
				{
					if (pomDatum->dajDen() < (*casObd)[2] || (pomDatum->dajDen() == (*casObd)[2] && pomDatum->dajHodinu() <= (*casObd)[3]))
					{
						cout << zas->toString() << endl;
						nicNenajdene = false;
					}
				}
			}
		}

		if (nicNenajdene)
		{
			cout << "Ziadne zasielky neboli v tomto casovom obdobi zrusene!" << endl;
		}
	}
	else
	{
		cout << "Chyba - Zle zadane casove obdobie!" << endl;
	}
	pomDatum = nullptr;
	delete casObd;
}

void System::vypisRegionSNajPoslanymiZas(structures::Array<int>* casObd)
{
	if (((*casObd)[0] == (*casObd)[2] && (*casObd)[1] < (*casObd)[3]) || ((*casObd)[0] < (*casObd)[2]))
	{
		structures::Array<int>* poctyOdosZas = new structures::Array<int>(POCET_REGIONOV);
		Datum * pomDatum;
		for (Zasielka* zas : *listZasielok_)
		{
			if (zas->dajStavZasielky() != zamietnuta)
			{
				pomDatum = zas->dajDatumOdoslania();

				if (pomDatum->dajDen() > (*casObd)[0] || (pomDatum->dajDen() == (*casObd)[0] && pomDatum->dajHodinu() >= (*casObd)[1]))
				{
					if (pomDatum->dajDen() < (*casObd)[2] || (pomDatum->dajDen() == (*casObd)[2] && pomDatum->dajHodinu() <= (*casObd)[3]))
					{
						(*poctyOdosZas)[zas->dajZaciatocnyRegion()] += 1;
					}
				}
			}
		}

		int maximum = 0;
		int maxRegion = 0;
		for (int i = 0; i < POCET_REGIONOV; i++)
		{
			if ((*poctyOdosZas)[i] > maximum)
			{
				maximum = (*poctyOdosZas)[i];
				maxRegion = i;
			}
		}

		cout << "Region s najviac odoslanymi zasielkami: " << to_string(maxRegion + 1) << " - odoslanych zasielok: " << to_string(maximum) << endl;
		delete poctyOdosZas;
		pomDatum = nullptr;
	}
	else
	{
		cout << "Chyba - Zle zadane casove obdobie!" << endl;
	}
	delete casObd;
}

void System::vypisRegionSNajDorucZas(structures::Array<int>* casObd)
{
	//zadany koniec cas. obdobia nemoze byt v buducnosti
	if ((*casObd)[2] < aktualnyCas_->dajDen() || ((*casObd)[2] == aktualnyCas_->dajDen() && (*casObd)[3] <= aktualnyCas_->dajHodinu()))
	{
		if (((*casObd)[0] == (*casObd)[2] && (*casObd)[1] < (*casObd)[3]) || ((*casObd)[0] < (*casObd)[2]))
		{
			structures::Array<int>* poctyDorucZas = new structures::Array<int>(POCET_REGIONOV);
			Datum * pomDatum;
			for (Zasielka* zas : *listZasielok_)
			{
				if (zas->dajStavZasielky() == vybavena)
				{
					pomDatum = zas->dajCasDorucenia();

					if (pomDatum->dajDen() > (*casObd)[0] || (pomDatum->dajDen() == (*casObd)[0] && pomDatum->dajHodinu() >= (*casObd)[1]))
					{
						if (pomDatum->dajDen() < (*casObd)[2] || (pomDatum->dajDen() == (*casObd)[2] && pomDatum->dajHodinu() <= (*casObd)[3]))
						{
							(*poctyDorucZas)[zas->dajKoncovyRegion()] += 1;
						}
					}
				}
			}

			int maximum = 0;
			int maxRegion = 0;
			for (int i = 0; i < POCET_REGIONOV; i++)
			{
				if ((*poctyDorucZas)[i] > maximum)
				{
					maximum = (*poctyDorucZas)[i];
					maxRegion = i;
				}
			}

			cout << "Region do ktoreho bolo dorucenych najviac zasielok " << to_string(maxRegion + 1) << " - dorucenych zasielok: " << to_string(maximum) << endl;
			delete poctyDorucZas;
			pomDatum = nullptr;
		}
		else
		{
			cout << "Chyba - Zle zadane casove obdobie!" << endl;
		}
	}
	else
	{
		cout << "Chyba - Zle zadane casove obdobie!" << endl;
	}

	delete casObd;
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
