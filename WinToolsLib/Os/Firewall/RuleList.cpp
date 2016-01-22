#include "RuleList.h"
#include "..\..\Exception.h"
#include "..\..\Com\Variant.h"
#include "..\..\Com\BStr.h"

namespace WinToolsLib { namespace Os { namespace Firewall
{
	RuleList::RuleList(Com::Ptr<INetFwRules>&& rules)
	{
		m_rules = std::move(rules);
	}

	RuleList::RuleList(RuleList&& other)
	{
		MoveFrom(other);
	}

	RuleList::~RuleList()
	{
	}

	RuleList& RuleList::operator=(RuleList&& other)
	{
		MoveFrom(other);
		return *this;
	}

	Bool RuleList::IsRuleExist(const WChar* name) const
	{
		Com::BStr bstrName(name);
		Com::Ptr<INetFwRule> rule;

		auto hr = m_rules->Item(bstrName, &rule);
		if (HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) == hr)
		{
			return False;
		}
		else if (SUCCEEDED(hr))
		{
			return True;
		}
		else
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Void RuleList::Add(Rule& rule)
	{
		auto hr = m_rules->Add(rule);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}
	}

	Rule RuleList::Item(const WChar* name) const
	{
		Com::BStr bstrName(name);
		Com::Ptr<INetFwRule> rule;

		auto hr = m_rules->Item(bstrName, &rule);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		return Rule(std::move(rule));
	}

	Void RuleList::EnumRules(EnumCallback callback)
	{
		Com::Ptr<IUnknown> newEnum;
		Com::Ptr<IEnumVARIANT> enumerator;

		auto hr = m_rules->get__NewEnum(&newEnum);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		hr = newEnum->QueryInterface(__uuidof(IEnumVARIANT), (PVoid*)&enumerator);
		if (FAILED(hr))
		{
			THROW_WIN32_EXCEPTION(hr);
		}

		Com::Variant item;
		ULONG fetched = 0;
		hr = enumerator->Next(1, &item, &fetched);
		while (hr != S_FALSE)
		{
			Com::Ptr<INetFwRule> rule;
			auto disp = item.ToIDispatch();
			hr = disp->QueryInterface(__uuidof(INetFwRule), reinterpret_cast<PVoid*>(&rule));
			if (SUCCEEDED(hr))
			{
				callback(Rule(std::move(rule)));
			}

			item.Clear();
			hr = enumerator->Next(1, &item, &fetched);
		}
	}

	Void RuleList::MoveFrom(RuleList& other)
	{
		if (this != &other)
		{
			m_rules = std::move(other.m_rules);
		}
	}
} } }