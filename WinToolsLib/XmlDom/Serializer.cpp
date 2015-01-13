#include "Serializer.h"

namespace WinToolsLib
{
	namespace XmlDom
	{
		Serializer::Serializer(Node* node) :
			m_node(node)
		{
		}

		Serializer::~Serializer()
		{
		}

#pragma region IStoreProperty

		Void Serializer::StoreProperty(const TChar* name, Bool value, BoolFormat format)
		{
			m_node->AddAttribute(name, value, format);
		}

		Void Serializer::StoreProperty(const TChar* name, Int8 value, IntFormat format)
		{
			if (IntFormat::Dec != format)
			{
				THROW_UNSUPPORTED_FORMAT_EXCEPTION();
			}
			m_node->AddAttribute(name, static_cast<Int32>(value));
		}

		Void Serializer::StoreProperty(const TChar* name, UInt8 value, IntFormat format)
		{
			m_node->AddAttribute(name, static_cast<UInt32>(value), format);
		}

		Void Serializer::StoreProperty(const TChar* name, Int16 value, IntFormat format)
		{
			if (IntFormat::Dec != format)
			{
				THROW_UNSUPPORTED_FORMAT_EXCEPTION();
			}
			m_node->AddAttribute(name, static_cast<Int32>(value));
		}

		Void Serializer::StoreProperty(const TChar* name, UInt16 value, IntFormat format)
		{
			m_node->AddAttribute(name, static_cast<UInt32>(value), format);
		}

		Void Serializer::StoreProperty(const TChar* name, Int32 value, IntFormat format)
		{
			if (IntFormat::Dec != format)
			{
				THROW_UNSUPPORTED_FORMAT_EXCEPTION();
			}
			m_node->AddAttribute(name, value);
		}

		Void Serializer::StoreProperty(const TChar* name, UInt32 value, IntFormat format)
		{
			m_node->AddAttribute(name, value, format);
		}

		Void Serializer::StoreProperty(const TChar* name, Int64 value, IntFormat format)
		{
			if (IntFormat::Dec != format)
			{
				THROW_UNSUPPORTED_FORMAT_EXCEPTION();
			}
			m_node->AddAttribute(name, value);
		}

		Void Serializer::StoreProperty(const TChar* name, UInt64 value, IntFormat format)
		{
			m_node->AddAttribute(name, value, format);
		}

		Void Serializer::StoreProperty(const TChar* name, Float value, FloatFormat format)
		{
			if (FloatFormat::Default != format)
			{
				THROW_UNSUPPORTED_FORMAT_EXCEPTION();
			}
			m_node->AddAttribute(name, value, format);
		}

		Void Serializer::StoreProperty(const TChar* name, Double value, FloatFormat format)
		{
			if (FloatFormat::Default != format)
			{
				THROW_UNSUPPORTED_FORMAT_EXCEPTION();
			}
			m_node->AddAttribute(name, value, format);
		}

		Void Serializer::StoreProperty(const TChar* name, const String& value)
		{
			m_node->AddAttribute(name, value);
		}

		Void Serializer::StoreProperty(const TChar* name, const Buffer& value)
		{
			m_node->AddAttribute(name, Buffer::ToString(value));
		}

		IStorePropertyPtr Serializer::StoreChild(const TChar* name)
		{
			auto child = m_node->AddChild(name);
			return IStorePropertyPtr(new Serializer(child));
		}

#pragma endregion

#pragma region ILoadProperty

		Bool Serializer::LoadProperty(const TChar* name, Bool defaultValue) const
		{
			return m_node->ReadAttributeAsBool(name, defaultValue);
		}

		Int8 Serializer::LoadProperty(const TChar* name, Int8 defaultValue) const
		{
			return static_cast<Int8>(m_node->ReadAttributeAsInt32(name, defaultValue));
		}

		UInt8 Serializer::LoadProperty(const TChar* name, UInt8 defaultValue) const
		{
			return static_cast<UInt8>(m_node->ReadAttributeAsUInt32(name, defaultValue));
		}

		Int16 Serializer::LoadProperty(const TChar* name, Int16 defaultValue) const
		{
			return static_cast<Int16>(m_node->ReadAttributeAsInt32(name, defaultValue));
		}

		UInt16 Serializer::LoadProperty(const TChar* name, UInt16 defaultValue) const
		{
			return static_cast<UInt16>(m_node->ReadAttributeAsUInt32(name, defaultValue));
		}

		Int32 Serializer::LoadProperty(const TChar* name, Int32 defaultValue) const
		{
			return m_node->ReadAttributeAsInt32(name, defaultValue);
		}

		UInt32 Serializer::LoadProperty(const TChar* name, UInt32 defaultValue) const
		{
			return m_node->ReadAttributeAsUInt32(name, defaultValue);
		}

		Int64 Serializer::LoadProperty(const TChar* name, Int64 defaultValue) const
		{
			return m_node->ReadAttributeAsInt64(name, defaultValue);
		}

		UInt64 Serializer::LoadProperty(const TChar* name, UInt64 defaultValue) const
		{
			return m_node->ReadAttributeAsUInt64(name, defaultValue);
		}

		Float Serializer::LoadProperty(const TChar* name, Float defaultValue) const
		{
			return m_node->ReadAttributeAsFloat(name, defaultValue);
		}

		Double Serializer::LoadProperty(const TChar* name, Double defaultValue) const
		{
			return m_node->ReadAttributeAsDouble(name, defaultValue);
		}

		String Serializer::LoadProperty(const TChar* name, const TChar* defaultValue) const
		{
			return m_node->ReadAttributeAsString(name, defaultValue);
		}

		Buffer Serializer::LoadProperty(const TChar* name, const Buffer& defaultValue) const
		{
			auto str = m_node->ReadAttributeAsString(name, Buffer::ToString(defaultValue));
			return Buffer::FromString(str);
		}

		ILoadPropertyPtr Serializer::LoadChild(const TChar* name) const
		{
			auto child = m_node->FindChild(name);
			if (!child)
			{
				return ILoadPropertyPtr();
			}
			return ILoadPropertyPtr(new Serializer(child));
		}

		Void Serializer::LoadChildren(const TChar* name, LoadChildrenCallback callback) const
		{
			XmlDom::NodeIterator iter;

			auto child = m_node->FindFirstChild(name, iter);
			while (child)
			{
				auto load = ILoadPropertyPtr(new Serializer(child));
				if (!callback(load.get()))
				{
					break;
				}
				child = m_node->FindNextChild(name, iter);
			}
		}

#pragma endregion
	}
}