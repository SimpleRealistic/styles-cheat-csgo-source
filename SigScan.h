#pragma once
#pragma once
#include <Windows.h>

class SigScan
{
private:
	char* base_addr;
	size_t base_len;


public:

	SigScan(void *base_addr, size_t base_len);
	SigScan(const char *module_name);
	SigScan(void *insider_addr);

	DWORD Scan(const char* signature, int offset = 0);
	static DWORD CalcAbsAddress(DWORD address);
	bool IsValid();

	char *GetBase();
	size_t GetLength();

};