// CEmployeeEdit.cpp: 实现文件
//

#include "stdafx.h"
#include "ICCardSystem.h"
#include "CEmployeeEdit.h"
#include "afxdialogex.h"


// CEmployeeEdit 对话框

IMPLEMENT_DYNAMIC(CEmployeeEdit, CDialogEx)

CEmployeeEdit::CEmployeeEdit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EMPLOYEEEDIT, pParent)
	, m_bSaveFromAdd(false)
{

}

CEmployeeEdit::~CEmployeeEdit()
{
}

void CEmployeeEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_COMBO_GRADEID, m_comboGradeID);
	DDX_Control(pDX, IDC_CHECK_ISUSING, m_checkIsUsing);
	DDX_Control(pDX, IDC_LIST_EMPLOYEE, m_listctrlEmployee);
}


BEGIN_MESSAGE_MAP(CEmployeeEdit, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEmployeeEdit::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CEmployeeEdit::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CEmployeeEdit::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEmployeeEdit::OnBnClickedButtonSave)
	ON_NOTIFY(NM_CLICK, IDC_LIST_EMPLOYEE, &CEmployeeEdit::OnNMClickListEmployee)
END_MESSAGE_MAP()


// CEmployeeEdit 消息处理程序


BOOL CEmployeeEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CEmployeeEdit::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSaveFromAdd = true;
	SetIntoEdit();
	CString strID;
	std::vector<CEmployee> vec_employee;
	m_edb.Select(_T("SELECT * FROM Employee ORDER BY ID DESC LIMIT 1"), vec_employee);
	if (vec_employee.size() == 1)
		strID.Format(_T("%d"), vec_employee[0].GetID() + 1);
	else
		strID = _T("100");
	m_editID.SetWindowText(strID);
	m_editName.SetWindowText(_T(""));
	m_editPassword.SetWindowText(_T(""));
	m_comboGradeID.SetCurSel(1);
	m_checkIsUsing.SetCheck(TRUE);
}


void CEmployeeEdit::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_editID.GetWindowText(str);
	if (str.IsEmpty())
	{
		AfxMessageBox(_T("请从列表中选择员工信息进行修改!"));
		return;
	}
	else if (_ttoi(str) == 99)
	{
		AfxMessageBox(_T("默认管理员无法修改!"));
		return;
	}
	m_bSaveFromAdd = false;
	SetIntoEdit();
}


void CEmployeeEdit::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	SetOutOfEdit();
	m_editID.SetWindowText(_T(""));
	m_editName.SetWindowText(_T(""));
	m_editPassword.SetWindowText(_T(""));
	POSITION pos = m_listctrlEmployee.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nItem = m_listctrlEmployee.GetNextSelectedItem(pos);
		int nID = _ttoi(m_listctrlEmployee.GetItemText(nItem, 0));
		InsertEditByID(nID);
	}
}


void CEmployeeEdit::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strID, strName, strPassword, strGrandID;
	m_editID.GetWindowText(strID);
	m_editName.GetWindowText(strName);
	m_editPassword.GetWindowText(strPassword);
	CEmployee employee(_ttoi(strID),strName,strPassword, m_comboGradeID.GetCurSel(), m_checkIsUsing.GetCheck());
	if (m_bSaveFromAdd)
	{
		if (m_edb.Insert(employee))
			AfxMessageBox(_T("员工信息添加成功!"));
		else
			AfxMessageBox(_T("员工信息添加失败!"));
	}
	else
	{
		if (m_edb.Update(employee))
			AfxMessageBox(_T("员工信息修改成功!"));
		else
			AfxMessageBox(_T("员工信息修改失败!"));
	}
	OnBnClickedButtonCancel();
	RefreshList();
}


void CEmployeeEdit::InitUI()
{
	// TODO: 在此处添加实现代码.
	CString sz_strGradeID[] = { _T("0-管理员"), _T("1-操作员") };
	for (int i = 0; i < sizeof(sz_strGradeID) / sizeof(CString); ++i)
		m_comboGradeID.AddString(sz_strGradeID[i]);
	m_comboGradeID.SetCurSel(1);
	const int nMax = 10;
	CRect rc;
	m_listctrlEmployee.GetClientRect(&rc);
	m_listctrlEmployee.SetExtendedStyle(m_listctrlEmployee.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listctrlEmployee.InsertColumn(0, _T("员工编号"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 0);
	m_listctrlEmployee.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 1);
	m_listctrlEmployee.InsertColumn(2, _T("等级"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 2);
	m_listctrlEmployee.InsertColumn(3, _T("是否启用"), LVCFMT_LEFT, rc.Width() * 3 / nMax, 3);
	SetOutOfEdit();
	RefreshList();
}


void CEmployeeEdit::UpdateList(const std::vector<CEmployee> & vec_employee)
{
	m_listctrlEmployee.DeleteAllItems();
	int nSize = vec_employee.size();
	for (int i = 0; i < nSize; ++i)
	{
		CString strID, strGradeID,strIsUsing;
		strID.Format(_T("%d"), vec_employee[i].GetID());
		CString sz_strGradeID[] = { _T("0-管理员"), _T("1-操作员") };
		if (vec_employee[i].GetGradeID() < 2)
			strGradeID = sz_strGradeID[vec_employee[i].GetGradeID()];
		if (vec_employee[i].IsUsing())
			strIsUsing = _T("是");
		else
			strIsUsing = _T("否");
		m_listctrlEmployee.InsertItem(i, strID);
		m_listctrlEmployee.SetItemText(i, 1, vec_employee[i].GetName());
		m_listctrlEmployee.SetItemText(i, 2, strGradeID);
		m_listctrlEmployee.SetItemText(i, 3, strIsUsing);
	}
}


void CEmployeeEdit::RefreshList()
{
	std::vector<CEmployee> vec_employee;
	m_edb.Select(_T("SELECT * FROM Employee ORDER BY ID"), vec_employee);
	UpdateList(vec_employee);
}


void CEmployeeEdit::SetIntoEdit()
{
	m_editID.EnableWindow(FALSE);
	m_editName.EnableWindow(TRUE);
	m_editPassword.EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	m_listctrlEmployee.EnableWindow(FALSE);
	//m_comboGradeID.EnableWindow(TRUE);
	m_checkIsUsing.EnableWindow(TRUE);
}


void CEmployeeEdit::SetOutOfEdit()
{
	m_editID.EnableWindow(FALSE);
	m_editName.EnableWindow(FALSE);
	m_editPassword.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	m_listctrlEmployee.EnableWindow(TRUE);
	//m_comboGradeID.EnableWindow(FALSE);
	m_checkIsUsing.EnableWindow(FALSE);
}


void CEmployeeEdit::InsertEdit(const CEmployee & employee)
{
	CString str;
	str.Format(_T("%d"), employee.GetID());
	m_editID.SetWindowText(str);
	m_editName.SetWindowText(employee.GetName());
	m_editPassword.SetWindowText(employee.GetPassword());
	m_comboGradeID.SetCurSel(employee.GetGradeID());
	m_checkIsUsing.SetCheck(employee.IsUsing());
}


void CEmployeeEdit::InsertEditByID(int nID)
{
	std::vector<CEmployee> vec_employee;
	CString strSql;
	strSql.Format(_T("SELECT * FROM Employee WHERE ID=%d ORDER BY ID"), nID);
	m_edb.Select(strSql, vec_employee);
	if (vec_employee.size() == 1)
	{
		InsertEdit(vec_employee[0]);
	}
}

void CEmployeeEdit::OnNMClickListEmployee(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int & nItem = pNMItemActivate->iItem;
	if (nItem != -1)
	{
		int nID = _ttoi(m_listctrlEmployee.GetItemText(nItem, 0));
		InsertEditByID(nID);
	}
}
