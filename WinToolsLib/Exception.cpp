#include "Exception.h"
#include "Log.h"

#include <Windows.h>
#include <tchar.h>

namespace WinToolsLib
{
	//
	// Base exception class
	//

	Exception::Exception(
		PCChar function,
		UInt32 line,
		PCChar file,
		String message) :
		m_function(function),
		m_line(line),
		m_file(file),
		m_message(message)
	{
		LOG_EXCEPTION(
			Text("%s in %S %S(%u)"),
			m_message.GetBuffer(),
			m_function,
			m_file,
			m_line);
	}

	//
	// Exception with error code
	//

	ExceptionWithCode::ExceptionWithCode(
		PCChar function,
		UInt32 line,
		PCChar file,
		UInt32 errorCode,
		String message):
		Exception(
			function,
			line,
			file,
			message.IsEmpty() ? CreateErrorMessage(errorCode) : message),
		m_errorCode(errorCode)
	{
	}

	String ExceptionWithCode::CreateErrorMessage(UInt32 errorCode)
	{
		return String::FormatS(
			Text("0x%08X"),
			errorCode);
	}

	//
	// Structured exception wrapper
	//

	StructuredException::StructuredException(
		PCChar function,
		UInt32 line,
		PCChar file,
		UInt32 errorCode,
		SizeT address,
		Bool continuable) :
		ExceptionWithCode(
			function,
			line,
			file,
			errorCode,
			CreateErrorMessage(
				errorCode,
				address,
				continuable)),
		m_address(address),
		m_continuable(continuable)
	{
	}

	StructuredException::~StructuredException()
	{
		if (!m_continuable)
		{
			::ExitProcess(m_errorCode);
		}
	}

	static Void SeTranslator(UInt32 errorCode, PEXCEPTION_POINTERS pExp)
	{
		SizeT address = 0;
		Bool continuable = True;
		if (pExp && pExp->ExceptionRecord)
		{
			address = (SizeT)pExp->ExceptionRecord->ExceptionAddress;
			if (pExp->ExceptionRecord->ExceptionFlags & EXCEPTION_NONCONTINUABLE)
			{
				continuable = False;
			}
		}
		throw StructuredException(__FUNCTION__, __LINE__, __FILE__, errorCode, address, continuable);
	}

	Void StructuredException::EnableInThisThread()
	{
		_set_se_translator(SeTranslator);
	}

	String StructuredException::CreateErrorMessage(
		UInt32 errorCode,
		SizeT address,
		Bool continuable)
	{
		return String::FormatS(
			Text("0x%08X at 0x%08IX (%s)"),
			errorCode,
			address,
			continuable ? Text("continuable") : Text("non-continuable"));
	}

	//
	// Win32 error wrapper
	//

	Win32Exception::Win32Exception(
		PCChar function, 
		UInt32 line, 
		PCChar file, 
		UInt32 errorCode) :
		ExceptionWithCode(
			function,
			line,
			file,
			errorCode,
			CreateErrorMessage(errorCode))
	{
	}

	String Win32Exception::CreateErrorMessage(UInt32 errorCode)
	{
		LPTSTR pszMsgBuf = NULL;
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&pszMsgBuf,
			0,
			NULL);
		if (pszMsgBuf)
		{
			String message(pszMsgBuf);
			auto end = message.Find(Text("\r\n"));
			::LocalFree((HLOCAL)pszMsgBuf);
			return message.SubString(0, end);
		}
		return ExceptionWithCode::CreateErrorMessage(errorCode);
	}

	//
	// Last error wrapper
	//

	LastErrorException::LastErrorException(PCChar function, UInt32 line, PCChar file) :
		Win32Exception(function, line, file, ::GetLastError())
	{
	}

	//
	// File not found error wrapper
	//

	FileNotFoundException::FileNotFoundException(PCChar function, UInt32 line, PCChar file, PCTChar path) :
		ExceptionWithCode(function, line, file, ERROR_FILE_NOT_FOUND, CreateErrorMessage(path)),
		m_path(path)
	{
	}

	String FileNotFoundException::CreateErrorMessage(PCTChar path)
	{
		return String::FormatS(Text("File not found: %s"), path);
	}

	//
	// Position is out of range
	//

	OutOfRangeException::OutOfRangeException(PCChar function, UInt32 line, PCChar file) :
		Exception(function, line, file, Text("Position is out of range"))
	{
	}

	//
	// Handle is invalid
	//

	InvalidHandleException::InvalidHandleException(PCChar function, UInt32 line, PCChar file) :
		Exception(function, line, file, Text("Handle is invalid"))
	{
	}

	//
	// Not PE image
	//

	BadImageFormatException::BadImageFormatException(PCChar function, UInt32 line, PCChar file) :
		Exception(function, line, file, Text("Not PE image"))
	{
	}

	//
	// Unsupported type of relocation
	//

	UnsupportedRelocationTypeException::UnsupportedRelocationTypeException(PCChar function, UInt32 line, PCChar file) :
		Exception(function, line, file, Text("Unsupported type of relocation"))
	{
	}

	//
	// Section was paged out
	//

	SectionPagedOutException::SectionPagedOutException(PCChar function, UInt32 line, PCChar file) :
		Exception(function, line, file, Text("Section was paged out"))
	{
	}

	//
	// Task canceled
	//

	TaskCanceledException::TaskCanceledException(PCChar function, UInt32 line, PCChar file) :
		Exception(function, line, file, Text("Task canceled"))
	{
	}
}