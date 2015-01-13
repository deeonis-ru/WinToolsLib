#pragma once
#include "Types.h"
#include "String.h"
#include "DebugOutput.h"

#if LOG_LEVEL & 0x0100
#define ENABLE_LOG_CHECKPOINT
#endif

#if LOG_LEVEL & 0x0010
#define ENABLE_LOG_INFO
#endif

#if LOG_LEVEL & 0x0008
#define ENABLE_LOG_EXCEPTION
#endif

#if LOG_LEVEL & 0x0004
#define ENABLE_LOG_WARNING
#endif

#if LOG_LEVEL & 0x0002
#define ENABLE_LOG_ERROR
#endif

#if LOG_LEVEL & 0x0001
#define ENABLE_LOG_FATAL
#endif

namespace WinToolsLib
{
#ifdef ENABLE_LOG_CHECKPOINT
#define LOG_CHECKPOINT(...) DebugOutput(Text("<P> %s"), String::FormatS(__VA_ARGS__).GetBuffer())
#else
#define LOG_CHECKPOINT(...)
#endif

#ifdef ENABLE_LOG_INFO
#define LOG_INFO(...) DebugOutput(Text("<I> %s"), String::FormatS(__VA_ARGS__).GetBuffer())
#else
#define LOG_INFO(...)
#endif
	
#ifdef ENABLE_LOG_EXCEPTION
#define LOG_EXCEPTION(...) DebugOutput(Text("<X> %s"), String::FormatS(__VA_ARGS__).GetBuffer())
#else
#define LOG_EXCEPTION(...)
#endif

#ifdef ENABLE_LOG_WARNING
#define LOG_WARNING(...) DebugOutput(Text("<W> %s"), String::FormatS(__VA_ARGS__).GetBuffer())
#else
#define LOG_WARNING(...)
#endif

#ifdef ENABLE_LOG_ERROR
#define LOG_ERROR(...) DebugOutput(Text("<E> %s"), String::FormatS(__VA_ARGS__).GetBuffer())
#else
#define LOG_ERROR(...)
#endif

#ifdef ENABLE_LOG_FATAL
#define LOG_FATAL(...) DebugOutput(Text("<F> %s"), String::FormatS(__VA_ARGS__).GetBuffer())
#else
#define LOG_FATAL(...)
#endif
}
