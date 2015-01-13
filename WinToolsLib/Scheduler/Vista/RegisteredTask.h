#pragma once
#include "..\..\Types.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

#include "TaskDefinition.h"

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	class RegisteredTask
	{
		RegisteredTask(const RegisteredTask& other);            // Non-copyable
		RegisteredTask& operator=(const RegisteredTask& other); // Non-copyable

	public:
		RegisteredTask(Com::Ptr<IRegisteredTask>&& task);
		RegisteredTask(RegisteredTask&& other);
		~RegisteredTask();

		RegisteredTask& operator=(RegisteredTask&& other);
		TaskDefinition GetDefinition() const;

		Void Run() const;
		Void Run(const WChar* param) const;

	protected:
		Void MoveFrom(RegisteredTask& other);

	private:
		Com::Ptr<IRegisteredTask> m_task;
	};

} } }