#include "stdafx.h"
#include <WinToolsLib\Version.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;

namespace Tests
{
	TEST_CLASS(VersionTest)
	{
	public:
		TEST_METHOD(VersionParse)
		{
			[]()
			{
				Version version(Text(""));

				Assert::AreEqual((SizeT)0, version.GetPartsCount());
			}();

			[]()
			{
				Version version(Text("1.22.333"));

				Assert::AreEqual((SizeT)3, version.GetPartsCount());
				Assert::AreEqual((SizeT)1, version[0]);
				Assert::AreEqual((SizeT)22, version[1]);
				Assert::AreEqual((SizeT)333, version[2]);
			}();

			[]()
			{
				Version version(Text("1.020.300beta"));

				Assert::AreEqual((SizeT)3, version.GetPartsCount());
				Assert::AreEqual((SizeT)1, version[0]);
				Assert::AreEqual((SizeT)20, version[1]);
				Assert::AreEqual((SizeT)300, version[2]);
			}();

			[]()
			{
				Version version(Text("smth_1.020.0003.beta"));

				Assert::AreEqual((SizeT)4, version.GetPartsCount());
				Assert::AreEqual((SizeT)1, version[0]);
				Assert::AreEqual((SizeT)20, version[1]);
				Assert::AreEqual((SizeT)3, version[2]);
				Assert::AreEqual((SizeT)0, version[3]);
			}();
		}
	
		TEST_METHOD(VersionComparision)
		{
			//
			// operator<
			//
			Assert::IsTrue(Version(Text("0.1.2")) < Version(Text("0.2.2")));
			Assert::IsFalse(Version(Text("0.2.2")) < Version(Text("0.1.2")));
			Assert::IsFalse(Version(Text("0.1.0")) < Version(Text("0.1")));
			Assert::IsFalse(Version(Text("0.1.1")) < Version(Text("0.1")));
			Assert::IsTrue(Version(Text("0.1")) < Version(Text("0.1.2")));
			Assert::IsFalse(Version(Text("0.1")) < Version(Text("0.1.0")));
			Assert::IsFalse(Version(Text("0.1.2")) < Version(Text("0.1.2")));

			//
			// operator>
			//
			Assert::IsTrue(Version(Text("0.2.2")) > Version(Text("0.1.2")));
			Assert::IsFalse(Version(Text("0.1.2")) > Version(Text("0.2.2")));
			Assert::IsFalse(Version(Text("0.1")) > Version(Text("0.1.0")));
			Assert::IsFalse(Version(Text("0.1")) > Version(Text("0.1.1")));
			Assert::IsTrue(Version(Text("0.1.2")) > Version(Text("0.1")));
			Assert::IsFalse(Version(Text("0.1.0")) > Version(Text("0.1")));
			Assert::IsFalse(Version(Text("0.1.2")) > Version(Text("0.1.2")));
			
			//
			// operator<=
			//
			Assert::IsFalse(Version(Text("0.2.2")) <= Version(Text("0.1.2")));
			Assert::IsTrue(Version(Text("0.1.2")) <= Version(Text("0.2.2")));
			Assert::IsTrue(Version(Text("0.1")) <= Version(Text("0.1.0")));
			Assert::IsTrue(Version(Text("0.1")) <= Version(Text("0.1.1")));
			Assert::IsFalse(Version(Text("0.1.2")) <= Version(Text("0.1")));
			Assert::IsTrue(Version(Text("0.1.0")) <= Version(Text("0.1")));
			Assert::IsTrue(Version(Text("0.1.2")) <= Version(Text("0.1.2")));

			//
			// operator>=
			//
			Assert::IsFalse(Version(Text("0.1.2")) >= Version(Text("0.2.2")));
			Assert::IsTrue(Version(Text("0.2.2")) >= Version(Text("0.1.2")));
			Assert::IsTrue(Version(Text("0.1.0")) >= Version(Text("0.1")));
			Assert::IsTrue(Version(Text("0.1.1")) >= Version(Text("0.1")));
			Assert::IsFalse(Version(Text("0.1")) >= Version(Text("0.1.2")));
			Assert::IsTrue(Version(Text("0.1")) >= Version(Text("0.1.0")));
			Assert::IsTrue(Version(Text("0.1.2")) >= Version(Text("0.1.2")));

			//
			// operator==
			//
			Assert::IsFalse(Version(Text("0.1.2")) == Version(Text("0.2.2")));
			Assert::IsFalse(Version(Text("0.1.2")) == Version(Text("0.1")));
			Assert::IsFalse(Version(Text("0.1")) == Version(Text("0.1.2")));
			Assert::IsTrue(Version(Text("0.1")) == Version(Text("0.1.0")));
			Assert::IsTrue(Version(Text("0.1")) == Version(Text("0.1")));

			//
			// operator!=
			//
			Assert::IsTrue(Version(Text("0.1.2")) != Version(Text("0.2.2")));
			Assert::IsTrue(Version(Text("0.1.2")) != Version(Text("0.1")));
			Assert::IsTrue(Version(Text("0.1")) != Version(Text("0.1.2")));
			Assert::IsFalse(Version(Text("0.1")) != Version(Text("0.1.0")));
			Assert::IsFalse(Version(Text("0.1")) != Version(Text("0.1")));
		}
	
		TEST_METHOD(VersionToString)
		{
			[]()
			{
				Version version(Text(""));
				Assert::AreEqual(Text(""), version.ToString());
			}();

			[]()
			{
				Version version(Text("1.22.333"));
				Assert::AreEqual(Text("1.22.333"), version.ToString());
			}();

			[]()
			{
				Version version(Text("1.020.300beta"));
				Assert::AreEqual(Text("1.20.300"), version.ToString());
			}();

			[]()
			{
				Version version(Text("smth_1.020.0003.beta"));
				Assert::AreEqual(Text("1.20.3.0"), version.ToString());
			}();
		}

		TEST_METHOD(VersionToInt)
		{
			[]()
			{
				Version version(Text(""));
				Assert::AreEqual(0, version.ToInt());
			}();

			[]()
			{
				Version version1(Text("1.22.333"));
				Version version2(Text("1.22.334"));
				Version version3(Text("1.23.333"));
				Version version4(Text("2.22.333"));

				Assert::IsTrue(version1.ToInt() < version2.ToInt());
				Assert::IsTrue(version2.ToInt() < version3.ToInt());
				Assert::IsTrue(version3.ToInt() < version4.ToInt());
			}();
		}
	};
}