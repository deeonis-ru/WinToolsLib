#pragma once
#include "Types.h"
#include "ChunkCollection.h"

namespace WinToolsLib
{
	class ExecutableChunk : protected ChunkBase
	{
	public:
		ExecutableChunk(SizeT begin, SizeT end, PCChar name, UInt32 rva);
		ExecutableChunk(const ExecutableChunk& other);

		using ChunkBase::operator<;

		using ChunkBase::GetBegin;
		using ChunkBase::GetEnd;
		
		PCChar GetName() const;
		UInt32 GetRva() const;
		UInt32 GetSize() const;

	protected:
		Void SetBegin(SizeT begin) override;
		Void SetEnd(SizeT end) override;

		Bool IsSameType(const ChunkBase& other) const override;

	private:
		PCChar m_name;
		UInt32 m_rva;
		UInt32 m_size;

		friend class ChunkCollection<ExecutableChunk>;
	};

	typedef ChunkCollection<ExecutableChunk> ExecutableChunks;

}
