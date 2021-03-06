#pragma once


// CUserIncomeStatistics 对话框

class CUserIncomeStatistics : public CDialogEx, public ZExcelExport::CCallBack
{
	DECLARE_DYNAMIC(CUserIncomeStatistics)

public:
	CUserIncomeStatistics(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CUserIncomeStatistics();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USERINCOMESTATISTICS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_listctrlEmployee;
	CEdit m_editEmployeeID;
	CEdit m_editEmployeeName;
	CDateTimeCtrl m_datetimeStart;
	CDateTimeCtrl m_datetimeEnd;
	bool m_bIsInitFinish;
	CString m_strTtitle;
	CString m_strSubTtitle;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStatistics();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedRadioThisyear();
	afx_msg void OnBnClickedRadioLastyear();
	afx_msg void OnBnClickedRadioThismonth();
	afx_msg void OnBnClickedRadioLastmonth();
	afx_msg void OnBnClickedRadioToday();
	afx_msg void OnBnClickedRadioYesterday();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	void InitUI();
	void GetEmployee(std::vector<CEmployee> & vec_employee);
	void GetData(const CEmployee & employee);
	void GetRecord(CRecordDB * p_db,const CString & strTableName, int nEmployeeID, const CString & strTimeStart, const CString &  strTimeEnd, std::vector<CString> & vec_str);
	void UpdateList(std::vector<std::vector<CString>> & vec2_strData);
	virtual void OnExportProgressUpdate(int nProgress);
	virtual void OnExportResult(bool bResult);
};
