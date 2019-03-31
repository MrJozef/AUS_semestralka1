#pragma once
#include "../structures/heap_monitor.h"
#include "const.h"

class RamecRozvrhu
{
private:
	///<summary>nabíja sa / pracuje</summary>
	StavDronu cinnost_;
	int casVykonavania_;

public:
	///<summary>Konštruktor pri vytváraní v programe</summary>
	RamecRozvrhu(StavDronu co, int akoDlho);
	///<summary>Konštruktor pre naèítavanie zo súboru</summary>
	RamecRozvrhu(fstream* inSubor);
	~RamecRozvrhu();
	///<summary>Umožòuje uloženie objektu do súboru</summary>
	void toSubor(fstream* outSubor);
	///<summary>Naèítanie zo suboru, volana konstruktorom</summary>
	void fromSubor(fstream* inSubor);

	///<summary>Odpoèítanie daného poètu minút z dåžky trvania èinnosti</summary>
	int aktualizuj(int prejdenyCas);
	int dajDobuTrvania();
	StavDronu dajCinnost();
};

inline RamecRozvrhu::RamecRozvrhu(StavDronu co, int akoDlho)
{
	cinnost_ = co;
	casVykonavania_ = akoDlho;
}

inline RamecRozvrhu::RamecRozvrhu(fstream* inSubor)
{
	fromSubor(inSubor);
}

inline RamecRozvrhu::~RamecRozvrhu()
{
}

inline void RamecRozvrhu::toSubor(fstream* outSubor)
{
	*outSubor << cinnost_ << "\n" << casVykonavania_ << "\n";
}

inline void RamecRozvrhu::fromSubor(fstream* inSubor)
{
	int pom;

	*inSubor >> pom;
	cinnost_ = static_cast<StavDronu>(pom);
	*inSubor >> casVykonavania_;
}

inline int RamecRozvrhu::aktualizuj(int prejdenyCas)
{
	casVykonavania_ -= prejdenyCas;
	return casVykonavania_;
}

inline int RamecRozvrhu::dajDobuTrvania()
{
	return casVykonavania_;
}

inline StavDronu RamecRozvrhu::dajCinnost()
{
	return cinnost_;
}
