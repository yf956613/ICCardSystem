
// ICCardSystemView.h: CICCardSystemView 类的接口
//

#pragma once


class CICCardSystemView : public CView
{
protected: // 仅从序列化创建
	CICCardSystemView() noexcept;
	DECLARE_DYNCREATE(CICCardSystemView)

// 特性
public:
	CICCardSystemDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CICCardSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
private:
	int m_nPrintType;
	CString * m_p_strTitle;
	CString * m_p_strSubTitle;
	std::vector<std::vector<CString>> * m_p_vec2_strData;
	int m_nRowHeight;//行高
	int m_nPageTopHeight;//上边距
	int m_nPageBottomHeight;//下边距
	int m_nPageLeftHeight;//左边距
	int m_nPageRightHeight;//右边距
	int m_nPageHeadHeight;//首页头高
	int m_nRowPerFirstPage;//首页行数
	int m_nRowPerOtherPage;//其他页行数
	int m_nPage;//总页数
	int m_nPageWidth;//页宽
	int m_nPageHeight;//页高
	int m_nEditWidth;//可编辑页宽
	std::vector<CRect> m_vec_rc;//每页rect
public:
	enum PRINT_TYPE
	{
		TYPE_USERLOG=0,
		TYPE_USERINCOME,
		TYPE_USERPRESTORE
	};
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	void DrawUserLog(CDC* pDC);
	void DrawUserIncome(CDC* pDC);
	void DrawUserPrestore(CDC* pDC);
};

#ifndef _DEBUG  // ICCardSystemView.cpp 中的调试版本
inline CICCardSystemDoc* CICCardSystemView::GetDocument() const
   { return reinterpret_cast<CICCardSystemDoc*>(m_pDocument); }
#endif

