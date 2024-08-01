#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#ifdef _DEBUG
#pragma comment(lib, "opencv_world480d")
#else
#pragma comment(lib, "opencv_world480")
#endif
#include <windows.h>
#include "opencv2/opencv.hpp"

using namespace cv;

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton;
	static Mat src;
	switch (msg)
	{
	case WM_CREATE:
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("変換"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SIZE:
		MoveWindow(hButton, 10, 10, 256, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			src.release();
			Mat tmpl, mapCC;
			src = imread("src.png"); //入力画像
			//tmpl = imread("find2.jpg"); //探索画像（テンプレート）
			tmpl = imread("find.png"); //探索画像（テンプレート）
			//探索処理（テンプレートマッチング）
			matchTemplate(src, tmpl, mapCC, TM_CCOEFF_NORMED);
			double fMax; //相互相関の最大値
			Point pos; //探索結果の位置
			minMaxLoc(mapCC, NULL, &fMax, NULL, &pos); //相互相関マップの最大値検索
			//入力画像に探索結果の位置に四角を描画
			rectangle(src, Rect(pos.x, pos.y, tmpl.cols, tmpl.rows), Scalar(255, 255, 255), 2);
			InvalidateRect(hWnd, 0, TRUE);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			if (!src.empty()) {
				SetDIBitsToDevice(hdc, 0, 0, src.cols, src.rows, 0, 0, 0, src.rows, src.data, (BITMAPINFO*)&BITMAPINFO { { sizeof(BITMAPINFOHEADER), src.cols, -src.rows, 1, (WORD)(src.channels() * 8), BI_RGB, 0, 0, 0, 0, 0 }, { 0,0,0,0 } }, DIB_RGB_COLORS);
			}
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		src.release();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
