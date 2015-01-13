#pragma once
#include <mutex>

namespace WinToolsLib
{
	template <class T>
	class Singleton
	{
		Singleton(const Singleton& other);            // Non-copyable
		Singleton& operator=(const Singleton& other); // Non-copyable
		
		Singleton(Singleton&& other);                 // Non-movable
		Singleton& operator=(Singleton&& other);      // Non-movable

	public:
		static T& Instance();

	protected:
		Singleton() {}

	private:
		static std::once_flag m_flag;
		static T* m_instancePtr;
	};

	template <class T>
	std::once_flag Singleton<T>::m_flag;

	template <class T>
	T* Singleton<T>::m_instancePtr = nullptr;

	template <class T>
	T& Singleton<T>::Instance()
	{
		std::call_once(m_flag, []() 
		{
			static T instance;
			Singleton<T>::m_instancePtr = &instance;
		});
		return *m_instancePtr;
	}
}