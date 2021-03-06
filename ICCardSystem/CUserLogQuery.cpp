// CUserLogQuery.cpp: 实现文件
//

#include "stdafx.h"
#include "ICCardSystem.h"
#include "CUserLogQuery.h"
#include "afxdialogex.h"


// CUserLogQuery 对话框

IMPLEMENT_DYNAMIC(CUserLogQuery, CDialogEx)

CUserLogQuery::CUserLogQuery(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USERLOGQUERY, pParent)
	, m_bIsInitFinish(false)
	, m_strTtitle(_T("会员卡日志"))
	, m_strSubTtitle(_T(""))
{

}

CUserLogQuery::~CUserLogQuery()
{
}

void CUserLogQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONDITION, m_comboCondition);
	DDX_Control(pDX, IDC_LIST_USER, m_listctrlUser);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_editKeyword);
	DDX_Control(pDX, IDC_COMBO_ITEM, m_comboItem);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_datetimeStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_datetimeEnd);
}


BEGIN_MESSAGE_MAP(CUserLogQuery, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CUserLogQuery::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CUserLogQuery::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_RADIO_THISYEAR, &CUserLogQuery::OnBnClickedRadioThisyear)
	ON_BN_CLICKED(IDC_RADIO_LASTYEAR, &CUserLogQuery::OnBnClickedRadioLastyear)
	ON_BN_CLICKED(IDC_RADIO_THISMONTH, &CUserLogQuery::OnBnClickedRadioThismonth)
	ON_BN_CLICKED(IDC_RADIO_LASTMONTH, &CUserLogQuery::OnBnClickedRadioLastmonth)
	ON_BN_CLICKED(IDC_RADIO_TODAY, &CUserLogQuery::OnBnClickedRadioToday)
	ON_BN_CLICKED(IDC_RADIO_YESTERDAY, &CUserLogQuery::OnBnClickedRadioYesterday)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CUserLogQuery::OnBnClickedButtonExport)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MSGRECVPRO, &CUserLogQuery::OnMsgrecvpro)
END_MESSAGE_MAP()


// CUserLogQuery 消息处理程序


BOOL CUserLogQuery::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	m_bIsInitFinish = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CUserLogQuery::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSql, strSelect, strWhere, strTableLeft, strTableRight(_T("User"));
	if (m_comboItem.GetCurSel())
	{
		strSelect = GetSqlSelect(m_comboItem.GetCurSel(), strTableLeft);
		strWhere = GetSqlWhere(m_comboCondition.GetCurSel(), strTableLeft);
		strSql.Format(_T("%s%s ORDER BY %s.ID,%s.Time "), strSelect, strWhere, strTableRight, strTableLeft);
	}
	else
	{
		CString str;
		for (int i = 1; i < 5; ++i)
		{	
			strSelect = GetSqlSelect(i, strTableLeft);
			strWhere = GetSqlWhere(m_comboCondition.GetCurSel(), strTableLeft);
			str = strSelect + strWhere;
			if (i < 4)
				strSql += str + _T(" UNION ");
			else
				strSql += str;
		}
		str = strSql;
		strSql.Format(_T("SELECT * FROM ( %s ) ORDER BY ID,Time "), str);
	}
	CUnionDB udb;
	udb.Select(strSql, m_vec2_strDataExport);
	UpdateList(m_vec2_strDataExport);
}


void CUserLogQuery::OnBnClickedButtonPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_sz_p_wnd[WND_MAINVIEW])
	{
		g_sz_p_wnd[WND_MAINVIEW]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strTtitle, MSGUSER_SETTITLE);
		g_sz_p_wnd[WND_MAINVIEW]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_strSubTtitle, MSGUSER_SETSUBTITLE);
		g_sz_p_wnd[WND_MAINVIEW]->PostMessage(WM_MSGRECVPRO, (WPARAM)&m_vec2_strDataExport, MSGUSER_PRINTUSERLOG);
	}
}


void CUserLogQuery::OnBnClickedButtonExport()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szFilter;
	szFilter = _T("Excel文件(*.xlsx)|*.xlsx|Excel文件(*.xls)|*.xls|所有文件(*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("xlsx"), m_strTtitle + _T("_") + m_strSubTtitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		m_strDestPath = fileDlg.GetPathName();
		m_strDemoPath = ZUtil::GetExeCatalogPath() + _T("\\DemoLog.xlsx");
		ZExcelExport excleexport(this);
		if (excleexport.Start())
		{
			GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);
		}
		else
			AfxMessageBox(_T("导出失败!"));
	}
}


void CUserLogQuery::OnBnClickedRadioThisyear()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart = CTime(timeCur.GetYear(), 1, 1, 0, 0, 0);
	CTime timeEnd = CTime(timeCur.GetYear(), timeCur.GetMonth(), timeCur.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserLogQuery::OnBnClickedRadioLastyear()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTimeSpan timespan(1, 0, 0, 0);
	CTime time = CTime(timeCur.GetYear(), 1, 1, 0, 0, 0)-timespan;
	CTime timeStart = CTime(time.GetYear(), 1, 1, 0, 0, 0);
	CTime timeEnd = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserLogQuery::OnBnClickedRadioThismonth()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart = CTime(timeCur.GetYear(), timeCur.GetMonth(), 1, 0, 0, 0);
	CTime timeEnd = CTime(timeCur.GetYear(), timeCur.GetMonth(), timeCur.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserLogQuery::OnBnClickedRadioLastmonth()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTimeSpan timespan(1, 0, 0, 0);
	CTime time = CTime(timeCur.GetYear(), timeCur.GetMonth(), 1, 0, 0, 0)- timespan;
	CTime timeStart = CTime(time.GetYear(), time.GetMonth(), 1, 0, 0, 0);
	CTime timeEnd= CTime(time.GetYear(), time.GetMonth(), time.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserLogQuery::OnBnClickedRadioToday()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart = CTime(timeCur.GetYear(), timeCur.GetMonth(), timeCur.GetDay(), 0, 0, 0);
	CTime timeEnd = CTime(timeCur.GetYear(), timeCur.GetMonth(), timeCur.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserLogQuery::OnBnClickedRadioYesterday()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime timeCur = CTime::GetCurrentTime();
	CTimeSpan timespan(1,0,0,0);
	CTime time = timeCur-timespan;
	CTime timeStart = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), 0, 0, 0);
	CTime timeEnd = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), 23, 59, 59);
	m_datetimeStart.SetTime(&timeStart);
	m_datetimeEnd.SetTime(&timeEnd);
}


void CUserLogQuery::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);
		const int nMax = 27;
		m_listctrlUser.SetRedraw(FALSE);
		m_listctrlUser.SetColumnWidth(0, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(1, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(2, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(3, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(4, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(5, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(6, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(7, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(8, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(9, rc.Width() * 4 / nMax);
		m_listctrlUser.SetColumnWidth(10, rc.Width() * 4 / nMax);
		m_listctrlUser.SetRedraw(TRUE);
	}
}


void CUserLogQuery::InitUI()
{
	// TODO: 在此处添加实现代码.
	CString sz_strCondition[] = { _T("客户编号"),_T("客户名称"), _T("客户手机号"), _T("客户地址"), _T("授权码") };
	for (int i = 0; i < sizeof(sz_strCondition) / sizeof(CString); ++i)
		m_comboCondition.AddString(sz_strCondition[i]);
	m_comboCondition.SetCurSel(0);
	CString sz_strHead[] = { _T("客户编号"),_T("客户名称"),_T("IC卡号"), _T("操作"), _T("原金额"), _T("操作金额"),_T("新金额"),_T("押金"),  _T("操作员"), _T("操作时间"), _T("备注信息") };
	const int nMax = 27;
	CRect rc;
	m_listctrlUser.GetClientRect(&rc);
	m_listctrlUser.SetExtendedStyle(m_listctrlUser.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listctrlUser.InsertColumn(0, sz_strHead[0], LVCFMT_LEFT, rc.Width() * 2 / nMax, 0);
	m_listctrlUser.InsertColumn(1, sz_strHead[1], LVCFMT_LEFT, rc.Width() * 2 / nMax, 1);
	m_listctrlUser.InsertColumn(2, sz_strHead[2], LVCFMT_LEFT, rc.Width() * 2 / nMax, 2);
	m_listctrlUser.InsertColumn(3, sz_strHead[3], LVCFMT_LEFT, rc.Width() * 2 / nMax, 3);
	m_listctrlUser.InsertColumn(4, sz_strHead[4], LVCFMT_LEFT, rc.Width() * 2 / nMax, 4);
	m_listctrlUser.InsertColumn(5, sz_strHead[5], LVCFMT_LEFT, rc.Width() * 2 / nMax, 5);
	m_listctrlUser.InsertColumn(6, sz_strHead[6], LVCFMT_LEFT, rc.Width() * 2 / nMax, 6);
	m_listctrlUser.InsertColumn(7, sz_strHead[7], LVCFMT_LEFT, rc.Width() * 2 / nMax, 7);
	m_listctrlUser.InsertColumn(8, sz_strHead[8], LVCFMT_LEFT, rc.Width() * 2 / nMax, 8);
	m_listctrlUser.InsertColumn(9, sz_strHead[9], LVCFMT_LEFT, rc.Width() * 4 / nMax, 9);
	m_listctrlUser.InsertColumn(10, sz_strHead[10], LVCFMT_LEFT, rc.Width() * 4 / nMax, 10);
	CString sz_strItem[] = { _T("所有"), _T("充值"), _T("退款"), _T("开卡押金"),_T("退押金") };
	for (int i = 0; i < sizeof(sz_strItem) / sizeof(CString); ++i)
		m_comboItem.AddString(sz_strItem[i]);
	m_comboItem.SetCurSel(0);
	CheckDlgButton(IDC_RADIO_TODAY, TRUE);
	OnBnClickedRadioToday();
}


CString CUserLogQuery::GetSqlSelect(int nSelItem, CString & strTableLeft)
{
	CString strSelect, strTableRight(_T("User")), strOperator;
	switch (nSelItem)
	{
	case 1:
	{
		strTableLeft = _T("PrestoreRecord");
		strOperator = _T("充值");
	}
	break;
	case 2:
	{
		strTableLeft = _T("RefundRecord");
		strOperator = _T("退款");
	}
	break;
	case 3:
	{
		strTableLeft = _T("InitDepositRecord");
		strOperator = _T("开卡押金");
	}
	break;
	case 4:
	{
		strTableLeft = _T("ReturnDepositRecord");
		strOperator = _T("退押金");
	}
	break;
	}
	strSelect.Format(_T("SELECT %s.ID, %s.Name,%s.CardNo, '%s', %s.AmountOld, %s.Amount, %s.AmountNew,%s.Deposit, %s.EmployeeID, %s.Time, %s.NoteInfo FROM  %s LEFT JOIN  %s on  %s.ID= %s.UserID "), strTableRight, strTableRight, strTableLeft, strOperator, strTableLeft, strTableLeft, strTableLeft, strTableLeft, strTableLeft, strTableLeft, strTableLeft, strTableLeft, strTableRight, strTableRight, strTableLeft);
	return strSelect;
}


CString CUserLogQuery::GetSqlWhere(int nSelCondition, const CString & strTableLeft)
{
	CString strKeyword, strWhere;
	m_editKeyword.GetWindowText(strKeyword);
	CTime timeStart, timeEnd;
	m_datetimeStart.GetTime(timeStart);
	m_datetimeEnd.GetTime(timeEnd);
	m_strSubTtitle.Format(_T("查询日期：%s - %s"), timeStart.Format(_T("%Y%m%d")), timeEnd.Format(_T("%Y%m%d")));
	if (strKeyword.IsEmpty())
	{
		strWhere.Format(_T("WHERE %s.Time>='%s' AND %s.Time<='%s' "), strTableLeft, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), strTableLeft, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S")));
	}
	else
	{
		switch (nSelCondition)
		{
		case 0:strWhere.Format(_T("WHERE User.ID=%d AND %s.Time>='%s' AND %s.Time<='%s' "), _ttoi(strKeyword), strTableLeft, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), strTableLeft, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"))); break;
		case 1:strWhere.Format(_T("WHERE User.Name LIKE '%%%s%%' AND %s.Time>='%s' AND %s.Time<='%s' "), strKeyword, strTableLeft, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), strTableLeft, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"))); break;
		case 2:strWhere.Format(_T("WHERE User.PhoneNo LIKE '%%%s%%' AND %s.Time>='%s' AND %s.Time<='%s' "), strKeyword, strTableLeft, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), strTableLeft, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"))); break;
		case 3:strWhere.Format(_T("WHERE User.Addr LIKE '%%%s%%' AND %s.Time>='%s' AND %s.Time<='%s' "), strKeyword, strTableLeft, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), strTableLeft, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"))); break;
		case 4:strWhere.Format(_T("WHERE User.License=%d AND %s.Time>='%s' AND %s.Time<='%s' "), _ttoi(strKeyword), strTableLeft, timeStart.Format(_T("%Y-%m-%d %H:%M:%S")), strTableLeft, timeEnd.Format(_T("%Y-%m-%d %H:%M:%S"))); break;
		}
	}
	return strWhere;
}


void CUserLogQuery::UpdateList(std::vector<std::vector<CString>> & vec2_strData)
{
	m_listctrlUser.DeleteAllItems();
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 11)
		{
			CString sz_strHead[] = { _T("客户编号"),_T("客户名称"),_T("IC卡号"), _T("操作"), _T("原金额"), _T("操作金额"),_T("新金额"),_T("押金"), _T("操作员"), _T("操作时间"), _T("备注信息") };
			for (int k = 0; k < nColumn; ++k)
				vec2_strData[0][k] = sz_strHead[k];
			for (int i = 1; i < nRow; ++i)
			{
				m_listctrlUser.InsertItem(i-1, vec2_strData[i][0]);
				for (int j = 1; j < nColumn; ++j)
				{
					CString str(vec2_strData[i][j]);
					if (j >= 4 && j <= 7)
					{
						str.Format(_T("%.02f"), (float)(_ttoi(str) / (float)g_nINT100));
						vec2_strData[i][j] = str;
					}
					m_listctrlUser.SetItemText(i - 1, j, str);
				}
			}
		}
	}
}


void CUserLogQuery::OnExportProgressUpdate(int nProgress)
{
	if (g_sz_p_wnd[WND_MAINFRAME])
	{
		g_sz_p_wnd[WND_MAINFRAME]->PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_UPDATEPROGRESS);
	}
}


void CUserLogQuery::OnExportResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_EXPORTRESULT);
}

void CUserLogQuery::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	StopRunExport();
}


afx_msg LRESULT CUserLogQuery::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_EXPORTRESULT:
	{
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(TRUE);
		if (wParam)
			AfxMessageBox(_T("导出成功!"));
		else
			AfxMessageBox(_T("导出失败!"));
	}
	break;
	}
	return 0;
}
