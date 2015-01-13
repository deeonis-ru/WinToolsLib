#pragma once
#include "..\Types.h"
#include "..\String.h"
#include "..\Buffer.h"
#include "..\Exception.h"

#include <functional>
#include <memory>

namespace WinToolsLib
{
	namespace Serialization
	{
		enum class BoolFormat
		{
			TrueFalse,
			YesNo,
			OneZero
		};

		enum class IntFormat
		{
			Dec,
			Hex
		};

		enum class FloatFormat
		{
			Default
		};

		struct IStoreProperty;
		typedef std::unique_ptr<IStoreProperty> IStorePropertyPtr;

		struct IStoreProperty
		{
			virtual ~IStoreProperty() {}

			virtual Void BeginStoreProperties() {}

			virtual Void StoreProperty(const TChar* name, Bool value, BoolFormat format = BoolFormat::TrueFalse) = 0;

			virtual Void StoreProperty(const TChar* name, Int8 value, IntFormat format = IntFormat::Dec) = 0;
			virtual Void StoreProperty(const TChar* name, UInt8 value, IntFormat format = IntFormat::Dec) = 0;

			virtual Void StoreProperty(const TChar* name, Int16 value, IntFormat format = IntFormat::Dec) = 0;
			virtual Void StoreProperty(const TChar* name, UInt16 value, IntFormat format = IntFormat::Dec) = 0;

			virtual Void StoreProperty(const TChar* name, Int32 value, IntFormat format = IntFormat::Dec) = 0;
			virtual Void StoreProperty(const TChar* name, UInt32 value, IntFormat format = IntFormat::Dec) = 0;
			
			virtual Void StoreProperty(const TChar* name, Int64 value, IntFormat format = IntFormat::Dec) = 0;
			virtual Void StoreProperty(const TChar* name, UInt64 value, IntFormat format = IntFormat::Dec) = 0;

			virtual Void StoreProperty(const TChar* name, Float value, FloatFormat format = FloatFormat::Default) = 0;
			virtual Void StoreProperty(const TChar* name, Double value, FloatFormat format = FloatFormat::Default) = 0;
			
			virtual Void StoreProperty(const TChar* name, const String& value) = 0;

			virtual Void StoreProperty(const TChar* name, const Buffer& value) = 0;

			virtual Void EndStoreProperties() {}

			virtual IStorePropertyPtr StoreChild(const TChar* name) = 0;
		};

		struct ILoadProperty;
		typedef std::unique_ptr<ILoadProperty> ILoadPropertyPtr;
		typedef std::function<Bool(ILoadProperty*)> LoadChildrenCallback;

		struct ILoadProperty
		{
			virtual ~ILoadProperty() {}

			virtual Bool LoadProperty(const TChar* name, Bool defaultValue) const = 0;

			virtual Int8 LoadProperty(const TChar* name, Int8 defaultValue) const = 0;
			virtual UInt8 LoadProperty(const TChar* name, UInt8 defaultValue) const = 0;

			virtual Int16 LoadProperty(const TChar* name, Int16 defaultValue) const = 0;
			virtual UInt16 LoadProperty(const TChar* name, UInt16 defaultValue) const = 0;

			virtual Int32 LoadProperty(const TChar* name, Int32 defaultValue) const = 0;
			virtual UInt32 LoadProperty(const TChar* name, UInt32 defaultValue) const = 0;
			
			virtual Int64 LoadProperty(const TChar* name, Int64 defaultValue) const = 0;
			virtual UInt64 LoadProperty(const TChar* name, UInt64 defaultValue) const = 0;
			
			virtual Float LoadProperty(const TChar* name, Float defaultValue) const = 0;
			virtual Double LoadProperty(const TChar* name, Double defaultValue) const = 0;
			
			virtual String LoadProperty(const TChar* name, const TChar* defaultValue) const = 0;

			virtual Buffer LoadProperty(const TChar* name, const Buffer& defaultValue) const = 0;

			virtual ILoadPropertyPtr LoadChild(const TChar* name) const = 0;
			virtual Void LoadChildren(const TChar* name, LoadChildrenCallback callback) const = 0;
		};

		struct ISerializer : public IStoreProperty, public ILoadProperty
		{
			virtual ~ISerializer() {}
		};

		struct ISerializable
		{
			virtual ~ISerializable() {}

			virtual Void Store(IStoreProperty* store) const = 0;
			virtual Void Load(const ILoadProperty* load) = 0;
		};

		class UnsupportedFormatException : public Exception
		{
		public:
			UnsupportedFormatException(PCChar function, UInt32 line, PCChar file) :
				Exception(function, line, file, Text("Unsupported format was used"))
			{
			}
		};

		#define THROW_UNSUPPORTED_FORMAT_EXCEPTION() \
			throw UnsupportedFormatException(__FUNCTION__, __LINE__, __FILE__)
	}
}