#pragma once
#include "Types.h"
#include "Log.h"
#include "Singleton.h"
#include "Concurrency\WorkQueue.h"

#if LOG_LEVEL & 0x0010
#define ENABLE_TRACE_INFO
#endif

#if LOG_LEVEL & 0x0008
#define ENABLE_TRACE_EXCEPTION
#endif

#if LOG_LEVEL & 0x0004
#define ENABLE_TRACE_WARNING
#endif

#if LOG_LEVEL & 0x0002
#define ENABLE_TRACE_ERROR
#endif

#if LOG_LEVEL & 0x0001
#define ENABLE_TRACE_FATAL
#endif

namespace WinToolsLib
{
	class TraceQueue : public Concurrency::WorkQueue, public Singleton<TraceQueue>
	{
	protected:
		TraceQueue() {}
		~TraceQueue() {}

		friend class Singleton<TraceQueue>;
	};

#ifdef ENABLE_TRACE_INFO
#define TRACE_INFO(...) \
	TraceQueue::Instance().PushBack([=] \
	{ \
		LOG_INFO(__VA_ARGS__); \
	});
#else
#define TRACE_INFO(...)
#endif

#ifdef ENABLE_TRACE_WARNING
#define TRACE_WARNING(...) \
	TraceQueue::Instance().PushBack([=] \
	{ \
		LOG_WARNING(__VA_ARGS__); \
	});
#else
#define TRACE_WARNING(...)
#endif

#ifdef ENABLE_TRACE_ERROR
#define TRACE_ERROR(...) \
	TraceQueue::Instance().PushBack([=] \
	{ \
		LOG_ERROR(__VA_ARGS__); \
	});
#else
#define TRACE_ERROR(...)
#endif

#ifdef ENABLE_TRACE_FATAL
#define TRACE_FATAL(...) \
	TraceQueue::Instance().PushBack([=] \
	{ \
		LOG_FATAL(__VA_ARGS__); \
	});
#else
#define TRACE_FATAL(...)
#endif

}



