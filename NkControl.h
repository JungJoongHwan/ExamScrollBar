#pragma once

#include "ColorConverter.h"
#include "NKUser.h"

#include "Gdiplus.h"
using namespace Gdiplus;

#ifdef _NKCOMMONUI_EXPORT
#define NKCOMMONUI_CLASS	__declspec(dllexport)
#define NKCOMMONUI_API		__declspec(dllexport)
#else
#define NKCOMMONUI_CLASS	__declspec(dllimport)
#define NKCOMMONUI_API		__declspec(dllimport)
#endif

class NKCOMMONUI_CLASS CNKControl
{
public:
	CNKControl();
	virtual ~CNKControl();

	virtual BOOL InitControl(LPCTSTR lpszClassName, LPCTSTR lpszCaption, CRect rcCtrl, DWORD dwStyle, UINT nID, CWnd* pParent);
	virtual void SetFontAttr(LPCTSTR lpszFontName, int nSize, int nFontStyle = FontStyleRegular);
	virtual void SetFontAttr(CFont* pFont) { m_pFont = pFont; }
	virtual void Draw(Graphics& g);
	virtual void MeasureString(LPCTSTR lpszText, RectF& rcAdjust);
	virtual void DrawBorder(Graphics& g);
	virtual void DrawString(Graphics& g, LPCTSTR lpszText, RectF rcSrc, StringFormat& textFormat, Color clBack, Color clText);
	virtual void ReDraw();
	virtual void ReDrawRect(CRect& rc);
	virtual void CtrlProc(CNKControl* pSender, UINT nMessage, WPARAM wParam, LPARAM lParam) {};

	virtual void MoveControl(CRect& rcNew);
	virtual void MoveControl(int nLeft, int nTop, int nWidth, int nHeight);
	virtual void SetControlRect(CRect& rc);
	virtual void LPtoDP(CPoint& pt, int noffset) {};
	virtual void LPtoDP(CRect& rcCtl) {};

	virtual void SelChangeItem(int nSelectedItem) {};
	virtual void GetEditControlRect(CRect& rcEdit) {};
	virtual int GetBorder() { return m_nBorder; }
	virtual void SetBorder(int nBorder) { m_nBorder = nBorder; }
	virtual CString GetCaption() { return m_strCaption; }
	virtual void SetCaption(LPCTSTR lpszCaption) { m_strCaption = lpszCaption; }
	virtual void SetFocusFlag(UINT nID) { m_nFocus = nID; };
	virtual void SetParent(CWnd* pParent) { m_pParent = pParent; }
	virtual void EnableControl(BOOL bEnable = TRUE) { m_bEnable = bEnable; ReDraw(); }
	virtual void SetBackImageFiles(CString& strPath, CStringArray& arrFiles) {};
	virtual BOOL IsRoundMode() { return m_bRoundMode; }
	virtual void SetRoundMode(BOOL bRound, float fDiameter = 1.0f, BOOL bFill = TRUE, REAL fBorder = 2.0f) 
	{ 
		m_bRoundMode = bRound;
		m_fDiameter = fDiameter;
		m_bFillMode = bFill;
		m_fBorder = fBorder;
	}
	virtual void SetFixedRound(BOOL bFixed)
	{
		m_bFixed = bFixed;
	}
	virtual void SetRoundHeader(int nHeight, Color clHeader) 
	{
		m_nRoundHeaderHeight = nHeight; 
		m_clRoundHeaderColor = clHeader;
	}
	virtual void OnMouseMove(UINT nFlags, CPoint point) {};
	virtual void OnMouseHover(UINT nFlags, CPoint point) {};
	virtual void OnMouseLeave() {};
	virtual void OnLButtonDown(UINT nFlags, CPoint point) {};
	virtual void OnLButtonUp(UINT nFlags, CPoint point) {};
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point) {};
	virtual void OnRButtonDown(UINT nFlags, CPoint point) {};
	virtual void OnRButtonUp(UINT nFlags, CPoint point) {};
	virtual void OnRButtonDblClk(UINT nFlags, CPoint point) {};
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) { return FALSE; }
	virtual void OnVScroll(UINT nFlags, UINT nPos) {};
	virtual void OnSetFocus(CNKControl* pOldCtrl) {};
	virtual void OnKillFocus(CNKControl* pNewCtrl) {};
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {};
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {};

	CNKControl* GetOwnerControl() { return m_pOwner; }
	void SetOwnerControl(CNKControl* pOwner) { m_pOwner = pOwner; }

	CWnd* GetParent() { return m_pParent; }

	UINT GetControlID() { return m_nControlID; }
	void SetControlID(UINT nID) { m_nControlID = nID; }

	UINT GetControlStyle() { return m_dwStyle; }
	void SetControlStyle(DWORD dwStyle) { m_dwStyle = dwStyle; }

	CRect GetControlRect() { return m_rcSelf; }
	void SetControlRect(int nX, int nY, int nCX, int nCY) { m_rcSelf.SetRect(nX, nY, nX + nCX, nY + nCY); }

	Color GetBorderColor() { return m_clBorder; }
	void SetBorderColor(Color clBorder) { m_clBorder = clBorder; }

	Color GetDisabledBorderColor() { return m_clDisabledBorder; }
	void SetDisabledBorderColor(Color clDisabledBorder) { m_clDisabledBorder = clDisabledBorder; }

	virtual Color GetTextColor() { return m_clText; }
	virtual void SetTextColor(Color clText) { m_clText = clText; }

	virtual Color GetBackColor() { return m_clBack; }
	virtual void SetBackColor(Color clBack) { m_clBack = clBack; }

	Color GetHoverColor() { return m_clHover; }
	void SetHoverColor(Color clHover) { m_clHover = clHover; }

	Color GetHoverTextColor() { return m_clHoverText; }
	void SetHoverTextColor(Color clHoverText) { m_clHoverText = clHoverText; }

	Color GetDownColor() { return m_clDown; }
	void SetDownColor(Color clDown) { m_clDown = clDown; }

	Color GetDownTextColor() { return m_clDownText; }
	void SetDownTextColor(Color clDownText) { m_clDownText = clDownText; }

	Color GetDisabledColor() { return m_clDisabled; }
	void SetDisabledColor(Color clDisabled) { m_clDisabled = clDisabled; }

	Color GetDisabledTextColor() { return m_clDisabledText; }
	void SetDisabledTextColor(Color clDisabledText) { m_clDisabledText = clDisabledText; }

	StringAlignment GetAlignment() { return m_Align; }
	void SetAlignment(StringAlignment align) { m_Align = align; }

	MouseStatus GetMouseStatus() { return m_MouseStatus; }
	void SetMouseStatus(MouseStatus status) { m_MouseStatus = status; }

	void SetItemHeight(int nHeight) { m_nItemHeight = nHeight; }
	void SetItemWidth(int nWidth) { m_nItemWidth = nWidth; }

	int GetBkMode() { return m_nBkMode;	}
	void SetBkMode(int nBkMode = OPAQUE) { m_nBkMode = nBkMode; };
	void OffsetRect(int nX, int nY) { m_rcSelf.OffsetRect(nX, nY); }
	LPCTSTR GetClassName() { return m_strClassName; }
	int IsVisibleControl() { return m_nVisible; }
	virtual void ShowControl(int nShow = SW_SHOW) { m_nVisible = nShow; }
	BOOL IsEnableControl() { return m_bEnable; }

	BOOL SetImageBack(CString& strFile) { return LoadControlImage(m_pImgBack, strFile); };
	void SetLanguageMode(int nMode) { m_nLangMode = nMode; }
	void SetCaptionLang(CStringArray& arrCaption);
	void DrawRound(Graphics& g, Rect rcItem, Color clBack, Color clBorder, float fDiameter = 1.0f, BOOL bFill = TRUE);
	void DrawFixedRound(Graphics& g, Rect rcItem, Color clBack, Color clBorder, float fDiameter = 1.0f, BOOL bFill = TRUE);
	void DrawIdBitmap(Graphics& g, int nImageId, Rect rcSrc, Color clBack);

protected:
	BOOL LoadControlImage(Image*& pImg, CString& strPath);

protected:
	CWnd* m_pParent;
	CNKControl* m_pOwner;
	CRect m_rcSelf;
	UINT m_nControlID;
	UINT m_dwStyle;
	CString m_strClassName;
	CString m_strCaption;
	CString m_strCaptionLang[2];

	Color m_clBack;
	Color m_clBorder;
	Color m_clText;

	Color m_clHover;
	Color m_clHoverText;
	Color m_clDown;
	Color m_clDownText;
	Color m_clDisabled;
	Color m_clDisabledText;
	Color m_clDisabledBorder;

	CString m_strFontName;
	StringAlignment m_Align;
	int m_nFontSize;
	int m_nFontStyle;
	int m_nBkMode;
	MouseStatus m_MouseStatus;
	CFont* m_pFont;

	BOOL m_nVisible;
	int m_nBorder;
	int m_nItemHeight;
	int m_nItemWidth;
	BOOL m_bEnable;

	Image* m_pImgBack;
	int m_nLangMode;
	UINT m_nFocus;
	BOOL m_bRoundMode;
	float m_fDiameter;
	BOOL m_bFillMode;
	REAL m_fBorder;
	int m_nRoundHeaderHeight;
	Color m_clRoundHeaderColor;
	BOOL m_bFixed;
};

typedef CArray< CNKControl*, CNKControl*> CArrayNKControl;
typedef CMap<CString, LPCTSTR, CNKControl*, CNKControl*> CMapNKControl;

