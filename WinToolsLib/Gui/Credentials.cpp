#include "Credentials.h"
#include "..\Exception.h"

#include <WinCred.h>
#pragma comment(lib, "Credui.lib")

namespace WinToolsLib { namespace Gui
{
	Credentials::Credentials(
		const TChar* userName,
		const TChar* password) :
		m_userName(userName ? userName : Text("")),
		m_password(password ? password : Text(""))
	{
	}

	Credentials::Credentials(Credentials&& other)
	{
		MoveFrom(other);
	}

	Credentials::Credentials(const Credentials& other)
	{
		CopyFrom(other);
	}

	Credentials::~Credentials()
	{
		::SecureZeroMemory(m_password.GetBuffer(), m_password.GetLength() * sizeof(TChar));
	}

	Credentials& Credentials::operator=(Credentials&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Credentials& Credentials::operator=(const Credentials& other)
	{
		CopyFrom(other);
		return *this;
	}

	Credentials Credentials::Prompt(
		HWND parent,
		const TChar* message,
		const TChar* caption,
		const TChar* target)
	{
		CREDUI_INFO info = {0};
		info.cbSize = sizeof(info);
		info.hwndParent = parent;
		info.pszMessageText = message;
		info.pszCaptionText = caption;

		TChar userName[CREDUI_MAX_USERNAME_LENGTH + 1] = {0};
		TChar password[CREDUI_MAX_PASSWORD_LENGTH + 1] = {0};

		const auto error = ::CredUIPromptForCredentials(
			&info,
			target,
			nullptr,
			0,
			userName,
			CREDUI_MAX_USERNAME_LENGTH,
			password,
			CREDUI_MAX_PASSWORD_LENGTH,
			nullptr,
			CREDUI_FLAGS_VALIDATE_USERNAME);
		
		if (NO_ERROR != error)
		{
			THROW_WIN32_EXCEPTION(error);
		}

		Credentials credentials(userName, password);
		::SecureZeroMemory(password, sizeof(password));
		return credentials;
	}

	Void Credentials::MoveFrom(Credentials& other)
	{
		if (&other != this)
		{
			m_userName = std::move(other.m_userName);
			m_password = std::move(other.m_password);
		}
	}

	Void Credentials::CopyFrom(const Credentials& other)
	{
		if (&other != this)
		{
			m_userName = other.m_userName;
			m_password = other.m_password;
		}
	}

} }