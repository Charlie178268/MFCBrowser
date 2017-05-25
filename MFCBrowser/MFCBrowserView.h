
// MFCBrowserView.h : CMFCBrowserView ��Ľӿ�
//

#pragma once
#include "MFCBrowserDoc.h"

class CMFCBrowserView : public CHtmlView
{
protected: // �������л�����
	CMFCBrowserView();
	DECLARE_DYNCREATE(CMFCBrowserView)

// ����
public:
	CMFCBrowserDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CMFCBrowserView();
	//����ļ���¼
	BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni=FALSE, BOOL bWipeIndexDat=FALSE);
	enum DEL_CACHE_TYPE
	{
		File,    //��ʾInternet��ʱ�ļ�
		Cookie   //��ʾCookie
	};
	BOOL DeleteUrlCache(DEL_CACHE_TYPE type);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBack();
	afx_msg void OnForword();
	afx_msg void OnHome();
	afx_msg void OnSearch();
	afx_msg void OnUpdate();
	afx_msg void OnStop();
	afx_msg void OnDownload();
	void OnDocumentComplete(LPCTSTR str);
	afx_msg void OnDelhistory();
	afx_msg void OnDelcookies();
	afx_msg void OnDeltemp();
	afx_msg void OnDeladdhistory();
	afx_msg void OnDelpasshistory();
};

#ifndef _DEBUG  // MFCBrowserView.cpp �еĵ��԰汾
inline CMFCBrowserDoc* CMFCBrowserView::GetDocument() const
   { return reinterpret_cast<CMFCBrowserDoc*>(m_pDocument); }
#endif

