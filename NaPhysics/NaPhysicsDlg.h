
// NaPhysicsDlg.h : header file
//

#pragma once

#include <vector>
#include <algorithm>

#include "Box2D/Box2D.h"

class PhysicsObject
{
public:
	PhysicsObject() : m_Size{ 0,0 } {};
	CRect GetRect()
	{
		CRect rc(0, 0, 2, 2);
		if (m_pBody)
		{
			auto position = m_pBody->GetPosition();
			
			//rc.OffsetRect()
			SIZE &s = m_Size;
			rc.left = position.x - s.cx / 2;
			rc.right = position.x + s.cx / 2;
			rc.top = position.y - s.cy;
			rc.bottom = position.y;
		}

		return rc;
	};

	b2Body *m_pBody;
	SIZE m_Size;
};

// CNaPhysicsDlg dialog
class CNaPhysicsDlg : public CDialogEx
{
// Construction
public:
	CNaPhysicsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAPHYSICS_DIALOG };
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

	b2Vec2 m_Gravity;
	b2World m_World;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStaticCanvas();
	afx_msg void OnBnClickedBtnTimer();
	void AddBox(int x, int y);
};
