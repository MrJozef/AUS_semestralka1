#pragma once
#include "../structures/heap_monitor.h"
#include "const.h"

class RamecRozvrhu
{
private:
	StavDronu cinnost_;
	int casVykonavania_;

public:
	RamecRozvrhu(StavDronu co, int akoDlho);
	RamecRozvrhu(fstream* inSubor);
	~RamecRozvrhu();
	void toSubor(fstream* outSubor);
	void fromSubor(fstream* inSubor);

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
