
// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "CEmployeeEdit.h"
#include "CPasswordEdit.h"
#include "CUserInfoEdit.h"
#include "CUserOpenCard.h"
#include "CUserPrestore.h"
#include "CUserRefund.h"
#include "CUserReturnDeposit.h"
#include "CUserCloseCard.h"
#include "CUserInfoQuery.h"
#include "CUserLogQuery.h"
#include "CUserIncomeStatistics.h"
#include "CUserPrestoreStatistics.h"
#include "CToolReadCard.h"
#include "CToolLicenseCard.h"
#include "CToolSetCard.h"
#include "CToolClearCard.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	CEmployeeEdit * m_p_eeDlg;
	CUserInfoEdit * m_p_uieDlg;
	//CUserOpenCard * m_p_uocDlg;
	CUserPrestore * m_p_upDlg;
	CUserRefund * m_p_urDlg;
	CUserReturnDeposit * m_p_urdDlg;
	CUserCloseCard * m_p_uccDlg;
	CUserInfoQuery * m_p_uiqDlg;
	CUserLogQuery * m_p_ulqDlg;
	CUserIncomeStatistics * m_p_uisDlg;
	CUserPrestoreStatistics * m_p_upsDlg;
	CToolReadCard * m_p_treadcDlg;
	CToolLicenseCard * m_p_tlicensecDlg;
	CToolSetCard * m_p_tsetcDlg;
	CToolClearCard * m_p_tcclearcDlg;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	void InitInCreate();
	afx_msg void OnSystemEmployee();
	afx_msg void OnUpdateSystemEmployee(CCmdUI *pCmdUI);
	afx_msg void OnSystemPassword();
	afx_msg void OnUserInfoedit();
	afx_msg void OnUserOpencard();
	afx_msg void OnUserPrestore();
	afx_msg void OnUserRefund();
	afx_msg void OnUserReturndeposit();
	afx_msg void OnUserClosecard();
	afx_msg void OnUserInfoquery();
	afx_msg void OnUserLogquery();
	afx_msg void OnUserIncomestatistics();
	afx_msg void OnUserPrestorestatistics();
	afx_msg void OnToolReadcard();
	afx_msg void OnToolLicensecard();
	afx_msg void OnToolSetcard();
	afx_msg void OnToolClearcard();
	afx_msg void OnUpdateToolClearcard(CCmdUI *pCmdUI);
};


