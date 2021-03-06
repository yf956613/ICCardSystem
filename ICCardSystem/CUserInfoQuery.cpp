// CUserInfoQuery.cpp: 实现文件
//

#include "stdafx.h"
#include "ICCardSystem.h"
#include "CUserInfoQuery.h"
#include "afxdialogex.h"


// CUserInfoQuery 对话框

IMPLEMENT_DYNAMIC(CUserInfoQuery, CDialogEx)

CUserInfoQuery::CUserInfoQuery(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USERINFOQUERY, pParent)
	, m_bIsInitFinish(false)
{

}

CUserInfoQuery::~CUserInfoQuery()
{
}

void CUserInfoQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONDITION, m_comboCondition);
	DDX_Control(pDX, IDC_LIST_USER, m_listctrlUser);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_editKeyword);
}


BEGIN_MESSAGE_MAP(CUserInfoQuery, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CUserInfoQuery::OnBnClickedButtonSearch)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUserInfoQuery 消息处理程序


BOOL CUserInfoQuery::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	m_bIsInitFinish = true;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CUserInfoQuery::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strKeyword, strSql;
	m_editKeyword.GetWindowText(strKeyword);
	std::vector<CUser> vec_user;
	if (strKeyword.IsEmpty())
		strSql.Format(_T("SELECT * FROM User ORDER BY ID"));
	else
	{
		switch (m_comboCondition.GetCurSel())
		{
		case 0:strSql.Format(_T("SELECT * FROM User WHERE ID=%d ORDER BY ID"), _ttoi(strKeyword)); break;
		case 1:strSql.Format(_T("SELECT * FROM User WHERE Name LIKE '%%%s%%'ORDER BY ID"), strKeyword); break;
		case 2:strSql.Format(_T("SELECT * FROM User WHERE PhoneNo LIKE '%%%s%%'ORDER BY ID"), strKeyword); break;
		case 3:strSql.Format(_T("SELECT * FROM User WHERE Addr LIKE '%%%s%%'ORDER BY ID"), strKeyword); break;
		case 4:strSql.Format(_T("SELECT * FROM User WHERE License=%d ORDER BY ID"), _ttoi(strKeyword)); break;
		}
	}
	if (m_comboCondition.GetCurSel() != -1)
	{
		g_udb.Select(strSql, vec_user);
		UpdateList(vec_user);
	}
}


void CUserInfoQuery::InitUI()
{
	// TODO: 在此处添加实现代码.
	CString sz_strCondition[] = { _T("客户编号"),_T("客户名称"), _T("客户手机号"), _T("客户地址"), _T("授权码") };
	for (int i = 0; i < sizeof(sz_strCondition) / sizeof(CString); ++i)
		m_comboCondition.AddString(sz_strCondition[i]);
	m_comboCondition.SetCurSel(0);
	CString sz_strHead[] = { _T("客户编号"),_T("客户名称") ,_T("手机") ,_T("地址") ,_T("客户状态") ,_T("身份证") ,_T("授权码") ,_T("客户累计充值") ,_T("备注"),_T("IC卡号") ,_T("会员卡类型") ,_T("截止日期") ,_T("开卡押金") ,_T("IC卡累计充值"),_T("IC卡状态"),_T("开卡时间"),_T("退卡时间") };
	const int nMax = 45;
	CRect rc;
	m_listctrlUser.GetClientRect(&rc);
	m_listctrlUser.SetExtendedStyle(m_listctrlUser.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listctrlUser.InsertColumn(0, sz_strHead[0], LVCFMT_LEFT, rc.Width() * 2 / nMax, 0);
	m_listctrlUser.InsertColumn(1, sz_strHead[1], LVCFMT_LEFT, rc.Width() * 2 / nMax, 1);
	m_listctrlUser.InsertColumn(2, sz_strHead[2], LVCFMT_LEFT, rc.Width() * 4 / nMax, 2);
	m_listctrlUser.InsertColumn(3, sz_strHead[3], LVCFMT_LEFT, rc.Width() * 4 / nMax, 3);
	m_listctrlUser.InsertColumn(4, sz_strHead[4], LVCFMT_LEFT, rc.Width() * 2 / nMax, 4);
	m_listctrlUser.InsertColumn(5, sz_strHead[5], LVCFMT_LEFT, rc.Width() * 4 / nMax, 5);
	m_listctrlUser.InsertColumn(6, sz_strHead[6], LVCFMT_LEFT, rc.Width() * 2 / nMax, 6);
	m_listctrlUser.InsertColumn(7, sz_strHead[7], LVCFMT_LEFT, rc.Width() * 2 / nMax, 7);
	m_listctrlUser.InsertColumn(8, sz_strHead[8], LVCFMT_LEFT, rc.Width() * 4 / nMax, 8);
	m_listctrlUser.InsertColumn(9, sz_strHead[9], LVCFMT_LEFT, rc.Width() * 2 / nMax, 9);
	m_listctrlUser.InsertColumn(10, sz_strHead[10], LVCFMT_LEFT, rc.Width() * 2 / nMax, 10);
	m_listctrlUser.InsertColumn(11, sz_strHead[11], LVCFMT_LEFT, rc.Width() * 2 / nMax, 11);
	m_listctrlUser.InsertColumn(12, sz_strHead[12], LVCFMT_LEFT, rc.Width() * 2 / nMax, 12);
	m_listctrlUser.InsertColumn(13, sz_strHead[13], LVCFMT_LEFT, rc.Width() * 2 / nMax, 13);
	m_listctrlUser.InsertColumn(14, sz_strHead[14], LVCFMT_LEFT, rc.Width() * 2 / nMax, 14);
	m_listctrlUser.InsertColumn(15, sz_strHead[15], LVCFMT_LEFT, rc.Width() * 3 / nMax, 15);
	m_listctrlUser.InsertColumn(16, sz_strHead[16], LVCFMT_LEFT, rc.Width() * 3 / nMax, 16);
}


void CUserInfoQuery::UpdateList(const std::vector<CUser> & vec_user)
{
	m_listctrlUser.DeleteAllItems();
	int nSizeSave = 0;
	int nSize = vec_user.size();
	for (int i = 0; i < nSize; ++i)
	{
		CString strID, strLicense, strState,strCardNo,strTotalAmount,strDeposit;
		strID.Format(_T("%d"), vec_user[i].GetID());
		if (vec_user[i].GetLicense())
			strLicense.Format(_T("%d"), vec_user[i].GetLicense());
		else
			strLicense.Empty();
		strTotalAmount.Format(_T("%.02f"), (float)(vec_user[i].GetTotalAmount() / (float)g_nINT100));
		CString sz_strState[] = { _T("0-未开卡"),_T("1-已开卡"), _T("2-已退卡") };
		strState = sz_strState[vec_user[i].GetState()];
		CString strSql;
		std::vector<CCard> vec_card;
		strSql.Format(_T("SELECT * FROM Card WHERE UserID=%d ORDER BY No"), vec_user[i].GetID());
		g_cdb.Select(strSql, vec_card);
		int nSizeCard = vec_card.size();
		if (nSizeCard)
		{
			for (int j = 0; j < nSizeCard; ++j)
			{
				m_listctrlUser.InsertItem(nSizeSave + j, strID);
				m_listctrlUser.SetItemText(nSizeSave + j, 1, vec_user[i].GetName());
				m_listctrlUser.SetItemText(nSizeSave + j, 2, vec_user[i].GetPhoneNo());
				m_listctrlUser.SetItemText(nSizeSave + j, 3, vec_user[i].GetAddr());
				m_listctrlUser.SetItemText(nSizeSave + j, 4, strState);
				m_listctrlUser.SetItemText(nSizeSave + j, 5, vec_user[i].GetIDNo());
				m_listctrlUser.SetItemText(nSizeSave + j, 6, strLicense);
				m_listctrlUser.SetItemText(nSizeSave + j, 7, strTotalAmount);
				m_listctrlUser.SetItemText(nSizeSave + j, 8, vec_user[i].GetNoteInfo());
				CString strCardType, strDeposit, strCardTotalAmount, strCardState;
				CString sz_strCardType[] = { _T("时长卡"),_T("充值卡") };
				if (vec_card[j].GetType() == CCardOperator::TYPE_USER_TIME)
					strCardType = sz_strCardType[0];
				else
					strCardType = sz_strCardType[1];
				strDeposit.Format(_T("%.02f"), (float)(vec_card[j].GetDeposit()) / (float)g_nINT100);
				strCardTotalAmount.Format(_T("%.02f"), (float)(vec_card[j].GetTotalAmount() / (float)g_nINT100));
				strCardState = sz_strState[vec_card[j].GetState()];
				m_listctrlUser.SetItemText(nSizeSave + j, 9, vec_card[j].GetNo());
				m_listctrlUser.SetItemText(nSizeSave + j, 10, strCardType);
				m_listctrlUser.SetItemText(nSizeSave + j, 11, vec_card[j].GetDeadline());
				m_listctrlUser.SetItemText(nSizeSave + j, 12, strDeposit);
				m_listctrlUser.SetItemText(nSizeSave + j, 13, strCardTotalAmount);
				m_listctrlUser.SetItemText(nSizeSave + j, 14, strCardState);
				m_listctrlUser.SetItemText(nSizeSave + j, 15, vec_card[j].GetTimeOpenCard());
				m_listctrlUser.SetItemText(nSizeSave + j, 16, vec_card[j].GetTimeCloseCard());
			}
		}
		else
		{
			m_listctrlUser.InsertItem(nSizeSave, strID);
			m_listctrlUser.SetItemText(nSizeSave, 1, vec_user[i].GetName());
			m_listctrlUser.SetItemText(nSizeSave, 2, vec_user[i].GetPhoneNo());
			m_listctrlUser.SetItemText(nSizeSave, 3, vec_user[i].GetAddr());
			m_listctrlUser.SetItemText(nSizeSave, 4, strState);
			m_listctrlUser.SetItemText(nSizeSave, 5, vec_user[i].GetIDNo());
			m_listctrlUser.SetItemText(nSizeSave, 6, strLicense);
			m_listctrlUser.SetItemText(nSizeSave, 7, strTotalAmount);
			m_listctrlUser.SetItemText(nSizeSave, 8, vec_user[i].GetNoteInfo());
			nSizeCard = 1;
		}
		nSizeSave += nSizeCard;
	}
}

void CUserInfoQuery::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish&&nType != SIZE_MINIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);
		const int nMax = 45;
		m_listctrlUser.SetRedraw(FALSE);
		m_listctrlUser.SetColumnWidth(0, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(1, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(2, rc.Width() * 4 / nMax);
		m_listctrlUser.SetColumnWidth(3, rc.Width() * 4 / nMax);
		m_listctrlUser.SetColumnWidth(4, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(5, rc.Width() * 4 / nMax);
		m_listctrlUser.SetColumnWidth(6, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(7, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(8, rc.Width() * 4 / nMax);
		m_listctrlUser.SetColumnWidth(9, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(10, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(11, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(12, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(13, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(14, rc.Width() * 2 / nMax);
		m_listctrlUser.SetColumnWidth(15, rc.Width() * 3 / nMax);
		m_listctrlUser.SetColumnWidth(16, rc.Width() * 3 / nMax);
		m_listctrlUser.SetRedraw(TRUE);
	}
}
