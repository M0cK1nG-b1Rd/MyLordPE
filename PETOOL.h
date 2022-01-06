#pragma once

#include "resource.h"


VOID InitProcessListView(HWND hDlg);
VOID InitProcessModuleView(HWND hDlg);
VOID EnumModule(HWND hListProcess, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK DialogProc(
	HWND hwndDlg,  // handle to dialog box
	UINT uMsg,     // message
	WPARAM wParam, // first message parameter
	LPARAM lParam  // second message parameter
	);