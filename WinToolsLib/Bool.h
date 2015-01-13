#pragma once

namespace WinToolsLib
{
	class Bool
	{
	public:
		Bool(bool value = false) : m_value(value) {}

		operator bool() const { return m_value; }

	private:
		bool m_value;
	};

#define True Bool(true)
#define False Bool(false)
}