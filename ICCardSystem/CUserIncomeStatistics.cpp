// CUserIncomeStatistics.cpp: 实现文件
//

#include "stdafx.h"
#include "ICCardSystem.h"
#include "CUserIncomeStatistics.h"
#include "afxdialogex.h"


// CUserIncomeStatistics 对话框

IMPLEMENT_DYNAMIC(CUserIncomeStatistics, CDialogEx)

CUserIncomeStatistics::CUserIncomeStatistics(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USERINCOMESTATISTICS, pParent)
	, m_bIsInitFinish(false)
	, m_strTtitle(_T("会员卡收入汇总表"))
	, m_strSubTtitle(_T(""))
{

}

CUserIncomeStatistics::~CUserIncomeStatistics()
{
}

void CUserIncomeStatistics::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EMPLOYEE, m_listctrlEmployee);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEEID, m_editEmployeeID);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEENAME, m_editEmployeeName);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_datetimeStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_datetimeEnd);
}


BEGIN_MESSAGE_MAP(CUserIncomeStatistics, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_STATISTICS, &CUserIncomeStatistics::OnBnClickedButtonStatistics)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CUserIncomeStatistics::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CUserIncomeStatistics::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_RADIO_THISYEAR, &CUserIncomeStatistics::OnBnClickedRadioThisyear)
	ON_BN_CLICKED(IDC_RADIO_LASTYEAR, &CUserIncomeStatistics::OnBnClickedRadioLastyear)
	ON_BN_CLICKED(IDC_RADIO_THISMONTH, &CUserIncomeStatistics::OnBnClickedRadioThismonth)
	ON_BN_CLICKED(IDC_RADIO_LASTMONTH, &CUserIncomeStatistics::OnBnClickedRadioLastmonth)
	ON_BN_CLICKED(IDC_RADIO_TODAY, &CUserIncomeStatistics::OnBnClickedRadioToday)
	ON_BN_CLICKED(IDC_RADIO_YESTERDAY, &CUserIncomeStatistics::OnBnClickedRadioYesterday)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MSGRECVPRO, &CUserIncomeStatistics::OnMsgrecvpro)
END_MESSAGE_MAP()


// CUserIncomeStatistics 消息处理程序


BOOL CUserIncomeStatistics::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	m_bIsInitFinish = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CUserIncomeStatistics::OnBnClickedButtonStatistics()
{
	// TODO: 在此添加控件通知处理程序代码
	m_vec2_strDataExport.clear();
	std::vector<CEmployee> vec_employee;
	GetEmployee(vec_employee);
	int nSize = vec_employee.size();
	for (int i = 0; i < nSize; ++i)
	{
		GetData(vec_employee[i]);
	}
	UpdateList(m_vec2_strDataExport);
}


void CUserIncomeStatistics::OnBnClickedButtonPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_sz_p_wnd[WND_MAINVIEW])
	{
		g_sz_p_wnd[WND_MAINVIEW]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strTtitle, MSGUSER_SETTITLE);
		g_sz_p_wnd[WND_MAINVIEW]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strSubTtitle, MSGUSER_SETSUBTITLE);
		g_sz_p_wnd[WND_MAINVIEW]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_vec2_strDataExport, MSGUSER_PRINTUSERINCOME);
	}
}


void CUserIncomeStatistics::OnBnClickedButtonExport()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szFilter;
	szFilter = _T("Excel文件(*.xlsx)|*.xlsx|Excel文件(*.xls)|*.xls|所有文件(*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("xlsx"), m_strTtitle+_T("_")+ m_strSubTtitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		m_strDestPath = fileDlg.GetPathName();
		m_strDemoPath = ZUtil::GetExeCatalogPath() + _T("\\DemoIncome.xlsx");
		m_nRowOffset = 1;
		ZExcelExport excleexport(this);
		if (excleexport.Start())
		{
			GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STATISTICS)->EnableWindow(FALSE);
		}
		else
			AfxMessageBox(_T("导出失败!"));
	}
}


void CUserIncomeStatistics::OnBnClickedRadioThisyear()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart = CTime(timeCur.GetYear(), 1, 1, 0, 0, 0);
	CTime timeEnd = CTime(timeCur.GetYear(), timeCur.GetMonth(), timeCur.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserIncomeStatistics::OnBnClickedRadioLastyear()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTimeSpan timespan(1, 0, 0, 0);
	CTime time = CTime(timeCur.GetYear(), 1, 1, 0, 0, 0) - timespan;
	CTime timeStart = CTime(time.GetYear(), 1, 1, 0, 0, 0);
	CTime timeEnd = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserIncomeStatistics::OnBnClickedRadioThismonth()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart = CTime(timeCur.GetYear(), timeCur.GetMonth(), 1, 0, 0, 0);
	CTime timeEnd = CTime(timeCur.GetYear(), timeCur.GetMonth(), timeCur.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserIncomeStatistics::OnBnClickedRadioLastmonth()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTimeSpan timespan(1, 0, 0, 0);
	CTime time = CTime(timeCur.GetYear(), timeCur.GetMonth(), 1, 0, 0, 0) - timespan;
	CTime timeStart = CTime(time.GetYear(), time.GetMonth(), 1, 0, 0, 0);
	CTime timeEnd = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserIncomeStatistics::OnBnClickedRadioToday()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart = CTime(timeCur.GetYear(), timeCur.GetMonth(), timeCur.GetDay(), 0, 0, 0);
	CTime timeEnd = CTime(timeCur.GetYear(), timeCur.GetMonth(), timeCur.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserIncomeStatistics::OnBnClickedRadioYesterday()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTimeSpan timespan(1, 0, 0, 0);
	CTime time = timeCur - timespan;
	CTime timeStart = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
	CTime timeEnd = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserIncomeStatistics::InitUI()
{
	// TODO: 在此处添加实现代码.
	CString sz_strHead[] = { _T("员工"),_T("开卡数量"), _T("开卡押金"), _T("退卡次数"), _T("退卡金额"),_T("充值次数"), _T("充值金额"),_T("退款次数"), _T("退款金额"),_T("退押次数"), _T("退押金额") };
	const int nMax = 23;
	CRect rc;
	m_listctrlEmployee.GetClientRect(&rc);
	m_listctrlEmployee.SetExtendedStyle(m_listctrlEmployee.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listctrlEmployee.InsertColumn(0, sz_strHead[0], LVCFMT_LEFT, rc.Width() * 2 / nMax, 0);
	m_listctrlEmployee.InsertColumn(1, sz_strHead[1], LVCFMT_LEFT, rc.Width() * 2 / nMax, 1);
	m_listctrlEmployee.InsertColumn(2, sz_strHead[2], LVCFMT_LEFT, rc.Width() * 2 / nMax, 2);
	m_listctrlEmployee.InsertColumn(3, sz_strHead[3], LVCFMT_LEFT, rc.Width() * 2 / nMax, 3);
	m_listctrlEmployee.InsertColumn(4, sz_strHead[4], LVCFMT_LEFT, rc.Width() * 2 / nMax, 4);
	m_listctrlEmployee.InsertColumn(5, sz_strHead[5], LVCFMT_LEFT, rc.Width() * 2 / nMax, 5);
	m_listctrlEmployee.InsertColumn(6, sz_strHead[6], LVCFMT_LEFT, rc.Width() * 2 / nMax, 6);
	m_listctrlEmployee.InsertColumn(7, sz_strHead[7], LVCFMT_LEFT, rc.Width() * 2 / nMax, 7);
	m_listctrlEmployee.InsertColumn(8, sz_strHead[8], LVCFMT_LEFT, rc.Width() * 2 / nMax, 8);
	m_listctrlEmployee.InsertColumn(9, sz_strHead[9], LVCFMT_LEFT, rc.Width() * 2 / nMax, 9);
	m_listctrlEmployee.InsertColumn(10, sz_strHead[10], LVCFMT_LEFT, rc.Width() * 2 / nMax, 10);
	CheckDlgButton(IDC_RADIO_TODAY, TRUE);
	OnBnClickedRadioToday();
}

void CUserIncomeStatistics::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);
		const int nMax = 23;
		m_listctrlEmployee.SetRedraw(FALSE);
		m_listctrlEmployee.SetColumnWidth(0, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(1, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(2, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(3, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(4, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(5, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(6, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(7, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(8, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(9, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetColumnWidth(10, rc.Width() * 2 / nMax);
		m_listctrlEmployee.SetRedraw(TRUE);
	}
}


void CUserIncomeStatistics::GetEmployee(std::vector<CEmployee> & vec_employee)
{
	CString strSql, strEmployeeID, strEmployeeName;
	CEmployeeDB edb;
	m_editEmployeeID.GetWindowText(strEmployeeID);
	m_editEmployeeName.GetWindowText(strEmployeeName);
	if (strEmployeeID.IsEmpty())
		strSql.Format(_T("SELECT * FROM Employee WHERE Name LIKE '%%%s%%'"), strEmployeeName);
	else
		strSql.Format(_T("SELECT * FROM Employee WHERE ID=%d AND Name LIKE '%%%s%%'"), _ttoi(strEmployeeID), strEmployeeName);
	edb.Select(strSql, vec_employee);
}


void CUserIncomeStatistics::GetData(const CEmployee & employee)
{
	CString strSql,str, strTimeStart, strTimeEnd;
	std::vector<CString> vec_str;
	int nSize,nTotalAmount;
	CTime timeStart, timeEnd;
	m_datetimeStart.GetTime(timeStart);
	m_datetimeEnd.GetTime(timeEnd);
	strTimeStart = timeStart.Format(_T("%Y-%m-%d %H:%M:%S"));
	strTimeEnd = timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_strSubTtitle.Format(_T("统计日期：%s - %s"), timeStart.Format(_T("%Y%m%d")), timeEnd.Format(_T("%Y%m%d")));
	//员工
	str.Format(_T("%s (%d)"), employee.GetName(), employee.GetID());
	vec_str.push_back(str);
	//开卡
	CCardDB cdb;
	std::vector<CCard> vec_card;
	strSql.Format(_T("SELECT * FROM Card WHERE EmployeeIDOpenCard=%d AND TimeOpenCard>='%s' AND TimeOpenCard<='%s'"), employee.GetID(),strTimeStart,strTimeEnd);
	cdb.Select(strSql, vec_card);
	nSize = vec_card.size();
	nTotalAmount=0;
	for(int i=0;i< nSize;++i)
	{
		nTotalAmount += vec_card[i].GetDeposit();
	}
	str.Format(_T("%d"), nSize);
	vec_str.push_back(str);
	str.Format(_T("%.02f"), (float)nTotalAmount/ (float)g_nINT100);
	vec_str.push_back(str);
	//退卡
	strSql.Format(_T("SELECT * FROM Card WHERE EmployeeIDCloseCard=%d AND TimeCloseCard>='%s' AND TimeCloseCard<='%s'"), employee.GetID(), strTimeStart, strTimeEnd);
	cdb.Select(strSql, vec_card);
	nSize = vec_card.size();
	nTotalAmount = 0;
	str.Format(_T("%d"), nSize);
	vec_str.push_back(str);
	str.Format(_T("%.02f"), (float)nTotalAmount / (float)g_nINT100);
	vec_str.push_back(str);
	//充值，退款，退押金，补卡
	CPrestoreRecordDB prdb;
	CRefundRecordDB rrdb;
	CReturnDepositRecordDB rdrdb;
	GetRecord(&prdb, _T("PrestoreRecord"), employee.GetID(), strTimeStart, strTimeEnd,vec_str);
	GetRecord(&rrdb, _T("RefundRecord"), employee.GetID(), strTimeStart, strTimeEnd, vec_str);
	GetRecord(&rdrdb, _T("ReturnDepositRecord"), employee.GetID(), strTimeStart, strTimeEnd, vec_str);
	m_vec2_strDataExport.push_back(vec_str);
}

void CUserIncomeStatistics::GetRecord(CRecordDB * p_db, const CString & strTableName,int nEmployeeID, const CString & strTimeStart, const CString &  strTimeEnd, std::vector<CString> & vec_str)
{
	CString strSql,str;
	std::vector<CRecord> vec_record;
	strSql.Format(_T("SELECT * FROM %s WHERE EmployeeID=%d AND Time>='%s' AND Time<='%s'"), strTableName,nEmployeeID, strTimeStart, strTimeEnd);
	p_db->Select(strSql, vec_record);
	int nSize = vec_record.size();
	int nTotalAmount = 0;
	for (int i = 0; i < nSize; ++i)
	{
		nTotalAmount += vec_record[i].GetAmount();
	}
	str.Format(_T("%d"), nSize);
	vec_str.push_back(str);
	str.Format(_T("%.02f"), (float)nTotalAmount / (float)g_nINT100);
	vec_str.push_back(str);
}

void CUserIncomeStatistics::UpdateList(std::vector<std::vector<CString>> & vec2_strData)
{
	m_listctrlEmployee.DeleteAllItems();
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 11)
		{
			for (int i = 0; i < nRow; ++i)
			{
				m_listctrlEmployee.InsertItem(i, vec2_strData[i][0]);
				for (int j = 1; j < nColumn; ++j)
					m_listctrlEmployee.SetItemText(i, j, vec2_strData[i][j]);
			}
		}
	}
}


void CUserIncomeStatistics::OnExportProgressUpdate(int nProgress)
{
	if (g_sz_p_wnd[WND_MAINFRAME])
	{
		g_sz_p_wnd[WND_MAINFRAME]->PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_UPDATEPROGRESS);
	}
}


void CUserIncomeStatistics::OnExportResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_EXPORTRESULT);
}

void CUserIncomeStatistics::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	StopRunExport();
}


afx_msg LRESULT CUserIncomeStatistics::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_EXPORTRESULT:
	{
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STATISTICS)->EnableWindow(TRUE);
		if (wParam)
			AfxMessageBox(_T("导出成功!"));
		else
			AfxMessageBox(_T("导出失败!"));
	}
	break;
	}
	return 0;
}
