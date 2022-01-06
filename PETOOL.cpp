// Demo3.cpp : ����Ӧ�ó������ڵ㡣
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

	// ��ʼ��
	memset(szPid, 0 ,0x20);
	memset(&lv,0,1);
	// ��ȡѡ����
	dwRowId = SendMessage(hListProcess, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (dwRowId == -1)
	{
		MessageBox(NULL, TEXT("��ѡ�����"), TEXT("������"), MB_OK);
		return ;
	}
	// ��ȡPID
	lv.iSubItem = 1;
	lv.pszText = szPid;
	lv.cchTextMax = 0x20;
	SendMessage(hListProcess, LVM_GETITEMTEXT, dwRowId, (DWORD)&lv);
	// ����PID����API��ȡ����ģ��
}

VOID InitProcessListView(HWND hDlg){
	//1����ʼ��������Ϣ��
	LV_COLUMN lv;
	HWND hListProcess;

	//��ʼ��
	memset(&lv,0,sizeof(LV_COLUMN));
	//��ȡIDC_LIST_PROCESS���
	hListProcess = GetDlgItem(hDlg,IDC_LIST_PROCESS);
	//��������ѡ��
	SendMessage(hListProcess,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

	//��һ��
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = TEXT("����");//�б���
	lv.cx = 150;
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);
	SendMessage(hListProcess,LVM_INSERTCOLUMN,0,(DWORD)&lv);
	//�ڶ���
	lv.pszText = TEXT("PID");
	lv.cx = 100;
	lv.iSubItem = 1;
	// ����������һ���ģ�ListView_InsertColumn��һ����
	//ListView_InsertColumn(hListProcess, 1, &lv);
	SendMessage(hListProcess,LVM_INSERTCOLUMN,1,(DWORD)&lv);
	//������
	lv.pszText = TEXT("�����ַ");
	lv.cx = 100;
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcess, 2, &lv);
	//������
	lv.pszText = TEXT("�����С");
	lv.cx = 100;
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcess, 3, &lv);


	// ����API��ȡ���еĽ�����Ϣ
	PROCESSENTRY32 pe32;
	// ��ʹ������ṹ֮ǰ�����������Ĵ�С
	pe32.dwSize = sizeof(pe32); 
	// ��ϵͳ�ڵ����н�����һ������
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("�޷���ȡ������Ϣ"), TEXT("������"), MB_OK);
	}
	
	// �������̿��գ�������ʾÿ�����̵���Ϣ
	BOOL bMore = Process32First(hProcessSnap, &pe32);
	while(bMore)
	{
		// ������Ϣ��ListView��

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
 
	// ��Ҫ���������snapshot����
	::CloseHandle(hProcessSnap);
	getchar();

	//2�����б�����������

	LV_ITEM vitem;

	//��ʼ��
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
	//1����ʼ��������Ϣ��
	LV_COLUMN lv;
	HWND hModuleProcess;

	//��ʼ��
	memset(&lv,0,sizeof(LV_COLUMN));
	//��ȡIDC_LIST_MODULE���
	hModuleProcess = GetDlgItem(hDlg,IDC_LIST_MODULE);
	//��������ѡ��
	SendMessage(hModuleProcess,LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

	//��һ��
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = TEXT("ģ������");//�б���
	lv.cx = 150;
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);
	SendMessage(hModuleProcess,LVM_INSERTCOLUMN,0,(DWORD)&lv);
	//�ڶ���
	lv.pszText = TEXT("ģ��λ��");
	lv.cx = 100;
	lv.iSubItem = 1;
	// ����������һ���ģ�ListView_InsertColumn��һ����
	//ListView_InsertColumn(hListProcess, 1, &lv);
	SendMessage(hModuleProcess,LVM_INSERTCOLUMN,1,(DWORD)&lv);




	//2�����б�����������

	LV_ITEM vitem;

	//��ʼ��
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
	// ͨ�ÿؼ���ʼ��
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	// ����ֻ���뵱ǰ��Ҫ��ͨ�ÿؼ���ICC_WIN95_CLASSESָʾ���볣�õ�ͨ�ÿؼ�
	icex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);



	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL,DialogProc);
}

