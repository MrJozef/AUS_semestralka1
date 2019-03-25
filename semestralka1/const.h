#pragma once
#include <string>
#include <fstream>
#include "../structures/heap_monitor.h"

const int ASCII_CISLO_CISLICA = 48;			//rozdiel medzi cislicou v ascii (char) a cislom, ktore predstavuje (int)

const int POCET_POLOZIEK_MENU = 8;
const std::string CESTA_K_SUBORU = "data.txt";

const int ZACIATOCNA_HOD_DNA = 7;
const int KONECNA_HOD_DNA = 21;
const int POSLEDNA_HOD_NA_VYZD_DRONOM = 20;

const int POCET_REGIONOV = 25;
const int REGION_ZILINA = 7;
const int STUPNE_FAHRENHEITA = 451;			//only Easter Egg, potrebujem nejake cislo do funkcie, ktora mi zabezpeci, aby som nenacital nulu a na takuto vzdialenost urcite nikdy dron nedokaze nic prepravit

//drony
const int POCET_TYPOV_DRONOV = 2;

const int DRON_1_MAX_NOSNOST = 2;
const int DRON_1_RYCHLOST = 80;
const int DRON_1_DOBA_LETU = 40;
const int DRON_1_NABITIE_10_PERC_BATERIE = 3;
const int DRON_2_MAX_NOSNOST = 5;
const int DRON_2_RYCHLOST = 40;
const int DRON_2_DOBA_LETU = 60;
const int DRON_2_NABITIE_10_PERC_BATERIE = 5;

inline int dajNosnostDronu(int typDronu)
{
	switch(typDronu)
	{
	case 1:
		return DRON_1_MAX_NOSNOST;
	case 2:
		return DRON_2_MAX_NOSNOST;
	default:
		return 0;
	}
}

inline int dajRychlostDronu(int typDronu)
{
	switch (typDronu)
	{
	case 1:
		return DRON_1_RYCHLOST;
	case 2:
		return DRON_2_RYCHLOST;
	default:
		return 0;
	}
}

inline int dajDobuLetuDronu(int typDronu)
{
	switch (typDronu)
	{
	case 1:
		return DRON_1_DOBA_LETU;
	case 2:
		return DRON_2_DOBA_LETU;
	default:
		return 0;
	}
}

inline int dajDobuNabijaniaDronu(int typDronu)
{
	switch (typDronu)
	{
	case 1:
		return DRON_1_NABITIE_10_PERC_BATERIE;
	case 2:
		return DRON_2_NABITIE_10_PERC_BATERIE;
	default:
		return 0;
	}
}

enum StavDronu
{
	volny,
	pracuje,
	nabija
};

inline std::string toStringStavDronu(StavDronu x)
{
	switch (x)
	{
	case pracuje:
		return "Dron aktualne pracuje\n";
	case nabija:
		return "Dron sa aktualne nabija\n";
	case volny:
	default:
		return "Dron je aktualne volny (nepracuje, nabija sa)\n";
	}
}

enum Region		//todo aktualne nepouzivane
{
	MA, BA, TT, TN, NR, KN, CA, ZA, MT, PD, LV, NO, LM, BB, ZV, KA, LC, PP, RA, SL, SN, PO, KE, HE, MI
};

//Objednavky

enum DovodZamietnutia
{
	nezamietnuta,
	neskorVyzdvihnutie,
	mimoRadius,
	velkaHmotnost,
	vozNeuvezie,
	plnePrekladisko,		//todo neskore implementovanie
	zrusZakaznikom
};

inline std::string toStringDovodZamietnutia(DovodZamietnutia x)
{
	switch(x)
	{
	case (neskorVyzdvihnutie):
		return "Zamietnutie A - Zasielka by musela byt vyzdvihnuta po 20:00\n";
	case (mimoRadius):
		return "Zamietnutie B - Zasielka je mimo radius dronov\n";
	case (velkaHmotnost):
		return "Zamietnutie C - Hmotnost zasielky prekracuje nosnost dronov\n";
	case (vozNeuvezie):
		return "Zamietnutie D - Zasielku nie je mozne previest dostupnymi vozidlami\n";
	case (plnePrekladisko):
		return "Zamietnutie E - Prekladisko je prilis zaneprazdnene\n";
	case (zrusZakaznikom):
		return "Zakaznik svoju objednavku zrusil\n";

	case (nezamietnuta):
	default:
		return "";
	}
}