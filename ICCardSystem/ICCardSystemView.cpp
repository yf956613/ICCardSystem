
// ICCardSystemView.cpp: CICCardSystemView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ICCardSystem.h"
#endif

#include "ICCardSystemDoc.h"
#include "ICCardSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CICCardSystemView

IMPLEMENT_DYNCREATE(CICCardSystemView, CView)

BEGIN_MESSAGE_MAP(CICCardSystemView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CICCardSystemView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_MSGRECVPRO, &CICCardSystemView::OnMsgrecvpro)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CICCardSystemView 构造/析构

CICCardSystemView::CICCardSystemView() noexcept
	: m_nPrintType(TYPE_USERLOG)
	, m_p_strTitle(NULL)
	, m_p_strSubTitle(NULL)
	, m_p_vec2_strData(NULL)
	, m_nRowHeight(100)
	, m_nPageTopHeight(200)
	, m_nPageBottomHeight(200)
	, m_nPageLeftHeight(200)
	, m_nPageRightHeight(200)
	, m_nPageHeadHeight(0)
	, m_nRowPerFirstPage(0)
	, m_nRowPerOtherPage(0)
	, m_nPage(0)
	, m_nPageWidth(0)
	, m_nPageHeight(0)
	, m_nEditWidth(0)
{
	// TODO: 在此处添加构造代码

}

CICCardSystemView::~CICCardSystemView()
{
}

BOOL CICCardSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CICCardSystemView 绘图

void CICCardSystemView::OnDraw(CDC* /*pDC*/)
{
	CICCardSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CICCardSystemView 打印


void CICCardSystemView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CICCardSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CICCardSystemView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 添加额外的打印前进行的初始化过程
	if (pInfo&&pInfo->GetMaxPage() == 0xFFFF)
	{
		switch (m_nPrintType)
		{
		case TYPE_USERINCOME:m_nPageHeadHeight = 300; break;
		default:m_nPageHeadHeight = 100; break;
		}
		m_nPageWidth = pDC->GetDeviceCaps(HORZSIZE);
		m_nPageHeight = pDC->GetDeviceCaps(VERTSIZE);
		m_nPageWidth *= 10;
		m_nPageHeight *= 10;
		int nRow = m_p_vec2_strData->size();
		m_nRowPerFirstPage = (m_nPageHeight - m_nPageTopHeight - m_nPageBottomHeight - m_nPageHeadHeight) / m_nRowHeight;
		m_nRowPerOtherPage = (m_nPageHeight - m_nPageTopHeight - m_nPageBottomHeight) / m_nRowHeight;
		m_nEditWidth = m_nPageWidth - m_nPageLeftHeight - m_nPageRightHeight;
		if (m_nRowPerFirstPage)
		{
			if (nRow <= m_nRowPerFirstPage)
				m_nPage = 1;
			else
			{
				m_nPage = (nRow- m_nRowPerFirstPage) / m_nRowPerOtherPage+1;
				if ((nRow - m_nRowPerFirstPage) % m_nRowPerOtherPage)
					++m_nPage;
			}
			pInfo->SetMaxPage(m_nPage);
			m_vec_rc.clear();
			for (int i = 0; i < m_nPage; ++i)
			{
				CRect rc;
				if(i)
					rc=CRect(m_nPageLeftHeight, -m_nPageTopHeight - i * m_nPageHeight, m_nPageWidth - m_nPageRightHeight, m_nPageBottomHeight - (i + 1)* m_nPageHeight);
				else
					rc = CRect(m_nPageLeftHeight, -m_nPageTopHeight - m_nPageHeadHeight- i * m_nPageHeight, m_nPageWidth - m_nPageRightHeight, m_nPageBottomHeight - (i + 1)* m_nPageHeight);
				m_vec_rc.push_back(rc);
			}
		}
	}
}

void CICCardSystemView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	pDC->SetMapMode(MM_LOMETRIC);
	if (pInfo)
	{
		pDC->SetWindowOrg(0, (1 - pInfo->m_nCurPage)*m_nPageHeight);
	}
	CView::OnPrepareDC(pDC, pInfo);
}


void CICCardSystemView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (m_nPrintType)
	{
	case TYPE_USERLOG:
		DrawUserLog(pDC);
		break;
	case TYPE_USERINCOME:
		DrawUserIncome(pDC);
		break;
	case TYPE_USERPRESTORE:
		DrawUserPrestore(pDC);
		break;
	}
	CView::OnPrint(pDC, pInfo);
}


void CICCardSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
	m_nPageWidth = m_nPageHeight = m_nEditWidth=0;
	m_nPage = 1;
}

void CICCardSystemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CICCardSystemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CICCardSystemView 诊断

#ifdef _DEBUG
void CICCardSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CICCardSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CICCardSystemDoc* CICCardSystemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CICCardSystemDoc)));
	return (CICCardSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CICCardSystemView 消息处理程序


afx_msg LRESULT CICCardSystemView::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_SETTITLE:
		m_p_strTitle = (CString *)wParam;
		break;
	case MSGUSER_SETSUBTITLE:
		m_p_strSubTitle = (CString *)wParam;
		break;
	case MSGUSER_PRINTUSERLOG:
	{
		m_p_vec2_strData = (std::vector<std::vector<CString>> *)wParam;
		m_nPrintType = TYPE_USERLOG;
		OnFilePrintPreview();
	}
	break;
	case MSGUSER_PRINTUSERINCOME:
	{
		m_p_vec2_strData = (std::vector<std::vector<CString>> *)wParam;
		m_nPrintType = TYPE_USERINCOME;
		OnFilePrintPreview();
	}
	break;
	case MSGUSER_PRINTUSERPRESTORE:
	{
		m_p_vec2_strData = (std::vector<std::vector<CString>> *)wParam;
		m_nPrintType = TYPE_USERPRESTORE;
		OnFilePrintPreview();
	}
	break;
	}
	return 0;
}


void CICCardSystemView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	g_sz_p_wnd[WND_MAINVIEW] = this;
	//修改默认打印机设置
	PRINTDLG pd;
	pd.lStructSize = (DWORD)sizeof(PRINTDLG);
	BOOL bRet = AfxGetApp()->GetPrinterDeviceDefaults(&pd);
	if (bRet)
	{
		DEVMODE FAR *pDevMode = (DEVMODE FAR *)::GlobalLock(pd.hDevMode);
		pDevMode->dmPaperSize = DMPAPER_A4;
		pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
		::GlobalUnlock(pd.hDevMode);
	}

}


void CICCardSystemView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CView::OnPaint()
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_BKGND);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap * p_bmpOld = dcMem.SelectObject(&bmp);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	CRect rc;
	GetClientRect(&rc);
	//dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	dc.SetStretchBltMode(HALFTONE);
	dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
}


void CICCardSystemView::DrawUserLog(CDC* pDC)
{
	CFont * p_fontOld;
	CFont fontTitle,fontContent;
	fontTitle.CreatePointFont(550, _T("微软雅黑"));
	fontContent.CreatePointFont(250,_T("微软雅黑"));
	p_fontOld=pDC->SelectObject(&fontTitle);
	if(m_p_strTitle)
		pDC->DrawText(*m_p_strTitle, CRect(m_nPageLeftHeight, 0, m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight), DT_SINGLELINE |DT_CENTER|DT_BOTTOM);
	pDC->SelectObject(&fontContent);
	int nRow = 0;
	if(m_p_vec2_strData)
		nRow=m_p_vec2_strData->size();
	if (nRow)
	{
		int nColumn = (*m_p_vec2_strData)[0].size();
		if (nColumn==11)
		{
			int sz_nOffset[] = { 0 ,1, 2, 3, 4, 5, 6, 7, 8, 9, 12 };
			int sz_nTimes[] = { 1 ,1, 1, 1, 1, 1, 1, 1, 1, 3, 3 };
			int nColumnWidth = m_nEditWidth / 15;
			//绘制Head
			if (m_p_strSubTitle)
				pDC->DrawText(*m_p_strSubTitle, CRect(m_nPageLeftHeight, -m_nPageTopHeight, m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight- m_nPageHeadHeight), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			//绘制Data
			int nCurRow = 0;
			for (int k = 0; k < m_nPage; ++k)
			{
				int nRowPerPage= m_nRowPerOtherPage;
				if (!k)
					nRowPerPage = m_nRowPerFirstPage;
				CPoint ori(m_vec_rc[k].TopLeft());
				for (int i = 0; i < nRowPerPage && nCurRow < nRow; ++i, ++nCurRow)
				{
					pDC->MoveTo(ori.x, ori.y - i * m_nRowHeight);
					pDC->LineTo(ori.x + m_nEditWidth, ori.y - i * m_nRowHeight);
					for (int j = 0; j < nColumn; ++j)
					{
						CPoint pt(ori.x + nColumnWidth * sz_nOffset[j], ori.y - i * m_nRowHeight);
						CSize size(nColumnWidth * sz_nTimes[j], -m_nRowHeight);
						CRect rc(pt, size);
						pDC->SetBkMode(TRANSPARENT);
						rc.DeflateRect(10, -10, 10, -10);
						if (j>=3&&j<=6&& nCurRow)
							pDC->DrawText((*m_p_vec2_strData)[nCurRow][j], rc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
						else
							pDC->DrawText((*m_p_vec2_strData)[nCurRow][j], rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
						pDC->MoveTo(pt.x, pt.y);
						pDC->LineTo(pt.x, pt.y - m_nRowHeight);
						if (j == nColumn - 1)
						{
							pDC->MoveTo(ori.x + m_nEditWidth, pt.y);
							pDC->LineTo(ori.x + m_nEditWidth, pt.y - m_nRowHeight);
						}
					}
					if (i == nRowPerPage - 1 || nCurRow == nRow - 1)
					{
						pDC->MoveTo(ori.x, ori.y - (i + 1) * m_nRowHeight);
						pDC->LineTo(ori.x + m_nEditWidth, ori.y - (i + 1) * m_nRowHeight);
					}
				}
			}
		}
	}
	pDC->SelectObject(p_fontOld);
}


void CICCardSystemView::DrawUserIncome(CDC* pDC)
{
	CFont * p_fontOld;
	CFont fontTitle, fontContent;
	fontTitle.CreatePointFont(550, _T("微软雅黑"));
	fontContent.CreatePointFont(250, _T("微软雅黑"));
	p_fontOld = pDC->SelectObject(&fontTitle);
	if (m_p_strTitle)
		pDC->DrawText(*m_p_strTitle, CRect(m_nPageLeftHeight, 0, m_nPageLeftHeight+m_nEditWidth, -m_nPageTopHeight), DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	pDC->SelectObject(&fontContent);
	int nRow = 0;
	if (m_p_vec2_strData)
		nRow=m_p_vec2_strData->size();
	if (nRow)
	{
		int nColumn = (*m_p_vec2_strData)[0].size();
		if (nColumn == 11)
		{
			int sz_nOffset[] = { 0 , 2, 3, 4, 5, 6, 7, 8, 9, 10,11 };
			int sz_nTimes[] = { 2 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
			int nColumnWidth = m_nEditWidth / 12;
			//绘制Head
			if (m_p_strSubTitle)
				pDC->DrawText(*m_p_strSubTitle, CRect(m_nPageLeftHeight, -m_nPageTopHeight, m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight - m_nPageHeadHeight*1/3), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			CString sz_strHeadOne[] = { _T("员工"),_T("开卡"), _T("退卡"),_T("充值"),_T("退款"),  _T("退押") };
			CString sz_strHeadTwo[] = { _T("员工"),_T("数量"), _T("押金"), _T("次数"), _T("金额"),_T("次数"), _T("金额"),_T("次数"), _T("金额"),_T("次数"), _T("金额") };
			pDC->MoveTo(m_nPageLeftHeight, -m_nPageTopHeight - m_nPageHeadHeight * 1 / 3);
			pDC->LineTo(m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight - m_nPageHeadHeight * 1 / 3);
			for (int i = 0; i < sizeof(sz_strHeadOne) / sizeof(CString); ++i)
			{
				if (i)
				{
					CRect rc(CPoint(m_nPageLeftHeight + nColumnWidth * i * 2, -m_nPageTopHeight- m_nPageHeadHeight * 1 / 3),CSize( nColumnWidth * 2, - m_nPageHeadHeight *1/ 3));
					rc.DeflateRect(10, -10, 10, -10);
					pDC->DrawText(sz_strHeadOne[i],rc , DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
				else
				{
					CRect rc(CPoint(m_nPageLeftHeight + nColumnWidth * i * 2, -m_nPageTopHeight - m_nPageHeadHeight * 1 / 3), CSize(nColumnWidth * 2, -m_nPageHeadHeight * 2 / 3));
					rc.DeflateRect(10, -10, 10, -10);
					pDC->DrawText(sz_strHeadOne[i], rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
				pDC->MoveTo(m_nPageLeftHeight + nColumnWidth * i * 2, -m_nPageTopHeight - m_nPageHeadHeight *1/3);
				pDC->LineTo(m_nPageLeftHeight + nColumnWidth * i * 2, -m_nPageTopHeight - m_nPageHeadHeight *2/ 3);
			}
			pDC->MoveTo(m_nPageLeftHeight+ sz_nTimes[0]* nColumnWidth, -m_nPageTopHeight- m_nPageHeadHeight*2/3);
			pDC->LineTo(m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight- m_nPageHeadHeight*2/3);
			for (int i = 0; i < sizeof(sz_strHeadTwo) / sizeof(CString); ++i)
			{
				if (i)
				{
					CRect rc(CPoint(m_nPageLeftHeight + nColumnWidth * sz_nOffset[i], -m_nPageTopHeight - m_nPageHeadHeight *2/ 3), CSize(nColumnWidth * sz_nTimes[i], -m_nPageHeadHeight *1/ 3));
					rc.DeflateRect(10, -10, 10, -10);
					pDC->DrawText(sz_strHeadTwo[i], rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
				pDC->MoveTo(m_nPageLeftHeight + nColumnWidth * sz_nOffset[i], -m_nPageTopHeight - m_nPageHeadHeight *2/ 3);
				pDC->LineTo(m_nPageLeftHeight + nColumnWidth * sz_nOffset[i], -m_nPageTopHeight - m_nPageHeadHeight);
			}
			pDC->MoveTo(m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight - m_nPageHeadHeight * 1 / 3);
			pDC->LineTo(m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight - m_nPageHeadHeight);
			//绘制Data
			int nCurRow = 0;
			for (int k = 0; k < m_nPage; ++k)
			{
				int nRowPerPage = m_nRowPerOtherPage;
				if (!k)
					nRowPerPage = m_nRowPerFirstPage;
				CPoint ori(m_vec_rc[k].TopLeft());
				for (int i = 0; i < nRowPerPage && nCurRow < nRow; ++i, ++nCurRow)
				{
					pDC->MoveTo(ori.x, ori.y - i * m_nRowHeight);
					pDC->LineTo(ori.x + m_nEditWidth, ori.y - i * m_nRowHeight);
					for (int j = 0; j < nColumn; ++j)
					{
						CPoint pt(ori.x + nColumnWidth * sz_nOffset[j], ori.y - i * m_nRowHeight);
						CSize size(nColumnWidth * sz_nTimes[j], -m_nRowHeight);
						CRect rc(pt, size);
						pDC->SetBkMode(TRANSPARENT);
						rc.DeflateRect(10, -10, 10, -10);
						if (j )
							pDC->DrawText((*m_p_vec2_strData)[nCurRow][j], rc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
						else
							pDC->DrawText((*m_p_vec2_strData)[nCurRow][j], rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
						pDC->MoveTo(pt.x, pt.y);
						pDC->LineTo(pt.x, pt.y - m_nRowHeight);
						if (j == nColumn - 1)
						{
							pDC->MoveTo(ori.x + m_nEditWidth, pt.y);
							pDC->LineTo(ori.x + m_nEditWidth, pt.y - m_nRowHeight);
						}
					}
					if (i == nRowPerPage - 1 || nCurRow == nRow - 1)
					{
						pDC->MoveTo(ori.x, ori.y - (i + 1) * m_nRowHeight);
						pDC->LineTo(ori.x + m_nEditWidth, ori.y - (i + 1) * m_nRowHeight);
					}
				}
			}
		}
	}
	pDC->SelectObject(p_fontOld);
}


void CICCardSystemView::DrawUserPrestore(CDC* pDC)
{
	CFont * p_fontOld;
	CFont fontTitle, fontContent;
	fontTitle.CreatePointFont(550, _T("微软雅黑"));
	fontContent.CreatePointFont(200, _T("微软雅黑"));
	p_fontOld = pDC->SelectObject(&fontTitle);
	if (m_p_strTitle)
		pDC->DrawText(*m_p_strTitle, CRect(m_nPageLeftHeight, 0, m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight), DT_SINGLELINE | DT_CENTER | DT_BOTTOM);
	pDC->SelectObject(&fontContent);
	int nRow = 0;
	if (m_p_vec2_strData)
		nRow=m_p_vec2_strData->size();
	if (nRow)
	{
		int nColumn = (*m_p_vec2_strData)[0].size();
		if (nColumn == 17)
		{
			int sz_nOffset[] = { 0 ,1, 3, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15 , 16, 17, 18, 19 };
			int sz_nTimes[] = { 1 ,2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
			int nColumnWidth = m_nEditWidth / 20;
			//绘制Head
			if (m_p_strSubTitle)
				pDC->DrawText(*m_p_strSubTitle, CRect(m_nPageLeftHeight, -m_nPageTopHeight, m_nPageLeftHeight + m_nEditWidth, -m_nPageTopHeight - m_nPageHeadHeight), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			//绘制Data
			int nCurRow = 0;
			for (int k = 0; k < m_nPage; ++k)
			{
				int nRowPerPage = m_nRowPerOtherPage;
				if (!k)
					nRowPerPage = m_nRowPerFirstPage;
				CPoint ori(m_vec_rc[k].TopLeft());
				for (int i = 0; i < nRowPerPage && nCurRow < nRow; ++i, ++nCurRow)
				{
					pDC->MoveTo(ori.x, ori.y - i * m_nRowHeight);
					pDC->LineTo(ori.x + m_nEditWidth, ori.y - i * m_nRowHeight);
					for (int j = 0; j < nColumn; ++j)
					{
						CPoint pt(ori.x + nColumnWidth * sz_nOffset[j], ori.y - i * m_nRowHeight);
						CSize size(nColumnWidth * sz_nTimes[j], -m_nRowHeight);
						CRect rc(pt, size);
						pDC->SetBkMode(TRANSPARENT);
						rc.DeflateRect(10, -10, 10, -10);
						if (j>=4 && nCurRow)
							pDC->DrawText((*m_p_vec2_strData)[nCurRow][j], rc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
						else
							pDC->DrawText((*m_p_vec2_strData)[nCurRow][j], rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
						pDC->MoveTo(pt.x, pt.y);
						pDC->LineTo(pt.x, pt.y - m_nRowHeight);
						if (j == nColumn - 1)
						{
							pDC->MoveTo(ori.x + m_nEditWidth, pt.y);
							pDC->LineTo(ori.x + m_nEditWidth, pt.y - m_nRowHeight);
						}
					}
					if (i == nRowPerPage - 1|| nCurRow==nRow-1)
					{
						pDC->MoveTo(ori.x, ori.y - (i + 1) * m_nRowHeight);
						pDC->LineTo(ori.x + m_nEditWidth, ori.y - (i + 1) * m_nRowHeight);
					}
				}
			}
		}
	}
	pDC->SelectObject(p_fontOld);
}
