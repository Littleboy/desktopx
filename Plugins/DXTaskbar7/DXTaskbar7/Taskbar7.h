///////////////////////////////////////////////////////////////////////////////////////////////
//
// DXTaskbar7 - Extended Taskbar Support for Windows 7
//
// Copyright (c) 2009-2011, Julien Templier
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////
// * $LastChangedRevision$
// * $LastChangedDate$
// * $LastChangedBy$
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

#pragma once

#include "stdafx.h"
#include <comsvcs.h>

#include "COMError.h"
#include "DXTaskbar7.h"
#include "Taskbar7Errors.h"
#include "resource.h"

// Header Files for Jump List features
#include <objectarray.h>
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>
#include <shlobj.h>

#define DESTINATION_TASKS    L"Tasks"
#define DESTINATION_FREQUENT L"DESTINATION_FREQUENT"
#define DESTINATION_RECENT   L"DESTINATION_RECENT"

#include <gdiplus.h>
using namespace Gdiplus;

#include <string>
#include <vector>
#include <map>
using namespace std;

// CAeroColor
class ATL_NO_VTABLE CTaskbar7 :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTaskbar7, &CLSID_Taskbar7>,
	public IDispatchImpl<ITaskbar7, &IID_ITaskbar7, &LIBID_DXTaskbar7Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public ISupportErrorInfo
{
public:
	CTaskbar7() {}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

		m_pTaskbarList = NULL;
		m_pCustomDestinationList = NULL;
		m_pApplicationDestinations = NULL;
		m_hwnd = NULL;
		m_objectID = NULL;
		m_processID = NULL;
		m_parentHwnd = NULL;

		m_isTabRegistered = false;
		m_isWindows7 = false;

		// Check that we are running on Windows 7
		OSVERSIONINFO versionInfo;
		versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		GetVersionEx(&versionInfo);
		if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 1)
			m_isWindows7 = true;

		return S_OK;
	}

	void FinalRelease()
	{
		GdiplusShutdown(m_gdiplusToken);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TASKBAR7)

DECLARE_NOT_AGGREGATABLE(CTaskbar7)

BEGIN_COM_MAP(CTaskbar7)
	COM_INTERFACE_ENTRY(ITaskbar7)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	private:
		enum Category {
			Task = 1,
			Custom = 2,
			Separator = 3,
			Frequent = 4,
			Recent = 5
		};

		struct ThumbButton
		{
			int id;
			wstring image;
			wstring tooltip;
			int flags;
		};

		struct Destination
		{
			Category type;
			wstring name;
			wstring path;
			wstring arguments;
			wstring icon;
			int iconIndex;
			wstring workingFolder;

			Destination() {};
			Destination(Category type, wstring name, wstring path, wstring arguments, wstring icon, int iconIndex, wstring workingFolder) :
						type(type),
						name(name),
						path(path),
						arguments(arguments),
						icon(icon),
						iconIndex(iconIndex),
						workingFolder(workingFolder) {};
			~Destination() {};
		};

		ULONG_PTR m_gdiplusToken;
		GdiplusStartupInput m_gdiplusStartupInput;

		// The COM instances
		ITaskbarList3* m_pTaskbarList;
		ICustomDestinationList* m_pCustomDestinationList;
		IApplicationDestinations* m_pApplicationDestinations;

		// Handle to the DX window
		HWND m_hwnd;
		DWORD m_objectID;

		// Main process window (the one with the taskbar icon)
		HWND m_parentHwnd;
		DWORD m_processID;

		bool m_isWindows7;
		bool m_isTabRegistered;
		wstring m_appID;

		// The list of thumbbar buttons to add
		vector<ThumbButton> m_buttons;
		map<wstring, vector<Destination>> destinations;

		bool IsWindows7();

		void GetMainWindowHandle();
		static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
		static LRESULT CALLBACK WindowProc(int nCode, WPARAM wParam, LPARAM lParam);

		void RegisterTab();

		void AddCustomDestination(Category type, wstring name, wstring category, wstring path, wstring arguments, wstring icon, int iconIndex, wstring workingFolder);
		HRESULT CreateShellLink(Destination destination, IShellLink **ppShellLink);
		HRESULT CreateSeparatorLink(IShellLink **ppShellLink);

		HRESULT LoadImageFromFile(wstring path, Bitmap** bitmap);
		HRESULT LoadButton(int id, wstring path, wstring tooltip, int flags, THUMBBUTTON* button);
	public:

		// Used by DXTaskbar
		void SetHWND(HWND hwnd) { m_hwnd = hwnd; }
		HWND GetHWND() { return m_hwnd; }
		HWND GetParentHWND() { return m_parentHwnd; }
		void SetObjectID(DWORD id) { m_objectID = id; }
		void Init();
		void Cleanup();

		// Internal use by static methods
		void SetMainHwnd(HWND hwnd) { m_parentHwnd = hwnd; }
		DWORD GetProcessID() { return m_processID; }
		void HookMainWindowMessages();

		//////////////////////////////////////////////////////////////////////////
		// ISupportErrorInfo
		//////////////////////////////////////////////////////////////////////////
		STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

		//////////////////////////////////////////////////////////////////////////
		// ITaskbar7
		//////////////////////////////////////////////////////////////////////////
		STDMETHOD(get_TabHwnd)(LONG* hwnd);
		STDMETHOD(SetTabsIcon)(BSTR icon);
		STDMETHOD(ConfigureTab)(BSTR name, LONG after);
		STDMETHOD(SetTabActive)();
		STDMETHOD(RemoveTab)();

		// ThumbBar
		STDMETHOD(SetupButton)(int id, BSTR image, BSTR tooltip, int flags);
		STDMETHOD(UpdateButton)(int id, BSTR image, BSTR tooltip, int flags);
		STDMETHOD(AddButtons)();

		// Overlay
		STDMETHOD(SetOverlayIcon)(BSTR path, BSTR description);

		// Progress
		STDMETHOD(SetProgressState)(int flag);
		STDMETHOD(SetProgressValue)(ULONGLONG ullCompleted, ULONGLONG ullTotal);

		// Tasks and destinations
		STDMETHOD(RemoveAllDestinations)();
		STDMETHOD(DeleteList)(BSTR appID);

		STDMETHOD(CommitList)();
		STDMETHOD(AbortList)();
		STDMETHOD(SetAppID)(BSTR appID);
		STDMETHOD(AddUserTask)(BSTR name, BSTR path, BSTR arguments, BSTR icon, int iconIndex, BSTR workingFolder);
		STDMETHOD(AddDestination)(BSTR category, BSTR name, BSTR path, BSTR arguments, BSTR icon, int iconIndex, BSTR workingFolder);
		STDMETHOD(AddSeparator)(BSTR category);
		STDMETHOD(AppendKnownCategory)(int knownDestCategory);

		// Recent
		STDMETHOD(AddRecent)(BSTR name, BSTR path, BSTR arguments, BSTR icon);
};

OBJECT_ENTRY_AUTO(__uuidof(Taskbar7), CTaskbar7)
