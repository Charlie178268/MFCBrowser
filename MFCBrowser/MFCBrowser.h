
// MFCBrowser.h : MFCBrowser Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMFCBrowserApp:
// �йش����ʵ�֣������ MFCBrowser.cpp
//

class CMFCBrowserApp : public CWinApp
{
public:
	CMFCBrowserApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCBrowserApp theApp;
