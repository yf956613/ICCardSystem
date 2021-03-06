#pragma once


// CEmployeeEdit 对话框

class CEmployeeEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CEmployeeEdit)

public:
	CEmployeeEdit(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEmployeeEdit();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMPLOYEEEDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_editID;
	CEdit m_editName;
	CEdit m_editPassword;
	CComboBox m_comboGradeID;
	CButton m_checkIsUsing;
	CListCtrl m_listctrlEmployee;
	CEmployeeDB m_edb;
	bool m_bSaveFromAdd;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonSave();
	void InitUI();
	void UpdateList(const std::vector<CEmployee> & vec_employee);
	void RefreshList();
	void SetIntoEdit();
	void SetOutOfEdit();
	void InsertEdit(const CEmployee & employee);
	void InsertEditByID(int nID);
	afx_msg void OnNMClickListEmployee(NMHDR *pNMHDR, LRESULT *pResult);
};
