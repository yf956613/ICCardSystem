
// ICCardSystem.h: ICCardSystem 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "ZClassLib.h"

// CICCardSystemApp:
// 有关此类的实现，请参阅 ICCardSystem.cpp
//

class CICCardSystemApp : public CWinAppEx
{
public:
	CICCardSystemApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CICCardSystemApp theApp;
enum WND_POINT
{
	WND_MAINFRAME =0,
	WND_MAINVIEW,
	WND_MAX
};
extern CWnd * g_sz_p_wnd[WND_MAX];
extern CEmployee g_employee;
extern const int g_nINT100;
extern CUserDB g_udb;
extern CCardDB g_cdb;
extern CInitDepositRecordDB g_idrdb;
extern CPrestoreRecordDB g_prdb;
extern CRefundRecordDB g_rrdb;
extern CReturnDepositRecordDB g_rdrdb;
#define WM_MSGRECVPRO WM_USER+1
enum MSGUSER
{
	MSGUSER_SETTITLE=0,
	MSGUSER_SETSUBTITLE,
	MSGUSER_PRINTUSERLOG,
	MSGUSER_PRINTUSERINCOME,
	MSGUSER_PRINTUSERPRESTORE,
	MSGUSER_UPDATEPROGRESS,
	MSGUSER_EXPORTRESULT
};