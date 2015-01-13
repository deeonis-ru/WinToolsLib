#pragma once
#include <functional>
#include <thread>
#include <memory>

#include <Windows.h>

#include "..\Types.h"
#include "..\String.h"
#include "..\Buffer.h"
#include "..\Handles\RegistryHandle.h"
#include "..\Event.h"

namespace WinToolsLib
{
	namespace Registry
	{
		using namespace Handles;

		enum class ValueType
		{
			String = REG_SZ,
			MultiString = REG_MULTI_SZ,
			ExpandString =  REG_EXPAND_SZ,
			DoubleWord = REG_DWORD,
			QuadWord = REG_QWORD,
			Binary = REG_BINARY
		};

		class Key
		{
			Key(const Key& other);
			Key& operator=(const Key& other);

		public:
			Key(HKEY hKey);
			Key(Key&& other);

			Key& operator=(Key&& other);

			typedef std::function<Void(const TChar* name)> EnumNameCallback;
			typedef std::function<Void(const TChar* name, UInt32 type, Buffer& data)> EnumValueCallback;
			typedef std::function<Void(const TChar* name, Key& key)> EnumSubKeyCallback;
			
			Void EnumName(const EnumNameCallback& callback);
			Void EnumValue(const EnumValueCallback& callback);
			Void EnumSubKey(const EnumSubKeyCallback& callback, UInt32 accessRights);

			Bool ValueExist(const TChar* valueName) const;
			Bool SetValue(const TChar* valueName, ValueType valueType, const Byte* data, UInt32 size);
			Bool GetValue(const TChar* valueName, ValueType valueType, Buffer& data) const;
			Bool DeleteValue(const TChar* valueName);

			operator HKEY() const;

			static PWChar RootToString(HKEY hRoot);
			static PWChar RootToFullString(HKEY hRoot);
			static HKEY StringToRootHkey(const String& rootString);

			static Key Open(HKEY hKey, const TChar* subKey, UInt32 accessRights);
			static Key Create(HKEY hKey, const TChar* subKey, UInt32 accessRights);
			static Bool Delete(HKEY hKey, const TChar* subKey, Bool recursive = False);
			static Bool Exists(HKEY hKey, const TChar* subKey);

			Event CreateChangeEvent(UInt32 filter);

		private:
			RegistryHandle m_handle;
		};

		inline Key::operator HKEY() const
		{
			return m_handle;
		}
	}
}

