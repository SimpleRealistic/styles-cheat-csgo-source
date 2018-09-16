
#include "Dumping.h"

#define DUMPIDTOFILE

void Dump::DumpClassIds()
{
#ifdef DUMPIDTOFILE
	U::EnableLogFile("ClassID.txt");
#endif
	ClientClass* cClass = I::Client->GetAllClasses();
	while (cClass)
	{
		U::Log("%s = %d,", cClass->m_pNetworkName, cClass->m_ClassID);
		cClass = cClass->m_pNext;
	}
}