#include "ChunkCollection.h"
#include <algorithm>

namespace WinToolsLib
{
	ChunkBase::ChunkBase(SizeT begin, SizeT end) :
		m_begin(begin),
		m_end(end)
	{
	}

	ChunkBase::ChunkBase(const ChunkBase& other) :
		m_begin(other.m_begin),
		m_end(other.m_end)
	{
	}

	SizeT ChunkBase::GetBegin() const
	{
		return m_begin;
	}

	SizeT ChunkBase::GetEnd() const
	{
		return m_end;
	}

	Void ChunkBase::SetBegin(SizeT begin)
	{
		m_begin = begin;
	}
	
	Void ChunkBase::SetEnd(SizeT end)
	{
		m_end = end;
	}

	Bool ChunkBase::IsSameType(const ChunkBase& other) const
	{
		return True;
	}

	Bool ChunkBase::operator<(const ChunkBase& other) const
	{
		return (Bool)(m_begin < other.m_begin);
	}
}