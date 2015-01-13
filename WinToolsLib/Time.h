#pragma once
#include "Types.h"
#include "String.h"

#include <Windows.h>

namespace WinToolsLib
{
	class Time
	{
	public:
		Time(
			UInt16 year = 1601,
			UInt16 month = 1,
			UInt16 day = 1,
			UInt16 hour = 0,
			UInt16 minute = 0,
			UInt16 second = 0,
			UInt16 milliseconds = 0);
		
		Time(Time&& other);
		Time(const Time& other);

		Time& operator=(Time&& other);
		Time& operator=(const Time& other);

		enum Format
		{
			Default,
			FileName,
			UserTime,
			ShortDate,
			ShortDateTime,
			LongDate,
			LongDateTime,
			WmiDateTime,
			IsoDateTime
		};

		String ToString(Format format = Default) const;
		Void ToFileTime(FILETIME* fileTime) const;
		std::time_t ToUnixTime() const;
		Time ToLocal() const;

		UInt16 GetYear() const;
		UInt16 GetMonth() const;
		UInt16 GetDay() const;
		UInt16 GetHour() const;
		UInt16 GetMinute() const;
		UInt16 GetSecond() const;
		UInt16 GetMilliseconds() const;

		Bool operator<(const Time& other) const;
		Bool operator>(const Time& other) const;

		Bool operator<=(const Time& other) const;
		Bool operator>=(const Time& other) const;

		Bool operator==(const Time& other) const;
		Bool operator!=(const Time& other) const;

		std::time_t Difference(const Time& other) const;

		static Time GetSystemTime();
		static Time FromFileTime(FILETIME* fileTime);
		static Time FromUnixTime(std::time_t unixTime);
		static Time FromWmiDateTime(const WChar* dateTime);
		static Time FromIsoDateTime(const WChar* dateTime);

	private:
		SYSTEMTIME m_time;
	};

	inline UInt16 Time::GetYear() const
	{
		return m_time.wYear;
	}

	inline UInt16 Time::GetMonth() const
	{
		return m_time.wMonth;
	}

	inline UInt16 Time::GetDay() const
	{
		return m_time.wDay;
	}

	inline UInt16 Time::GetHour() const
	{
		return m_time.wHour;
	}

	inline UInt16 Time::GetMinute() const
	{
		return m_time.wMinute;
	}

	inline UInt16 Time::GetSecond() const
	{
		return m_time.wSecond;
	}

	inline UInt16 Time::GetMilliseconds() const
	{
		return m_time.wMilliseconds;
	}

}
