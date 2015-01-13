#pragma once
#include "..\..\Serialization\ISerializer.h"

namespace WinToolsLib
{
	namespace Data
	{
		namespace Sqlite
		{
			class IStatement : public Serialization::IStoreProperty
			{
			public:
				virtual ~IStatement() {}

				virtual Void Finalize() = 0;
				virtual Int64 Execute() = 0;

				virtual Void StoreNull(const TChar* name) = 0;

				virtual Int64 GetLastId() const = 0;
				virtual Int32 GetRowsModified() const = 0;
			};

			typedef std::unique_ptr<IStatement> StatementPtr;
		}
	}
}