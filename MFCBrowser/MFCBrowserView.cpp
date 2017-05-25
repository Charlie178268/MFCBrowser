
// MFCBrowserView.cpp : CMFCBrowserView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CMFCBrowserView ����/����

CMFCBrowserView::CMFCBrowserView()
{
	// TODO: �ڴ˴���ӹ������

}

CMFCBrowserView::~CMFCBrowserView()
{
}

BOOL CMFCBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CHtmlView::PreCreateWindow(cs);
}

void CMFCBrowserView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("http://www.baidu.com"),NULL,NULL);
}


// CMFCBrowserView ��ӡ



// CMFCBrowserView ���

#ifdef _DEBUG
void CMFCBrowserView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CMFCBrowserView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CMFCBrowserDoc* CMFCBrowserView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCBrowserDoc)));
	return (CMFCBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCBrowserView ��Ϣ�������

//go��ť�¼�����



void CMFCBrowserView::OnBack()
{
	// TODO: �ڴ���������������
	GoBack();
}


void CMFCBrowserView::OnForword()
{
	// TODO: �ڴ���������������
	GoForward();
}


void CMFCBrowserView::OnHome()
{
	// TODO: �ڴ���������������
	GoHome();
}


void CMFCBrowserView::OnSearch()
{
	// TODO: �ڴ���������������
	GoSearch();
}


void CMFCBrowserView::OnUpdate()
{
	// TODO: �ڴ���������������
	Refresh();
}


void CMFCBrowserView::OnStop()
{
	// TODO: �ڴ���������������
	Stop();
}


void CMFCBrowserView::OnDownload()
{
	// TODO: �ڴ���������������
}


void CMFCBrowserView::OnDocumentComplete(LPCTSTR str)
{
	((CMainFrame*)GetParentFrame())->SetPage(str);
}

// OnDelIntenetTempFile��OnDelCookies��OnDelHistory
BOOL CMFCBrowserView::EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni, BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;        //�ļ����
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;
	sFindFilter = szPath;         //·����
	sFindFilter += _T("\\*.*");    //��ʾ�����ļ���Ҫɾ��
								   //�ҵ���һ���ļ�
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
		sFullPath += wfd.cFileName;     //�ļ�������·��������
										//����ļ�����ֻ�����ԣ���ôӦ��ȥ����������ɾ��
		dwAttributes = GetFileAttributes(sFullPath);       //�õ��ļ�����
		if (dwAttributes&FILE_ATTRIBUTE_READONLY)        //�ж��Ƿ�ֻ��
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;      //ɾ��ֻ������
			SetFileAttributes(sFullPath, dwAttributes);   //����ɾ��
		}
		//�жϸ��ļ��Ƿ���һ��Ŀ¼�������Ŀ¼��ݹ���������Ŀ¼
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath, bDeleteDesktopIni, bWipeIndexDat);
			RemoveDirectory(sFullPath);
		}
		else          //����һ���ļ�
		{
			DeleteFile(sFullPath);           //ɾ�����ļ�
		}
	} while (FindNextFile(hFind, &wfd));        //������һ���ļ�
	FindClose(hFind);                       //�ر��ļ����
	return TRUE;
}

BOOL CMFCBrowserView::DeleteUrlCache(DEL_CACHE_TYPE type)
//  OnDelCookies()��OnDelIntenetTempFile()  ��Ҫ������ǵ�Cache�������������ļ�����
{
	BOOL bRet = FALSE;
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;   //Cache���ָ��
	DWORD dwEntrySize;
	dwEntrySize = 0;
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize); //�ҵ���һ��URL Cache���
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];//����ռ�
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
	// TODO: �ڴ���������������
	TCHAR szPath[MAX_PATH];
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_RECENT, FALSE))
	{
		EmptyDirectory(szPath);
	}
	SHDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\CurrentVersion\\Explorer\\RecentDocs"));
}


void CMFCBrowserView::OnDelcookies()
{
	// TODO: �ڴ���������������
	TCHAR szPath[MAX_PATH];
	DeleteUrlCache(Cookie);         //ɾ��URL��Cache
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
	{
		EmptyDirectory(szPath);
	}
}


void CMFCBrowserView::OnDeltemp()
{
	// TODO: �ڴ���������������
	TCHAR szPath[MAX_PATH];
	DeleteUrlCache(File);   //ɾ��URL��Cache
	if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_INTERNET_CACHE, FALSE))
	{
		//�õ���ʱĿ¼�������
		EmptyDirectory(szPath);
	}
}


void CMFCBrowserView::OnDeladdhistory()
{
	// TODO: �ڴ���������������
	//�������ַ����ʷ��¼�����
	SHDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
}


void CMFCBrowserView::OnDelpasshistory()
{
	// TODO: �ڴ���������������
	//���������ʷ��¼
	SHDeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\IntelliForms"));
}
