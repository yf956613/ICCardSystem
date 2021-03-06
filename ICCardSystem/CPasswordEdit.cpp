// CPasswordEdit.cpp: 实现文件
//

#include "stdafx.h"
#include "ICCardSystem.h"
#include "CPasswordEdit.h"
#include "afxdialogex.h"


// CPasswordEdit 对话框

IMPLEMENT_DYNAMIC(CPasswordEdit, CDialogEx)

CPasswordEdit::CPasswordEdit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PASSWORDEDIT, pParent)
{

}

CPasswordEdit::~CPasswordEdit()
{
}

void CPasswordEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPasswordEdit, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CPasswordEdit::OnBnClickedButtonModify)
END_MESSAGE_MAP()


// CPasswordEdit 消息处理程序


BOOL CPasswordEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPasswordEdit::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPasswordOld, strPasswordNew, strPasswordConfirm;
	GetDlgItemText(IDC_EDIT_PASSWORDOLD, strPasswordOld);
	GetDlgItemText(IDC_EDIT_PASSWORDNEW, strPasswordNew);
	GetDlgItemText(IDC_EDIT_PASSWORDCONFIRM, strPasswordConfirm);
	if (strPasswordNew != strPasswordConfirm)
	{
		AfxMessageBox(_T("两次输入的新密码不一样!"));
		return;
	}
	if(g_employee.GetPassword()!= strPasswordOld)
	{
		AfxMessageBox(_T("原密码不正确!"));
		return;
	}
	g_employee.SetPassword(strPasswordNew);
	CEmployeeDB edb;
	if (edb.Update(g_employee))
		AfxMessageBox(_T("修改成功!"));
	else
		AfxMessageBox(_T("修改失败!"));
}
