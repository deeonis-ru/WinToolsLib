#include "Time.h"

#include <sstream>

namespace WinToolsLib
{
	Time::Time(
		UInt16 year,
		UInt16 month,
		UInt16 day,
		UInt16 hour,
		UInt16 minute,
		UInt16 second,
		UInt16 milliseconds)
	{
		ZeroMemory(&m_time, sizeof(m_time));
		
		m_time.wYear = year;
		m_time.wMonth = month;
		m_time.wDay = day;
		m_time.wHour = hour;
		m_time.wMinute = minute;
		m_time.wSecond = second;
		m_time.wMilliseconds = milliseconds;
	}

	Time::Time(Time&& other)
	{
		CopyMemory(&m_time, &other.m_time, sizeof(m_time));
		ZeroMemory(&other.m_time, sizeof(m_time));
	}

	Time::Time(const Time& other)
	{
		CopyMemory(&m_time, &other.m_time, sizeof(m_time));
	}

	Time& Time::operator=(Time&& other)
	{
		CopyMemory(&m_time, &other.m_time, sizeof(m_time));
		ZeroMemory(&other.m_time, sizeof(m_time));
		return *this;
	}

	Time& Time::operator=(const Time& other)
	{
		CopyMemory(&m_time, &other.m_time, sizeof(m_time));
		return *this;
	}

	String Time::ToString(Format format) const
	{
		switch (format)
		{
		case LongDateTime:
			{
				TChar date[32];
				::GetDateFormat(
					MAKELCID(LOCALE_USER_DEFAULT, SORT_DEFAULT),
					DATE_LONGDATE,
					&m_time,
					NULL,
					date,
					31);

				TChar time[32];
				::GetTimeFormat(
					MAKELCID(LOCALE_USER_DEFAULT, SORT_DEFAULT),
					0,
					&m_time,
					NULL,
					time,
					31);

				String string(date);
				string += Text(" ");
				string += time;
				return string;
			}
		case LongDate:
			{
				TChar date[32];
				::GetDateFormat(
					MAKELCID(LOCALE_USER_DEFAULT, SORT_DEFAULT),
					DATE_LONGDATE,
					&m_time,
					NULL,
					date,
					31);
				return String(date);
			}
		case ShortDateTime:
			{
				TChar date[32];
				::GetDateFormat(
					MAKELCID(LOCALE_USER_DEFAULT, SORT_DEFAULT),
					DATE_SHORTDATE,
					&m_time,
					NULL,
					date,
					31);

				TChar time[32];
				::GetTimeFormat(
					MAKELCID(LOCALE_USER_DEFAULT, SORT_DEFAULT),
					0,
					&m_time,
					NULL,
					time,
					31);

				String string(date);
				string += Text(" ");
				string += time;
				return string;
			}
		case ShortDate:
			{
				TChar date[32];
				::GetDateFormat(
					MAKELCID(LOCALE_USER_DEFAULT, SORT_DEFAULT),
					DATE_SHORTDATE,
					&m_time,
					NULL,
					date,
					31);
				return String(date);
			}
		case UserTime:
			{
				TChar time[32];
				::GetTimeFormat(
					MAKELCID(LOCALE_USER_DEFAULT, SORT_DEFAULT),
					0,
					&m_time,
					NULL,
					time,
					31);
				return String(time);
			}
		case FileName:
			{
				return String::FormatS(
					Text("%04u-%02u-%02u_%02u-%02u-%02u"),
					m_time.wYear,
					m_time.wMonth,
					m_time.wDay,
					m_time.wHour,
					m_time.wMinute,
					m_time.wSecond);
			}
		case WmiDateTime:
			{
				return String::FormatS(
					Text("%04u%02u%02u%02u%02u%02u.%06u"),
					m_time.wYear,
					m_time.wMonth,
					m_time.wDay,
					m_time.wHour,
					m_time.wMinute,
					m_time.wSecond,
					m_time.wMilliseconds);
			}
		case IsoDateTime:
			{
				return String::FormatS(
					Text("%04u-%02u-%02uT%02u:%02u:%02u"),
					m_time.wYear,
					m_time.wMonth,
					m_time.wDay,
					m_time.wHour,
					m_time.wMinute,
					m_time.wSecond);
			}
		default:
			{
				return String::FormatS(
					Text("%04u-%02u-%02u %02u:%02u:%02u"),
					m_time.wYear,
					m_time.wMonth,
					m_time.wDay,
					m_time.wHour,
					m_time.wMinute,
					m_time.wSecond);
			}
		}
	}

	Void Time::ToFileTime(FILETIME* fileTime) const
	{
		::SystemTimeToFileTime(&m_time, fileTime);
	}

	std::time_t Time::ToUnixTime() const
	{
		FILETIME fileTime = {0};
		ToFileTime(&fileTime);

		Int64 tmp = 0;
		tmp = ((tmp | fileTime.dwHighDateTime) << 32) + fileTime.dwLowDateTime;
		return tmp / 10000000 - 11644473600;
	}

	Time Time::ToLocal() const
	{
		Time time;
		::SystemTimeToTzSpecificLocalTime(NULL, &m_time, &time.m_time);
		return time;
	}

	Bool Time::operator<(const Time& other) const
	{
		if (m_time.wYear < other.m_time.wYear) return True;
		if (m_time.wYear > other.m_time.wYear) return False;

		if (m_time.wMonth < other.m_time.wMonth) return True;
		if (m_time.wMonth > other.m_time.wMonth) return False;

		if (m_time.wDay < other.m_time.wDay) return True;
		if (m_time.wDay > other.m_time.wDay) return False;

		if (m_time.wHour < other.m_time.wHour) return True;
		if (m_time.wHour > other.m_time.wHour) return False;

		if (m_time.wMinute < other.m_time.wMinute) return True;
		if (m_time.wMinute > other.m_time.wMinute) return False;

		if (m_time.wSecond < other.m_time.wSecond) return True;
		if (m_time.wSecond > other.m_time.wSecond) return False;

		if (m_time.wMilliseconds < other.m_time.wMilliseconds) return True;
		return False;
	}

	Bool Time::operator>(const Time& other) const
	{
		return !operator<=(other);
	}

	Bool Time::operator<=(const Time& other) const
	{
		if (m_time.wYear < other.m_time.wYear) return True;
		if (m_time.wYear > other.m_time.wYear) return False;

		if (m_time.wMonth < other.m_time.wMonth) return True;
		if (m_time.wMonth > other.m_time.wMonth) return False;

		if (m_time.wDay < other.m_time.wDay) return True;
		if (m_time.wDay > other.m_time.wDay) return False;

		if (m_time.wHour < other.m_time.wHour) return True;
		if (m_time.wHour > other.m_time.wHour) return False;

		if (m_time.wMinute < other.m_time.wMinute) return True;
		if (m_time.wMinute > other.m_time.wMinute) return False;

		if (m_time.wSecond < other.m_time.wSecond) return True;
		if (m_time.wSecond > other.m_time.wSecond) return False;

		if (m_time.wMilliseconds <= other.m_time.wMilliseconds) return True;
		return False;
	}

	Bool Time::operator>=(const Time& other) const
	{
		return !operator<(other);
	}

	Bool Time::operator==(const Time& other) const
	{
		return
			m_time.wYear         == other.m_time.wYear &&
			m_time.wMonth        == other.m_time.wMonth &&
			m_time.wDay          == other.m_time.wDay &&
			m_time.wHour         == other.m_time.wHour &&
			m_time.wMinute       == other.m_time.wMinute &&
			m_time.wSecond       == other.m_time.wSecond &&
			m_time.wMilliseconds == other.m_time.wMilliseconds;
	}

	Bool Time::operator!=(const Time& other) const
	{
		return !operator==(other);
	}

	std::time_t Time::Difference(const Time& other) const
	{
		FILETIME selfTime = {0};
		::SystemTimeToFileTime(&m_time, &selfTime);
		FILETIME otherTime = {0};
		::SystemTimeToFileTime(&other.m_time, &otherTime);

		std::time_t selfUnix = selfTime.dwHighDateTime;
		selfUnix = (selfUnix << 32) + selfTime.dwLowDateTime;
		selfUnix = selfUnix / 10000000 - 11644473600LL;

		std::time_t otherUnix = otherTime.dwHighDateTime;
		otherUnix = (otherUnix << 32) + otherTime.dwLowDateTime;
		otherUnix = otherUnix / 10000000 - 11644473600LL;

		return selfUnix - otherUnix;
	}

	Time Time::GetSystemTime()
	{
		Time time;
		::GetSystemTime(&time.m_time);
		return time;
	}

	Time Time::FromFileTime(FILETIME* fileTime)
	{
		Time time;
		::FileTimeToSystemTime(fileTime, &time.m_time);
		return time;
	}

	Time Time::FromUnixTime(std::time_t unixTime)
	{
		FILETIME fileTime = {0};
		Int64 tmp = Int32x32To64(unixTime, 10000000) + 116444736000000000;
		fileTime.dwLowDateTime = (DWORD)tmp;
		fileTime.dwHighDateTime = tmp >> 32;

		return FromFileTime(&fileTime);
	}
	
	//
	// Format: yyyymmddHHMMSS.xxxxxxsUUU
	//
	// yyyy   Four-digit year (0000 through 9999).
	// mm     Two-digit month (01 through 12).
	// dd     Two-digit day of the month (01 through 31).
	// HH     Two-digit hour of the day using the 24-hour clock (00 through 23).
	// MM     Two-digit minute in the hour (00 through 59).
	// SS     Two-digit number of seconds in the minute (00 through 59).
	// xxxxxx Six-digit number of microseconds in the second (000000 through 999999).
	// s      Plus sign (+) or minus sign (-) to indicate a positive or negative offset from Coordinated Universal Times (UTC).
	// UUU    Three-digit offset indicating the number of minutes that the originating time zone deviates from UTC.
	//
	// FIXME: Needs to be rewritten in more safely way!
	Time Time::FromWmiDateTime(const WChar* dateTime)
	{
		WChar buffer[26];
		wcscpy_s(buffer, dateTime);

		auto length = wcslen(buffer);
		if (length < 25)
		{
			return Time();
		}

		Time time;

		auto ptr = buffer + length - 3;
		auto zone = wcstoul(ptr, nullptr, 10);
		
		*ptr = L'\0';
		ptr -= 1;

		auto sign = *ptr;
		
		*ptr = L'\0';
		ptr -= 3;
		*ptr = L'\0';
		ptr -= 3;

		time.m_time.wMilliseconds = (WORD)wcstoul(ptr, nullptr, 10);
		
		ptr -= 1;
		*ptr = L'\0';
		ptr -= 2;

		time.m_time.wSecond = (WORD)wcstoul(ptr, nullptr, 10);
		
		*ptr = L'\0';
		ptr -= 2;

		time.m_time.wMinute = (WORD)wcstoul(ptr, nullptr, 10);
		
		*ptr = L'\0';
		ptr -= 2;

		time.m_time.wHour = (WORD)wcstoul(ptr, nullptr, 10);
		
		*ptr = L'\0';
		ptr -= 2;

		time.m_time.wDay = (WORD)wcstoul(ptr, nullptr, 10);
		
		*ptr = L'\0';
		ptr -= 2;

		time.m_time.wMonth = (WORD)wcstoul(ptr, nullptr, 10);
		
		*ptr = L'\0';
		ptr -= 4;

		time.m_time.wYear = (WORD)wcstoul(ptr, nullptr, 10);
		return time;
	}
	
	//
	// Supported format: YYYY-MM-DDTHH:MM:SS
	// Not implemented:  YYYY-MM-DDTHH:MM:SS+HH:MM
	//                   YYYY-MM-DDTHH:MM:SS-HH:MM
	//                   YYYY-MM-DDTHH:MM:SSZ
	//
	// YYYY   Four-digit year (0000 through 9999).
	// MM     Two-digit month (01 through 12).
	// DD     Two-digit day of the month (01 through 31).
	// HH     Two-digit hour of the day using the 24-hour clock (00 through 23).
	// MM     Two-digit minute in the hour (00 through 59).
	// SS     Two-digit number of seconds in the minute (00 through 59).
	// +|-|Z  Plus sign (+) or minus sign (-) or Z to indicate a positive or negative or zero offset from Coordinated Universal Time (UTC).
	//
	Time Time::FromIsoDateTime(const WChar* dateTime)
	{
		std::wistringstream stream(dateTime);
		WChar skip;
		
		Time time;
		stream >> time.m_time.wYear;
		stream >> skip;
		stream >> time.m_time.wMonth;
		stream >> skip;
		stream >> time.m_time.wDay;
		stream >> skip;
		stream >> time.m_time.wHour;
		stream >> skip;
		stream >> time.m_time.wMinute;
		stream >> skip;
		stream >> time.m_time.wSecond;
		return time;
	}
}