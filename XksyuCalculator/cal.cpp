#include "lib.h"
#include <math.h>

INT store = 0, read = 0; WCHAR op; BOOL once_op = FALSE, once_op_temp = FALSE;
extern HWND hPutMain, hPutLite;
BOOL divZero = FALSE;

static INT cal(INT a, INT b, WCHAR op)
{
	switch (op)
	{
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		if (b == 0)
		{
			divZero = TRUE;
			return 0;
		}
		return a / b;
	case '^':
		return pow(a, b);
	}
}

static VOID update_cal()
{
	if (once_op_temp || !once_op)
	{
		std::wstring str = std::to_wstring(read);
		LPCWSTR output = str.c_str();
		SendMessage(hPutLite, WM_SETTEXT, FALSE, (LPARAM)output);
	}
	else
	{
		std::wstring str = std::to_wstring(cal(store, read, op));
		LPCWSTR output = str.c_str();
		SendMessage(hPutLite, WM_SETTEXT, FALSE, (LPARAM)output);
	}
}

VOID clear()
{
	store = 0; read = 0;
	once_op = FALSE; once_op_temp = FALSE;
	LPCWSTR output = L"0";
	SendMessage(hPutMain, WM_SETTEXT, FALSE, (LPARAM)output);
	SendMessage(hPutLite, WM_SETTEXT, FALSE, (LPARAM)output);
	divZero = FALSE;
}

VOID update_equal()
{
	if (divZero)
	{
		MessageBox(
			hWndMain, L"cannot divide zero",
			MB_OK, NULL
		);
		clear();
		return;
	}
	update_cal();
	read = cal(store, read, op);
	std::wstring str = std::to_wstring(read);
	LPCWSTR output = str.c_str();
	SendMessage(hPutMain, WM_SETTEXT, FALSE, (LPARAM)output);
	once_op = FALSE;
	once_op_temp = FALSE;
}

static BOOL update_equal_fromOP()
{
	if (divZero)
	{
		MessageBox(
			hWndMain, L"cannot divide zero",
			MB_OK, NULL
		);
		clear();
		return FALSE;
	}
	update_cal();
	store = cal(store, read, op);
	std::wstring str = std::to_wstring(store);
	LPCWSTR output = str.c_str();
	SendMessage(hPutMain, WM_SETTEXT, FALSE, (LPARAM)output);
	read = 0;
	return TRUE;
}

static BOOL update_input(LPCWSTR x)
{
	INT length = GetWindowTextLength(hPutMain);
	LPWSTR buffer = (LPWSTR)malloc((length + 1) * sizeof(WCHAR));
	SendMessage(hPutMain, WM_GETTEXT, 100, (LPARAM)buffer);

	BOOL is_op = !(x[0] != '+' && x[0] != '-' && x[0] != '*' && x[0] != '/' && x[0] != '^');

	if (is_op)
	{
		if (!once_op)
		{
			store = read;
			read = 0;
		}
		else if (once_op_temp)
			return FALSE;
		else
			if (!update_equal_fromOP())
				return FALSE;
		op = x[0];
		once_op = TRUE;
		once_op_temp = TRUE;
	}
	else
	{
		read = read * 10 + _wtoi(x);
		once_op_temp = FALSE;
	}

	length = GetWindowTextLength(hPutMain);
	if(buffer != NULL && length == 1 && buffer[0] == L'0' && !is_op)
		SendMessage(hPutMain, WM_SETTEXT, FALSE, (LPARAM)x);
	else
	{
		SendMessage(hPutMain, EM_SETSEL, length, length);
		SendMessage(hPutMain, EM_REPLACESEL, FALSE, (LPARAM)x);
	}
	return TRUE;
}

VOID update(LPCWSTR x)
{
	divZero = FALSE;
	if(update_input(x))
		update_cal();
}



VOID del()
{
	if (read == 0&& !once_op_temp)
		return;

	INT length = GetWindowTextLength(hPutMain);
	SendMessage(hPutMain, EM_SETSEL, length-1, length);
	SendMessage(hPutMain, EM_REPLACESEL, FALSE, NULL);

	LPWSTR buffer = (LPWSTR)malloc((length + 1) * sizeof(WCHAR));
	SendMessage(hPutMain, WM_GETTEXT, 100, (LPARAM)buffer);

	if (once_op_temp)
	{
		read = store;
		once_op = FALSE;
		once_op_temp = FALSE;
	}
	else
		read /= 10;
	update_cal();
}