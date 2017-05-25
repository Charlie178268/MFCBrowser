
// MFCBrowserView.cpp : CMFCBrowserView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCBrowser.h"
#endif

#include "MFCBrowserDoc.h"
#include "MFCBrowserView.h"
#include "MainFrm.h"
#include "shlwapi.h"
#include "wininet.h" 
#include "UrlHist.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCBrowserView

IMPLEMENT_DYNCREATE(CMFCBrowserView, CHtmlView)

BEGIN_MESSAGE_MAP(CMFCBrowserView, CHtmlView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	
	ON_COMMAND(ID_BACK, &CMFCBrowserView::OnBack)
	ON_COMMAND(ID_FORWORD, &CMFCBrowserView::OnForword)
	ON_COMMAND(ID_HOME, &CMFCBrowserView::OnHome)
	ON_COMMAND(ID_SEARCH, &CMFCBrowserView::OnSearch)
	ON_COMMAND(ID_UPDATE, &CMFCBrowserView::OnUpdate)
	ON_COMMAND(ID_STOP, &CMFCBrowserView::OnStop)
	ON_COMMAND(ID_DOWNLOAD, &CMFCBrowserView::OnDownload)
	ON_COMMAND(ID_DelHistory, &CMFCBrowserView::OnDelhistory)
	ON_COMMAND(ID_DelCookies, &CMFCBrowserView::OnDelcookies)
	ON_COMMAND(ID_Deltemp, &CMFCBrowserView::OnDeltemp)
	ON_COMMAND(ID_DelAddhistory, &CMFCBrowserView::OnDeladdhistory)
	ON_COMMAND(ID_DelPassHistory, &CMFCBrowserView::OnDelpasshistory)
END_MESSAGE_MAP()

// CMFCBrowserView 构造/析构

CMFCBrowserView::CMFCBrowserView()
{
	// TODO: 在此处添加构造代码

}

CMFCBrowserView::~CMFCBrowserView()
{
}

BOOL CMFCBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CHtmlView::PreCreateWindow(cs);
}

void CMFCBrowserView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("http://www.baidu.com"),NULL,NULL);
}


// CMFCBrowserView 打印



// CMFCBrowserView 诊断

#ifdef _DEBUG
void CMFCBrowserView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CMFCBrowserView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CMFCBrowserDoc* CMFCBrowserView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCBrowserDoc)));
	return (CMFCBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCBrowserView 消息处理程序

//go按钮事件监听



void CMFCBrowserView::OnBack()
{
	// TODO: 在此添加命令处理程序代码
	GoBack();
}


void CMFCBrowserView::OnForword()
{
	// TODO: 在此添加命令处理程序代码
	GoForward();
}


void CMFCBrowserView::OnHome()
{
	// TODO: 在此添加命令处理程序代码
	GoHome();
}


void CMFCBrowserView::OnSearch()
{
	// TODO: 在此添加命令处理程序代码
	GoSearch();
}


void CMFCBrowserView::OnUpdate()
{
	// TODO: 在此添加命令处理程序代码
	Refresh();
}


void CMFCBrowserView::OnStop()
{
	// TODO: 在此添加命令处理程序代码
	Stop();
}


void CMFCBrowserView::OnDownload()
{
	// TODO: 在此添加命令处理程序代码
}


void CMFCBrowserView::OnDocumentComplete(LPCTSTR str)
{
	((CMainFrame*)GetParentFrame())->SetPage(str);
}

// OnDelIntenetTempFile、OnDelCookies和OnDelHistory
BOOL CMFCBrowserView::EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni, BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;        //文件句柄
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;
	sFindFilter = szPath;         //路径名
	sFindFilter += _T("\\*.*");    //表示所有文件都要删除
								   //找到第一个文件
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	do
	{
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 ||
			_tcscmp(wfd.cFileName, _T("..")) == 0 ||
			(bDeleteDesktopIni == FALSE&&_tcscmp(wfd.cFileName, _T("desktop.ini")) == 0))
		{
			continue;
		}
		sFullPath = szPath;
		sFullPath += _T('\\');
		sFullPath += wfd.cFileName;     //文件完整的路径和名称
										//如果文件含有只读属性，那么应该去掉该属性再删除
		dwAttributes = GetFileAttributes(sFullPath);       //得到文件属性
		if (dwAttributes&FILE_ATTRIBUTE_READONLY)        //判断是否只读
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;      //删除只读属性
			SetFileAttributes(sFullPath, dwAttributes);   //重新删除
		}
		//判断该文件是否是一个目录，如果是目录则递归调用清空子目录
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath, bDeleteDesktopIni, bWipeIndexDat);
			RemoveDirectory(sFullPath);
		}
		else          //这是一个文件
		{
			DeleteFile(sFullPath);           //删除该文件
		}
	} while (FindNextFile(hFind, &wfd));        //查找下一个文件
	FindClose(hFind);                       //关闭文件句柄
	return TRUE;
}

BOOL CMFCBrowserView::DeleteUrlCache(DEL_CACHE_TYPE type)
//  OnDelCookies()和OnDelIntenetTempFile()  需要清除他们的Cache，而不仅仅是文件本身
{
	BOOL bRet = FALSE;
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;   //Cache入口指针
	DWORD dwEntrySize;
	dwEntrySize = 0;
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize); //找到第一个URL Cache入口
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];//分配空间
	hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);
	if (!hEntry)
	{
		goto cleanup;
	}
	do
	{
		if (type == File&&!(lpCacheEntry->CacheEntryType&COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		else if (type == Cookie && (lpCacheEntry->CacheEntryType&COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		dwEntrySize = 0;
		FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
		delete[] lpCacheEntry;
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	} while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));
	bRet = TRUE;
cleanup:
	if (lpCacheEntry)
	{
		delete[] lpCacheEntry;
	}
	return bRet;
}

void CMFCBrowserView::OnDelhistory()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szPath[MAX_PATH];
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_RECENT, FALSE))
	{
		EmptyDirectory(szPath);
	}
	SHDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\CurrentVersion\\Explorer\\RecentDocs"));
}


void CMFCBrowserView::OnDelcookies()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szPath[MAX_PATH];
	DeleteUrlCache(Cookie);         //删除URL的Cache
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
	{
		EmptyDirectory(szPath);
	}
}


void CMFCBrowserView::OnDeltemp()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szPath[MAX_PATH];
	DeleteUrlCache(File);   //删除URL的Cache
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_INTERNET_CACHE, FALSE))
	{
		//得到临时目录并清空它
		EmptyDirectory(szPath);
	}
}


void CMFCBrowserView::OnDeladdhistory()
{
	// TODO: 在此添加命令处理程序代码
	//浏览器地址栏历史记录的清除
	SHDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
}


void CMFCBrowserView::OnDelpasshistory()
{
	// TODO: 在此添加命令处理程序代码
	//清除密码历史记录
	SHDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\IntelliForms"));
}
