// CUserInfoEdit.cpp: 实现文件
//

#include "stdafx.h"
#include "ICCardSystem.h"
#include "CUserInfoEdit.h"
#include "afxdialogex.h"
#include "CUserOpenCard.h"

// CUserInfoEdit 对话框

IMPLEMENT_DYNAMIC(CUserInfoEdit, CDialogEx)

CUserInfoEdit::CUserInfoEdit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USERINFOEDIT, pParent)
	, m_bSaveFromAdd(false)
{

}

CUserInfoEdit::~CUserInfoEdit()
{
}

void CUserInfoEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONDITION, m_comboCondition);
	DDX_Control(pDX, IDC_LIST_USER, m_listctrlUser);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_editKeyword);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_IDNO, m_editIDNo);
	DDX_Control(pDX, IDC_EDIT_PHONENO, m_editPhoneNo);
	DDX_Control(pDX, IDC_EDIT_ADDR, m_editAddr);
	DDX_Control(pDX, IDC_EDIT_LICENSE, m_editLicense);
	DDX_Control(pDX, IDC_EDIT_TOTALAMOUNT, m_editTotalAmount);
	DDX_Control(pDX, IDC_EDIT_STATE, m_editState);
	DDX_Control(pDX, IDC_EDIT_NOTEINFO, m_editNoteInfo);
}


BEGIN_MESSAGE_MAP(CUserInfoEdit, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CUserInfoEdit::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CUserInfoEdit::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CUserInfoEdit::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CUserInfoEdit::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CUserInfoEdit::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_OPENCARD, &CUserInfoEdit::OnBnClickedButtonOpencard)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USER, &CUserInfoEdit::OnNMClickListUser)
END_MESSAGE_MAP()


// CUserInfoEdit 消息处理程序


BOOL CUserInfoEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitEdit();
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CUserInfoEdit::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strKeyword,strSql;
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


void CUserInfoEdit::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSaveFromAdd = true;
	SetIntoEdit();
	SetEditEmpty();
	CString strID;
	std::vector<CUser> vec_user;
	g_udb.Select(_T("SELECT * FROM User ORDER BY ID DESC LIMIT 1"), vec_user);
	if (vec_user.size() == 1)
		strID.Format(_T("%d"), vec_user[0].GetID() + 1);
	else
		strID = _T("1");
	m_editID.SetWindowText(strID);
	m_editTotalAmount.SetWindowText(_T("0.00"));
	m_editState.SetWindowText(_T("0-未开卡"));
}


void CUserInfoEdit::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strID;
	m_editID.GetWindowText(strID);
	if (strID.IsEmpty())
	{
		AfxMessageBox(_T("请从列表中选择客户信息进行修改!"));
		return;
	}
	m_bSaveFromAdd = false;
	SetIntoEdit();
	std::vector<CUser> vec_user;
	CString strSql;
	strSql.Format(_T("SELECT * FROM User WHERE ID=%d ORDER BY ID"), _ttoi(strID));
	g_udb.Select(strSql, vec_user);
	if (vec_user.size() == 1)
	{
		if(vec_user[0].GetState()!=CUser::STATE_UNOPEN)
			m_editLicense.EnableWindow(FALSE);
	}
}


void CUserInfoEdit::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	SetOutOfEdit();
	SetEditEmpty();
	POSITION pos=m_listctrlUser.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nItem = m_listctrlUser.GetNextSelectedItem(pos);
		int nID = _ttoi(m_listctrlUser.GetItemText(nItem, 0));
		InsertEditByID(nID);
	}
}


void CUserInfoEdit::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<CString> vec_str;
	GetEditText(vec_str);
	if (vec_str.size()==9)
	{
		CUser user(_ttoi(vec_str[0]), vec_str[1], vec_str[2], vec_str[3], vec_str[4], _ttoi(vec_str[5]), _ttoi(vec_str[6]), _ttoi(vec_str[7]), vec_str[8], g_employee.GetID(), _T(""), g_employee.GetID(), _T(""));
		if (m_bSaveFromAdd)
		{
			if (g_udb.Insert(user))
				AfxMessageBox(_T("客户信息添加成功!"));
			else
				AfxMessageBox(_T("客户信息添加失败!"));
		}
		else
		{
			if (g_udb.UpdateModifyInfo(user))
				AfxMessageBox(_T("客户信息修改成功!"));
			else
				AfxMessageBox(_T("客户信息修改失败!"));
		}
	}
	OnBnClickedButtonCancel();
	OnBnClickedButtonSearch();
}


void CUserInfoEdit::OnBnClickedButtonOpencard()
{
	// TODO: 在此添加控件通知处理程序代码
	CUserOpenCard uocDlg;
	uocDlg.DoModal();
}


void CUserInfoEdit::OnNMClickListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int & nItem = pNMItemActivate->iItem;
	if (nItem != -1)
	{
		int nID = _ttoi(m_listctrlUser.GetItemText(nItem, 0));
		InsertEditByID(nID);
	}
}


void CUserInfoEdit::InitUI()
{
	// TODO: 在此处添加实现代码.
	CString sz_strCondition[] = { _T("客户编号"),_T("客户名称"), _T("客户手机号"), _T("客户地址"), _T("授权码") };
	for(int i=0;i<sizeof(sz_strCondition)/sizeof(CString);++i)
		m_comboCondition.AddString(sz_strCondition[i]);
	m_comboCondition.SetCurSel(0);
	const int nMax = 20;
	CRect rc;
	m_listctrlUser.GetClientRect(&rc);
	m_listctrlUser.SetExtendedStyle(m_listctrlUser.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listctrlUser.InsertColumn(0, _T("客户编号"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 0);
	m_listctrlUser.InsertColumn(1, _T("客户名称"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 1);
	m_listctrlUser.InsertColumn(2, _T("手机"), LVCFMT_LEFT, rc.Width() * 4 / nMax, 2);
	m_listctrlUser.InsertColumn(3, _T("地址"), LVCFMT_LEFT, rc.Width() * 4 / nMax, 3);
	m_listctrlUser.InsertColumn(4, _T("客户状态"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 4);
	m_listctrlUser.InsertColumn(5, _T("授权码"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 5);
	m_listctrlUser.InsertColumn(6, _T("备注"), LVCFMT_LEFT, rc.Width() * 3 / nMax, 6);
	SetOutOfEdit();
}


void CUserInfoEdit::InitEdit()
{
	CEdit * p_edit = NULL;
	p_edit = &m_editID;
	m_vec_edit.push_back(p_edit);
	p_edit = &m_editName;
	m_vec_edit.push_back(p_edit);
	p_edit = &m_editIDNo;
	m_vec_edit.push_back(p_edit);
	p_edit = &m_editPhoneNo;
	m_vec_edit.push_back(p_edit);
	p_edit = &m_editAddr;
	m_vec_edit.push_back(p_edit);
	p_edit = &m_editLicense;
	m_vec_edit.push_back(p_edit);
	p_edit = &m_editTotalAmount;
	m_vec_edit.push_back(p_edit);
	p_edit = &m_editState;
	m_vec_edit.push_back(p_edit);
	p_edit = &m_editNoteInfo;
	m_vec_edit.push_back(p_edit);
}


void CUserInfoEdit::UpdateList(const std::vector<CUser> & vec_user)
{
	m_listctrlUser.DeleteAllItems();
	int nSize = vec_user.size();
	for (int i = 0; i < nSize; ++i)
	{
		CString strID,strLicense,strState;
		strID.Format(_T("%d"), vec_user[i].GetID());
		if (vec_user[i].GetLicense())
			strLicense.Format(_T("%d"), vec_user[i].GetLicense());
		else
			strLicense.Empty();
		CString sz_strState[] = { _T("0-未开卡"),_T("1-已开卡"), _T("2-已退卡") };
		strState = sz_strState[vec_user[i].GetState()];
		m_listctrlUser.InsertItem(i, strID);
		m_listctrlUser.SetItemText(i, 1, vec_user[i].GetName());
		m_listctrlUser.SetItemText(i, 2, vec_user[i].GetPhoneNo());
		m_listctrlUser.SetItemText(i, 3, vec_user[i].GetAddr());
		m_listctrlUser.SetItemText(i, 4, strState);
		m_listctrlUser.SetItemText(i, 5, strLicense);
		m_listctrlUser.SetItemText(i, 6, vec_user[i].GetNoteInfo());
	}
}


void CUserInfoEdit::GetEditText(std::vector<CString> & vec_str)
{
	int nSize = m_vec_edit.size();
	for (int i = 0; i < nSize; ++i)
	{
		CString str;
		m_vec_edit[i]->GetWindowText(str);
		vec_str.push_back(str);
	}
}


void CUserInfoEdit::SetEditEmpty()
{
	int nSize = m_vec_edit.size();
	for (int i = 0; i < nSize; ++i)
		m_vec_edit[i]->SetWindowText(_T(""));
}


void CUserInfoEdit::SetIntoEdit()
{
	int nSize = m_vec_edit.size();
	for (int i = 1; i < 5; ++i)
		m_vec_edit[i]->EnableWindow(TRUE);
	m_editNoteInfo.EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	m_listctrlUser.EnableWindow(FALSE);
}


void CUserInfoEdit::SetOutOfEdit()
{
	int nSize = m_vec_edit.size();
	for (int i = 0; i < nSize; ++i)
		m_vec_edit[i]->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	m_listctrlUser.EnableWindow(TRUE);
}


void CUserInfoEdit::InsertEdit(const CUser & user)
{
	CString str;
	str.Format(_T("%d"), user.GetID());
	m_editID.SetWindowText(str);
	m_editName.SetWindowText(user.GetName());
	m_editIDNo.SetWindowText(user.GetIDNo());
	m_editPhoneNo.SetWindowText(user.GetPhoneNo());
	m_editAddr.SetWindowText(user.GetAddr());
	if (user.GetLicense())
		str.Format(_T("%d"), user.GetLicense());
	else
		str.Empty();
	m_editLicense.SetWindowText(str);
	str.Format(_T("%.02f"), (float)(user.GetTotalAmount()/ (float)g_nINT100));
	m_editTotalAmount.SetWindowText(str);
	CString sz_strState[] = { _T("0-未开卡"),_T("1-已开卡"), _T("2-已退卡") };
	str = sz_strState[user.GetState()];
	m_editState.SetWindowText(str);
	m_editNoteInfo.SetWindowText(user.GetNoteInfo());
}


void CUserInfoEdit::InsertEditByID(int nID)
{
	std::vector<CUser> vec_user;
	CString strSql;
	strSql.Format(_T("SELECT * FROM User WHERE ID=%d ORDER BY ID"), nID);
	g_udb.Select(strSql, vec_user);
	if (vec_user.size() == 1)
	{
		InsertEdit(vec_user[0]);
	}
}