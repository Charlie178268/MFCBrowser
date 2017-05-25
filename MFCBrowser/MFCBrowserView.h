
// MFCBrowserView.h : CMFCBrowserView 类的接口
//

#pragma once
#include "MFCBrowserDoc.h"

class CMFCBrowserView : public CHtmlView
{
protected: // 仅从序列化创建
	CMFCBrowserView();
	DECLARE_DYNCREATE(CMFCBrowserView)

// 特性
public:
	CMFCBrowserDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CMFCBrowserView();
	//清除文件记录
	BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni=FALSE, BOOL bWipeIndexDat=FALSE);
	enum DEL_CACHE_TYPE
	{
		File,    //表示Internet临时文件
		Cookie   //表示Cookie
	};
	BOOL DeleteUrlCache(DEL_CACHE_TYPE type);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // MFCBrowserView.cpp 中的调试版本
inline CMFCBrowserDoc* CMFCBrowserView::GetDocument() const
   { return reinterpret_cast<CMFCBrowserDoc*>(m_pDocument); }
#endif

