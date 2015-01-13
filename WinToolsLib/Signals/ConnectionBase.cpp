#include "ConnectionBase.h"

namespace WinToolsLib { namespace Signals
{
	ConnectionBase::ConnectionBase(SlotId id) :
		m_slotId(id)
	{
	}

	ConnectionBase::~ConnectionBase()
	{
	}

	ConnectionBase0::ConnectionBase0(SlotId id) :
		ConnectionBase(id)
	{
	}

	ConnectionBase0::~ConnectionBase0()
	{
	}
} }