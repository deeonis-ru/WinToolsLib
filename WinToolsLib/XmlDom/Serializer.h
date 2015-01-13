#pragma once
#include "..\Types.h"
#include "..\String.h"
#include "..\Exception.h"
#include "..\Serialization\ISerializer.h"
#include "XmlDom.h"

namespace WinToolsLib
{
	namespace XmlDom
	{
		using namespace Serialization;

		class Serializer : public ISerializer
		{
		public:
			Serializer(Node* node);
			~Serializer();

#pragma region IStoreProperty

			Void StoreProperty(const TChar* name, Bool value, BoolFormat format = BoolFormat::TrueFalse) override;

			Void StoreProperty(const TChar* name, Int8 value, IntFormat format = IntFormat::Dec) override;
			Void StoreProperty(const TChar* name, UInt8 value, IntFormat format = IntFormat::Dec) override;

			Void StoreProperty(const TChar* name, Int16 value, IntFormat format = IntFormat::Dec) override;
			Void StoreProperty(const TChar* name, UInt16 value, IntFormat format = IntFormat::Dec) override;

			Void StoreProperty(const TChar* name, Int32 value, IntFormat format = IntFormat::Dec) override;
			Void StoreProperty(const TChar* name, UInt32 value, IntFormat format = IntFormat::Dec) override;

			Void StoreProperty(const TChar* name, Int64 value, IntFormat format = IntFormat::Dec) override;
			Void StoreProperty(const TChar* name, UInt64 value, IntFormat format = IntFormat::Dec) override;

			Void StoreProperty(const TChar* name, Float value, FloatFormat format = FloatFormat::Default) override;
			Void StoreProperty(const TChar* name, Double value, FloatFormat format = FloatFormat::Default) override;

			Void StoreProperty(const TChar* name, const String& value) override;

			Void StoreProperty(const TChar* name, const Buffer& value) override;

			IStorePropertyPtr StoreChild(const TChar* name) override;

#pragma endregion

#pragma region ILoadProperty

			Bool LoadProperty(const TChar* name, Bool defaultValue) const override;

			Int8 LoadProperty(const TChar* name, Int8 defaultValue) const override;
			UInt8 LoadProperty(const TChar* name, UInt8 defaultValue) const override;

			Int16 LoadProperty(const TChar* name, Int16 defaultValue) const override;
			UInt16 LoadProperty(const TChar* name, UInt16 defaultValue) const override;

			Int32 LoadProperty(const TChar* name, Int32 defaultValue) const override;
			UInt32 LoadProperty(const TChar* name, UInt32 defaultValue) const override;

			Int64 LoadProperty(const TChar* name, Int64 defaultValue) const override;
			UInt64 LoadProperty(const TChar* name, UInt64 defaultValue) const override;

			Float LoadProperty(const TChar* name, Float defaultValue) const override;
			Double LoadProperty(const TChar* name, Double defaultValue) const override;

			String LoadProperty(const TChar* name, const TChar* defaultValue) const override;

			Buffer LoadProperty(const TChar* name, const Buffer& defaultValue) const override;

			ILoadPropertyPtr LoadChild(const TChar* name) const override;
			Void LoadChildren(const TChar* name, LoadChildrenCallback callback) const override;

#pragma endregion

		private:
			Node* m_node;
		};
	}
}

