// RLogin.h : RLogin アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル
#include "Data.h"

#define	WM_SOCKSEL		(WM_USER + 0)
#define WM_GETHOSTADDR	(WM_USER + 1)
#define	WM_ICONMSG		(WM_USER + 2)
#define WM_THREADCMD	(WM_USER + 3)
#define WM_SEARCHNEXT	(WM_USER + 4)

class CCommandLineInfoEx : public CCommandLineInfo
{
public:
	int m_PasStat;
	int m_Proto;
	CString m_Addr;
	CString m_Port;
	CString m_User;
	CString m_Pass;
	CString m_Term;
	CString m_Name;
	BOOL m_InUse;

	CCommandLineInfoEx();
	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
	BOOL ParseUrl(const TCHAR* pszParam);
	void GetString(CString &str);
	void SetString(LPCSTR str);
};

// CRLoginApp:
// このクラスの実装については、RLogin.cpp を参照してください。
//

class CRLoginApp : public CWinApp
{
public:
	int m_NextSock;
	CPtrArray m_SocketIdle;
	class CFontChache m_FontData;
	WSADATA wsaData;
	CString m_BaseDir;
	CCommandLineInfoEx *m_pCmdInfo;

#ifdef	USE_DIRECTWRITE
	ID2D1Factory *m_pD2DFactory;
	IDWriteFactory *m_pDWriteFactory;

	inline ID2D1Factory *GetD2D1Factory() { return m_pD2DFactory; }
	inline IDWriteFactory *GetDWriteFactory() { return m_pDWriteFactory; }
#endif

	void SetSocketIdle(class CExtSocket *pSock);
	void DelSocketIdle(class CExtSocket *pSock);
	void GetProfileData(LPCTSTR lpszSection, LPCTSTR lpszEntry, void *lpBuf, int nBufLen, void *lpDef = NULL);
	void GetProfileBuffer(LPCTSTR lpszSection, LPCTSTR lpszEntry, CBuffer &Buf);
	void GetProfileArray(LPCTSTR lpszSection, CStringArrayExt &array);
	void WriteProfileArray(LPCTSTR lpszSection, CStringArrayExt &array);
	int GetProfileSeqNum(LPCTSTR lpszSection, LPCTSTR lpszEntry);
	void GetProfileKeys(LPCTSTR lpszSection, CStringArrayExt &array);
	void DelProfileEntry(LPCTSTR lpszSection, LPCTSTR lpszEntry);
	void RegisterShellProtocol(LPCSTR pSection, LPCSTR pOption);
	void RegisterDelete(HKEY hKey, LPCSTR pSection, LPCSTR pKey);
	void RegisterSave(HKEY hKey, LPCSTR pSection, CBuffer &buf);
	void RegisterLoad(HKEY hKey, LPCSTR pSection, CBuffer &buf);
	void SSL_Init();

	void OpenProcsCmd(CCommandLineInfoEx *pCmdInfo);
	BOOL InUseCheck();

	CRLoginApp();

// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRLoginApp theApp;
extern void ExDwmEnableWindow(HWND hWnd);