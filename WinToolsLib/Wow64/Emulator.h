#pragma once
#include "..\Types.h"
#include "..\String.h"

namespace WinToolsLib
{
	namespace Wow64
	{
		class Emulator
		{
			Emulator() {} // Singleton, use GetInstance()

			Emulator(const Emulator& other);            // Non-copyable
			Emulator& operator=(const Emulator& other); // Non-copyable

			Emulator(Emulator&& other);                 // Non-movable
			Emulator& operator=(Emulator&& other);      // Non-movable

		public:
			static Emulator& GetInstance();

			String EmulateFsRedirection(const TChar* path);
		};
	}
}
