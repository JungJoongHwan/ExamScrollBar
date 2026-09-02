#pragma once
#include <afxwin.h>

class CEnvListScrollbar;

// 스크롤 이벤트 종류 정의
enum ScrollType {
	SCROLL_LINEUP,    // 위 화살표 클릭 (1행 위로)
	SCROLL_LINEDOWN,  // 아래 화살표 클릭 (1행 아래로)
	SCROLL_PAGEUP,    // Thumb 위 트랙 클릭 (1페이지 위로)
	SCROLL_PAGEDOWN   // Thumb 아래 트랙 클릭 (1페이지 아래로)
};

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

	void OnScrollbarAction(int nScrollType); // 화살표 및 빈 공간 클릭 이벤트 핸들러

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
	void DrawArrowButton(CDC* pDC, CRect rc, bool bTop);

	void OnThumbDrag(int newPos);
	void OnScrollAction(ScrollType type); // 스크롤 명령 전달 헬퍼

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