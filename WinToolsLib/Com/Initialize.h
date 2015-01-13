#pragma once
#include "..\Types.h"

namespace WinToolsLib
{
	namespace Com
	{
		enum class Model
		{
			SingleThreaded,
			ApartmentThreaded,
			Multithreaded
		};

		class Initialize
		{
		public:
			Initialize(Model model);
			~Initialize();

		private:
			Bool m_successful;
		};
	}
}

