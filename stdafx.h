#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <psapi.h>
#include <time.h>
#include <process.h>
#include <vector>
#include <map>
#include <ostream>
#include <Shlobj.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <cmath>
#include <float.h>
#include <codecvt>
using namespace std;
#include <atlbase.h>
#include <atlcom.h>
#include <sapi.h>
#include <algorithm>
#include <iterator>

DWORD WINAPI CheatMain(LPVOID lpThreadParameter);

#include "Hooks.h"