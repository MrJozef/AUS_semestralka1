#pragma once
#include "../structures/heap_monitor.h"
#include "const.h"

class RamecRozvrhu
{
private:
	///<summary>nab�ja sa / pracuje</summary>
	StavDronu cinnost_;
	int casVykonavania_;

public:
	///<summary>Kon�truktor pri vytv�ran� v programe</summary>
	RamecRozvrhu(StavDronu co, int akoDlho);
	///<summary>Kon�truktor pre na��tavanie zo s�boru</summary>
	RamecRozvrhu(fstream* inSubor);
	~RamecRozvrhu();
	///<summary>Umo��uje ulo�enie objektu do s�boru</summary>
	void toSubor(fstream* outSubor);
	///<summary>Na��tanie zo suboru, volana konstruktorom</summary>
	void fromSubor(fstream* inSubor);

	///<summary>Odpo��tanie dan�ho po�tu min�t z d�ky trvania �innosti</summary>
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
