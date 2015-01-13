#pragma once
#include "..\Types.h"
#include "..\String.h"
#include "CommandList.h"

#include <deque>

namespace WinToolsLib
{
	namespace CommandLine
	{
		class ArgumentList
		{
		public:
			ArgumentList(const TChar* commandLine = nullptr);

			Void PushBack(const TChar* argument);
			Void PushFront(const TChar* argument);
			String PopFront();

			Void Append(const CommandList& list);

			const String& operator[](SizeT index) const;
			String& operator[](SizeT index);

			SizeT GetSize() const;
			String ToCommandLine() const;
			CommandList ToCommandList() const;

		private:
			std::deque<String> m_arguments;
		};

		inline Void ArgumentList::PushBack(const TChar* argument)
		{
			m_arguments.push_back(argument);
		}

		inline Void ArgumentList::PushFront(const TChar* argument)
		{
			m_arguments.push_front(argument);
		}
		
		inline String ArgumentList::PopFront()
		{
			String front = std::move(m_arguments.front());
			m_arguments.pop_front();
			return front;
		}

		inline const String& ArgumentList::operator[](SizeT index) const
		{
			return m_arguments[index];
		}

		inline String& ArgumentList::operator[](SizeT index)
		{
			return m_arguments[index];
		}

		inline SizeT ArgumentList::GetSize() const
		{
			return m_arguments.size();
		}
	}
}
