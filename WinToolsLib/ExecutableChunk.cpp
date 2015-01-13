#include "ExecutableChunk.h"

namespace WinToolsLib
{
	ExecutableChunk::ExecutableChunk(SizeT begin, SizeT end, PCChar name, UInt32 rva) :
		ChunkBase(begin, end),
		m_name(name),
		m_rva(rva)
	{
		m_size = (UInt32)(m_end - m_begin);
	}

	ExecutableChunk::ExecutableChunk(const ExecutableChunk& other) :
		ChunkBase(other),
		m_name(other.m_name),
		m_rva(other.m_rva),
		m_size(other.m_size)
	{
	}

	PCChar ExecutableChunk::GetName() const
	{
		return m_name;
	}

	UInt32 ExecutableChunk::GetRva() const
	{
		return m_rva;
	}

	UInt32 ExecutableChunk::GetSize() const
	{
		return m_size;
	}

	Void ExecutableChunk::SetBegin(SizeT begin)
	{
		auto offset = (UInt32)(begin - m_begin);
		m_rva += offset;
		ChunkBase::SetBegin(begin);
		m_size = (UInt32)(m_end - m_begin);
	}

	Void ExecutableChunk::SetEnd(SizeT end)
	{
		ChunkBase::SetEnd(end);
		m_size = (UInt32)(m_end - m_begin);
	}

	Bool ExecutableChunk::IsSameType(const ChunkBase& other) const
	{
		const ExecutableChunk& otherChunk = static_cast<const ExecutableChunk&>(other);
		if (strcmp(m_name, otherChunk.m_name) == 0)
		{
			return True;
		}

		return False;
	}
}