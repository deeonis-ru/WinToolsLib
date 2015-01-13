// stdafx.cpp : source file that includes just the standard includes
// Tests.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <WinToolsLib\Trace.h>

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

TEST_MODULE_CLEANUP(OnCleanup)
{
	// Destructor of static object must be trivial.
	// With this call we are making TraceQueue dectructor kinda trivial
	// to avoid an exception in std::mutex::lock()
	// called from ConcurrentQueue<T>::PushBack
	// called from WorkQueue::~WorkQueue()
	// called from TraceQueue::~TraceQueue()
	// being called from DllMain with DLL_PROCESS_DETACH.
	WinToolsLib::TraceQueue::Instance().Exit();
}