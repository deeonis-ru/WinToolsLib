#pragma once
#include "..\Types.h"
#include "..\String.h"

#include <list>
#include <vector>

namespace WinToolsLib
{
	namespace CommandLine
	{
		class Command;

		typedef std::list<Command> CommandList;
		typedef std::vector<String> ParameterList;

		class Command
		{
		public:
			Command(const TChar* name);
			Command(Command&& other);
			Command(const Command& other);
			
			Command& operator=(Command&& other);
			Command& operator=(const Command& other);
			Bool operator==(const Command& other) const;

			const String& GetName() const;

			Void AddParameter(const TChar* parameter);
			const ParameterList& GetParameters() const;

		protected:
			Void MoveFrom(Command& other);
			Void CopyFrom(const Command& other);

		private:
			String m_name;
			std::vector<String> m_parameters;
		};

		inline Command::Command(const TChar* name) :
			m_name(name)
		{
		}

		inline Command::Command(Command&& other)
		{
			MoveFrom(other);
		}

		inline Command::Command(const Command& other)
		{
			CopyFrom(other);
		}

		inline Command& Command::operator=(Command&& other)
		{
			MoveFrom(other);
			return *this;
		}

		inline Command& Command::operator=(const Command& other)
		{
			CopyFrom(other);
			return *this;
		}

		inline Bool Command::operator==(const Command& other) const
		{
			return m_name == other.m_name;
		}

		inline const String& Command::GetName() const
		{
			return m_name;
		}

		inline Void Command::AddParameter(const TChar* parameter)
		{
			m_parameters.push_back(parameter);
		}

		inline const ParameterList& Command::GetParameters() const
		{
			return m_parameters;
		}
	}
}
