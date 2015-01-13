#include "AnonymousConnection.h"

namespace WinToolsLib { namespace Signals
{
	AnonymousConnection0::AnonymousConnection0(SlotId id, AnonymousFunction& function) :
		m_function(function),
		ConnectionBase0(id)
	{
	}
} }