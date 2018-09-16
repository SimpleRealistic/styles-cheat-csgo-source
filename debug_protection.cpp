#include <iostream>
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>
#include <WinInet.h>
#include <gdiplus.h>
#include <algorithm>

#include "debug_protection.h"

DWORD GetProcessIdByName(LPCTSTR name)
{
	PROCESSENTRY32 pe32;
	HANDLE snapshot = NULL;
	DWORD pid = 0;

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot != INVALID_HANDLE_VALUE) {
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(snapshot, &pe32)) {
			do {
				std::string sName = pe32.szExeFile;
				std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);

				if (!lstrcmp(sName.c_str(), name)) {
					pid = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &pe32));
		}

		CloseHandle(snapshot);
	}

	return pid;
}

bool DebugProtection::IsDebugger()
{
	std::vector<std::string> m_processList;
	m_processList.push_back("ollydbg.exe");
	m_processList.push_back("wireshark.exe");
	m_processList.push_back("lordpe.exe");
	m_processList.push_back("hookshark.exe");
	m_processList.push_back("idag.exe");
	m_processList.push_back("MPGH Virus Scan Tool v6.exe");

	for (unsigned int ax = 0; ax < m_processList.size(); ax++)
	{
		std::string sProcess = m_processList.at(ax);
		if (GetProcessIdByName(sProcess.c_str()) != 0)
		{
			return true;
		}
	}

	return false;
}