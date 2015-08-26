// Metronome.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Main.h"
#include "Metronome.h"

#define MAX_LOADSTRING 100

// Operation to be performed on the two given integers.
enum operation { ADD, SUB, MULT, REM };

// Global Variables
operation op = ADD;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_METRONOME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_METRONOME));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_METRONOME));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= CreateSolidBrush(RGB(180, 180, 180));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_METRONOME);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 400, 300, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   // Initialize metronome
   Metronome::getInstance().Initialize();

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		// Create the LHS edit box
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("0"),
			WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
			20, 10, 85, 25, hWnd, (HMENU)IDC_LHS, GetModuleHandle(NULL), NULL);

		// Create the RHS edit box
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("0"),
			WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
			20, 45, 85, 25, hWnd, (HMENU)IDC_RHS, GetModuleHandle(NULL), NULL);

		// Create the Result edit box
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("0"),
			WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
			20, 85, 85, 25, hWnd, (HMENU)IDC_RESULT, GetModuleHandle(NULL), NULL);

		// Create the Operation button
		CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("+"),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP,
			115, 45, 30, 25, hWnd, (HMENU)IDC_OPBUTTON, GetModuleHandle(NULL), NULL);

		// Create the Start button
		CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("START"),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP,
			140, 70, 50, 40, hWnd, (HMENU)IDC_STARTBUTTON, GetModuleHandle(NULL), NULL);

		// Create the End button
		CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("END"),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP,
			140, 110, 50, 40, hWnd, (HMENU)IDC_STOPBUTTON, GetModuleHandle(NULL), NULL);

		CreateWindowEx(WS_EX_STATICEDGE, TEXT("COMBOBOX"), TEXT("MyCombo1"),
			CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
			0, 0, 100, 20, hWnd, (HMENU)130, GetModuleHandle(NULL), NULL); // 100 = ID of this control

		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case IDC_STARTBUTTON:
				Metronome::getInstance().Start();
				break;
			case IDC_STOPBUTTON:
				Metronome::getInstance().Stop();
				break;
			case IDM_ADD:
				SetDlgItemText(hWnd, IDC_OPBUTTON, TEXT("+"));
				op = ADD;
				break;
			case IDM_SUB:
				SetDlgItemText(hWnd, IDC_OPBUTTON, TEXT("-"));
				op = SUB;
				break;
			case IDM_MULT:
				SetDlgItemText(hWnd, IDC_OPBUTTON, TEXT("x"));
				op = MULT;
				break;
			case IDM_REM:
				SetDlgItemText(hWnd, IDC_OPBUTTON, TEXT("%"));
				op = REM;
				break;
			case IDC_OPBUTTON:{
				BOOL success = false;
				int lhs = GetDlgItemInt(hWnd, IDC_LHS, &success, true);
				if (!success)
				{
					MessageBox(hWnd, TEXT("The first expression is not an integer."), TEXT("Error"), MB_OK);
					break;
				}

				int rhs = GetDlgItemInt(hWnd, IDC_RHS, &success, true);
				if (!success)
				{
					MessageBox(hWnd, TEXT("The second expression is not an integer."), TEXT("Error"), MB_OK);
					break;
				}

				int result = 0;
				switch (op)
				{
				case ADD:
					result = lhs + rhs;
					break;
				case SUB:
					result = lhs - rhs;
					break;
				case MULT:
					result = lhs * rhs;
					break;
				case REM:
					if (rhs == 0) {
						SetDlgItemText(hWnd, IDC_RESULT, TEXT("Undefined"));
						return 0;
					}
					else
					{
						result = lhs % rhs;
					}
				}
				SetDlgItemInt(hWnd, IDC_RESULT, result, true);
				break;
			}
		}
		
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		MoveToEx(hdc, 10, 78, NULL);
		LineTo(hdc, 115, 78);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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
