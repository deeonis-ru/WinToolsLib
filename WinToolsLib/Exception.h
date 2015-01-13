#pragma once
#include "Types.h"
#include "String.h"

namespace WinToolsLib
{
	//
	// Base exception class
	//

	class Exception
	{
	public:
		Exception(
			PCChar function,
			UInt32 line,
			PCChar file,
			String message);

		PCChar GetFunctionName() const;
		UInt32 GetLineNumber() const;
		PCChar GetFileName() const;
		String GetErrorMessage() const;

	protected:
		PCChar m_function;
		UInt32 m_line;
		PCChar m_file;
		String m_message;
	};

	inline PCChar Exception::GetFunctionName() const
	{
		return m_function;
	}

	inline UInt32 Exception::GetLineNumber() const
	{
		return m_line;
	}

	inline PCChar Exception::GetFileName() const
	{
		return m_file;
	}

	inline String Exception::GetErrorMessage() const
	{
		return m_message;
	}

	//
	// Exception with error code
	//

	class ExceptionWithCode : public Exception
	{
	public:
		ExceptionWithCode(
			PCChar function,
			UInt32 line,
			PCChar file,
			UInt32 errorCode,
			String message = String());
		
		UInt32 GetErrorCode() const;

	protected:
		static String CreateErrorMessage(UInt32 errorCode);

	protected:
		UInt32 m_errorCode;
	};
		
	inline UInt32 ExceptionWithCode::GetErrorCode() const
	{
		return m_errorCode;
	}

	//
	// Structured exception wrapper
	//

	class StructuredException : public ExceptionWithCode
	{
	public:
		StructuredException(
			PCChar function,
			UInt32 line,
			PCChar file,
			UInt32 errorCode,
			SizeT address,
			Bool continuable);
		~StructuredException();

		static Void EnableInThisThread();

	protected:
		static String CreateErrorMessage(
			UInt32 errorCode,
			SizeT address,
			Bool continuable);

	protected:
		SizeT m_address;
		Bool m_continuable;
	};

	//
	// Win32 error wrapper
	//

	class Win32Exception : public ExceptionWithCode
	{
	public:
		Win32Exception(
			PCChar function,
			UInt32 line,
			PCChar file,
			UInt32 errorCode);
		
	protected:
		String CreateErrorMessage(UInt32 errorCode);
	};

	#define THROW_WIN32_EXCEPTION(errorCode) \
		throw Win32Exception(__FUNCTION__, __LINE__, __FILE__, errorCode)

	//
	// Last error wrapper
	//

	class LastErrorException : public Win32Exception
	{
	public:
		LastErrorException(PCChar function, UInt32 line, PCChar file);
	};

	#define THROW_LAST_ERROR_EXCEPTION() \
		throw LastErrorException(__FUNCTION__, __LINE__, __FILE__)

	//
	// File not found error wrapper
	//

	class FileNotFoundException : public ExceptionWithCode
	{
	public:
		FileNotFoundException(PCChar function, UInt32 line, PCChar file, PCTChar path);

		String GetPath() const;
		
	protected:
		String CreateErrorMessage(PCTChar path);

	private:
		String m_path;
	};

	inline String FileNotFoundException::GetPath() const
	{
		return m_path;
	}

	#define THROW_FILE_NOT_FOUND_EXCEPTION(path) \
		throw FileNotFoundException(__FUNCTION__, __LINE__, __FILE__, path)

	//
	// Position is out of range
	//

	class OutOfRangeException : public Exception
	{
	public:
		OutOfRangeException(PCChar function, UInt32 line, PCChar file);
	};

	#define THROW_OUT_OF_RANGE_EXCEPTION() \
		throw OutOfRangeException(__FUNCTION__, __LINE__, __FILE__)

	//
	// Handle is invalid
	//

	class InvalidHandleException : public Exception
	{
	public:
		InvalidHandleException(PCChar function, UInt32 line, PCChar file);
	};

	#define THROW_INVALID_HANDLE_EXCEPTION() \
		throw InvalidHandleException(__FUNCTION__, __LINE__, __FILE__)

	//
	// Not PE image
	//

	class BadImageFormatException : public Exception
	{
	public:
		BadImageFormatException(PCChar function, UInt32 line, PCChar file);
	};

	#define THROW_BAD_IMAGE_FORMAT_EXCEPTION() \
		throw BadImageFormatException(__FUNCTION__, __LINE__, __FILE__)

	//
	// Unsupported type of relocation
	//

	class UnsupportedRelocationTypeException : public Exception
	{
	public:
		UnsupportedRelocationTypeException(PCChar function, UInt32 line, PCChar file);
	};

	#define THROW_UNSUPPORTED_RELOCATION_TYPE_EXCEPTION() \
		throw UnsupportedRelocationTypeException(__FUNCTION__, __LINE__, __FILE__)

	//
	// Section was paged out
	//

	class SectionPagedOutException : public Exception
	{
	public:
		SectionPagedOutException(PCChar function, UInt32 line, PCChar file);
	};

	#define THROW_SECTION_PAGED_OUT_EXCEPTION() \
		throw SectionPagedOutException(__FUNCTION__, __LINE__, __FILE__)

	//
	// Task canceled
	//

	class TaskCanceledException : public Exception
	{
	public:
		TaskCanceledException(PCChar function, UInt32 line, PCChar file);
	};

	#define THROW_TASK_CANCELED_EXCEPTION() \
		throw TaskCanceledException(__FUNCTION__, __LINE__, __FILE__)
}
