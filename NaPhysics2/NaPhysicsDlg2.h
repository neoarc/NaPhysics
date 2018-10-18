
// NaPhysicsDlg.h : header file
//

#pragma once

#include <vector>
#include <algorithm>

class PhysicsObject
{
public:
	PhysicsObject() : m_Size{ 30, 30 }, m_Speed{ 0,0 }, m_Collide(false) {};

	// #TODO Change to RectF
	CRect GetRect()
	{
		CRect rc(0, 0, 2, 2);
		{
			auto position = m_Position;

			//rc.OffsetRect()
			SIZE &s = m_Size;
			rc.left = position.x - s.cx / 2;
			rc.right = position.x + s.cx / 2;
			rc.top = position.y - s.cy;
			rc.bottom = position.y;
		}

		return rc;
	};

	struct Position2D {
		float x;
		float y;
	} m_Position;

	struct Speed2D {
		float x;
		float y;
	} m_Speed;

	SIZE m_Size;
	bool m_Collide;
};

// CNaPhysicsDlg dialog
class CNaPhysicsDlg2 : public CDialogEx
{
// Construction
public:
	CNaPhysicsDlg2(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAPHYSICS2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnUpdate();
	void Update();
	void Render();
	std::vector<PhysicsObject*> m_vecObjects;

	afx_msg void OnBnClickedBtnTimer();
	void AddBox(int x, int y);
	afx_msg void OnStnClickedStaticCanvas();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
