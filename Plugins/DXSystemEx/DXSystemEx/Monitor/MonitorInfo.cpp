///////////////////////////////////////////////////////////////////////////////////////////////
//
// DXSystemEx - Extended System Information
//
// Copyright (c) 2009-2011, Julien Templier
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. The name of the author may not be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
//  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
//  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonitorInfo.h"

void CMonitorInfo::Init(const pair<RECT, bool> &info)
{
	m_rect.top = info.first.top;
	m_rect.left = info.first.left;
	m_rect.bottom = info.first.bottom;
	m_rect.right = info.first.right;

	m_primary = info.second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CMonitorInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] =
	{
		&IID_IMonitorInfo
	};

	for (unsigned int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ISystemEx
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CMonitorInfo::get_IsPrimary(VARIANT_BOOL* isPrimary)
{
	if (m_primary)
		*isPrimary = VARIANT_TRUE;
	else
		*isPrimary = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CMonitorInfo::get_Left(int* monitorLeft)
{
	*monitorLeft = m_rect.left;

	return S_OK;
}

STDMETHODIMP CMonitorInfo::get_Top(int* monitorTop)
{
	*monitorTop = m_rect.top;

	return S_OK;
}

STDMETHODIMP CMonitorInfo::get_Bottom(int* monitorBottom)
{
	*monitorBottom = m_rect.bottom;

	return S_OK;
}

STDMETHODIMP CMonitorInfo::get_Right(int* monitorRight)
{
	*monitorRight = m_rect.right;

	return S_OK;
}

STDMETHODIMP CMonitorInfo::get_Width(int* width)
{
	*width = abs(m_rect.right - m_rect.left);

	return S_OK;
}

STDMETHODIMP CMonitorInfo::get_Height(int* height)
{
	*height = abs(m_rect.bottom - m_rect.top);

	return S_OK;
}