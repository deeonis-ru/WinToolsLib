#include "SqliteException.h"
#include "Details\sqlite3.h"

namespace WinToolsLib { namespace Data { namespace Sqlite
{
	SqliteException::SqliteException(
		PCChar function,
		UInt32 line,
		PCChar file,
		UInt32 errorCode,
		PCChar errorMessage) :
		ExceptionWithCode(
			function,
			line,
			file,
			errorCode,
			CreateErrorMessage(errorCode, errorMessage))
	{
	}

	String SqliteException::CreateErrorMessage(UInt32 errorCode, PCChar errorMessage)
	{
		if (errorMessage != nullptr)
		{
			return String::FormatS(Text("SQLite: %s (%S)"), ErrorCodeToMessage(errorCode), errorMessage);
		}
		else
		{
			return String::FormatS(Text("SQLite: %s"), ErrorCodeToMessage(errorCode));
		}
	}

	const TChar* SqliteException::ErrorCodeToMessage(UInt32 errorCode)
	{
		switch (errorCode)
		{
		case SQLITE_OK:
			return Text("Successful result");
		case SQLITE_ERROR:
			return Text("SQL error or missing database");
		case SQLITE_INTERNAL:
			return Text("Internal logic error in SQLite");
		case SQLITE_PERM:
			return Text("Access permission denied");
		case SQLITE_ABORT:
			return Text("Callback routine requested an abort");
		case SQLITE_BUSY:
			return Text("The database file is locked");
		case SQLITE_LOCKED:
			return Text("A table in the database is locked");
		case SQLITE_NOMEM:
			return Text("A malloc() failed");
		case SQLITE_READONLY:
			return Text("Attempt to write a readonly database");
		case SQLITE_INTERRUPT:
			return Text("Operation terminated by sqlite3_interrupt()");
		case SQLITE_IOERR:
			return Text("Some kind of disk I/O error occurred");
		case SQLITE_CORRUPT:
			return Text("The database disk image is malformed");
		case SQLITE_NOTFOUND:
			return Text("Unknown opcode in sqlite3_file_control()");
		case SQLITE_FULL:
			return Text("Insertion failed because database is full");
		case SQLITE_CANTOPEN:
			return Text("Unable to open the database file");
		case SQLITE_PROTOCOL:
			return Text("Database lock protocol error");
		case SQLITE_EMPTY:
			return Text("Database is empty");
		case SQLITE_SCHEMA:
			return Text("The database schema changed");
		case SQLITE_TOOBIG:
			return Text("String or BLOB exceeds size limit");
		case SQLITE_CONSTRAINT:
			return Text("Abort due to constraint violation");
		case SQLITE_MISMATCH:
			return Text("Data type mismatch");
		case SQLITE_MISUSE:
			return Text("Library used incorrectly");
		case SQLITE_NOLFS:
			return Text("Uses OS features not supported on host");
		case SQLITE_AUTH:
			return Text("Authorization denied");
		case SQLITE_FORMAT:
			return Text("Auxiliary database format error");
		case SQLITE_RANGE:
			return Text("2nd parameter to sqlite3_bind out of range");
		case SQLITE_NOTADB:
			return Text("File opened that is not a database file");
		case SQLITE_ROW:
			return Text("sqlite3_step() has another row ready");
		case SQLITE_DONE:
			return Text("sqlite3_step() has finished executing");
		default:
			return Text("Unknow error code");
		}
	}
	
	ColumnNotFoundException::ColumnNotFoundException(PCChar function, UInt32 line, PCChar file, PCWChar name) :
		Exception(function, line, file, CreateErrorMessage(name))
	{
	}

	String ColumnNotFoundException::CreateErrorMessage(PCWChar name)
	{
		return String::FormatS(Text("Column not found: %s"), name);
	}

} } }