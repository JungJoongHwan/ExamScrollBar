#include "stdafx.h"
#include "NKCommonUI/NKNewUX/NKControl.h"
#include "NKCommonUI/NKNewUX/NKUser.h"
#include <NKGdiMgr/NKGdiMgr.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNKGdiManager* GetGDIMng();


CNKControl::CNKControl()
{
	m_pParent = nullptr;
	m_pOwner = nullptr;

	m_rcSelf.SetRectEmpty();
	m_nControlID = 0;

	m_clBack = Color(30, 31, 36);
	m_clBorder = Color::Black;
	m_clText = Color::White;

	m_clHover = Color(98, 124, 163);
	m_clHoverText = Color::White;
	m_clDown = Color(51, 55, 61);
	m_clDownText = Color::White;
	m_clDisabled = Color(144, 154, 168);
	m_clDisabledText = Color::White;
	m_clDisabledBorder = Color(144, 154, 168);

	m_strFontName = _T("굴림");
	m_Align = StringAlignmentNear;
	m_nFontSize = 9;
	m_nFontStyle = FontStyleRegular;
	m_nBkMode = OPAQUE;
	m_MouseStatus = MouseStatusNormal;
	m_nVisible = SW_SHOW;
	m_nBorder = ControlBorderNone;
	m_pFont = nullptr;

	m_nItemHeight = 20;
	m_nItemWidth = 50;
	m_bEnable = TRUE;

	m_pImgBack = nullptr;
	m_nLangMode = 1;
	m_nFocus = WA_INACTIVE;
	m_bRoundMode = FALSE;
	m_fDiameter = 1.0f;
	m_bFillMode = TRUE;
	m_fBorder = 2.0f;
	m_nRoundHeaderHeight = 0;
	m_bFixed = FALSE;
}


CNKControl::~CNKControl()
{
	SAFE_DELETE(m_pImgBack);
}


BOOL CNKControl::InitControl(LPCTSTR lpszClassName, LPCTSTR lpszCaption, CRect rcCtrl, DWORD dwStyle, UINT nID, CWnd* pParent)
{
	m_strClassName = lpszClassName;
	m_strCaption = lpszCaption;
	m_rcSelf = rcCtrl;
	m_dwStyle = dwStyle;
	m_nControlID = nID;
	m_pParent = pParent;
	return TRUE;
}


void CNKControl::Draw(Graphics& g)
{
	if (m_pParent)
		return;
}


void CNKControl::DrawRound(Graphics& g, Rect rcItem, Color clBack, Color clBorder, float fDiameter, BOOL bFill)
{
	GraphicsState state = g.Save();
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	GraphicsPath path;
	int diameter = int(rcItem.Height * fDiameter);

	if (0 > fDiameter)
		diameter = int(abs(fDiameter));

	path.AddArc(rcItem.X, rcItem.Y, diameter, diameter, 180, 90); // 좌상단
	path.AddArc(rcItem.GetRight() - diameter, rcItem.Y, diameter, diameter, 270, 90); // 우상단
	path.AddArc(rcItem.GetRight() - diameter, rcItem.GetBottom() - diameter, diameter, diameter, 0, 90); // 우하단
	path.AddArc(rcItem.X, rcItem.GetBottom() - diameter, diameter, diameter, 90, 90); // 좌하단
	path.CloseFigure();

	if (bFill)
	{
		SolidBrush brush(clBack);
		g.FillPath(&brush, &path);

	}

	if (0 < m_nRoundHeaderHeight)
	{
		GraphicsPath headpath;

		headpath.AddArc(rcItem.X, rcItem.Y, diameter, diameter, 180, 90); // 좌상단
		headpath.AddLine(rcItem.X + diameter, rcItem.Y, rcItem.GetRight() - diameter, rcItem.Y);
		headpath.AddArc(rcItem.GetRight() - diameter, rcItem.Y, diameter, diameter, 270, 90); // 우상단
		headpath.AddLine(rcItem.GetRight(), rcItem.Y + diameter, rcItem.GetRight(), rcItem.Y + m_nRoundHeaderHeight);
		headpath.AddLine(rcItem.GetRight(), rcItem.Y + m_nRoundHeaderHeight, rcItem.X, rcItem.Y + m_nRoundHeaderHeight);
		headpath.AddLine(rcItem.X, rcItem.Y + m_nRoundHeaderHeight, rcItem.X, rcItem.Y + diameter);
		headpath.CloseFigure();

		SolidBrush brush(m_clRoundHeaderColor);
		g.FillPath(&brush, &headpath);
	}

	Pen pen(clBorder, m_fBorder);
	g.DrawPath(&pen, &path);

	g.Restore(state);
}

void CNKControl::DrawFixedRound(Graphics& g, Rect rcItem, Color clBack, Color clBorder, float fDiameter, BOOL bFill)
{
	GraphicsState state = g.Save();
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	GraphicsPath path;
	int diameter(0); 

	if (fDiameter < 1.0 && !(m_dwStyle & 0x80000000))
		diameter = int(400 * fDiameter);  // 컨트롤의 높이가 작으면 모서리의 round도 같이 작아지는 문제를 수정. 높이 400으로 무조건 고정.
	else
		diameter = int(rcItem.Height * fDiameter);

	if (0 > fDiameter)
		diameter = int(abs(fDiameter));

	path.AddArc(rcItem.X, rcItem.Y, diameter, diameter, 180, 90); // 좌상단
	path.AddArc(rcItem.GetRight() - diameter, rcItem.Y, diameter, diameter, 270, 90); // 우상단
	path.AddArc(rcItem.GetRight() - diameter, rcItem.GetBottom() - diameter, diameter, diameter, 0, 90); // 우하단
	path.AddArc(rcItem.X, rcItem.GetBottom() - diameter, diameter, diameter, 90, 90); // 좌하단
	path.CloseFigure();

	if (bFill)
	{
		SolidBrush brush(clBack);
		g.FillPath(&brush, &path);
	}

	if (0 < m_nRoundHeaderHeight)
	{
		GraphicsPath headpath;

		headpath.AddArc(rcItem.X, rcItem.Y, diameter, diameter, 180, 90); // 좌상단
		headpath.AddLine(rcItem.X + diameter, rcItem.Y, rcItem.GetRight() - diameter, rcItem.Y);
		headpath.AddArc(rcItem.GetRight() - diameter, rcItem.Y, diameter, diameter, 270, 90); // 우상단
		headpath.AddLine(rcItem.GetRight(), rcItem.Y + diameter, rcItem.GetRight(), rcItem.Y + m_nRoundHeaderHeight);
		headpath.AddLine(rcItem.GetRight(), rcItem.Y + m_nRoundHeaderHeight, rcItem.X, rcItem.Y + m_nRoundHeaderHeight);
		headpath.AddLine(rcItem.X, rcItem.Y + m_nRoundHeaderHeight, rcItem.X, rcItem.Y + diameter);
		headpath.CloseFigure();

		SolidBrush brush(m_clRoundHeaderColor);
		g.FillPath(&brush, &headpath);
	}

	Pen pen(clBorder, m_fBorder);
	g.DrawPath(&pen, &path);

	g.Restore(state);
}


void CNKControl::DrawIdBitmap(Graphics& g, int nImageId, Rect rcSrc, Color clBack)
{
	CDC* pDC = m_pParent->GetDC();

	CDC tmpdc;
	tmpdc.CreateCompatibleDC(pDC);

	CBitmap tmpBmp;
	tmpBmp.CreateCompatibleBitmap(pDC, rcSrc.Width, rcSrc.Height);
	CBitmap* pOldBmp = tmpdc.SelectObject(&tmpBmp);

	DWORD dwBGColorRGBA = GetGDIMng()->GetBitmapMng()->ConvertRGBToRGBA(RGB(clBack.GetR(), clBack.GetG(), clBack.GetB()));
	GetGDIMng()->GetBitmapMng()->DrawIdBmpEx(&tmpdc, CRect(0, 0, rcSrc.Width, rcSrc.Height), nImageId, dwBGColorRGBA);
	Bitmap bitmap(tmpBmp, NULL);

	g.DrawImage(&bitmap, rcSrc.X, rcSrc.Y);

	tmpdc.SelectObject(pOldBmp);
	m_pParent->ReleaseDC(pDC);
}


void CNKControl::DrawBorder(Graphics& g)
{
//	g.DrawRectangle(&Pen(Color(m_clBorder)), m_rcSelf.left, m_rcSelf.top, m_rcSelf.Width(), m_rcSelf.Height());

	if (ControlBorderNone < m_nBorder)
	{
		Color clBorder = m_clBorder;
		if (!m_bEnable)
			clBorder = m_clDisabled;

		Pen border(clBorder);
		if (ControlBorderAll == m_nBorder)
		{
			g.DrawRectangle(&border, m_rcSelf.left, m_rcSelf.top, m_rcSelf.Width(), m_rcSelf.Height());
		}
		else
		{
			if (m_nBorder & int(ControlBorderLeft))
				g.DrawLine(&border, Point(m_rcSelf.left, m_rcSelf.top), Point(m_rcSelf.left, m_rcSelf.bottom));

			if (m_nBorder & int(ControlBorderTop))
				g.DrawLine(&border, Point(m_rcSelf.left, m_rcSelf.top), Point(m_rcSelf.right, m_rcSelf.top));

			if (m_nBorder & int(ControlBorderRight))
				g.DrawLine(&border, Point(m_rcSelf.right, m_rcSelf.top), Point(m_rcSelf.right, m_rcSelf.bottom));

			if (m_nBorder & int(ControlBorderBottom))
				g.DrawLine(&border, Point(m_rcSelf.left, m_rcSelf.bottom), Point(m_rcSelf.right, m_rcSelf.bottom));
		}
	}
}


void CNKControl::MeasureString(LPCTSTR lpszText, RectF& rcAdjust)
{
	if (!m_pFont) return;

	CDC* pDC = m_pParent->GetDC();
	CFont* pOldFont = pDC->SelectObject(m_pFont);

	CSize szText = pDC->GetOutputTextExtent(lpszText);
	rcAdjust.Width = szText.cx + 1;
	rcAdjust.Height = szText.cy + 1;

	pDC->SelectObject(pOldFont);
	m_pParent->ReleaseDC(pDC);
}


void CNKControl::DrawString(Graphics& g, LPCTSTR lpszText, RectF rcSrc, StringFormat& textFormat, Color clBack, Color clText)
{
	if (!m_pFont) return;

	CDC* pDC = m_pParent->GetDC();

	CDC tmpdc;
	tmpdc.CreateCompatibleDC(pDC);

	CRect rcTemp(0, 0, int(rcSrc.Width), int(rcSrc.Height));

	CBitmap tmpBmp;
	tmpBmp.CreateCompatibleBitmap(pDC, rcTemp.Width(), rcTemp.Height());
	CBitmap* pOldBmp = tmpdc.SelectObject(&tmpBmp);
	CFont* pOldFont = tmpdc.SelectObject(m_pFont);

	UINT nFormat = DT_VCENTER;
	if (StringAlignmentNear == textFormat.GetAlignment())
		nFormat |= DT_LEFT;
	else if (StringAlignmentCenter == textFormat.GetAlignment())
		nFormat |= DT_CENTER;
	else
		nFormat |= DT_RIGHT;

	tmpdc.FillSolidRect(rcTemp, clBack.ToCOLORREF());
	tmpdc.SetTextColor(clText.ToCOLORREF());
	tmpdc.DrawText(lpszText, rcTemp, nFormat);

	Bitmap bitmap(tmpBmp, NULL);

	GraphicsState state = g.Save();
	g.SetInterpolationMode(InterpolationModeNearestNeighbor);
	g.SetPixelOffsetMode(PixelOffsetModeHalf);
	g.DrawImage(&bitmap, rcSrc.X, rcSrc.Y);
	g.Restore(state);

	tmpdc.SelectObject(pOldFont);
	tmpdc.SelectObject(pOldBmp);
	m_pParent->ReleaseDC(pDC);
}


void CNKControl::MoveControl(CRect& rcNew)
{
	m_rcSelf = rcNew;
}


void CNKControl::SetControlRect(CRect& rc)
{
	m_rcSelf = rc;
}


void CNKControl::ReDraw()
{
	if (m_pParent)
	{
		CNKControl* pOwner = GetOwnerControl();
		if (pOwner)
		{
			CString strClassname = pOwner->GetClassName();
			if (_T("scrolllabel") == strClassname.MakeLower())
			{
				CRect rcTemp = m_rcSelf;
				pOwner->LPtoDP(rcTemp);
				m_pParent->InvalidateRect(rcTemp);
				return;
			}
		}

		m_pParent->InvalidateRect(m_rcSelf);
	}
}


void CNKControl::ReDrawRect(CRect& rc)
{
	if (m_pParent)
		m_pParent->InvalidateRect(rc);
}


void CNKControl::MoveControl(int nLeft, int nTop, int nWidth, int nHeight)
{
	CRect rcNew;
	rcNew.SetRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);

	MoveControl(rcNew);
}


void CNKControl::SetFontAttr(LPCTSTR lpszFontName, int nSize, int nFontStyle)
{
	m_strFontName = lpszFontName;
	m_nFontSize = nSize;
	m_nFontStyle = nFontStyle;
}


BOOL CNKControl::LoadControlImage(Image*& pImg, CString& strPath)
{
	if (pImg)
		SAFE_DELETE(pImg);

	BOOL bResult = FALSE;

	pImg = Image::FromFile(strPath);

	if (!pImg)
		return bResult;

	if (Ok == pImg->GetLastStatus())
		bResult = TRUE;

	return bResult;
}


void CNKControl::SetCaptionLang(CStringArray& arrCaption)
{
	for (int i = 0; i < 2; ++i)
		m_strCaptionLang[i] = arrCaption[i];

	SetCaption(m_strCaptionLang[m_nLangMode]);
}

