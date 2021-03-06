#pragma once


// CUserInfoQuery 对话框

class CUserInfoQuery : public CDialogEx
{
	DECLARE_DYNAMIC(CUserInfoQuery)

public:
	CUserInfoQuery(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CUserInfoQuery();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERINFOQUERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_comboCondition;
	CListCtrl m_listctrlUser;
	CEdit m_editKeyword;
	bool m_bIsInitFinish;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitUI();
	void UpdateList(const std::vector<CUser> & vec_user);
};
