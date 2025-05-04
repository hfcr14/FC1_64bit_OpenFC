// XTDelegate.h : header file
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#if !defined(__XTDELEGATE_H__)
#define __XTDELEGATE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Summary: A base class for delegates defined here
template <class TCallback>
class CXTDelegateBase
{
public:
	using CB = TCallback;

protected:

    CB       m_cb;     // A callback.
    CObject* m_target; // A target on which to invoke the callback.

public:
	CXTDelegateBase(CObject* target, CB cb)
	: m_target(target)
	, m_cb(cb)
	{	}

	operator bool () const
	{
		return m_target != 0 && m_cb != 0;
	}

	template<typename F>
	void Set(CObject* target, F cb)
	{
		m_target = target;
		m_cb = (CB)cb;
	}

	bool Remove(CObject* target)
	{
		bool removed = m_target == target;
		if (removed)
		{
			m_target = 0;
			m_cb = 0;
		}
		return removed;
	}

	bool Remove(CObject* target, CB cb)
	{
		bool removed = (m_target == target) && (m_cb == cb);
		if (removed)
		{
			m_target = 0;
			m_cb = 0;
		}
		return removed;
	}

};

// General delegate template for any number of parameters and typed return.
template <typename TReturn, typename... TParams>
class CXTDelegate : public CXTDelegateBase<TReturn(CObject::*)(TParams...)>
{
public:
	using CB = TReturn(CObject::*)(TParams...);

	CXTDelegate()
		: CXTDelegateBase<CB>(nullptr, nullptr)
	{
	}

	template<typename F>
	CXTDelegate(CObject* target = nullptr, F cb = nullptr)
		: CXTDelegateBase<CB>(target,(CB)cb)
	{}

	TReturn operator() (TParams... params) const
	{
		if constexpr (std::is_void<TReturn>())
		{
			(m_target->*m_cb)(std::forward<TParams>(params)...);
		}
		else
		{
			return (m_target->*m_cb)(std::forward<TParams>(params)...);
		}
	}
};

// Summary: A multicast delegate.
template <typename... TParams>
class CXTMultiCastDelegate
	: public CArray<CXTDelegate<void, TParams...>, const CXTDelegate<void, TParams...>&>
{
public:
#if _MSC_VER < 1200 // MFC 5.0
	typedef TDelegate DELEGATE;
	typedef DELEGATE::CB CB;
#else
	using DELEGATE = CXTDelegate<void, TParams...>;
	using ARRAY = CArray<DELEGATE, const DELEGATE&>;
	using CB = typename DELEGATE::CB;
#endif

	void operator() (TParams... params) const
	{
		for (int i = 0; i < GetSize(); i++)
		{
			const DELEGATE& delegate = ElementAt(i);
			delegate(std::forward<TParams>(params)...);
		}
 	}

	// Input:	target - Points to a CObject object.
	// Summary:	Removes all registrations for a target.
	void Remove(CObject* target)
	{
		for (int i = GetUpperBound(); i >= 0; i--)
		{
			if (ElementAt(i).Remove(target))
			{
				RemoveAt(i);
			}
		}
		ReleaseMemory();
	}

	// Summary: Frees up the memory once there is nothing in the array.
	void ReleaseMemory()
	{
		if (GetSize() == 0)
		{
			FreeExtra();
		}
	}


	// Input:	target - Points to a CObject object.
	//			cb - delegate
	// Summary:	removes all registrations for a target.
	template<typename F>
	void Remove(CObject* target, F cb)
	{
		for (INT_PTR i = GetUpperBound(); i >= 0; i--)
		{
			if (ElementAt(i).Remove(target, (CB)cb))
			{
				RemoveAt(i);
			}
		}
		ReleaseMemory();
	}

	// Input:	target - Pointst to a CObject object.
	//			cb - delegate.
	// Returns:	Index into array where callback was added.
	// Summary:	Adds new callback.
	template<typename F>
	int Add(CObject* target, F cb)
	{
		return ARRAY::Add(DELEGATE(target, (CB)cb));
	}
};

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#endif 
// __XTDELEGATE_H__
