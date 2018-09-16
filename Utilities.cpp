
#pragma once
#define _CRT_SECURE_NO_WARNINGS

// Includes
#include "Utilities.h"
#include <fstream>
#include <Psapi.h>
#include "Interfaces.h"

#define InRange(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (InRange((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (InRange(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

bool FileLog = false;
std::ofstream logFile;

// --------         U Core           ------------ //
// Opens a debug console
void U::OpenConsole(std::string Title)
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	SetConsoleTitle(Title.c_str());
}

// Closes the debug console
void U::CloseConsole()
{
	FreeConsole();
}

// Outputs text to the console
void U::Log(const char *fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	//Output to console
	if (logBuf[0] != '\0')
	{
		SetConsoleColor(FOREGROUND_MAGENTA);
		printf("[%s]", GetTimeString().c_str());
		SetConsoleColor(FOREGROUND_INTENSE_WHITE);
		printf(": %s\n", logBuf);
	}

	if (FileLog)
	{
		logFile << logBuf << std::endl;
	}
}

// Gets the current time as a string
std::string U::GetTimeString()
{
	//Time related variables
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];

	//Get current time
	time(&current_time);
	time_info = localtime(&current_time);

	//Get current time as string
	strftime(timeString, sizeof(timeString), "%I:%M%p", time_info);
	return timeString;
}

// Sets the console color for upcoming text
void U::SetConsoleColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Enables writing all log calls to a file
void U::EnableLogFile(std::string filename)
{
	logFile.open(filename.c_str());
	if (logFile.is_open())
		FileLog = true;
}

DWORD U::Memory::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}

ULONG U::Memory::PatternSearch(std::string sModuleName, PBYTE pbPattern, std::string sMask, ULONG uCodeBase, ULONG uSizeOfCode)
{
	BOOL bPatternDidMatch = FALSE;
	HMODULE hModule = GetModuleHandle(sModuleName.c_str());

	if (!hModule)
		return 0x0;

	PIMAGE_DOS_HEADER pDsHeader = PIMAGE_DOS_HEADER(hModule);
	PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS(LONG(hModule) + pDsHeader->e_lfanew);
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

	if (uCodeBase == 0x0)
		uCodeBase = (ULONG)hModule + pOptionalHeader->BaseOfCode;

	if (uSizeOfCode == 0x0)
		uSizeOfCode = pOptionalHeader->SizeOfCode;

	ULONG uArraySize = sMask.length();

	if (!uCodeBase || !uSizeOfCode || !uArraySize)
		return 0x0;

	for (size_t i = uCodeBase; i <= uCodeBase + uSizeOfCode; i++)
	{
		for (size_t t = 0; t < uArraySize; t++)
		{
			if (*((PBYTE)i + t) == pbPattern[t] || sMask.c_str()[t] == '?')
				bPatternDidMatch = TRUE;

			else
			{
				bPatternDidMatch = FALSE;
				break;
			}
		}

		if (bPatternDidMatch)
			return i;
	}

	return 0x0;
}

DWORD U::Memory::FindPatternVersion2(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;    //one ?
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}


DWORD U::Memory::FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

DWORD U::Memory::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	int len = strlen(string);
	char* szMask = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		szMask[i] = 'x';
	}
	szMask[len] = '\0';

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

void* U::Memory::CaptureInterfaceSZ(const char* chHandle, const char* chInterfaceName)
{
	volatile auto HandleGotten = (GetModuleHandleA(chHandle) != nullptr);
	while (!GetModuleHandleA(chHandle)) Sleep(100);
	void* fnFinal = nullptr;
	char* PossibleInterfaceName = new char[strlen(chInterfaceName) + 4];
	auto TestInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandleA(chHandle), XorStr("CreateInterface")));
	for (auto i = 100; i > 0; i--)
	{
		sprintf(PossibleInterfaceName, XorStr("%s%03i"), chInterfaceName, i);
		fnFinal = static_cast<void*>(TestInterface(PossibleInterfaceName, nullptr));

		if (fnFinal != nullptr)
			break;
	}
	delete[] PossibleInterfaceName;
	return fnFinal;
}

void* U::Memory::Hook(void* pInstance, int iIndex, void* pHookFunc)
{
	DWORD dwVTable = *(DWORD*)pInstance;
	DWORD dwVFunc = dwVTable + sizeof(DWORD) * iIndex;
	intptr_t pOrigFunc = *((DWORD*)dwVFunc);
	DWORD dwOldProtect;
	VirtualProtect((LPVOID)dwVFunc, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	*((DWORD*)dwVFunc) = (DWORD)pHookFunc;
	VirtualProtect((LPVOID)dwVFunc, sizeof(DWORD), dwOldProtect, &dwOldProtect);

	return (void*)pOrigFunc;
}

int U::FindNet(const char *table, const char *var)
{
	return GET_NETVAR(charenc(table), charenc(var));
}

// --------         Utilities Memory VMT Manager       ------------ //

bool U::Memory::VMTManager::Initialise(DWORD* InstancePointer)
{
	// Store the instance pointers and such, and work out how big the table is
	Instance = InstancePointer;
	OriginalTable = (DWORD*)*InstancePointer;
	int VMTSize = MethodCount(InstancePointer);
	size_t TableBytes = VMTSize * 4;

	// Allocate some memory and copy the table
	CustomTable = (DWORD*)malloc(TableBytes + 8);
	if (!CustomTable) return false;
	memcpy((void*)CustomTable, (void*)OriginalTable, VMTSize * 4);
	
	// Change the pointer
	*InstancePointer = (DWORD)CustomTable;

	initComplete = true;
	return true;
}

int	 U::Memory::VMTManager::MethodCount(DWORD* InstancePointer)
{
	DWORD *VMT = (DWORD*)*InstancePointer;
	int Index = 0;
	int Amount = 0;
	while (!IsBadCodePtr((FARPROC)VMT[Index]))
	{
		if (!IsBadCodePtr((FARPROC)VMT[Index]))
		{
			Amount++;
			Index++;
		}
	}

	return Amount;
}

DWORD U::Memory::VMTManager::HookMethod(DWORD NewFunction, int Index)
{
	if (initComplete)
	{
		CustomTable[Index] = NewFunction;
		return OriginalTable[Index];
	}
	else
		return NULL;
}

void U::Memory::VMTManager::UnhookMethod(int Index)
{
	if (initComplete)
		CustomTable[Index] = OriginalTable[Index];
	return;
}

void U::Memory::VMTManager::RestoreOriginal()
{
	if (initComplete)
	{
		*Instance = (DWORD)OriginalTable;
	}
	return;
}

void U::Memory::VMTManager::RestoreCustom()
{
	if (initComplete)
	{
		*Instance = (DWORD)CustomTable;
	}
	return;
}

DWORD U::Memory::VMTManager::GetOriginalFunction(int Index)
{
	return OriginalTable[Index];
}
