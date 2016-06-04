#include "stdafx.h"
#include "MainSurfaceWindow.h"

#include "resource.h"
#include "cDeviceManager.h"
#include "cGameObjectManager.h"
#include "ObjObject.h"
#include "LightObject.h"
#include "HierarchyWindow.h"
#include "cCamera.h"

namespace
{


enum ExtensionTable
{
	kX,
	kObj,
	kUnknown,
};

ExtensionTable AnalyzeExtension( 
	const std::string& filePath )
{
	int i = filePath.size( )-1; 
	while ( i >= 0 &&
		filePath[i] != '.' )
	{
		--i;
	}

	const std::string extension = 
		filePath.substr( i+1, filePath.size( ));

	if ( extension == "x" )
	{
		return ExtensionTable::kX;
	}
	else if ( extension == "obj" )
	{
		return ExtensionTable::kObj;
	}
	else
	{
		return ExtensionTable::kUnknown;
	}
}


}

HWND g_mainWndHandle;

MainSurfaceWindow::MainSurfaceWindow( ) :
	AbstractWindow(
		L"3DMapTool",
		WS_EX_ACCEPTFILES,
		WS_OVERLAPPEDWINDOW,
		nullptr,
		this->MakeWindowClass( ),
		GetSystemMetrics( SM_CXSCREEN )/2-(MainWindowWidth/2),
		GetSystemMetrics( SM_CYSCREEN )/2-(MainWindowHeight/2),
		MainWindowWidth,
		MainWindowHeight
	),
	m_dropQueryPath( new char[MAX_PATH] )
{
}

MainSurfaceWindow::~MainSurfaceWindow( )
{
	PostQuitMessage( 0 );
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT MainSurfaceWindow::MessageProc( 
	HWND wndHandle, 
	UINT msg, 
	WPARAM wParam, 
	LPARAM lParam )
{
	switch ( msg )
	{
	case WM_CREATE:
		{
			g_mainWndHandle = wndHandle;

			int x, y;
			this->GetPosition( &x, &y );
			m_prevPos = { x, y };
		}
		break;

	case WM_DROPFILES:
		{
			DragQueryFileA( reinterpret_cast<HDROP>( wParam ), 0, 
				&m_dropQueryPath[0], MAX_PATH );
			
			static int32_t hierarchyObjIndex = 0;
			ExtensionTable extension = AnalyzeExtension( 
				m_dropQueryPath.get( ));

			switch ( extension )
			{
			case ExtensionTable::kX:
				break;
			case ExtensionTable::kObj:
				{
					static int32_t createCount = 0;
					std::wstring str = L"object_";
					str += std::to_wstring( createCount++ );

					ObjObject* obj = new ObjObject( str.c_str(), m_dropQueryPath.get());
					static_cast<HierarchyWindow*>( this->GetChildByName( 
						L"Hierarchy" ))->AddListItem( str );
				}
				break;

			case ExtensionTable::kUnknown:
				MessageBox( wndHandle, L"Unknown file extension.", L"WARNING",
					MB_OK | MB_ICONEXCLAMATION );
				break;
			}
		}
		break;

	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다.
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(g_instHandle, MAKEINTRESOURCE(IDD_ABOUTBOX), wndHandle, About);
				break;
			case IDM_EXIT:
				DestroyWindow( wndHandle );
				break;
			}
		}
		break;

	case WM_MOVE:
		{
			int x, y;
			this->GetPosition( &x, &y );
			
			POINT currPos{ x, y };
			
			for ( auto& childElem : GetChildRepo( ) )
			{
				childElem->Move(
					( currPos.x-m_prevPos.x ),
					( currPos.y-m_prevPos.y )
				);
			}

			m_prevPos = currPos;
		}
		break;
	}

	return DefWindowProc( wndHandle, msg, wParam, lParam );
}

void MainSurfaceWindow::OnIdle( )
{
	g_pD3DDevice->SetRenderState(
		D3DRENDERSTATETYPE::D3DRS_LIGHTING,
		TRUE
	);

	cGameObjectManager::Get( )->Update( );
	cGameObjectManager::Get( )->Render( );
}

void MainSurfaceWindow::OnMove( 
	AbstractWindow* sender, 
	int x, 
	int y )
{
	sender->Move( x, y );
}

void MainSurfaceWindow::OnSize( 
	AbstractWindow* sender, 
	int width, 
	int height )
{
}

WNDCLASSEXW MainSurfaceWindow::MakeWindowClass( )
{
	WNDCLASSEXW wcex {0};
	HINSTANCE instanceHandle = GetModuleHandle( nullptr );

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = nullptr; // Ignore, using member function
	wcex.hInstance = instanceHandle;
	wcex.hIcon = LoadIcon( g_instHandle, MAKEINTRESOURCE( IDI_MY3DMAPTOOL ));
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW+1 );
	wcex.lpszMenuName = MAKEINTRESOURCEW( IDC_MY3DMAPTOOL );
	wcex.lpszClassName = L"MainSurfaceWindowClass";
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ));

	return wcex;
}