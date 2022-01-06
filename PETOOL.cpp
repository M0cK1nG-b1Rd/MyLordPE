// Demo3.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PETOOL.h"


BOOL CALLBACK DialogProc(
	HWND hwndDlg,  // handle to dialog box
	UINT uMsg,     // message
	WPARAM wParam, // first message parameter
	LPARAM lParam  // second message parameter
	)
{
	switch(uMsg)
	{
	case  WM_INITDIALOG :
		{
			InitProcessListView(hwndDlg);
			InitProcessModuleView(hwndDlg);
			return TRUE ;
		}

	case  WM_COMMAND :							
		{
			switch (LOWORD (wParam))						
			{						
			case   IDC_BUTTON_QUIT :
				EndDialog(hwndDlg, 0);					
				return TRUE;
			}
			return FALSE;
		}


	case WM_NOTIFY:
		{
			NMHDR* pNMHDR = (NMHDR*) lParam;
			if(wParam == IDC_LIST_PROCESS && pNMHDR->code == NM_CLICK){
				EnumModule(GetDlgItem(hwndDlg,IDC_LIST_PROCESS),wParam,lParam);
				return TRUE;
			}
			return FALSE;
		}

	case WM_CLOSE:
		{
			EndDialog(hwndDlg, 0);
			return TRUE;
		}

	}
	return FALSE ;
}

VOID EnumModule(HWND hListProcess, WPARAM wParam, LPARAM lParam){
	DWORD dwRowId;
	TCHAR szPid[0x20];
	LV_ITEM lv;

	// 初始化
	memset(szPid, 0 ,0x20);
	memset(&lv,0,1);
	// 获取选择行
	dwRowId = SendMessage(hListProcess, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (dwRowId == -1)
	{
		MessageBox(NULL, TEXT("请选择进程"), TEXT("出错啦"), MB_OK);
		return ;
	}
	// 获取PID
	lv.iSubItem = 1;
	lv.pszText = szPid;
	lv.cchTextMax = 0x20;
	SendMessage(hListProcess, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);
	// 根据PID调用API获取进程模块
}

VOID InitProcessListView(HWND hDlg){
	//1、初始化列名信息：
	LV_COLUMN lv;
	HWND hListProcess;

	//初始化
	memset(&lv,0,sizeof(LV_COLUMN));
	//获取IDC_LIST_PROCESS句柄
	hListProcess = GetDlgItem(hDlg,IDC_LIST_PROCESS);
	//设置整行选中
	SendMessage(hListProcess,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

	//第一列
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = TEXT("进程");//列标题
	lv.cx = 150;
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);
	SendMessage(hListProcess,LVM_INSERTCOLUMN,0,(DWORD)&lv);
	//第二列
	lv.pszText = TEXT("PID");
	lv.cx = 100;
	lv.iSubItem = 1;
	// 两个函数是一样的，ListView_InsertColumn是一个宏
	//ListView_InsertColumn(hListProcess, 1, &lv);
	SendMessage(hListProcess,LVM_INSERTCOLUMN,1,(DWORD)&lv);
	//第三列
	lv.pszText = TEXT("镜像基址");
	lv.cx = 100;
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcess, 2, &lv);
	//第四列
	lv.pszText = TEXT("镜像大小");
	lv.cx = 100;
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcess, 3, &lv);


	// 调用API获取所有的进程信息
	PROCESSENTRY32 pe32;
	// 在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32); 
	// 给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("无法获取进程信息"), TEXT("出错啦"), MB_OK);
	}
	
	// 遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = Process32First(hProcessSnap, &pe32);
	while(bMore)
	{
		// 插入信息到ListView中

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
 
	// 不要忘记清除掉snapshot对象
	::CloseHandle(hProcessSnap);
	getchar();

	//2、向列表中新增数据

	LV_ITEM vitem;

	//初始化
	memset(&vitem,0,sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	vitem.pszText = TEXT("csrss.exe");
	vitem.iItem = 0;
	vitem.iSubItem = 0;
	//ListView_InsertItem(hListProcess, &vitem);
	SendMessage(hListProcess, LVM_INSERTITEM,0,(DWORD)&vitem);

	vitem.pszText = TEXT("448");
	vitem.iItem = 0;
	vitem.iSubItem = 1;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = TEXT("56590000");
	vitem.iItem = 0;
	vitem.iSubItem = 2;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = TEXT("000F0000");
	vitem.iItem = 0;
	vitem.iSubItem = 3;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = TEXT("winlogon.exe");
	vitem.iItem = 1;
	vitem.iSubItem = 0;
	//ListView_InsertItem(hListProcess, &vitem);
	SendMessage(hListProcess, LVM_INSERTITEM,0,(DWORD)&vitem);

	vitem.pszText = TEXT("456");
	vitem.iSubItem = 1;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = TEXT("10000000");
	vitem.iSubItem = 2;
	ListView_SetItem(hListProcess, &vitem);

	vitem.pszText = TEXT("000045800");
	vitem.iSubItem = 3;
	ListView_SetItem(hListProcess, &vitem);


}


VOID InitProcessModuleView(HWND hDlg){
	//1、初始化列名信息：
	LV_COLUMN lv;
	HWND hModuleProcess;

	//初始化
	memset(&lv,0,sizeof(LV_COLUMN));
	//获取IDC_LIST_MODULE句柄
	hModuleProcess = GetDlgItem(hDlg,IDC_LIST_MODULE);
	//设置整行选中
	SendMessage(hModuleProcess,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

	//第一列
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = TEXT("模块名称");//列标题
	lv.cx = 150;
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);
	SendMessage(hModuleProcess,LVM_INSERTCOLUMN,0,(DWORD)&lv);
	//第二列
	lv.pszText = TEXT("模块位置");
	lv.cx = 100;
	lv.iSubItem = 1;
	// 两个函数是一样的，ListView_InsertColumn是一个宏
	//ListView_InsertColumn(hListProcess, 1, &lv);
	SendMessage(hModuleProcess,LVM_INSERTCOLUMN,1,(DWORD)&lv);




	//2、向列表中新增数据

	LV_ITEM vitem;

	//初始化
	memset(&vitem,0,sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	vitem.pszText = TEXT("csrss.exe");
	vitem.iItem = 0;
	vitem.iSubItem = 0;
	//ListView_InsertItem(hListProcess, &vitem);
	SendMessage(hModuleProcess, LVM_INSERTITEM,0,(DWORD)&vitem);

	vitem.pszText = TEXT("448");
	vitem.iItem = 0;
	vitem.iSubItem = 1;
	ListView_SetItem(hModuleProcess, &vitem);


}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	// 通用控件初始化
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	// 可以只引入当前需要的通用控件，ICC_WIN95_CLASSES指示引入常用的通用控件
	icex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);



	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL,DialogProc);
}

