#pragma once

#include <list>
#include <functional>
#include <string>
#include <windows.h>
#include "xbyak/xbyak.h"

template <class T, class TA1, class TA2, class TA3, class TA4, class TR>
struct StdCallThunk : public Xbyak::CodeGenerator {
    void init(T *t, TR(T::*p)(TA1, TA2, TA3, TA4)) {
        mov(ecx, reinterpret_cast<intptr_t>(t));
        jmp(*reinterpret_cast<void **>(&p));
    }
};

typedef std::basic_string<TCHAR> tstring;

class Window {
protected:
    HINSTANCE hInst;
    tstring windowClass;
    StdCallThunk<Window, HWND, UINT, WPARAM, LPARAM, LRESULT> wndProc;
    void OnMouseDown(int button, WPARAM wParam, LPARAM lParam);
    void OnMouseUp(int button, WPARAM wParam, LPARAM lParam);

public:
    HWND hWnd;
    ATOM MyRegisterClass(HINSTANCE hInstance, const tstring &windowClass);
    BOOL InitInstance(const tstring &title, int nCmdShow);
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    std::list<std::function<bool(int, int)>> Command;
    std::list<std::function<void(HDC)>> Paint;
    std::list<std::function<void(int, int, int, WPARAM)>> MouseDown, MouseUp;
    std::list<std::function<void(int, int, WPARAM)>> MouseMove;
};
