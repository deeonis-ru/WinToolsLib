#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Com\Ptr.h"

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

namespace WinToolsLib { namespace Scheduler { namespace Vista
{
	class ExecAction
	{
		ExecAction(const ExecAction& other);            // Non-copyable
		ExecAction& operator=(const ExecAction& other); // Non-copyable

	public:
		ExecAction(Com::Ptr<IExecAction>&& action);
		ExecAction(ExecAction&& other);
		~ExecAction();

		ExecAction& operator=(ExecAction&& other);

		StringW GetPath() const;
		Void SetPath(const WChar* path);

		StringW GetArguments() const;
		Void SetArguments(const WChar* arguments);

	protected:
		Void MoveFrom(ExecAction& other);

	private:
		Com::Ptr<IExecAction> m_action;
	};

} } }