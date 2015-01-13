#pragma once
#include "..\..\Types.h"
#include "..\..\String.h"
#include "..\..\Exception.h"

namespace WinToolsLib
{
	namespace Data
	{
		namespace Sqlite
		{
			class SqliteException : public ExceptionWithCode
			{
			public:
				SqliteException(
					PCChar function,
					UInt32 line,
					PCChar file,
					UInt32 errorCode,
					PCChar errorMessage = nullptr);

			protected:
				static String CreateErrorMessage(UInt32 errorCode, PCChar errorMessage);
				static const TChar* ErrorCodeToMessage(UInt32 errorCode);
			};

			#define THROW_SQLITE_EXCEPTION(errorCode) \
				throw SqliteException(__FUNCTION__, __LINE__, __FILE__, errorCode)

			#define THROW_SQLITE_EXCEPTION2(errorCode, errorMessage) \
				throw SqliteException(__FUNCTION__, __LINE__, __FILE__, errorCode, errorMessage)

			class ColumnNotFoundException : public WinToolsLib::Exception
			{
			public:
				ColumnNotFoundException(
					PCChar function,
					UInt32 line,
					PCChar file,
					PCWChar name);

			protected:
				static String CreateErrorMessage(PCWChar name);
			};

			#define THROW_COLUMN_NOT_FOUND_EXCEPTION(name) \
				throw ColumnNotFoundException(__FUNCTION__, __LINE__, __FILE__, name)
		}
	}
}

