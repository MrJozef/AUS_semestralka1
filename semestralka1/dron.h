#pragma once
#include "../structures/heap_monitor.h"
#include "../structures/queue/explicit_queue.h"
#include "datum.h"
#include "ramecRozvrhu.h"

class Dron
{
private:
	Datum* zarDoEvidencie_;
	int serioveCislo_;
	short typ_;
	int nalietMinuty_;
	int prepravZasielky_;

	///<summary>Nabitie je v %</summary>
	int nabitie_;
	///<summary>Stav ur�uje �i dron aktu�lne (na prelome hod�n) pracuje, je pr�zdny alebo sa nab�ja</summary>
	StavDronu stav_;
	///<summary>Rozvrh dne�nho d�a</summary>
	structures::ExplicitQueue<RamecRozvrhu*>* rozvrh_;
	int minutyZaneprazd_;

	///<summary>Pripraven� rozvrh na zajtraj�� de�</summary>
	structures::ExplicitQueue<RamecRozvrhu*>* buduciRozvrh_;
	///<summary>Potrebn� pre pl�novanie zajtraj�ka - 5e</summary>
	int buduceNabitie_;
	///<summary>Potrebn� pre pl�novanie zajtraj�ka - 5e</summary>
	int buduceMinutyZaneprazd_;

public:
	///<summary>Kon�truktor pri vytv�ran� v programe</summary>
	Dron(int cislo, int typ, Datum* zaradenie);
	///<summary>Kon�truktor pre na��tavanie zo s�boru</summary>
	Dron(fstream* inSubor);
	~Dron();

	string toString();
	///<summary>Umo��uje ulo�enie objektu do s�boru</summary>
	void toSubor(fstream* outSubor);

	int dajTyp();
	StavDronu dajStav();
	int dajNabitie();
	int dajSerioveCislo();
	///<summary>Vr�ti true/false podla toho, �i dolet� na dan� vzdialenos�</summary>
	bool doletis(int vzdialenost);

	int dajMinutyZaneprazd();
	///<summary>Vr�ti �as vyzdvihnutia</summary>
	int overCasVyzdvihnutia(int vzdialenost, Transport trans);
	///<summary>Vrati celkovy cas vyzdvihnutia zasielky + vratenie sa do prekladiska</summary>
	int transportujZasielku(int vzdialenost);
	///<summary>Men� �asov� rozvrh a aktualizuje potrebn� attr</summary>
	void dalsiaHodina();
	///<summary>Predov�etk�m vymie�a bud�ci rozvrh za s��asn� a priprav� sa na jeho vykon�vanie</summary>
	void dalsiaNoc();

	int dajBuduceMinutyZaneprazd();
	int dajBuduceNabitie();
	///<summary>Zarad� zasielku do svojho rozvrhu a prepravi ju za dany cas</summary>
	int pridajZasielkuNaPrepravu(int vzdialenost);

	int dajNalietaneMin();

private:
	///<summary>Interna funkcia, vypocita cas potrebny na nabijanie pred vyzdvihnutim zasielky</summary>
	int casNabijania(int vzdialenost, int nabitie);
	///<summary>Vrati trvanie letu na danu vzdialenost</summary>
	int dobaTrvaniaLetu(int vzdialenost);
	///<summary>Nacitanie zo suboru, volana konstruktorom</summary>
	void fromSubor(fstream* inSubor);
};