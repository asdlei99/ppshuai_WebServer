// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnTrayIcon(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == IDI_NOTIFYICON)//Ϊ����������ͼ��
	{
		switch (lParam)//��Ϣ������
		{
		case WM_LBUTTONDBLCLK://˫�����
							  //ʵϰ���ڵ����ػ���ʾ
			ShowWindow(SW_SHOW);//���ش���
			SetForegroundWindow(m_hWnd);
			break;

		case WM_RBUTTONDBLCLK://˫���Ҽ�
		{
			//DestroyWindow();//�رմ���
			break;
		}
		case WM_RBUTTONDOWN://�һ����̣���ʾ�˵�
		{
			POINT pt = { 0 };
			//�����Ҽ��˵� 
			HMENU hMenu = CreatePopupMenu();
			::GetCursorPos(&pt); //��ȡ��ǰ���λ��
			CString strText(_T(""));
			GetDlgItem(IDC_BUTTON_START).GetWindowText(strText);
			::AppendMenu(hMenu, MF_BYCOMMAND, IDC_BUTTON_START, strText);
			::AppendMenu(hMenu, MF_BYCOMMAND, IDCANCEL, _T("�˳�����"));
			::TrackPopupMenu(hMenu, TPM_RIGHTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hWnd, NULL);
			break;
		}
		}
	}
	return 0;
}
LRESULT CMainDlg::OnNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case NM_CLICK:          // Fall through to the next case.
	case NM_RETURN:
	{
		PNMLINK pNMLink = (PNMLINK)lParam;
		LITEM   item = pNMLink->item;

		if ((((LPNMHDR)lParam)->hwndFrom == GetDlgItem(IDC_SYSLINK_URL)) && (item.iLink == 0))
		{
			ShellExecute(NULL, _T("OPEN"), item.szUrl, NULL, NULL, SW_SHOW);
		}

		else if (wcscmp(item.szID, L"idInfo") == 0)
		{
			//MessageBox(hDlg, L"This isn't much help.", L"Example", MB_OK);
		}

		break;
	}
	break;
	}
	return 0;
}
LRESULT CMainDlg::OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (wParam == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE); // ����С��ʱ������������
	}
	else
	{
		bHandled = false;
	}
	return 0;
}
LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	GetDlgItem(IDOK).SetWindowText(_T("�رճ���"));
	GetDlgItem(IDCANCEL).SetWindowText(_T("�˳�����"));
	GetDlgItem(ID_APP_ABOUT).SetWindowText(_T("��������"));
	GetDlgItem(IDC_BUTTON_START).SetWindowText(_T("��������"));
	GetDlgItem(IDC_STATIC).SetWindowText(_T("δ����"));
	SetWindowText(_T("΢��WEB������"));

	OnStart(0, 0, 0, bHandled);

	{
		NOTIFYICONDATA notifyicondata;//NOTIFYICONDATA�ṹ

		notifyicondata.cbSize = sizeof(NOTIFYICONDATA);//Ϊnotifyicondata�ṹ�����ռ�

		//Ϊnotifyicondata�ṹ�����������ֵ
		notifyicondata.hWnd = m_hWnd;//���ڵľ��

		notifyicondata.uID = IDI_NOTIFYICON; //ID��ʶ

		notifyicondata.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

		notifyicondata.uCallbackMessage = WM_TRAYICON;//�ص���Ϣ�ı�ʶ;����ҪΪ�ص���Ϣ�ı�ʶ����û���Ϣӳ��

		notifyicondata.hIcon = GetIcon();//��������ͼ��

		lstrcpy(notifyicondata.szTip, _T("΢��WEB������ - (����������)"));

		Shell_NotifyIcon(NIM_ADD, &notifyicondata);       //װ������ͼ��
	}
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	// if UI is the last thread, no need to wait
	if(_Module.GetLockCount() == 1)
	{
		_Module.m_dwTimeOut = 0L;
		_Module.m_dwPause = 0L;
	}
	_Module.Unlock();

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnStart(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strText(_T(""));
	GetDlgItem(IDC_BUTTON_START).GetWindowText(strText);
	WCHAR wValue = (WORD)strText.GetAt(0);
	switch (wValue)
	{
	case 0x542F:
	{
		int	data = 1234567;
		char *argv[] = { "", "-ports", LISTENING_PORT, NULL };
		int	argc = sizeof(argv) / sizeof(argv[0]) - 1;
		/*
		* Initialize SHTTPD context.
		* Attach folder c:\ to the URL /my_c  (for windows), and
		* /etc/ to URL /my_etc (for UNIX). These are Apache-like aliases.
		* Set WWW root to current directory.
		* Start listening on ports 8080 and 8081
		*/
		ctx = shttpd_init(argc, (char **)argv);
		//shttpd_set_option(ctx, "ssl_cert", "shttpd.pem");
		shttpd_set_option(ctx, "aliases", ALIAS_URI "=" ALIAS_DIR);
		//shttpd_set_option(ctx, "ports", "8080,8081s");

		/* Register an index page under two URIs */
		shttpd_register_uri(ctx, "/", &show_index, (void *)&data);
		shttpd_register_uri(ctx, "/abc.html", &show_index, (void *)&data);

		/* Register a callback on wildcard URI */
		shttpd_register_uri(ctx, "/users/*/", &show_users, NULL);

		/* Show how to use password protection */
		shttpd_register_uri(ctx, "/secret", &show_secret, NULL);
		shttpd_set_option(ctx, "protect", "/secret=passfile");

		/* Show how to use stateful big data transfer */
		shttpd_register_uri(ctx, "/huge", &show_huge, NULL);

		/* Register URI for file upload */
		shttpd_register_uri(ctx, "/post", &show_post, NULL);

		/* Register SSI callbacks */
		shttpd_register_ssi_func(ctx, "true", ssi_test_true, NULL);
		shttpd_register_ssi_func(ctx, "false", ssi_test_false, NULL);
		shttpd_register_ssi_func(ctx, "print_stuff", ssi_print_stuff, NULL);

		shttpd_handle_error(ctx, 404, show_404, NULL);
		
		// Create timer
		SetTimer(WEBSERVER_TIMER, 1000, NULL);
		GetDlgItem(IDC_STATIC).SetWindowText(CString(_T("����״̬:����������")));
		GetDlgItem(IDC_SYSLINK_URL).SetWindowText(CString(CString(_T("<a href=\"http://localhost:")) + _T(LISTENING_PORT) + _T("\">�����ַ:") + _T("http://localhost:")) + _T(LISTENING_PORT) + _T("</a>"));
		GetDlgItem(IDC_BUTTON_START).SetWindowText(_T("ֹͣ����"));

		{
			NOTIFYICONDATA notifyicondata;//NOTIFYICONDATA�ṹ

			notifyicondata.cbSize = sizeof(NOTIFYICONDATA);//Ϊnotifyicondata�ṹ�����ռ�

														   //Ϊnotifyicondata�ṹ�����������ֵ
			notifyicondata.hWnd = m_hWnd;//���ڵľ��

			notifyicondata.uID = IDI_NOTIFYICON; //ID��ʶ

			notifyicondata.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

			notifyicondata.uCallbackMessage = WM_TRAYICON;//�ص���Ϣ�ı�ʶ;����ҪΪ�ص���Ϣ�ı�ʶ����û���Ϣӳ��

			notifyicondata.hIcon = GetIcon();//��������ͼ��

			lstrcpy(notifyicondata.szTip, _T("΢��WEB������ - (����������)"));

			Shell_NotifyIcon(NIM_MODIFY, &notifyicondata);       //װ������ͼ��
		}
	}
	break;
	case 0x505C:
	{
		KillTimer(WEBSERVER_TIMER);
		
		/* Probably unreached, because we will be killed by a signal */
		shttpd_fini(ctx);

		GetDlgItem(IDC_STATIC).SetWindowText(CString(_T("����״̬:����δ��������ֹͣ")));
		GetDlgItem(IDC_SYSLINK_URL).SetWindowText(CString(_T("")));
		SetDlgItemText(IDC_BUTTON_START, _T("��������"));

		{
			NOTIFYICONDATA notifyicondata;//NOTIFYICONDATA�ṹ

			notifyicondata.cbSize = sizeof(NOTIFYICONDATA);//Ϊnotifyicondata�ṹ�����ռ�

														   //Ϊnotifyicondata�ṹ�����������ֵ
			notifyicondata.hWnd = m_hWnd;//���ڵľ��

			notifyicondata.uID = IDI_NOTIFYICON; //ID��ʶ

			notifyicondata.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

			notifyicondata.uCallbackMessage = WM_TRAYICON;//�ص���Ϣ�ı�ʶ;����ҪΪ�ص���Ϣ�ı�ʶ����û���Ϣӳ��

			notifyicondata.hIcon = GetIcon();//��������ͼ��

			lstrcpy(notifyicondata.szTip, _T("΢��WEB������ - (����δ����)"));

			Shell_NotifyIcon(NIM_MODIFY, &notifyicondata);       //װ������ͼ��
		}
	}
	break;
	default:
		break;
	}
	return 0;
}
LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	/* Serve connections infinitely until someone kills us */
	shttpd_poll(ctx, 0);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	// Close timer
	KillTimer(WEBSERVER_TIMER);

	/* Probably unreached, because we will be killed by a signal */
	shttpd_fini(ctx);

	{
		NOTIFYICONDATA notifyicondata;

		notifyicondata.cbSize = sizeof(NOTIFYICONDATA);

		notifyicondata.uFlags = 0;

		notifyicondata.hWnd = m_hWnd;

		notifyicondata.uID = IDI_NOTIFYICON;

		Shell_NotifyIcon(NIM_DELETE, &notifyicondata);//ж������ͼ��
	}

	DestroyWindow();
	::PostQuitMessage(nVal);
}
