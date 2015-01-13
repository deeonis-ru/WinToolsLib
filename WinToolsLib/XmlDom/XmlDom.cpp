#include "XmlDom.h"
#include <cassert>
#include <fstream>

namespace WinToolsLib
{
	namespace XmlDom
	{
		Void Base::AssignName(TChar* value, SizeT length, Bool transferCharacter)
		{
			Base::AssignString(m_name, m_nameAllocated, value, length, transferCharacter);
		}

		Void Base::AssignValue(TChar* value, SizeT length, Bool transferCharacter)
		{
			Base::AssignString(m_value, m_valueAllocated, value, length, transferCharacter);
		}

		Void Base::AssignString(TChar* &str, Bool allocated, TChar* buffer, SizeT length, Bool transferCharacter)
		{
			TChar* found = NULL;
			if (!transferCharacter || (found = _tmemchr(buffer, Text('&'), length)) == NULL)
			{
				if (allocated && str != NULL)
				{
					delete[] str;
				}
				allocated = False;
				str = buffer;
				str[length] = 0;
				return;
			}
			String temp;
			for (; found != NULL; found = _tmemchr(buffer, Text('&'), length))
			{
				temp.Append(buffer, found - buffer);
				length -= (found - buffer + 1);
				buffer = found + 1;
				if (length >= 5)
				{
					if (_tcsncmp(buffer, Text("quot;"), 5) == 0)
					{
						buffer += 5;
						length -= 5;
						temp.Append(1, Text('\"'));
						continue;
					}
					else if (_tcsncmp(buffer, Text("apos;"), 5) == 0)
					{
						buffer += 5;
						length -= 5;
						temp.Append(1, Text('\''));
						continue;
					}
				}
				if (length >= 4)
				{
					if (_tcsncmp(buffer, Text("amp;"), 4) == 0)
					{
						buffer += 4;
						length -= 4;
						temp.Append(1, Text('&'));
						continue;
					}
				}
				if (length >= 3)
				{
					if (_tcsncmp(buffer, Text("lt;"), 3) == 0)
					{
						buffer += 3;
						length -= 3;
						temp.Append(1, Text('<'));
						continue;
					}
					else if (_tcsncmp(buffer, Text("gt;"), 3) == 0)
					{
						buffer += 3;
						length -= 3;
						temp.Append(1, Text('>'));
						continue;
					}
				}
				temp.Append(1, Text('&'));
			}
			temp.Append(buffer, length);
			SizeT actualLength = temp.GetLength();

			if (allocated && str != NULL)
			{
				delete[] str;
			}
			str = new TChar[actualLength + 1];
			allocated = True;

			memcpy(str, temp.GetBuffer(), sizeof(TChar) * actualLength);
			str[actualLength] = 0;
		}

		Node::Node(NodeType type, Node* parent) :
			m_type(type),
			m_parent(parent)
		{
		}

		Node::~Node()
		{
			ClearAttribute();
			ClearChild();
		}

		Node* Node::FindChild(const TChar* name) const
		{
			assert(name != NULL);
			for (NodeList::const_iterator iter = m_children.begin();
				iter != m_children.end();
				++iter)
			{
				Node* child = *iter;
				assert(child != NULL);
				if (_tcscmp(child->m_name, name) == 0)
				{
					return child;
				}
			}
			return NULL;
		}

		Node* Node::FindFirstChild(const TChar* name, NodeIterator& iter) const
		{
			assert(name != NULL);
			iter = m_children.begin();
			while (iter != m_children.end())
			{
				Node* child = *iter;
				assert(child != NULL);
				if (_tcscmp(child->m_name, name) == 0)
				{
					return child;
				}
				++iter;
			}
			return NULL;
		}

		Node* Node::FindNextChild(const TChar* name, NodeIterator& iter) const
		{
			assert(name != NULL);
			if (iter != m_children.end())
			{
				while (++iter != m_children.end())
				{
					Node* child = *iter;
					assert(child != NULL);
					if (_tcscmp(child->m_name, name) == 0)
					{
						return child;
					}
				}
			}
			return NULL;
		}

		SizeT Node::GetChildCount(const TChar* name) const
		{
			assert(name != NULL);

			SizeT count = 0;
			for (NodeIterator iter = m_children.begin();
				iter != m_children.end();
				++iter)
			{
				Node* child = *iter;
				assert(child != NULL);
				if (_tcscmp(child->m_name, name) == 0)
				{
					++count;
				}
			}
			return count;
		}

		Void Node::RemoveChild(Node* node)
		{
			assert(node != NULL);
			for (NodeList::iterator iter = m_children.begin();
				iter != m_children.end();
				++iter)
			{
				if (*iter == node)
				{
					delete node;
					m_children.erase(iter);
					return;
				}
			}
		}

		Void Node::ClearChild()
		{
			for (NodeList::iterator iter = m_children.begin();
				iter != m_children.end();
				++iter)
			{
				Node* child = *iter;
				assert(child != NULL);
				delete child;
			}
			m_children.clear();
		}

		Node* Node::AddChild(const TChar* name, NodeType type)
		{
			if (type != COMMENT && type != ELEMENT)
			{
				return NULL;
			}
			Node* child = new Node(type, this);
			if (name != NULL)
			{
				child->SetName(name);
			}
			m_children.push_back(child);
			return child;
		}

		Attribute* Node::AddAttribute(const TChar* name, const TChar* value)
		{
			Attribute* attribute = new Attribute;
			if (name != NULL)
			{
				attribute->SetName(name);
			}
			if (value != NULL)
			{
				attribute->SetString(value);
			}
			m_attributes.push_back(attribute);
			return attribute;
		}

		Attribute* Node::AddAttribute(const TChar* name, Bool value, BoolFormat format)
		{
			Attribute* attribute = AddAttribute(name);
			attribute->SetBool(value, format);
			return attribute;
		}

		Attribute* Node::AddAttribute(const TChar* name, Int32 value)
		{
			Attribute* attribute = AddAttribute(name);
			attribute->SetInt32(value);
			return attribute;
		}

		Attribute* Node::AddAttribute(const TChar* name, Int64 value)
		{
			Attribute* attribute = AddAttribute(name);
			attribute->SetInt64(value);
			return attribute;
		}

		Attribute* Node::AddAttribute(const TChar* name, UInt32 value, IntFormat format)
		{
			Attribute* attribute = AddAttribute(name);
			attribute->SetUInt32(value, format);
			return attribute;
		}

		Attribute* Node::AddAttribute(const TChar* name, UInt64 value, IntFormat format)
		{
			Attribute* attribute = AddAttribute(name);
			attribute->SetUInt64(value, format);
			return attribute;
		}

		Attribute* Node::AddAttribute(const TChar* name, Float value, FloatFormat format)
		{
			Attribute* attribute = AddAttribute(name);
			attribute->SetFloat(value, format);
			return attribute;
		}

		Attribute* Node::AddAttribute(const TChar* name, Double value, FloatFormat format)
		{
			Attribute* attribute = AddAttribute(name);
			attribute->SetDouble(value, format);
			return attribute;
		}

		Attribute* Node::FindAttribute(const TChar* name) const
		{
			for (AttributeList::const_iterator iter = m_attributes.begin();
				iter != m_attributes.end();
				++iter)
			{
				Attribute* attribute = *iter;
				assert(attribute != NULL);
				if (_tcscmp(attribute->GetName(), name) == 0)
				{
					return attribute;
				}
			}
			return NULL;
		}

		const TChar* Node::ReadAttributeAsString(const TChar* name, const TChar* defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			return attribute->GetString();
		}

		Bool Node::ReadAttributeAsBool(const TChar* name, Bool defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			return attribute->GetBool();
		}

		Int32 Node::ReadAttributeAsInt32(const TChar* name, Int32 defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			return attribute->GetInt32();
		}

		Int64 Node::ReadAttributeAsInt64(const TChar* name, Int64 defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			return attribute->GetInt64();
		}

		UInt32 Node::ReadAttributeAsUInt32(const TChar* name, UInt32 defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			return attribute->GetUInt32();
		}

		UInt64 Node::ReadAttributeAsUInt64(const TChar* name, UInt64 defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			return attribute->GetUInt64();
		}

		Float Node::ReadAttributeAsFloat(const TChar* name, Float defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			return attribute->GetFloat();
		}

		Double Node::ReadAttributeAsDouble(const TChar* name, Double defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			return attribute->GetDouble();
		}

		Void Node::ReadAttributeAsIntArray(const TChar* name, Int32* out, UInt32 length, Int32 defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				for (UInt32 i = 0; i < length; ++i)
				{
					out[i] = defaultValue;
				}
				return;
			}
			auto stringLen = _tcsclen(attribute->GetString());
			TChar* tempBuffer = new TChar[stringLen+1];
			_tcscpy_s(tempBuffer, stringLen, attribute->GetString());
			TChar* current = tempBuffer;

			UInt32 index = 0;
			while (index < length)
			{
				TChar* found = (TChar*)_tmemchr(current, Text(','), stringLen);
				if (found != NULL)
				{
					*found = 0;
				}
				out[index] = _ttoi(current);
				stringLen -= (found - current + 1);
				current = found + 1;
				++index;
				if (found == NULL)
				{
					break;
				}
			}
			for (; index < length; ++index)
			{
				out[index] = defaultValue;
			}
			delete[] tempBuffer;
		}

		UInt32 Node::ReadAttributeAsEnum(const TChar* name, const TChar* const* enumNames, UInt32 enumCount, UInt32 defaultValue) const
		{
			Attribute* attribute = FindAttribute(name);
			if (attribute == NULL)
			{
				return defaultValue;
			}
			for (UInt32 i = 0; i < enumCount; ++i)
			{
				if (_tcscmp(enumNames[i], attribute->GetString()) == 0)
				{
					return i;
				}
			}
			return defaultValue;
		}

		Void Node::RemoveAttribute(Attribute* attribute)
		{
			assert(attribute != NULL);
			for (AttributeList::iterator iter = m_attributes.begin();
				iter != m_attributes.end();
				++iter)
			{
				if (*iter == attribute)
				{
					delete attribute;
					m_attributes.erase(iter);
					return;
				}
			}
		}

		Void Node::ClearAttribute()
		{
			for (AttributeList::iterator iter = m_attributes.begin();
				iter != m_attributes.end();
				++iter)
			{
				delete *iter;
			}
			m_attributes.clear();
		}

		Void Node::WriteNode(String& output, int depth) const
		{
			if (depth < 0)
			{
				//root node is not a real node, it has nothing to write
				return WriteChildNodes(output, depth);
			}
			//add tabs
			int iTabCount = depth;
			while (iTabCount-- > 0)
			{
				output += Text('	');
			}
			//comment node is special. it has no children, and has no attributes either
			if (m_type == COMMENT)
			{
				output += Text("<!--");
				output += m_name;
				output += Text("-->\r\n");
				return;
			}
			//start label
			output += Text('<');
			WriteTransferredString(output, m_name);
			//write attributes
			for (AttributeList::const_iterator iter = m_attributes.begin();
				iter != m_attributes.end();
				++iter)
			{
				Attribute* attribute = *iter;
				assert(attribute != NULL);
				output += Text(' ');
				WriteTransferredString(output, attribute->GetName());
				output += Text("=\"");
				WriteTransferredString(output, attribute->GetString());
				output += Text('"');
			}

			if (IsEmpty())
			{
				output += Text("/>\r\n");
				return;
			}
			output += Text(">");

			if (HasChild())
			{
				output += Text("\r\n");
				WriteChildNodes(output, depth);
				iTabCount = depth;
				while (iTabCount-- > 0)
				{
					output += Text('	');
				}
			}
			else
			{
				//leaf node
				WriteTransferredString(output, m_value);
			}
			//end label
			output += Text("</");
			WriteTransferredString(output, m_name);
			output += Text(">\r\n");
		}

		Void Node::WriteChildNodes(String& output, int depth) const
		{
			for (NodeList::const_iterator iter = m_children.begin();
				iter != m_children.end();
				++iter)
			{
				Node* child = *iter;
				assert(child != NULL);
				child->WriteNode(output, depth + 1);
			}
		}

		Void Node::WriteTransferredString(String& output, const TChar* input) const
		{
			if (input == NULL)
			{
				return;
			}

			for (; *input != 0; ++input)
			{
				if (*input == Text('<'))
				{
					output += Text("&lt;");
				}
				else if (*input == Text('>'))
				{
					output += Text("&gt;");
				}
				else if (*input == Text('&'))
				{
					output += Text("&amp;");
				}
				else if (*input == Text('\"'))
				{
					output += Text("&quot;");
				}
				else if (*input == Text('\''))
				{
					output += Text("&apos;");
				}
				else
				{
					output += *input;
				}
			}
		}

		//class CXmlDocument

		Document::Document()
			: Node(DOCUMENT, NULL)
			, m_buffer(NULL)
		{
		}

		Document::~Document()
		{
			if (m_buffer != NULL)
			{
				delete[] m_buffer;
			}
		}

		Bool Document::LoadFromFile(const TChar* filename)
		{
			assert(filename != NULL);
			std::fstream file;
			file.open(filename, std::ios_base::in | std::ios_base::binary);
			if (!file.is_open())
			{
				return False;
			}
			Bool succeeded = LoadFromStream(file);
			file.close();
			return succeeded;
		}

		Bool Document::LoadFromStream(std::istream& input)
		{
			input.seekg(0, std::ios::end);
			SizeT size = static_cast<SizeT>(input.tellg());
			input.seekg(0, std::ios::beg);

			char* buffer = new char[size];
			input.read(buffer, size);

			Bool succeeded = ReallyLoadFromMemory(buffer, size, True);
			if (!succeeded)
			{
				ClearChild();
				if (m_buffer != NULL)
				{
					delete[] m_buffer;
					m_buffer = NULL;
				}
			}
			return succeeded;
		}

		Bool Document::LoadFromMemory(const char* buffer, SizeT size)
		{
			return ReallyLoadFromMemory(const_cast<char*>(buffer), size, False);
		}

		Bool Document::ReallyLoadFromMemory(char* buffer, SizeT size, Bool copiedMemory)
		{
			ClearChild();
			if (m_buffer != NULL)
			{
				delete[] m_buffer;
				m_buffer = NULL;
			}
			if (size < 3)
			{
				return False;
			}
			//get encode
			Encode encode = ANSI;
			Bool multiBytes = False;
			const unsigned char* bom = reinterpret_cast<const unsigned char*>(buffer);
			if (bom[0] == 0xfe && bom[1] == 0xff)
			{
				encode = UTF_16_BIG_ENDIAN;
			}
			else if (bom[0] == 0xff && bom[1] == 0xfe)
			{
				encode = UTF_16;
			}
			else if (bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf)
			{
				encode = UTF_8;
			}
			else
			{
				encode = DetectEncode(buffer, size, multiBytes);
			}

			TChar* text;	//for parser
			SizeT characterCount;

#ifdef SLIM_USE_WCHAR
			if (encode == UTF_16)
			{
				//skip bom
				characterCount = (size - 2) / 2;
				if (copiedMemory)
				{
					m_buffer = buffer;
				}
				else
				{
					m_buffer = new char[size];
					memcpy(m_buffer, buffer, size);
				}
				text = reinterpret_cast<wchar_t*>(buffer + 2);
			}
			else if (encode == UTF_16_BIG_ENDIAN)
			{
				//swap. can be faster
				characterCount = (size - 2) / 2;
				m_buffer = new char[characterCount * sizeof(wchar_t)];
				const wchar_t* src = reinterpret_cast<const wchar_t*>(buffer + 2);
				const wchar_t* srcEnd = src + characterCount;
				wchar_t* dst = (wchar_t*)m_buffer;
				for (; src < srcEnd; ++src, ++dst)
				{
					*((char*)dst) = *(((const char*)src) + 1);
					*(((char*)dst) + 1) = *((const char*)src);
				}
				text = (wchar_t*)m_buffer;
				if (copiedMemory)
				{
					delete[] buffer;
				}
			}
			else if (encode == UTF_8 || encode == UTF_8_NO_MARK || (encode == ANSI && !multiBytes))
			{
				m_buffer = new char[size * sizeof(wchar_t)];
				text = (wchar_t*)m_buffer;
				if (encode == UTF_8)
				{
					//skip bom
					characterCount = Utf8ToUtf16(buffer + 3, size - 3, text, size);
				}
				else
				{
					characterCount = Utf8ToUtf16(buffer, size, text, size);
				}
				if (copiedMemory)
				{
					delete[] buffer;
				}
			}
			else
			{
				if (copiedMemory)
				{
					delete[] buffer;
				}
				return False;
			}
#elif defined(UNICODE)
			if (encode == UTF_8)
			{
				//skip bom
				characterCount = size - 3;
				if (copiedMemory)
				{
					m_buffer = buffer;
				}
				else
				{
					m_buffer = new char[size];
					memcpy(m_buffer, buffer, size);
				}
				text = m_buffer + 3;
			}
			else if (encode == UTF_16)
			{
				characterCount = (size - 2) / 2;
				m_buffer = new char[characterCount * 4];
				//it's not really character count here
				characterCount = Utf16ToUtf8(reinterpret_cast<const wchar_t*>(buffer + 2), characterCount, m_buffer, characterCount * 4);
				text = m_buffer;
				if (copiedMemory)
				{
					delete[] buffer;
				}
			}
			else if (encode == UTF_16_BIG_ENDIAN)
			{
				characterCount = (size - 2) / 2;
				//swap
				wchar_t* leBuffer = new wchar_t[characterCount];
				const wchar_t* src = reinterpret_cast<const wchar_t*>(buffer + 2);
				const wchar_t* srcEnd = src + characterCount;
				wchar_t* dst = leBuffer;
				for (; src < srcEnd; ++src, ++dst)
				{
					*((char*)dst) = *(((const char*)src) + 1);
					*(((char*)dst) + 1) = *((const char*)src);
				}
				m_buffer = new char[characterCount * 4];
				//it's not really character count here
				characterCount = Utf16ToUtf8(leBuffer, characterCount, m_buffer, characterCount * 4);
				text = m_buffer;
				delete[] leBuffer;
				if (copiedMemory)
				{
					delete[] buffer;
				}
			}
			else if (encode == UTF_8_NO_MARK)
			{
				characterCount = size;
				if (copiedMemory)
				{
					m_buffer = buffer;
				}
				else
				{
					m_buffer = new char[size];
					memcpy(m_buffer, buffer, size);
				}
				text = m_buffer;
			}
			else
			{
				if (copiedMemory)
				{
					delete[] buffer;
				}
				return False;
			}
#else
			if (encode != ANSI)
			{
				return False;
			}
			if (copiedMemory)
			{
				m_buffer = buffer;
			}
			else
			{
				m_buffer = new char[size];
				memcpy(m_buffer, buffer, size);
			}
			characterCount = size;
			text = m_buffer;
#endif	//#ifdef SLIM_USE_WCHAR

			return Parse(text, characterCount);
		}

		Bool Document::Save(String& output, Encode encode) const
		{
#ifdef SLIM_USE_WCHAR
			if (encode == ANSI)
			{
				return False;
			}
			if (encode == UTF_16 || encode == UTF_16_BIG_ENDIAN)
			{
				output = Text("<?xml version=\"1.0\" encoding=\"UTF-16\"?>\r\n");
			}
			else if (encode == UTF_8 || encode == UTF_8_NO_MARK)
			{
				output = Text("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
			}
#elif defined (UNICODE)
			if (encode != UTF_8 && encode != UTF_8_NO_MARK)
			{
				return False;
			}
			output = Text("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
#else
			if (encode != ANSI)
			{
				return False;
			}
			//don't know how to get encode string of current code page
			output = Text("<?xml version=\"1.0\"?>\r\n");
#endif

			WriteNode(output, -1);
			return True;
		}

		Bool Document::Save(const TChar* filename, Encode encode) const
		{
			assert(filename != NULL);

			String output;
			if (!Save(output, encode))
			{
				return False;
			}

			std::fstream file;
			file.open(filename, std::ios_base::out | std::ios_base::binary);
			if (!file.is_open())
			{
				return False;
			}

#ifdef SLIM_USE_WCHAR
			unsigned char bom[3];
			if (encode == UTF_16)
			{
				bom[0] = 0xff;
				bom[1] = 0xfe;
				file.write((char*)bom, 2);
				file.write((char*)output.GetBuffer(), output.GetSize() * 2);
			}
			else if (encode == UTF_16_BIG_ENDIAN)
			{
				bom[0] = 0xfe;
				bom[1] = 0xff;
				file.write((char*)bom, 2);
				//swap
				wchar_t* buffer = new wchar_t[output.GetSize()];
				const wchar_t* src = output.GetBuffer();
				const wchar_t* srcEnd = src + output.GetSize();
				wchar_t* dst = buffer;
				for (; src < srcEnd; ++src, ++dst)
				{
					*((char*)dst) = *(((const char*)src) + 1);
					*(((char*)dst) + 1) = *((const char*)src);
				}
				file.write((char*)buffer, output.GetSize() * 2);
				delete[] buffer;
			}
			else if (encode == UTF_8 || encode == UTF_8_NO_MARK)
			{
				if (encode == UTF_8)
				{
					bom[0] = 0xef;
					bom[1] = 0xbb;
					bom[2] = 0xbf;
					file.write((char*)bom, 3);
				}
				SizeT tempBufferSize = output.GetSize() * 4;	//up to 4 bytes per character
				char* tempBuffer = new char[tempBufferSize];
				SizeT converted = Utf16ToUtf8(output.GetBuffer(), output.GetSize(), tempBuffer, tempBufferSize);
				file.write(tempBuffer, converted);
				delete[] tempBuffer;
			}
#else
#if defined (UNICODE)
			if (encode == UTF_8)
			{
				unsigned char bom[3];
				bom[0] = 0xef;
				bom[1] = 0xbb;
				bom[2] = 0xbf;
				file.write((char*)bom, 3);
			}
#endif
			file.write(output.GetBuffer(), output.size());
#endif

			file.close();
			return True;
		}

		Bool Document::Parse(TChar* input, SizeT size)
		{
			TChar* cur = input;
			TChar* end = input + size;

			TChar* label = NULL;
			SizeT labelSize = 0;
			int depth = 0;		Node* currentNode = this;

			while (cur < end)
			{
				assert(depth >= 0);
				assert(currentNode != NULL);

				TChar* lastPos = cur;
				if (!FindLabel(cur, end - cur, label, labelSize))
				{
					break;
				}
				switch (*label)
				{
				case Text('/'):	//node ending
					if (depth < 1)
					{
						return False;
					}
					if (currentNode->GetType() == ELEMENT && !currentNode->HasChild())
					{
						currentNode->AssignValue(lastPos, label - lastPos - 1, True);
					}
					currentNode = currentNode->GetParent();
					--depth;
					break;
				case Text('?'):	//xml define node, ignore
					break;
				case Text('!'):	//comment node
					{
						//ignore !-- and --
						if (labelSize < 5)
						{
							return False;
						}
						Node* comment = currentNode->AddChild(NULL, COMMENT);
						comment->AssignName(label + 3, labelSize - 5, False);
					}
					break;
				default:	//node start
					{
						Node* newNode = currentNode->AddChild(NULL, ELEMENT);

						Bool emptyNode = ParseLabel(newNode, label, labelSize);

						if (!emptyNode)
						{
							currentNode = newNode;
							++depth;
						}
					}
				}
			} // while(cur < end)

			if (depth != 0)
			{
				return False;
			}
			assert(currentNode == this);
			return True;
		}

		Bool Document::FindLabel(TChar* &begin, SizeT size, TChar* &label, SizeT &labelSize)
		{
			label = (TChar*)_tmemchr(begin, Text('<'), size);
			if (label == NULL)
			{
				return False;
			}
			++label;
			size -= (label - begin);

			//comment is special, won't end without "-->"
			if (size > 6 //_tcsclen(Text("!---->"))
				&& label[0] == Text('!')
				&& label[1] == Text('-')
				&& label[2] == Text('-'))
			{
				//buffer is not NULL-terminated, so we can't use strstr, shit! is there a "safe" version of strstr?
				TChar* cur = label + 3;	//skip !--
				size -= 5; //(_tcsclen(Text("!---->")) - 1);
				while (True)
				{
					TChar* end = (TChar*)_tmemchr(cur, Text('-'), size);
					if (end == NULL)
					{
						return False;
					}
					if (*(end+1) == Text('-') && *(end+2) == Text('>'))
					{
						//get it
						labelSize = end - label + 2;
						begin = end + 3;
						return True;
					}
					size -= (end - cur + 1);
					cur = end + 1;
				}
			}
			begin = (TChar*)_tmemchr(label, Text('>'), size);
			if (begin == NULL)
			{
				return False;
			}
			labelSize = begin - label;
			++begin;
			if (labelSize == 0)
			{
				return False;
			}
			return True;
		}

		Bool Document::ParseLabel(Node* node, TChar* label, SizeT labelSize)
		{
			//get name
			TChar* cur = label;
			while (*cur != Text(' ') && *cur != Text('/') && *cur != Text('>'))
			{
				++cur;
			}
			TChar next = *cur;
			node->AssignName(label, cur - label, True);
			if (next != Text(' '))
			{
				return (next == Text('/')) ? True : False;
			}
			//get attributes
			TChar* end = label + labelSize;
			++cur;
			while (cur < end)
			{
				while (*cur == Text(' '))
				{
					++cur;
				}
				//attribute name
				TChar* attrName = cur;
				while (*cur != Text(' ') && *cur != Text('=') && *cur != Text('/') && *cur != Text('>'))
				{
					++cur;
				}
				next = *cur;
				SizeT attrNameSize = cur - attrName;

				//attribute value
				cur = (TChar*)_tmemchr(cur, Text('"'), end - cur);
				if (cur == NULL)
				{
					break;
				}
				TChar* attrValue = ++cur;
				cur = (TChar*)_tmemchr(cur, Text('"'), end - cur);
				if (NULL == cur)
				{
					return False;
				}
				SizeT attrValueSize = cur - attrValue;
				Attribute* attribute = node->AddAttribute();
				attribute->AssignName(attrName, attrNameSize, True);
				attribute->AssignValue(attrValue, attrValueSize, True);
				++cur;
			}
			return (next == Text('/')) ? True : False;
		}

		SizeT Utf8ToUtf16(const char* u8, SizeT size, wchar_t* u16, SizeT outBufferSize)
		{
			SizeT converted = 0;

			while (size > 0)
			{
				if ((*u8 & 0x80) == 0)
				{
					*(u16++) = *(u8++);
					--size;
					++converted;
				}
				else if ((*u8 & 0xe0) == 0xc0)
				{
					if (size < 2)
					{
						break;
					}
					*(u16++) = (*u8 & 0x1f) | ((*(u8+1) & 0x3f) << 5);
					u8 += 2;
					size -= 2;
					++converted;
				}
				else if ((*u8 & 0xf0) == 0xe0)
				{
					if (size < 3)
					{
						break;
					}
					*u16 = ((*u8 & 0x0f) << 12) | ((*(u8+1) & 0x3f) << 6) | (*(u8+2) & 0x3f);
					u8 += 3;
					++u16;
					size -= 3;
					++converted;
				}
				else
				{
					break;
				}
				if (converted == outBufferSize)
				{
					break;
				}
			}
			return converted;
		}

		SizeT Utf16ToUtf8(const wchar_t* u16, SizeT size, char* u8, SizeT outBufferSize)
		{
			SizeT converted = 0;

			while (size > 0)
			{
				if (*u16 < 0x80)
				{
					//1 byte
					if (converted == outBufferSize)
					{
						break;
					}
					*(u8++) = static_cast<char>(*(u16++));
					--size;
					++converted;
				}
				else if (*u16 < 0x800)
				{
					//2 bytes
					if (converted + 2 > outBufferSize)
					{
						break;
					}
					*u8 = (*u16 >> 6) | 0xc0;
					*(u8+1) = (*u16 & 0x3f) | 0x80;
					u8 += 2;
					++u16;
					--size;
					converted += 2;
				}
				else
				{
					//3 bytes
					if (converted + 3 > outBufferSize)
					{
						break;
					}
					*u8 = (*u16 >> 12) | 0xe0;
					*(u8+1) = ((*u16 >> 6) & 0x3f) | 0x80;
					*(u8+2) = (*u16 & 0x3f) | 0x80;
					u8 += 3;
					++u16;
					--size;
					converted += 3;
				}
			}
			return converted;
		}

		Encode DetectEncode(const char* str, SizeT size, Bool& multiBytes)
		{
			while (size > 0)
			{
				if ((*str & 0x80) == 0)
				{
					//1 byte
					++str;
					--size;
				}
				else
				{
					multiBytes = True;
					if ((*str & 0xf0) == 0xe0)
					{
						//3 bytes
						if (size < 3)
						{
							return ANSI;
						}
						if ((*(str+1) & 0xc0) != 0x80 || (*(str+2) & 0xc0) != 0x80)
						{
							return ANSI;
						}
						str += 3;
						size -= 3;
					}
					else if ((*str & 0xe0) == 0xc0)
					{
						//2 bytes
						if (size < 2)
						{
							return ANSI;
						}
						if ((*(str+1) & 0xc0) != 0x80)
						{
							return ANSI;
						}
						str += 2;
						size -= 2;
					}
					else if ((*str & 0xf8) == 0xf0)
					{
						//4 bytes
						if (size < 4)
						{
							return ANSI;
						}
						if ((*(str+1) & 0xc0) != 0x80 || (*(str+2) & 0xc0) != 0x80 || (*(str+3) & 0xc0) != 0x80)
						{
							return ANSI;
						}
						str += 4;
						size -= 4;
					}
					else
					{
						return ANSI;
					}
				}
			}
		#if defined(UNICODE)
			return UTF_8_NO_MARK;
		#else
			return multiBytes ? UTF_8_NO_MARK : ANSI;
		#endif
		}
	}
}
