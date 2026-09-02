#include "pch.h"
#include "EnvCtrls.h"


BEGIN_MESSAGE_MAP(CEnvHeader, CHeaderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CEnvHeader::OnCustomDraw)
END_MESSAGE_MAP()

void CEnvHeader::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pDraw = reinterpret_cast<NMCUSTOMDRAW*>(pNMHDR);

	switch (pDraw->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		CDC dc;
		dc.Attach(pDraw->hdc);

		// 헤더 영역 가져오기
		CRect rc = pDraw->rc;

		// 텍스트 가져오기
		TCHAR szText[256] = { 0 };
		HDITEM item = { 0 };
		item.mask = HDI_TEXT;
		item.pszText = szText;
		item.cchTextMax = 255;
		GetItem(pDraw->dwItemSpec, &item);

		CFont* pFont = nullptr;
		CFont* pOldFont = nullptr;
		pFont = GetParent()->GetFont();// m_nFontIndex);
		pOldFont = dc.SelectObject(pFont);

		// 텍스트 그리기
		dc.SetTextColor(m_clrText);
		dc.SetBkColor(m_clrBk);
		dc.SetBkMode(OPAQUE);
		dc.FillSolidRect(&rc, m_clrBk);
		dc.DrawText(szText, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		dc.SelectObject(pOldFont);

		dc.Detach();
		*pResult = CDRF_SKIPDEFAULT;
		break;
	}
	break;
	case CDDS_ITEMPOSTPAINT:
	{
		// 여기서 텍스트를 다시 그려주는 것도 가능
		// 또는 배경을 덮어쓰지 않도록 처리
		*pResult = CDRF_SKIPDEFAULT;
	}
	break;
	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}



//--------------------------------------------------------------------------

CEnvList::CEnvList()
{
	m_clrHeaderBk = RGB(34, 36, 43);// RGB(200, 200, 255);  // 헤더 배경색
	m_clrText = RGB(238, 238, 238);// RGB(0, 0, 0);        // 텍스트 색상
	m_clrBk = RGB(29, 31, 37);// RGB(240, 240, 240);  // 바디 배경색

	m_pScrollbar = nullptr;
}

CEnvList::~CEnvList() {}

BEGIN_MESSAGE_MAP(CEnvList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CEnvList::OnCustomDraw)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_USER + 100, OnUpdateScrollbar)
END_MESSAGE_MAP()

void CEnvList::InitList()
{
	// LVS_EX_GRIDLINES 제거하고 직접 그리기
	SetExtendedStyle(LVS_EX_FULLROWSELECT);  // GRIDLINES 제거

	// 헤더 서브클래싱
	CHeaderCtrl* pRawHeader = GetHeaderCtrl();
	if (pRawHeader && ::IsWindow(pRawHeader->GetSafeHwnd()))
	{
		m_Header.SubclassWindow(pRawHeader->GetSafeHwnd());
		m_Header.m_clrText = m_clrText;
		m_Header.m_clrBk = m_clrHeaderBk;
	}

	SetSelBkColor(RGB(29, 71, 142));
}

// 구현
void CEnvList::UpdateHeaderColors()
{
	if (::IsWindow(m_Header.GetSafeHwnd()))
	{
		m_Header.m_clrText = m_clrText;
		m_Header.m_clrBk = m_clrHeaderBk;
		m_Header.Invalidate();
	}
}


void CEnvList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// 기본적인 배경 그리기
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;

	// 배경색
	pDC->FillSolidRect(&rect, RGB(240, 240, 240));

	// 텍스트 예시
	CString text = _T("Custom Draw Item");
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rect.left + 5, rect.top + 2, text);
}

void CEnvList::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
	{
		*pResult = CDRF_NOTIFYPOSTPAINT; // 모든 항목에 대해 POSTPAINT 요청
		//int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		//BOOL bSelected = GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED;
		//
		//if (bSelected)
		//{
		//	// 선택된 항목은 직접 그리기 위해 POSTPAINT 요청
		//	*pResult = CDRF_NOTIFYPOSTPAINT;
		//}
		//else
		//{
		//	// 일반 항목은 기본 배경/텍스트 색상 적용
		//	pLVCD->clrTextBk = m_clrBk;
		//	pLVCD->clrText = m_clrText;
		//	*pResult = CDRF_DODEFAULT;
		//}
		break;
	}

	case CDDS_ITEMPOSTPAINT:
	{
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		BOOL bSelected = GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED;

		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

		CRect rc;
		GetItemRect(nItem, &rc, LVIR_BOUNDS);
		rc.left += 2;

		// 선택된 배경색으로 채우기
		if (bSelected)
			pDC->FillSolidRect(&rc, m_clrSelBk); // 선택 배경색
		else
			pDC->FillSolidRect(&rc, m_clrBk);

		// 컬럼 수 가져오기
		int nColCount = GetHeaderCtrl()->GetItemCount();

		CFont* pFont = nullptr;
		CFont* pOldFont = nullptr;
		pFont = GetParent()->GetFont();// m_nFontIndex);
		pOldFont = pDC->SelectObject(pFont);

		for (int iCol = 0; iCol < nColCount; ++iCol)
		{
			CRect rcSubItem;
			GetSubItemRect(nItem, iCol, LVIR_LABEL, rcSubItem);

			CString text = GetItemText(nItem, iCol);

			pDC->SetTextColor(m_clrText);
			pDC->SetBkMode(TRANSPARENT);
			// 여백
			if (iCol == 0)	rcSubItem.left += 2;
			else			rcSubItem.left += 6;
			pDC->DrawText(text, &rcSubItem, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
		}

		pDC->SelectObject(pOldFont);

		*pResult = CDRF_SKIPDEFAULT; // 기본 그리기 무시
		break;
	}
	}
}

void CEnvList::OnPaint()
{
	CListCtrl::OnPaint();

}

BOOL CEnvList::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, m_clrBk);
	return TRUE; // 배경 지움 완료
}

void CEnvList::AttachScrollbar(CEnvListScrollbar* pScrollbar)
{
	m_pScrollbar = pScrollbar;
	if (m_pScrollbar) {
		m_pScrollbar->SetOwner(this);

		// 1단계: ShowScrollBar
		ShowScrollBar(SB_BOTH, FALSE);

		// 2단계: 스타일 제거
		DWORD dwStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
		dwStyle &= ~(WS_VSCROLL | WS_HSCROLL);
		SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle);

		// 3단계: 프레임 변경 적용
		SetWindowPos(nullptr, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		// 4단계: 비활성화
		EnableScrollBar(SB_BOTH, ESB_DISABLE_BOTH);
	}
}

void CEnvList::UpdateScrollbar()
{
	if (!m_pScrollbar) return;

	int totalItems = GetItemCount();
	int visibleItems = GetCountPerPage();

	if (totalItems <= visibleItems) {
		m_pScrollbar->ShowWindow(SW_HIDE);
		return;
	}

	m_pScrollbar->ShowWindow(SW_SHOW);
	m_pScrollbar->SetScrollRange(0, totalItems - visibleItems);
	m_pScrollbar->SetPageSize(visibleItems);

	// 현재 스크롤 위치 동기화
	int currentPos = GetTopIndex();
	m_pScrollbar->SetScrollPos(currentPos);
}

void CEnvList::OnScrollbarThumbMoved(int newPos)
{
	// 리스트를 해당 위치로 스크롤
	EnsureVisible(newPos, FALSE);

	// 또는 더 정확한 제어를 원한다면:
	// Scroll(CSize(0, (newPos - GetTopIndex()) * GetItemHeight()));
}

void CEnvList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 기본 스크롤 처리
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	// 커스텀 스크롤바와 동기화
	if (m_pScrollbar && ::IsWindow(m_pScrollbar->GetSafeHwnd()))
	{
		int currentPos = GetTopIndex();
		m_pScrollbar->SetScrollPos(currentPos);
	}
}

BOOL CEnvList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL result = CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
	if (m_pScrollbar) UpdateScrollbar(); // 마우스 휠 후 스크롤바 동기화
	return result;
}

void CEnvList::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
	if (m_pScrollbar) UpdateScrollbar(); // 크기 변경 시 스크롤바 업데이트
}

void CEnvList::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);

	// 스크롤바 영역을 0으로 만들어버리기
	if (bCalcValidRects)
	{
		// 클라이언트 영역을 전체 윈도우와 같게 (스크롤바 영역 제거)
		//	rgrc[0] : 새로운 클라이언트 영역(결과값),
		//	rgrc[1] : 새로운 윈도우 전체 영역,
		//	rgrc[2] : 이전 윈도우 전체 영역
		//	rgrc[0]는 rgrc[1]에서 테두리, 타이틀바, 스크롤바 등을 뺀 영역이므로.. rgrc[0] = rgrc[1]를 하면 스크롤바가 사라짐.
		lpncsp->rgrc[0] = lpncsp->rgrc[1];
	}

	// 이렇게 해도 되는데.. 위의 방법이 더 깔끔.
	//lpncsp->rgrc[0].right += GetSystemMetrics(SM_CXVSCROLL); // 스크롤바 너비만큼 확장
	//lpncsp->rgrc[0].bottom += GetSystemMetrics(SM_CYHSCROLL); // 스크롤바 높이만큼 확장
}

void CEnvList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	// 키보드로 스크롤 시 커스텀 스크롤바 동기화
	switch (nChar)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:  // Page Up
	case VK_NEXT:   // Page Down
	case VK_HOME:
	case VK_END:
		if (m_pScrollbar && ::IsWindow(m_pScrollbar->GetSafeHwnd()))
		{
			// 키 처리 후 약간의 지연을 두고 동기화
			PostMessage(WM_USER + 100); // 커스텀 메시지
		}
		break;
	}
}

LRESULT CEnvList::OnUpdateScrollbar(WPARAM wParam, LPARAM lParam)
{
	if (m_pScrollbar) UpdateScrollbar();
	return 0;
}

int CEnvList::InsertItem(int nItem, LPCTSTR lpszItem)
{
	int result = CListCtrl::InsertItem(nItem, lpszItem);
	if (result != -1 && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

int CEnvList::InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
{
	int result = CListCtrl::InsertItem(nItem, lpszItem, nImage);
	if (result != -1 && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}
int CEnvList::InsertItem(const LVITEM* pItem)
{
	int result = CListCtrl::InsertItem(pItem);
	if (result != -1 && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

BOOL CEnvList::DeleteItem(int nItem)
{
	BOOL result = CListCtrl::DeleteItem(nItem);
	if (result && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

BOOL CEnvList::DeleteAllItems()
{
	BOOL result = CListCtrl::DeleteAllItems();
	if (result && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

void CEnvList::SetItemCount(int nItems)
{
	CListCtrl::SetItemCount(nItems);
	if (m_pScrollbar) {
		UpdateScrollbar();
	}
}

BOOL CEnvList::SetItemCountEx(int iCount, DWORD dwFlags)
{
	BOOL result = CListCtrl::SetItemCountEx(iCount, dwFlags);
	if (result && m_pScrollbar) {
		UpdateScrollbar();
	}
	return result;
}

void CEnvList::OnScrollbarAction(int nScrollType)
{
	int currentTop = GetTopIndex();
	int visibleItems = GetCountPerPage();
	int totalItems = GetItemCount();
	int newPos = currentTop;

	switch (nScrollType)
	{
	case SCROLL_LINEUP:   // 1행 위로
		newPos = max(0, currentTop - 1);
		break;

	case SCROLL_LINEDOWN: // 1행 아래로
		newPos = min(totalItems - visibleItems, currentTop + 1);
		break;

	case SCROLL_PAGEUP:   // 1페이지 위로
		newPos = max(0, currentTop - visibleItems);
		break;

	case SCROLL_PAGEDOWN: // 1페이지 아래로
		newPos = min(totalItems - visibleItems, currentTop + visibleItems);
		break;
	}

	// 위치 변경이 발생했을 때만 스크롤 수행
	if (newPos != currentTop)
	{
		CRect rcItem;
		if (GetItemRect(0, &rcItem, LVIR_BOUNDS))
		{
			int itemHeight = rcItem.Height();
			int dy = (newPos - currentTop) * itemHeight;

			// 실제 리스트 픽셀 단위 스크롤
			Scroll(CSize(0, dy));

			// 스크롤바 위치 동기화
			UpdateScrollbar();
		}
	}
}

//---------------------------------------------------------------------------


CEnvListScrollbar::CEnvListScrollbar() :
	m_pOwner(nullptr), m_nMin(0), m_nMax(100), m_nPos(0), m_bDragging(false), m_nPageSize(10)
{}

BEGIN_MESSAGE_MAP(CEnvListScrollbar, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CEnvListScrollbar::SetOwner(CEnvList* pList)
{
	m_pOwner = pList;
}

void CEnvListScrollbar::SetScrollRange(int min, int max)
{
	m_nMin = min;
	m_nMax = max;
	UpdateThumbRect();
	Invalidate();
}

void CEnvListScrollbar::SetScrollPos(int pos)
{
	m_nPos = max(m_nMin, min(pos, m_nMax));
	UpdateThumbRect();
	Invalidate();
}

// 스크롤바에서
void CEnvListScrollbar::SetPageSize(int pageSize)
{
	m_nPageSize = pageSize;
	UpdateThumbRect();
	Invalidate();
}

int CEnvListScrollbar::GetScrollPos() const
{
	return m_nPos;
}

void CEnvListScrollbar::UpdateThumbRect()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nBtnHeight = GetSystemMetrics(SM_CYVSCROLL);
	int trackLength = rcClient.Height() - (nBtnHeight * 2); // 화살표 2개 높이 제외
	int range = m_nMax - m_nMin;

	if (range <= 0 || m_nPageSize <= 0 || trackLength <= 0) {
		m_rcThumb.SetRectEmpty();
		return;
	}

	// Thumb 최소 크기 확보 및 계산
	int thumbSize = max(16, trackLength * m_nPageSize / (range + m_nPageSize));

	// Thumb 위치 계산 (위쪽 화살표 바로 아래부터 시작)
	int thumbPos = nBtnHeight + ((trackLength - thumbSize) * (m_nPos - m_nMin) / range);

	m_rcThumb = CRect(
		rcClient.left,
		rcClient.top + thumbPos,
		rcClient.right,
		rcClient.top + thumbPos + thumbSize
	);

	//CRect rcClient;
	//GetClientRect(&rcClient);

	//int trackLength = rcClient.Height();  // 세로 스크롤 기준
	//int range = m_nMax - m_nMin;

	//if (range <= 0 || m_nPageSize <= 0) {
	//	m_rcThumb.SetRectEmpty();
	//	return;
	//}

	//// Thumb 크기 계산
	//int thumbSize = max(10, trackLength * m_nPageSize / (range + m_nPageSize));

	//// Thumb 위치 계산
	//int thumbPos = (trackLength - thumbSize) * (m_nPos - m_nMin) / range;

	//m_rcThumb = CRect(
	//	rcClient.left,
	//	rcClient.top + thumbPos,
	//	rcClient.right,
	//	rcClient.top + thumbPos + thumbSize
	//);
}

void CEnvListScrollbar::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	// 1. 전체 트랙 배경 (어두운 배경색)
	dc.FillSolidRect(&rcClient, RGB(29, 31, 37));

	int nBtnHeight = GetSystemMetrics(SM_CYVSCROLL); // 표준 화살표 버튼 높이 (보통 17px)

	// A. 위쪽 화살표 영역
	CRect rcTopArrow = rcClient;
	rcTopArrow.bottom = rcTopArrow.top + nBtnHeight;

	// B. 아래쪽 화살표 영역
	CRect rcBottomArrow = rcClient;
	rcBottomArrow.top = rcBottomArrow.bottom - nBtnHeight;

	// 2. 위/아래 화살표 버튼 그리기
	DrawArrowButton(&dc, rcTopArrow, true);
	DrawArrowButton(&dc, rcBottomArrow, false);

	// 3. Thumb (스크롤 핸들) 그리기
	if (!m_rcThumb.IsRectEmpty())
	{
		// Thumb 배경 채우기
		dc.FillSolidRect(&m_rcThumb, RGB(103, 108, 128));

		// 윈도우 스타일 입체 테두리선 (밝은 상/좌, 어두운 우/하)
		dc.Draw3dRect(&m_rcThumb, RGB(140, 145, 165), RGB(60, 65, 80));
	}

	//CPaintDC dc(this);

	//// 전체 배경
	//CRect rcClient;
	//GetClientRect(&rcClient);
	//dc.FillSolidRect(rcClient, RGB(29, 31, 37));

	//// Thumb
	//if (!m_rcThumb.IsRectEmpty())
	//{
	//	// 라운드 영역 생성
	//	CRgn rgn;
	//	rgn.CreateRoundRectRgn(
	//		m_rcThumb.left,
	//		m_rcThumb.top,
	//		m_rcThumb.right + 1,  // +1 안 하면 오른쪽/하단이 잘릴 수 있음
	//		m_rcThumb.bottom + 1,
	//		8, 8  // 둥근 모서리 반지름
	//	);

	//	// 클리핑 영역 설정
	//	dc.SelectClipRgn(&rgn);

	//	// Thumb 색상 채우기
	//	dc.FillSolidRect(m_rcThumb, RGB(103, 108, 128));

	//	// 클리핑 해제
	//	dc.SelectClipRgn(nullptr);
	//}
}

// 화살표 버튼 전용 렌더링 함수 (헤더에 void DrawArrowButton(CDC* pDC, CRect rc, bool bTop); 선언 필요)
void CEnvListScrollbar::DrawArrowButton(CDC* pDC, CRect rc, bool bTop)
{
	// 1. 버튼 배경 및 입체 테두리
	pDC->FillSolidRect(&rc, RGB(45, 48, 56));
	pDC->Draw3dRect(&rc, RGB(70, 73, 82), RGB(20, 22, 26));

	// 2. 화살표 삼각 폴리곤 그리기
	CPen pen(PS_SOLID, 1, RGB(180, 185, 195));
	CBrush brush(RGB(180, 185, 195));

	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CPoint pts[3];
	int cx = rc.CenterPoint().x;
	int cy = rc.CenterPoint().y;

	if (bTop) // 위쪽 화살표 ▲
	{
		pts[0] = CPoint(cx, cy - 3);
		pts[1] = CPoint(cx - 4, cy + 2);
		pts[2] = CPoint(cx + 4, cy + 2);
	}
	else // 아래쪽 화살표 ▼
	{
		pts[0] = CPoint(cx - 4, cy - 2);
		pts[1] = CPoint(cx + 4, cy - 2);
		pts[2] = CPoint(cx, cy + 3);
	}

	pDC->Polygon(pts, 3);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CEnvListScrollbar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nBtnHeight = GetSystemMetrics(SM_CYVSCROLL);

	CRect rcTopArrow(rcClient.left, rcClient.top, rcClient.right, rcClient.top + nBtnHeight);
	CRect rcBottomArrow(rcClient.left, rcClient.bottom - nBtnHeight, rcClient.right, rcClient.bottom);

	// 1. 위쪽 화살표 클릭 -> 1줄 위로 스크롤
	if (rcTopArrow.PtInRect(point))
	{
		OnScrollAction(SCROLL_LINEUP);
	}
	// 2. 아래쪽 화살표 클릭 -> 1줄 아래로 스크롤
	else if (rcBottomArrow.PtInRect(point))
	{
		OnScrollAction(SCROLL_LINEDOWN);
	}
	// 3. Thumb(핸들) 클릭 -> 드래그 시작
	else if (m_rcThumb.PtInRect(point))
	{
		m_bDragging = true;
		m_nDragOffset = point.y - m_rcThumb.top;
		SetCapture();
	}
	// 4. 빈 공간(트랙) 클릭 -> Page Up / Page Down 처리
	else
	{
		if (point.y < m_rcThumb.top)
		{
			// Thumb보다 위쪽 빈 공간 클릭 -> Page Up
			OnScrollAction(SCROLL_PAGEUP);
		}
		else if (point.y > m_rcThumb.bottom)
		{
			// Thumb보다 아래쪽 빈 공간 클릭 -> Page Down
			OnScrollAction(SCROLL_PAGEDOWN);
		}
	}

	CWnd::OnLButtonDown(nFlags, point);

	/*if (m_rcThumb.PtInRect(point)) {
		m_bDragging = true;
		m_nDragOffset = point.y - m_rcThumb.top;
		SetCapture();
	}
	CWnd::OnLButtonDown(nFlags, point);*/
}

void CEnvListScrollbar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging) {
		CRect rcClient;
		GetClientRect(&rcClient);
		int height = rcClient.Height();
		int thumbHeight = m_rcThumb.Height();
		int newY = point.y - m_nDragOffset;
		newY = max(0, min(newY, height - thumbHeight));

		int range = m_nMax - m_nMin;
		int newPos = m_nPos;

		if (height > thumbHeight && range > 0) {
			newPos = m_nMin + (range * newY) / (height - thumbHeight);
			newPos = max(m_nMin, min(newPos, m_nMax)); // 범위 보정
			SetScrollPos(newPos);
			//            if (m_pOwner) m_pOwner->ScrollTo(newPos);
			TRACE(_T("===> MyScroll newPos : %d\n"), newPos);
			OnThumbDrag(newPos);
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CEnvListScrollbar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging) {
		m_bDragging = false;
		ReleaseCapture();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CEnvListScrollbar::OnThumbDrag(int newPos)
{
	if (m_pOwner)
		m_pOwner->OnScrollbarThumbMoved(newPos);
}

void CEnvListScrollbar::OnScrollAction(ScrollType type)
{
	if (m_pOwner)
	{
		m_pOwner->OnScrollbarAction(type);
	}
}

