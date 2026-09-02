
// ExamScrollBarDlg.h: 헤더 파일
//

#pragma once

#include "EnvCtrls.h"

// CExamScrollBarDlg 대화 상자
class CExamScrollBarDlg : public CDialogEx
{
// 생성입니다.
public:
	CExamScrollBarDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMSCROLLBAR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CEnvList          m_LinkList;
	CEnvListScrollbar m_ListScrollbar;

	CFont m_dlgFont;
public:
	void SetDialogFont(int nPtSize, LPCTSTR lpszFaceName);	// 전체폰트적용함수
	void InsertListItem(CString strNo, CString strName);
};
