#include "stdafx.h"
#include <WinToolsLib\Time.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(TimeTest)
	{
	public:
		TEST_METHOD(TimeFromWmiDateTime)
		{
			{
				auto time = Time::FromWmiDateTime(L"00000000000000.000000+000");
				Assert::AreEqual(time.ToString(), L"0000-00-00 00:00:00");
			}
			{
				auto time = Time::FromWmiDateTime(L"20011224113047.000000-480");
				Assert::AreEqual(time.ToString(), L"2001-12-24 11:30:47");
			}
			{
				auto time = Time::FromWmiDateTime(L"20020903000000.000000+480");
				Assert::AreEqual(time.ToString(), L"2002-09-03 00:00:00");
			}
		}

		TEST_METHOD(TimeFromIsoDateTime)
		{
			{
				auto time = Time::FromIsoDateTime(L"0000-00-00T00:00:00");
				Assert::AreEqual(time.ToString(), L"0000-00-00 00:00:00");
			}
			{
				auto time = Time::FromIsoDateTime(L"1970-12-31T23:00:59");
				Assert::AreEqual(time.ToString(), L"1970-12-31 23:00:59");
			}
			{
				auto time = Time::FromIsoDateTime(L"1234-12-31T12:34");
				Assert::AreEqual(time.ToString(), L"1234-12-31 12:34:00");
			}
			{
				auto time = Time::FromIsoDateTime(L"1234-12-31T12");
				Assert::AreEqual(time.ToString(), L"1234-12-31 12:00:00");
			}
			{
				auto time = Time::FromIsoDateTime(L"1234-12-31");
				Assert::AreEqual(time.ToString(), L"1234-12-31 00:00:00");
			}
		}

		TEST_METHOD(TimeToString)
		{
			Time time(2013, 7, 12, 16, 28, 3, 789);
			Assert::AreEqual(time.ToString(),                  L"2013-07-12 16:28:03");
			Assert::AreEqual(time.ToString(Time::Default),     L"2013-07-12 16:28:03");
			Assert::AreEqual(time.ToString(Time::FileName),    L"2013-07-12_16-28-03");
			Assert::AreEqual(time.ToString(Time::IsoDateTime), L"2013-07-12T16:28:03");
			Assert::AreEqual(time.ToString(Time::WmiDateTime), L"20130712162803.000789");
		}

		TEST_METHOD(TimeFromFileTime)
		{
			SYSTEMTIME systemTime = {0};
			systemTime.wYear = 2013;
			systemTime.wMonth = 7;
			systemTime.wDay = 12;
			systemTime.wHour = 16;
			systemTime.wMinute = 28;
			systemTime.wSecond = 3;
			systemTime.wMilliseconds = 789;

			FILETIME fileTime = {0};
			::SystemTimeToFileTime(&systemTime, &fileTime);

			auto time = Time::FromFileTime(&fileTime);
			Assert::AreEqual(systemTime.wYear, time.GetYear());
			Assert::AreEqual(systemTime.wMonth, time.GetMonth());
			Assert::AreEqual(systemTime.wDay, time.GetDay());
			Assert::AreEqual(systemTime.wHour, time.GetHour());
			Assert::AreEqual(systemTime.wMinute, time.GetMinute());
			Assert::AreEqual(systemTime.wSecond, time.GetSecond());
			Assert::AreEqual(systemTime.wMilliseconds, time.GetMilliseconds());
		}

		TEST_METHOD(TimeToFileTime)
		{
			SYSTEMTIME systemTime = {0};
			systemTime.wYear = 2013;
			systemTime.wMonth = 7;
			systemTime.wDay = 12;
			systemTime.wHour = 16;
			systemTime.wMinute = 28;
			systemTime.wSecond = 3;
			systemTime.wMilliseconds = 789;

			FILETIME fileTime1 = {0};
			::SystemTimeToFileTime(&systemTime, &fileTime1);

			Time time(2013, 7, 12, 16, 28, 3, 789);

			FILETIME fileTime2 = {0};
			time.ToFileTime(&fileTime2);

			Assert::AreEqual(fileTime1.dwLowDateTime, fileTime2.dwLowDateTime);
			Assert::AreEqual(fileTime1.dwHighDateTime, fileTime2.dwHighDateTime);
		}

		TEST_METHOD(TimeComparision)
		{
			//
			// operator<
			//
			Assert::IsTrue(Time(1999, 12, 31, 23, 59, 59, 999) < Time(2000));
			Assert::IsTrue(Time(2013) < Time(2013, 2));
			Assert::IsTrue(Time(2013, 3, 19) < Time(2013, 3, 20));
			Assert::IsTrue(Time(2013, 5, 10) < Time(2013, 5, 10, 23));
			Assert::IsTrue(Time(2013, 7, 30, 12) < Time(2013, 7, 30, 12, 30));
			Assert::IsTrue(Time(2013, 9, 15, 20, 30) < Time(2013, 9, 15, 20, 30, 1));
			Assert::IsTrue(Time(2013, 9, 15, 20, 30, 40) < Time(2013, 9, 15, 20, 30, 40, 333));

			//
			// operator>
			//
			Assert::IsFalse(Time(1999, 12, 31, 23, 59, 59, 999) > Time(2000));
			Assert::IsFalse(Time(2013) > Time(2013, 2));
			Assert::IsFalse(Time(2013, 3, 19) > Time(2013, 3, 20));
			Assert::IsFalse(Time(2013, 5, 10) > Time(2013, 5, 10, 23));
			Assert::IsFalse(Time(2013, 7, 30, 12) > Time(2013, 7, 30, 12, 30));
			Assert::IsFalse(Time(2013, 9, 15, 20, 30) > Time(2013, 9, 15, 20, 30, 1));
			Assert::IsFalse(Time(2013, 9, 15, 20, 30, 40) > Time(2013, 9, 15, 20, 30, 40, 333));
			Assert::IsFalse(Time(2013, 12, 31, 23, 59, 59, 999) > Time(2013, 12, 31, 23, 59, 59, 999));
			
			//
			// operator<=
			//
			Assert::IsTrue(Time(1999, 12, 31, 23, 59, 59, 999) <= Time(2000));
			Assert::IsTrue(Time(2013) <= Time(2013, 2));
			Assert::IsTrue(Time(2013, 3, 19) <= Time(2013, 3, 20));
			Assert::IsTrue(Time(2013, 5, 10) <= Time(2013, 5, 10, 23));
			Assert::IsTrue(Time(2013, 7, 30, 12) <= Time(2013, 7, 30, 12, 30));
			Assert::IsTrue(Time(2013, 9, 15, 20, 30) <= Time(2013, 9, 15, 20, 30, 1));
			Assert::IsTrue(Time(2013, 9, 15, 20, 30, 40) <= Time(2013, 9, 15, 20, 30, 40, 333));
			Assert::IsTrue(Time(2013, 12, 31, 23, 59, 59, 999) <= Time(2013, 12, 31, 23, 59, 59, 999));
			
			//
			// operator>=
			//
			Assert::IsFalse(Time(1999, 12, 31, 23, 59, 59, 999) >= Time(2000));
			Assert::IsFalse(Time(2013) >= Time(2013, 2));
			Assert::IsFalse(Time(2013, 3, 19) >= Time(2013, 3, 20));
			Assert::IsFalse(Time(2013, 5, 10) >= Time(2013, 5, 10, 23));
			Assert::IsFalse(Time(2013, 7, 30, 12) >= Time(2013, 7, 30, 12, 30));
			Assert::IsFalse(Time(2013, 9, 15, 20, 30) >= Time(2013, 9, 15, 20, 30, 1));
			Assert::IsFalse(Time(2013, 9, 15, 20, 30, 40) >= Time(2013, 9, 15, 20, 30, 40, 333));
			
			//
			// operator==
			//
			Assert::IsTrue(Time() == Time());
			Assert::IsTrue(Time() == Time(1601));
			Assert::IsTrue(Time() == Time(1601, 1));
			Assert::IsTrue(Time() == Time(1601, 1, 1));
			Assert::IsTrue(Time() == Time(1601, 1, 1, 0));
			Assert::IsTrue(Time() == Time(1601, 1, 1, 0, 0));
			Assert::IsTrue(Time() == Time(1601, 1, 1, 0, 0, 0));
			Assert::IsTrue(Time() == Time(1601, 1, 1, 0, 0, 0, 0));
			Assert::IsTrue(Time(1999, 12, 31, 23, 59, 59, 999) == Time(1999, 12, 31, 23, 59, 59, 999));
			
			//
			// operator!=
			//
			Assert::IsTrue(Time(2000, 12, 31, 23, 59, 59, 999) != Time(1999, 12, 31, 23, 59, 59, 999));
			Assert::IsTrue(Time(2000, 12, 31, 23, 59, 59, 999) != Time(2000, 11, 31, 23, 59, 59, 999));
			Assert::IsTrue(Time(2000, 12, 31, 23, 59, 59, 999) != Time(2000, 12, 30, 23, 59, 59, 999));
			Assert::IsTrue(Time(2000, 12, 31, 23, 59, 59, 999) != Time(2000, 12, 31, 22, 59, 59, 999));
			Assert::IsTrue(Time(2000, 12, 31, 23, 59, 59, 999) != Time(2000, 12, 31, 23, 58, 59, 999));
			Assert::IsTrue(Time(2000, 12, 31, 23, 59, 59, 999) != Time(2000, 12, 31, 23, 59, 58, 999));
			Assert::IsTrue(Time(2000, 12, 31, 23, 59, 59, 999) != Time(2000, 12, 31, 23, 59, 59, 998));
		}

		TEST_METHOD(TimeArithmetic)
		{
			auto time1 = Time(1999, 12, 31, 23, 59, 59, 999);
			auto time2 = Time(1999, 12, 30, 23, 59, 59, 999);

			Assert::IsTrue(time1.Difference(time2) == 60 * 60 * 24);
			Assert::IsTrue(time2.Difference(time1) == -60 * 60 * 24);
		}

		TEST_METHOD(TimeAndUnixTime)
		{
			auto now = std::time(nullptr);
			auto time = Time::FromUnixTime(now);
			auto unixTime = time.ToUnixTime();

			Assert::AreEqual(now, unixTime);
		}
	};
}