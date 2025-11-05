#include "lib.h"

VOID clear();
VOID update(LPCWSTR x);
VOID update_equal();
VOID del();

HWND hPutMain, hPutLite, hNumberButton[10], hSymbolButton[10], hEqualButton;

const INT posi_y[5] = {190,275,360,445,530}, posi_x[4] = {15,130,245,360};

constexpr static INT getNumID(INT x)
{
    return 2000 + x;
}

static VOID conInit_main_NumberButton(HWND hwnd, INT i, INT x, INT y, LPCWSTR text, HFONT hFont)
{
    INT id = 2000 + i;
    hNumberButton[i] = CreateWindow(
        L"BUTTON", text,
        WS_VISIBLE | WS_CHILD,
        x, y, 100, 70,
        hwnd,
        (HMENU)id, (HINSTANCE)GetWindowLongPtr(hPutLite, GWLP_HINSTANCE), NULL
    );
    SendMessage(hNumberButton[i], WM_SETFONT, (WPARAM)hFont, TRUE);
}

static VOID conInit_main_SymbolButton(HWND hwnd, INT i, INT x, INT y, LPCWSTR text, HFONT hFont)
{
    INT id = 3000 + i;
    hSymbolButton[i] = CreateWindow(
        L"BUTTON", text,
        WS_VISIBLE | WS_CHILD,
        x, y, 100, 70,
        hwnd,
        (HMENU)id, (HINSTANCE)GetWindowLongPtr(hPutLite, GWLP_HINSTANCE), NULL
    );
    SendMessage(hSymbolButton[i], WM_SETFONT, (WPARAM)hFont, TRUE);
}

static VOID conInit_main(HWND hwnd)
{
    HFONT hFontLarge = CreateFont(
        -65, 0, 0, 0,
        FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_MODERN,
        L"Î¢ÈíÑÅºÚ"
    );
    HFONT hFont = CreateFont(
        -40, 0, 0, 0,
        FALSE, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_MODERN,
        L"Î¢ÈíÑÅºÚ"
    );
    HFONT hFontLite = CreateFont(
        -35, 0, 0, 0,
        FALSE, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_MODERN,
        L"Î¢ÈíÑÅºÚ"
    );


    hPutMain = CreateWindow(
        L"EDIT", _T("0"),
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
        15, 15, 445, 100,
        hwnd,
        (HMENU)ID_PUT_MAIN, (HINSTANCE)GetWindowLongPtr(hPutMain, GWLP_HINSTANCE), NULL
    );
    SendMessage(hPutMain, WM_SETFONT, (WPARAM)hFontLarge, TRUE);

    hPutLite = CreateWindow(
        L"EDIT", _T("0"),
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
        15, 115, 445, 50,
        hwnd,
        (HMENU)ID_PUT_LITE, (HINSTANCE)GetWindowLongPtr(hPutLite, GWLP_HINSTANCE), NULL
    );
    SendMessage(hPutLite, WM_SETFONT, (WPARAM)hFontLite, TRUE);

    //200x
    conInit_main_NumberButton(hwnd, 7, posi_x[0], posi_y[1], _T("7"), hFont);
    conInit_main_NumberButton(hwnd, 8, posi_x[1], posi_y[1], _T("8"), hFont);
    conInit_main_NumberButton(hwnd, 9, posi_x[2], posi_y[1], _T("9"), hFont);
    conInit_main_NumberButton(hwnd, 4, posi_x[0], posi_y[2], _T("4"), hFont);
    conInit_main_NumberButton(hwnd, 5, posi_x[1], posi_y[2], _T("5"), hFont);
    conInit_main_NumberButton(hwnd, 6, posi_x[2], posi_y[2], _T("6"), hFont);
    conInit_main_NumberButton(hwnd, 1, posi_x[0], posi_y[3], _T("1"), hFont);
    conInit_main_NumberButton(hwnd, 2, posi_x[1], posi_y[3], _T("2"), hFont);
    conInit_main_NumberButton(hwnd, 3, posi_x[2], posi_y[3], _T("3"), hFont);
    conInit_main_NumberButton(hwnd, 0, posi_x[1], posi_y[4], _T("0"), hFont);
    
    //300x
    conInit_main_SymbolButton(hwnd, 1, posi_x[1], posi_y[0], _T("C"), hFont);
    conInit_main_SymbolButton(hwnd, 2, posi_x[2], posi_y[0], _T("Del"), hFont);
    conInit_main_SymbolButton(hwnd, 3, posi_x[3], posi_y[0], _T("/"), hFont);
    conInit_main_SymbolButton(hwnd, 4, posi_x[3], posi_y[1], _T("*"), hFont);
    conInit_main_SymbolButton(hwnd, 5, posi_x[3], posi_y[2], _T("-"), hFont);
    conInit_main_SymbolButton(hwnd, 6, posi_x[3], posi_y[3], _T("+"), hFont);

    hEqualButton = CreateWindow(
        L"BUTTON", L"=",
        WS_VISIBLE | WS_CHILD,
        posi_x[2], posi_y[4], 220, 70,
        hwnd,
        (HMENU)ID_OP_EQ, (HINSTANCE)GetWindowLongPtr(hPutLite, GWLP_HINSTANCE), NULL
    );
    SendMessage(hEqualButton, WM_SETFONT, (WPARAM)hFont, TRUE);

    conInit_main_SymbolButton(hwnd, 8, posi_x[0], posi_y[4], _T("."), hFont);
    conInit_main_SymbolButton(hwnd, 9, posi_x[0], posi_y[0], _T("^"), hFont);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        conInit_main(hwnd);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_COMMAND:
    {
        WORD wmId = LOWORD(wParam);
        switch (wmId)
        {
        case getNumID(1):
            update(L"1");
            break;
        case getNumID(2):
            update(L"2");
            break;
        case getNumID(3):
            update(L"3");
            break;
        case getNumID(4):
            update(L"4");
            break;
        case getNumID(5):
            update(L"5");
            break;
        case getNumID(6):
            update(L"6");
            break;
        case getNumID(7):
            update(L"7");
            break;
        case getNumID(8):
            update(L"8");
            break;
        case getNumID(9):
            update(L"9");
            break;
        case getNumID(0):
            update(L"0");
            break;
        case ID_OP_D:
            update(L"/");
            break;
        case ID_OP_T:
            update(L"*");
            break;
        case ID_OP_M:
            update(L"-");
            break;
        case ID_OP_P:
            update(L"+");
            break;
        case ID_OP_POW:
            update(L"^");
            break;
        case ID_OP_EQ:
            update_equal();
            break;
        case ID_OP_C:
            clear();
            break;
        case ID_OP_DEL:
            del();
            break;
        default:
            break;
        }
        SetFocus(hWndMain);
        break;
    }

    case WM_KEYDOWN:
    {
        if (wParam >= '0' && wParam <= '9')
            SendMessage(hNumberButton[wParam-'0'], BM_CLICK, 0, 0);
        else if(wParam >=VK_NUMPAD0 && wParam <=VK_NUMPAD9)
            SendMessage(hNumberButton[wParam-VK_NUMPAD0], BM_CLICK, 0, 0);
        else
        {
            switch (wParam)
            {
            case VK_BACK:
            case VK_DELETE:
                SendMessage(hSymbolButton[2], BM_CLICK, 0, 0);
                break;
            case VK_ESCAPE:
                SendMessage(hSymbolButton[1], BM_CLICK, 0, 0);
                break;
            case VK_ADD:
                SendMessage(hSymbolButton[6], BM_CLICK, 0, 0);
                break;
            case VK_MULTIPLY:
                SendMessage(hSymbolButton[4], BM_CLICK, 0, 0);
                break;
            case VK_SUBTRACT:
                SendMessage(hSymbolButton[5], BM_CLICK, 0, 0);
                break;
            case VK_DIVIDE:
                SendMessage(hSymbolButton[3], BM_CLICK, 0, 0);
                break;
            case VK_RETURN:
                SendMessage(hEqualButton, BM_CLICK, 0, 0);
                break;
            }
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}