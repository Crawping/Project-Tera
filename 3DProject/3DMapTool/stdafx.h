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

#pragma comment( lib, "d3d9.lib")
#ifdef _DEBUG
#	pragma comment( lib, "d3dx9d.lib")
#else
#	pragma comment( lib, "d3dx9.lib")
#endif


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

enum
{
	MainWindowX = 0, // Not used ( because of middle showing )
	MainWindowY = 0, // Not used ( because of middle showing )
	MainWindowWidth = 800,
	MainWindowHeight = 800,

	HierarchyWindowX = MainWindowWidth, // Follow parent's
	HierarchyWindowY = MainWindowY, // Follow parent's
	HierarchyWindowWidth = HierarchyWindowX+200,
	HierarchyWindowHeight = HierarchyWindowY+(MainWindowHeight/2),	// Follow parent's

	InspectorWindowX = 0,
	InspectorWindowY = 0,
	InspectorWindowWidth = 0,
	InspectorWindowHeight = 0,
};


#define MAINWINDOW_TITLENAME L"3DMapTool"
#define HIERARCHYWINDOW_TITLENAME L"Hierarchy"
#define INSPECTORWINDOW_TITLENAME L"Inspector"


extern HINSTANCE g_instHandle;
extern HWND g_mainWndHandle;		// D3DX Surface window & Menu bar
extern HWND g_hierarchyWndHandle;	// Contains object hierarchy listbox
extern HWND g_inspectorWndHandle;