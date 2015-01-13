#include "Key.h"
#include "..\Exception.h"

namespace WinToolsLib
{
	namespace Registry
	{
		Key::Key(HKEY hKey) :
			m_handle(hKey)
		{
		}

		Key::Key(Key&& other) :
			m_handle(std::move(other.m_handle))
		{
		}

		Key& Key::operator=(Key&& other)
		{
			if (this != &other)
			{
				m_handle = std::move(other.m_handle);
			}
			return *this;
		}

		Void Key::EnumName(const EnumNameCallback& callback)
		{
			for (UInt32 i = 0;; i++)
			{
				TCHAR name[1024]; // FIXME: Use dynamic memory allocation instead
				DWORD nameSize = 1024;

				auto error = ::RegEnumValue(m_handle, i, name, &nameSize, 0, NULL, NULL, NULL);
				if (ERROR_SUCCESS != error)
				{
					if (ERROR_NO_MORE_ITEMS == error)
					{
						break;
					}
					THROW_WIN32_EXCEPTION(error);
				}

				callback(name);
			}
		}

		Void Key::EnumValue(const EnumValueCallback& callback)
		{
			for (UInt32 i = 0;; i++)
			{
				TCHAR name[1024]; // FIXME: Use dynamic memory allocation instead
				DWORD nameSize = 1024;
				DWORD dataSize = 0;
				DWORD type = 0;

				auto error = ::RegEnumValue(m_handle, i, name, &nameSize, 0, &type, NULL, &dataSize);
				if (ERROR_SUCCESS != error)
				{
					if (ERROR_NO_MORE_ITEMS == error)
					{
						break;
					}
					THROW_WIN32_EXCEPTION(error);
				}

				if (nameSize < 1024)
				{
					nameSize++;
				}
				Buffer data(dataSize);

				error = ::RegEnumValue(m_handle, i, name, &nameSize, 0, &type, data.GetBuffer(), &dataSize);
				if (ERROR_SUCCESS != error)
				{
					if (ERROR_NO_MORE_ITEMS == error)
					{
						break;
					}
					THROW_WIN32_EXCEPTION(error);
				}

				callback(name, type, data);
			}
		}

		Void Key::EnumSubKey(const EnumSubKeyCallback& callback, UInt32 accessRights)
		{
			for (UInt32 i = 0;; i++)
			{
				TCHAR name[1024]; // FIXME: Use dynamic memory allocation instead
				DWORD nameSize = 1024;

				auto error = ::RegEnumKeyEx(m_handle, i, name, &nameSize, NULL, NULL, NULL, NULL);
				if (ERROR_SUCCESS != error)
				{
					if (ERROR_NO_MORE_ITEMS == error)
					{
						break;
					}
					THROW_WIN32_EXCEPTION(error);
				}

				callback(name, Key::Open(m_handle, name, accessRights));
			}
		}

		Bool Key::ValueExist(const TChar* valueName) const
		{
			DWORD dataSize = 0;
			DWORD type = 0;

			auto error = ::RegQueryValueEx(m_handle, valueName, NULL, &type, NULL, &dataSize);
			switch (error)
			{
			case ERROR_SUCCESS:
				return True;

			case ERROR_FILE_NOT_FOUND:
				return False;

			default:
				THROW_WIN32_EXCEPTION(error);
			}
		}

		Bool Key::SetValue(const TChar* valueName, ValueType valueType, const Byte* data, UInt32 size)
		{
			if ((ValueType::String == valueType || ValueType::MultiString == valueType ||
				ValueType::ExpandString == valueType) && data != nullptr)
			{
				size++;
			}

			auto error = ::RegSetValueEx(m_handle, valueName, 0, (UInt32)valueType, data, size);
			if (ERROR_SUCCESS != error)
			{
				return False;
			}
			return True;
		}

		Bool Key::GetValue(const TChar* valueName, ValueType valueType, Buffer& data) const
		{
			DWORD dataSize = 0;
			DWORD type = 0;

			auto error = ::RegQueryValueEx(m_handle, valueName, NULL, &type, NULL, &dataSize);
			if (error != ERROR_SUCCESS || (ValueType)type != valueType)
				return False;

			auto sizeForString = dataSize;
			if (ValueType::String == (ValueType)type || ValueType::MultiString == (ValueType)type ||
				ValueType::ExpandString == (ValueType)type)
				sizeForString += sizeof(TChar);

			Buffer tmp(sizeForString);
			error = ::RegQueryValueEx(m_handle, valueName, NULL, NULL, tmp.GetBuffer(), &dataSize);
			if (error != ERROR_SUCCESS)
				return False;

			data = std::move(tmp);
			return True;
		}

		Bool Key::DeleteValue(const TChar* valueName)
		{
			auto error = ::RegDeleteValue(m_handle, valueName);
			if (ERROR_SUCCESS != error)
			{
				return False;
			}
			return True;
		}

		PWChar Key::RootToString(HKEY hRoot)
		{
			switch ((UInt32)hRoot)
			{
				case (UInt32)HKEY_CLASSES_ROOT:
					return L"HKCR";

				case (UInt32)HKEY_CURRENT_USER:
					return L"HKCU";

				case (UInt32)HKEY_LOCAL_MACHINE:
					return L"HKLM";

				case (UInt32)HKEY_USERS:
					return L"HKU";

				case (UInt32)HKEY_CURRENT_CONFIG:
					return L"HKCC";

				default:
					return L"";
			}
		}

		PWChar Key::RootToFullString(HKEY hRoot)
		{
			switch ((UInt32)hRoot)
			{
				case (UInt32)HKEY_CLASSES_ROOT:
					return L"HKEY_CLASSES_ROOT";

				case (UInt32)HKEY_CURRENT_USER:
					return L"HKEY_CURRENT_USER";

				case (UInt32)HKEY_LOCAL_MACHINE:
					return L"HKEY_LOCAL_MACHINE";

				case (UInt32)HKEY_USERS:
					return L"HKEY_USERS";

				case (UInt32)HKEY_CURRENT_CONFIG:
					return L"HKEY_CURRENT_CONFIG";

				default:
					return L"";
			}
		}

		HKEY Key::StringToRootHkey(const String& rootString)
		{
			if (Text("HKCR") == rootString)
				return HKEY_CLASSES_ROOT;
			else if (Text("HKCU") == rootString)
				return HKEY_CURRENT_USER;
			else if (Text("HKLM") == rootString)
				return HKEY_LOCAL_MACHINE;
			else if (Text("HKU") == rootString)
				return HKEY_USERS;
			else if (Text("HKCC") == rootString)
				return HKEY_CURRENT_CONFIG;
			else
				return NULL;
		}

		Key Key::Open(HKEY hKey, const TChar* subKey, UInt32 accessRights)
		{
			HKEY hKeyResult = NULL;

			auto error = ::RegOpenKeyEx(hKey, subKey, 0, accessRights, &hKeyResult);
			if (ERROR_SUCCESS != error)
			{
				if (ERROR_FILE_NOT_FOUND == error)
				{
					String path(Key::RootToString(hKey));
					if (!path.IsEmpty())
					{
						path += Text("\\");
					}
					path += subKey;
					THROW_FILE_NOT_FOUND_EXCEPTION(path);
				}
				else
				{
					THROW_WIN32_EXCEPTION(error);
				}
			}

			return Key(hKeyResult);
		}

		Key Key::Create(HKEY hKey, const TChar* subKey, UInt32 accessRights)
		{
			HKEY hKeyResult = NULL;

			auto error = ::RegCreateKeyEx(hKey, subKey, 0, NULL, 0, accessRights, NULL, &hKeyResult, NULL);
			if (ERROR_SUCCESS != error)
			{
				THROW_WIN32_EXCEPTION(error);
			}

			return Key(hKeyResult);
		}

		Bool Key::Delete(HKEY hKey, const TChar* subKey, Bool recursive)
		{
			if (recursive)
			{
				HKEY hSubKey = NULL;
				auto error = ::RegOpenKeyEx(hKey, subKey, 0, KEY_ENUMERATE_SUB_KEYS, &hSubKey);
				if (ERROR_SUCCESS == error)
				{
					Key sub(hSubKey);
					for (UInt32 i = 0;; i++)
					{
						TCHAR name[1024]; // FIXME: Use dynamic memory allocation instead
						DWORD nameSize = 1024;

						error = ::RegEnumKeyEx(sub, i, name, &nameSize, NULL, NULL, NULL, NULL);
						if (ERROR_SUCCESS == error)
						{
							auto success = Key::Delete(sub, name, True);
							if (!success)
							{
								break; // We wouldn't be able to delete key if it has sub keys
							}
						}
						else if (ERROR_NO_MORE_ITEMS == error)
						{
							break;
						}
					}
				}
			}

			auto error = ::RegDeleteKey(hKey, subKey);
			if (ERROR_SUCCESS != error)
			{
				return False;
			}

			return True;
		}

		Bool Key::Exists(HKEY hKey, const TChar* subKey)
		{
			HKEY hKeyResult = NULL;

			auto error = ::RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hKeyResult);
			if (ERROR_SUCCESS != error)
			{
				if (ERROR_FILE_NOT_FOUND == error)
				{
					return False;
				}
				else
				{
					THROW_WIN32_EXCEPTION(error);
				}
			}

			::RegCloseKey(hKeyResult);
			return True;
		}

		Event Key::CreateChangeEvent(UInt32 filter)
		{
			Event event;
			event.Create(True, False);

			if (::RegNotifyChangeKeyValue(m_handle, True, filter, event, True) != ERROR_SUCCESS)
				THROW_LAST_ERROR_EXCEPTION();

			return std::move(event);
		}
	}
}