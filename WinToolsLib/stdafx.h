// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// C RunTime Header Files
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <mbstring.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdexcept>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <tchar.h>
#include <time.h>

// Windows Header Files
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <WinSock2.h>

#include <CommDlg.h>
#include <comutil.h>
#include <IntShCut.h>
#include <IPHlpApi.h>
#include <mscat.h>
#include <MSTask.h>
#include <Objbase.h>
#include <OleAuto.h>
#include <Rpc.h>
#include <ShellAPI.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <Shtypes.h>
#include <SoftPub.h>
#include <TlHelp32.h>
#include <Unknwn.h>
#include <WbemIdl.h>
#include <WinCred.h>
#include <WinCrypt.h>
#include <WinInet.h>
#include <WinTrust.h>
#include <WtsApi32.h>

// STL Header Files
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <istream>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <thread>
#include <utility>
#include <vector>
