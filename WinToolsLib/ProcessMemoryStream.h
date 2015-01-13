#pragma once
#include "Types.h"
#include "IStream.h"
#include "Handles\Handle.h"
#include "CommonMemoryStream.h"

namespace WinToolsLib
{
	using namespace Handles;

	class ProcessMemoryStream : public CommonMemoryStream
	{
	public:
		explicit ProcessMemoryStream(UInt32 processId);

		Void Read(PByte buffer, SizeT bytes) override;
		Void Write(PByte buffer, SizeT bytes) override;

	protected:
		Handle m_hProcess;
	};
}
