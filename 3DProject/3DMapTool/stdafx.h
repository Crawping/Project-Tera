// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows dependent ��� ����:
#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <CommCtrl.h>
#include <shellapi.h>

// C++ ��Ÿ�� ��� �����Դϴ�.
#include <cstdlib>
#include <memory>
#include <string>
#include <map>
#include <initializer_list>
#include <array>
#include <cstdint>
#include <vector>
#include <cassert>

// CommCtrl
#pragma comment( lib, "uxtheme" )
#pragma comment( lib, "comctl32" )

// D3DX
#pragma comment( lib, "d3d9.lib")
#ifdef _DEBUG
#	pragma comment( lib, "d3dx9d.lib")
#else
#	pragma comment( lib, "d3dx9.lib")
#endif


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

enum
{
	MainWindowWidth = 1000,
	MainWindowHeight = 1000,

	HierarchyWindowWidth = 300,
	HierarchyWindowHeight = MainWindowHeight/2,	// Follow parent's

	InspectorWindowWidth = HierarchyWindowWidth,
	InspectorWindowHeight = MainWindowHeight/2,
};


#define MAINWINDOW_TITLENAME L"3DMapTool"
#define HIERARCHYWINDOW_TITLENAME L"Hierarchy"
#define INSPECTORWINDOW_TITLENAME L"Inspector"


extern HINSTANCE g_instHandle;
extern HWND g_mainWndHandle;		// D3DX Surface window & Menu bar
extern HWND g_hierarchyWndHandle;	// Contains object hierarchy listbox
extern HWND g_inspectorWndHandle;