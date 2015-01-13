#include "stdafx.h"
#include <WinToolsLib\Os\Shell.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace WinToolsLib;
using namespace WinToolsLib::Os;

namespace WinToolsLib { namespace Os
{
	extern Bool KnownFolderRegistryIsConsistent();

} }

namespace Tests
{
	TEST_CLASS(ShellTest)
	{
	public:
		TEST_METHOD(ShellGetKnownFolder)
		{
			Assert::IsTrue(KnownFolderRegistryIsConsistent());
		}
	};
}