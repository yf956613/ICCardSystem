#pragma once


// CUserPrestore 对话框

class CUserPrestore : public CDialogEx
{
	DECLARE_DYNAMIC(CUserPrestore)

public:
	CUserPrestore(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CUserPrestore();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERPRESTORE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_editID;
	CEdit m_editName;
	CEdit m_editIDNo;
	CEdit m_editPhoneNo;
	CEdit m_editAddr;
	CEdit m_editLicense;
	CEdit m_editState;
	CEdit m_editBalance;
	CEdit m_editCardNo;
	CEdit m_editAmount;
	CEdit m_editBalanceNew;
	CEdit m_editLastAmount;
	CEdit m_editTotalAmount;
	CEdit m_editPrestoreTimes;
	CEdit m_editNoteInfo;
	CUser m_SelectUser;
	CCard m_SelectCard;
	int m_nDeposit;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonReadcard();
	afx_msg void OnBnClickedButtonPrestore();
	afx_msg void OnEnChangeEditAmount();
	void GetDeposit(const CUser & user, const CCard & card);
	void InsertEdit(const CUser & user,const CCard & card);
};
