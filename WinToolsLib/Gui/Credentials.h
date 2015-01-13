#pragma once
#include "..\Types.h"
#include "..\String.h"

#include <Windows.h>

namespace WinToolsLib { namespace Gui
{
	class Credentials
	{
	public:
		Credentials(
			const TChar* userName,
			const TChar* password);
		
		Credentials(Credentials&& other);
		Credentials(const Credentials& other);

		~Credentials();

		Credentials& operator=(Credentials&& other);
		Credentials& operator=(const Credentials& other);

		const String& GetUserName() const;
		const String& GetPassword() const;

		static Credentials Prompt(
			HWND parent,
			const TChar* message,
			const TChar* caption,
			const TChar* target);

	protected:
		Void MoveFrom(Credentials& other);
		Void CopyFrom(const Credentials& other);

	private:
		String m_userName;
		String m_password;
	};

	inline const String& Credentials::GetUserName() const
	{
		return m_userName;
	}

	inline const String& Credentials::GetPassword() const
	{
		return m_password;
	}

} }

