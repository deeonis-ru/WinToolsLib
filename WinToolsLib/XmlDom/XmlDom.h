//
// Based on SlimXml sources provided by New BSD License
//

#pragma once
#include "..\Types.h"
#include "..\String.h"
#include "..\Exception.h"
#include "..\Serialization\ISerializer.h"

#include <list>
#include <istream>
#include <tchar.h>

namespace WinToolsLib
{
	namespace XmlDom
	{
		#undef SLIM_USE_WCHAR
		#if defined (UNICODE)
			#define SLIM_USE_WCHAR
		#endif

		enum Encode
		{
			ANSI = 0,
			UTF_8,
			UTF_8_NO_MARK,
			UTF_16,
			UTF_16_BIG_ENDIAN,

		#if defined (SLIM_USE_WCHAR)
			DefaultEncode = UTF_8
		#else
			DefaultEncode = ANSI
		#endif
		};

		#ifndef _tmemchr
			#ifdef _UNICODE
				#define _tmemchr wmemchr
			#else
				#define _tmemchr memchr
			#endif
		#endif

		class Attribute;
		class Node;

		typedef std::list<Attribute*> AttributeList;
		typedef std::list<Node*> NodeList;

		typedef AttributeList::const_iterator AttributeIterator;
		typedef NodeList::const_iterator NodeIterator;

		enum NodeType
		{
			DOCUMENT = 0,
			ELEMENT,
			COMMENT,
			DECLARATION
		};

		class BadAttributeValueException : public Exception
		{
		public:
			BadAttributeValueException(PCChar function, UInt32 line, PCChar file) :
				Exception(function, line, file, Text("Bad attribute value was found"))
			{
			}
		};

		#define THROW_BAD_ATTRIBUTE_VALUE_EXCEPTION() \
			throw BadAttributeValueException(__FUNCTION__, __LINE__, __FILE__)

		using namespace Serialization;

		class Base
		{
			friend class Document;

		public:
			Base();
			~Base();

			const TChar* GetName() const;
			Void SetName(const TChar* name);

			const TChar* GetString() const;
			Bool GetBool() const;
			Int32 GetInt32() const;
			Int64 GetInt64() const;
			UInt32 GetUInt32() const;
			UInt64 GetUInt64() const;
			Float GetFloat() const;
			Double GetDouble() const;

			Void SetString(const TChar* value);
			Void SetString(const String& value);
			Void SetBool(Bool value, BoolFormat format = BoolFormat::TrueFalse);
			Void SetInt32(Int32 value);
			Void SetInt64(Int64 value);
			Void SetUInt32(UInt32 value, IntFormat format = IntFormat::Dec);
			Void SetUInt64(UInt64 value, IntFormat format = IntFormat::Dec);
			Void SetFloat(Float value, FloatFormat format = FloatFormat::Default);
			Void SetDouble(Double value, FloatFormat format = FloatFormat::Default);

		protected:
			Void AssignName(TChar* value, SizeT length, Bool transferCharacter);
			Void AssignValue(TChar* value, SizeT length, Bool transferCharacter);
			
			static Void AssignString(TChar* &str, Bool allocated, TChar* value, SizeT length, Bool transferCharacter);

		protected:
			TChar* m_name;
			TChar* m_value;
			Bool m_nameAllocated;
			Bool m_valueAllocated;
		};

		class Attribute : public Base
		{
		};

		class Node : public Base
		{
		public:
			Node(NodeType type, Node* parent);
			~Node();

		public:
			NodeType GetType() const;
			Node* GetParent() const;
			
			Bool IsEmpty() const;
			Bool HasChild() const;

			Node* GetFirstChild(NodeIterator& iter) const;
			Node* GetNextChild(NodeIterator& iter) const;
			SizeT GetChildCount() const;

			Node* FindChild(const TChar* name) const;
			Node* FindFirstChild(const TChar* name, NodeIterator& iter) const;
			Node* FindNextChild(const TChar* name, NodeIterator& iter) const;
			SizeT GetChildCount(const TChar* name) const;

			Void RemoveChild(Node* node);
			Void ClearChild();

			Node* AddChild(const TChar* name = NULL, NodeType = ELEMENT);

			Bool HasAttribute() const;

			Attribute* FindAttribute(const TChar* name) const;

			const TChar* ReadAttributeAsString(const TChar* name, const TChar* defaultValue = Text("")) const;
			Bool ReadAttributeAsBool(const TChar* name, Bool defaultValue = False) const;
			Int32 ReadAttributeAsInt32(const TChar* name, Int32 defaultValue = 0) const;
			Int64 ReadAttributeAsInt64(const TChar* name, Int64 defaultValue = 0) const;
			UInt32 ReadAttributeAsUInt32(const TChar* name, UInt32 defaultValue = 0) const;
			UInt64 ReadAttributeAsUInt64(const TChar* name, UInt64 defaultValue = 0) const;
			Float ReadAttributeAsFloat(const TChar* name, Float defaultValue = 0.0f) const;
			Double ReadAttributeAsDouble(const TChar* name, Double defaultValue = 0.0) const;
			Void ReadAttributeAsIntArray(const TChar* name, Int32* out, UInt32 length, Int32 defaultValue = 0) const;
			UInt32 ReadAttributeAsEnum(const TChar* name, const TChar* const* enumNames, UInt32 enumCount, UInt32 defaultValue = 0) const;

			Attribute* GetFirstAttribute(AttributeIterator& iter) const;
			Attribute* GetNextAttribute(AttributeIterator& iter) const;

			Void RemoveAttribute(Attribute* attribute);
			Void ClearAttribute();

			Attribute* AddAttribute(const TChar* name = NULL, const TChar* value = NULL);
			Attribute* AddAttribute(const TChar* name, Bool value, BoolFormat format = BoolFormat::TrueFalse);
			Attribute* AddAttribute(const TChar* name, Int32 value);
			Attribute* AddAttribute(const TChar* name, Int64 value);
			Attribute* AddAttribute(const TChar* name, UInt32 value, IntFormat format = IntFormat::Dec);
			Attribute* AddAttribute(const TChar* name, UInt64 value, IntFormat format = IntFormat::Dec);
			Attribute* AddAttribute(const TChar* name, Float value, FloatFormat format = FloatFormat::Default);
			Attribute* AddAttribute(const TChar* name, Double value, FloatFormat format = FloatFormat::Default);

		protected:
			Void WriteNode(String& output, int depth) const;
			Void WriteChildNodes(String& output, int depth) const;
			Void WriteTransferredString(String& output, const TChar* input) const;

		private:
			NodeType      m_type;
			AttributeList m_attributes;
			Node*         m_parent;
			NodeList      m_children;
		};

		class Document : public Node
		{
		public:
			Document();
			~Document();

			Bool LoadFromFile(const TChar* filename);
			Bool LoadFromStream(std::istream& input);
			Bool LoadFromMemory(const char* buffer, SizeT size);

			Bool Save(String& output, Encode encode = DefaultEncode) const;
			Bool Save(const TChar* filename, Encode encode = DefaultEncode) const;

		protected:
			Bool ReallyLoadFromMemory(char* buffer, SizeT size, Bool copiedMemory);
			Bool Parse(TChar* input, SizeT size);

			Bool FindLabel(TChar* &begin, SizeT size, TChar* &label, SizeT &labelSize);
			Bool ParseLabel(Node* node, TChar* label, SizeT labelSize);

		private:
			char* m_buffer;
		};

		inline Base::Base() :
		m_name(const_cast<TChar*>(Text(""))),
			m_value(const_cast<TChar*>(Text(""))),
			m_nameAllocated(False),
			m_valueAllocated(False)
		{
		}

		inline Base::~Base()
		{
			if (m_nameAllocated && m_name != NULL)
			{
				delete m_name;
			}
			if (m_valueAllocated && m_value != NULL)
			{
				delete m_value;
			}
		}

		inline const TChar* Base::GetName() const
		{
			return m_name;
		}

		inline Void Base::SetName(const TChar* name)
		{
			if (m_nameAllocated && m_name != NULL)
			{
				delete[] m_name;
			}
			auto size = _tcsclen(name) + 1;
			m_name = new TChar[size];
			_tcscpy_s(m_name, size, name);
			m_nameAllocated = True;
		}

		inline const TChar* Base::GetString() const
		{
			return m_value;
		}

		inline Bool Base::GetBool() const
		{
			if (_tcsicmp(m_value, Text("True")) == 0 ||
				_tcsicmp(m_value, Text("Yes")) == 0 ||
				_tcscmp(m_value, Text("1")) == 0)
			{
				return True;
			}

			if (_tcsicmp(m_value, Text("False")) == 0 ||
				_tcsicmp(m_value, Text("No")) == 0 ||
				_tcscmp(m_value, Text("0")) == 0)
			{
				return False;
			}

			THROW_BAD_ATTRIBUTE_VALUE_EXCEPTION();
		}

		inline Int32 Base::GetInt32() const
		{
			return _tcstol(m_value, NULL, 0);
		}

		inline Int64 Base::GetInt64() const
		{
			return _tcstoi64(m_value, NULL, 0);
		}

		inline UInt32 Base::GetUInt32() const
		{
			return _tcstoul(m_value, NULL, 0);
		}

		inline UInt64 Base::GetUInt64() const
		{
			return _tcstoui64(m_value, NULL, 0);
		}

		inline Float Base::GetFloat() const
		{
			return static_cast<Float>(_tcstod(m_value, NULL));
		}

		inline Double Base::GetDouble() const
		{
			return _tcstod(m_value, NULL);
		}

		inline Void Base::SetString(const TChar* value)
		{
			if (m_valueAllocated && m_value != NULL)
			{
				delete[] m_value;
			}
			auto size = _tcsclen(value) + 1;
			m_value = new TChar[size];
			_tcscpy_s(m_value, size, value);
			m_valueAllocated = True;
		}

		inline Void Base::SetString(const String& value)
		{
			SetString(value.GetBuffer());
		}

		inline Void Base::SetBool(Bool value, BoolFormat format)
		{
			switch (format)
			{
			case BoolFormat::TrueFalse:
				SetString(value ? Text("True") : Text("False"));
				break;

			case BoolFormat::YesNo:
				SetString(value ? Text("Yes") : Text("No"));
				break;

			default:
				SetString(value ? Text("1") : Text("0"));
				break;
			}
		}

		inline Void Base::SetInt32(Int32 value)
		{
			TChar sz[128];
			_stprintf_s(sz, Text("%I32d"), value);
			SetString(sz);
		}

		inline Void Base::SetInt64(Int64 value)
		{
			TChar sz[128];
			_stprintf_s(sz, Text("%I64d"), value);
			SetString(sz);
		}

		inline Void Base::SetUInt32(UInt32 value, IntFormat format)
		{
			TChar sz[128];
			if (IntFormat::Hex == format)
				_stprintf_s(sz, Text("0x%I32X"), value);
			else
				_stprintf_s(sz, Text("%I32u"), value);
			SetString(sz);
		}

		inline Void Base::SetUInt64(UInt64 value, IntFormat format)
		{
			TChar sz[128];
			if (IntFormat::Hex == format)
				_stprintf_s(sz, Text("0x%I64X"), value);
			else
				_stprintf_s(sz, Text("%I64u"), value);
			SetString(sz);
		}

		inline Void Base::SetFloat(Float value, FloatFormat format)
		{
			TChar sz[128];
			_stprintf_s(sz, Text("%.10g"), value);
			SetString(sz);
		}

		inline Void Base::SetDouble(Double value, FloatFormat format)
		{
			TChar sz[128];
			_stprintf_s(sz, Text("%.17g"), value);
			SetString(sz);
		}

		inline NodeType Node::GetType() const
		{
			return m_type;
		}

		inline Node* Node::GetParent() const
		{
			return m_parent;
		}

		inline Bool Node::IsEmpty() const
		{
			return (!HasChild() && (m_value == NULL || m_value[0] == 0)) ? True : False;
		}

		inline Bool Node::HasChild() const
		{
			return (!m_children.empty()) ? True : False;
		}

		inline Node* Node::GetFirstChild(NodeIterator& iter) const
		{
			iter = m_children.begin();
			if (iter != m_children.end())
			{
				return *iter;
			}
			return NULL;
		}

		inline Node* Node::GetNextChild(NodeIterator& iter) const
		{
			if (iter != m_children.end())
			{
				++iter;
				if (iter != m_children.end())
				{
					return *iter;
				}
			}
			return NULL;
		}

		inline SizeT Node::GetChildCount() const
		{
			return m_children.size();
		}

		inline Bool Node::HasAttribute() const
		{
			return (!m_attributes.empty()) ? True : False;
		}

		inline Attribute* Node::GetFirstAttribute(AttributeIterator& iter) const
		{
			iter = m_attributes.begin();
			if (iter != m_attributes.end())
			{
				return *iter;
			}
			return NULL;
		}

		inline Attribute* Node::GetNextAttribute(AttributeIterator& iter) const
		{
			if (iter != m_attributes.end())
			{
				++iter;
				if (iter != m_attributes.end())
				{
					return *iter;
				}
			}
			return NULL;
		}

		SizeT Utf8ToUtf16(const char* u8, SizeT size, wchar_t* u16, SizeT outBufferSize);
		SizeT Utf16ToUtf8(const wchar_t* u16, SizeT size, char* u8, SizeT outBufferSize);
		Encode DetectEncode(const char* str, SizeT size, Bool& multiBytes);
	}
}

