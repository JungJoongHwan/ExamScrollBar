#pragma once

#define dfIDC_EDITBOX_CTRL					1002

#define WMU_INIT_ENV_CTRL					(WM_USER + 777)

//-----------------------------------------------------------------------------
// CEnvCheck

class CEnvCheck : public CButton
{
	DECLARE_DYNAMIC(CEnvCheck)

public:
	struct ST_ENV_CHECK_INFO
	{
		//CBitmap		bmpMark;
		// 이미지 리소스 ID
		int			nNormal;
		int			nNormaldis;
		int			nSelected;
		int			nSelecteddis;

		BOOL		bCheck;
		int			nMarkWidth;
		int			nMarkHeight;
		UINT		nAlignFormat;

		CString		strFontName;
		int			nFontSize;
		int			nFontStyle;
		BOOL		bFontBold;

		COLORREF	crText;
		COLORREF	crBack;
	};

	CEnvCheck();
	virtual ~CEnvCheck();

protected:
	DECLARE_MESSAGE_MAP()
public:
	ST_ENV_CHECK_INFO m_stInfo;
	CFont* m_pFont;

public:
	//BOOL SetMarkImage(UINT nImageID);
	void DrawImageMark(CPaintDC* pdc, CRect rScreen, BOOL bCheck);
	void SetMarkSize(int w, int h) { m_stInfo.nMarkWidth = w; m_stInfo.nMarkHeight = h; };
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetCheck(BOOL bCheck);
	BOOL GetCheck(void);
	void DrawText(CPaintDC *pdc, CString strText, int nFontSize, COLORREF crFontColor, CRect rArea, UINT nFormat, BOOL bFontBold);
	void SetWindowText(CString strText);
	void SetAlignFormat(UINT nAlignFormat);
	void SetFontAttr(LPCTSTR lpszFontName, int nSize, int nFontStyle = FontStyleRegular);
	void SetFont(CFont* pFont);
	void SetBkColor(COLORREF cr) { m_stInfo.crBack = cr; }

protected:
	virtual void PreSubclassWindow();

public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnInitEnvCtrl(WPARAM wParam, LPARAM lParam);
};

//-----------------------------------------------------------------------------
// CEnvRadio

class CEnvRadio : public CButton
{
	DECLARE_DYNAMIC(CEnvRadio)

public:
	struct ST_ENV_RADIO_INFO
	{
		//CBitmap		bmpMark;
		// 이미지 리소스 ID
		int			nNormal;
		int			nNormaldis;
		int			nSelected;
		int			nSelecteddis;

		BOOL		bCheck;
		int			nMarkWidth;
		int			nMarkHeight;
		UINT		nAlignFormat;

		CString		strFontName;
		int			nFontSize;
		int			nFontStyle;
		BOOL		bFontBold;

		COLORREF	crText;
		COLORREF	crBack;
	};

	CEnvRadio();
	virtual ~CEnvRadio();

protected:
	DECLARE_MESSAGE_MAP()
public:
	ST_ENV_RADIO_INFO m_stInfo;
	CFont* m_pFont;

	afx_msg LRESULT OnInitEnvCtrl(WPARAM wParam, LPARAM lParam);


public:
	//BOOL SetMarkImage(UINT nImageID);
	void SetImages(int nNormal, int nNormaldis, int nSelected, int nSelecteddis);
	void SetMarkSize(int w, int h) { m_stInfo.nMarkWidth = w; m_stInfo.nMarkHeight = h; };
	void DrawImageMark(CPaintDC* pdc, CRect rScreen, BOOL bCheck);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetCheck(BOOL bCheck);
	BOOL GetCheck();
	void OnBMSetCheck(BOOL bCheck);
	LRESULT OnRadioInfo(WPARAM wparam, LPARAM);
	void DrawText(CPaintDC* pdc, CString strText, int nFontSize, COLORREF crFontColor, CRect rArea, UINT nFormat, BOOL bFontBold);
	void SetWindowText(CString strText);
	void SetAlignFormat(UINT nAlignFormat);
	void SetFontAttr(LPCTSTR lpszFontName, int nSize, int nFontStyle = FontStyleRegular);
	void SetFont(CFont* pFont);
	void SetBkColor(COLORREF cr) { m_stInfo.crBack = cr; }
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

//--------------------------------------------------------------------------------
// CEnvListBox

class CEnvListBox : public CListBox
{
	DECLARE_DYNAMIC(CEnvListBox)

public:
	struct ST_LISTBOX_INFO
	{
		UINT		nAlignFormat;
		int			nFontSize;
		BOOL		bFontBold;
		COLORREF	crText;
		COLORREF	crItemBack;
		COLORREF	crSelText;
		COLORREF	crSelItemBack;
		COLORREF	crItemDivideLine;
		COLORREF	crBack;
	};

	CEnvListBox();
	virtual ~CEnvListBox();

	int GetVisibleItemCount();

protected:
	DECLARE_MESSAGE_MAP()
public:
	ST_LISTBOX_INFO m_stInfo;
	CFont* m_pFont;
	CRect m_rcClient;

	BOOL m_bInitialized;
	int m_nItemHeight;
	int m_nVisibleItems;
	int m_nTopIndex;

	CRect m_rcScrollBar;

	int m_nThumbPos;
	BOOL m_bScrollBarVisible;
	int m_nScrollBarWidth;
	BOOL m_bScrollBarDragging;

	BOOL m_bCheckImage;
	int m_nNormal;
	int m_nChecked;

public:
	void DrawText(CPaintDC* pdc, CString strText, int nFontSize, COLORREF crFontColor, CRect rArea, UINT nFormat, BOOL bFontBold);
	void DrawCustomScrollbar(CDC* pDC);
	void DrawCustomScrollBarGDIPlus(Graphics* g);
	void HandleScrollBarClick(CPoint point);
	int ItemFromPoint(CPoint point);
	void SetImageCheckable(LPCTSTR lpszFileNormal, LPCTSTR lpszFileChecked);
	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	
	void DrawItemBack(CPaintDC* pdc, COLORREF crBack, COLORREF crDivideLine, CRect rArea);
	void DrawLine(CPaintDC* pdc, CPoint po1, CPoint po2, COLORREF crLine, int nLineTick);
	void SetFontColor(COLORREF crFont);
	void SetFontBold(BOOL bBold);
	void SetFontSize(int nFontSize);
	void SetAlignFormat(UINT nAlignFormat);
	void SetFont(CFont* pFont);
	void UpdateScrollInfo();
	

	COLORREF GetFontColor();
	void SetItemBackColor(COLORREF crItemBack);
	COLORREF GetItemBackColor();
	void SetItemDivideLineColor(COLORREF crItemDivideLine);
	COLORREF GetItemDivideLineColor();
	void SetSelFontColor(COLORREF crSelFont);
	COLORREF GetSelFontColor();
	void SetSelItemBackColor(COLORREF crSelItemBack);
	COLORREF GetSelItemBackColor();
	void SetBackColor(COLORREF crBack);
	COLORREF GetBackColor();
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};


//-----------------------------------------------------------------------------

class CEnvPopupList : public CWnd
{
public:
	CEnvPopupList();
	virtual ~CEnvPopupList();

	BOOL Create(CWnd* pParentWnd, const CRect& rect);
	void SetItems(const CStringArray& items);
	void SetSelectedIndex(int index);
	int GetSelectedIndex() { return m_selectedIndex; }
	void SetFont(CFont* pFont) { m_pFont = pFont; }
	

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	DECLARE_MESSAGE_MAP()

private:
	CStringArray m_items;
	int m_selectedIndex;
	int m_hoverIndex;
	CFont* m_pFont;
	int m_itemHeight;
	CWnd* m_pParent;
};



//------------------------------------------------------------------------------

class CStaticUX : public CStatic
{
public:
	CStaticUX();

	void SetBkColor(COLORREF color);
	void SetBorderColor(COLORREF color);
	void SetTextColor(COLORREF color);
	void SetCornerRadius(int radius);

protected:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_bkColor;
	COLORREF m_borderColor;
	COLORREF m_textColor;
	int m_cornerRadius;
};

//------------------------------------------------------------------------------

class CRoundedColorBox : public CStatic
{
public:
	CRoundedColorBox(COLORREF color = RGB(100, 100, 255));
	void SetColor(COLORREF color);

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_color;
};

//------------------------------------------------------------------------------

class CEnvEdit : public CWnd
{
	DECLARE_DYNAMIC(CEnvEdit)

public:
	CEnvEdit();
	virtual ~CEnvEdit();

	// 서브클래싱을 위한 함수
	BOOL SubclassDlgItem(UINT nID, CWnd* pParent);

	// 생성 및 초기화
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void SetText(LPCTSTR lpszText);
	CString GetText();
	void GetText(CString& strText);
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	CEdit* GetInnerEdit() { return m_pEdit; }

	// DDX를 위한 연산자 오버로딩
	operator CString() const;
	CEnvEdit& operator=(const CString& str);

	// 스타일 설정
	void SetCornerRadius(int nRadius) { m_nCornerRadius = nRadius; Invalidate(); }
	void SetBorderColor(COLORREF clr) { m_clrBorder = clr; Invalidate(); }
	void SetBackgroundColor(COLORREF clr) { m_clrBackground = clr; Invalidate(); }
	void SetBorderWidth(int nWidth) { m_nBorderWidth = nWidth; UpdateEditPosition(); }
	HWND GetEditHwnd();

protected:
	CEdit* m_pEdit;             // 내부 Edit 컨트롤
	int m_nCornerRadius;        // 모서리 반지름
	int m_nBorderWidth;         // 테두리 두께
	COLORREF m_clrBorder;       // 테두리 색상
	COLORREF m_clrBackground;   // 배경색상

	// 메시지 맵
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	void UpdateEditPosition();
	void DrawRoundedRect(CDC* pDC, const CRect& rect, int nRadius);
	void CreateInternalEdit();
	BOOL m_bSubclassed;         // 서브클래싱 여부
};

//-------------------------------------------------------------------------

class CEnvStatic : public CWnd
{
	DECLARE_DYNAMIC(CEnvStatic)

public:
	CEnvStatic();
	virtual ~CEnvStatic();

	// 생성 함수
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

	// 서브클래싱 함수 (리소스 컨트롤과 연동)
	BOOL SubclassDlgItem(UINT nID, CWnd* pParent);

	// 텍스트 설정/가져오기
	void SetText(LPCTSTR lpszText);
	CString GetText() const;
	void GetText(CString& strText);

	// 색상 설정
	void SetBackColor(COLORREF color);
	void SetTextColor(COLORREF color);
	void SetBorderColor(COLORREF color);

	// 라운드 반지름 설정
	void SetRoundRadius(int radius);

	// 폰트 설정
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);

protected:
	// 메시지 맵
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// 서브클래싱 후 초기화
	virtual void PreSubclassWindow();

private:
	CString m_strText;          // 표시할 텍스트
	COLORREF m_colorBack;       // 배경색
	COLORREF m_colorText;       // 텍스트색
	COLORREF m_colorBorder;     // 테두리색
	int m_nRoundRadius;         // 라운드 반지름
	CFont* m_pFont;             // 폰트

	// GDI+ 관련
	ULONG_PTR m_gdiplusToken;

	// 초기화
	void InitGdiPlus();
	void CleanupGdiPlus();

	// 그리기 헬퍼 함수
	void DrawRoundRect(Gdiplus::Graphics& graphics, const Gdiplus::RectF& rect, float radius, const Gdiplus::Color& fillColor, const Gdiplus::Color& borderColor);
};


//-------------------------------------------------------------------------

class CEnvListScrollbar;

class CEnvHeader : public CHeaderCtrl
{
protected:
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	COLORREF m_clrText;
	COLORREF m_clrBk;
};


class CEnvList : public CListCtrl
{
public:
	CEnvList();
	virtual ~CEnvList();

	void InitList();
	void UpdateHeaderColors(); // 헤더 함수 추가
	void SetHeaderBkColor(COLORREF clr) { m_clrHeaderBk = clr; }
	void SetBkColor(COLORREF clr) { m_clrBk = clr; }
	void SetTxtColor(COLORREF clr) { m_clrText = clr; }
	void SetSelBkColor(COLORREF clr) { m_clrSelBk = clr; }

	void AttachScrollbar(CEnvListScrollbar* pScrollbar);
	void UpdateScrollbar();
	void OnScrollbarThumbMoved(int newPos);

	int InsertItem(int nItem, LPCTSTR lpszItem);
	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	int InsertItem(const LVITEM* pItem);
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();
	void SetItemCount(int nItems);
	BOOL SetItemCountEx(int iCount, DWORD dwFlags);

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnUpdateScrollbar(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CEnvHeader m_Header;
	COLORREF m_clrHeaderBk;
	COLORREF m_clrText;
	COLORREF m_clrBk;
	COLORREF m_clrSelBk;

	CEnvListScrollbar* m_pScrollbar;

};

//--------------------------------------------------------------------

class CEnvListScrollbar : public CWnd
{
public:
	CEnvListScrollbar();
	virtual ~CEnvListScrollbar() {};

	void SetOwner(CEnvList* pList);
	void SetScrollRange(int min, int max);
	void SetScrollPos(int pos);
	int GetScrollPos() const;
	void SetPageSize(int pageSize);

	void UpdateThumbRect();
	//void UpdateThumbRect(int totalItems, int visibleItems, int scrollPos);

	void OnThumbDrag(int newPos);

protected:
	CEnvList* m_pOwner;
	int m_nMin;
	int m_nMax;
	int m_nPos;
	int m_nPageSize;
	CRect m_rcThumb;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:
	bool m_bDragging;
	int m_nDragOffset;
};
