
// NaPhysicsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NaPhysics2.h"
#include "NaPhysicsDlg2.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNaPhysicsDlg2 dialog



CNaPhysicsDlg2::CNaPhysicsDlg2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NAPHYSICS2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNaPhysicsDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNaPhysicsDlg2, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TIMER, &CNaPhysicsDlg2::OnBnClickedBtnTimer)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &CNaPhysicsDlg2::OnBnClickedBtnUpdate)
	ON_STN_CLICKED(IDC_STATIC_CANVAS, &CNaPhysicsDlg2::OnStnClickedStaticCanvas)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNaPhysicsDlg2 message handlers

BOOL CNaPhysicsDlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	AddBox(50, 50);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNaPhysicsDlg2::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		Render();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNaPhysicsDlg2::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNaPhysicsDlg2::OnBnClickedBtnTimer()
{
	SetTimer(0, 1000 / 60, NULL);
}

void CNaPhysicsDlg2::AddBox(int x, int y)
{
	auto pObj = new PhysicsObject();
	pObj->m_Position.x = x;
	pObj->m_Position.y = y;

	m_vecObjects.push_back(pObj);
}


void CNaPhysicsDlg2::OnBnClickedBtnUpdate()
{
	Update();
}

void CNaPhysicsDlg2::Update()
{
	CRect rcWindow;
	auto wnd = GetDlgItem(IDC_STATIC_CANVAS);
	wnd->GetWindowRect(&rcWindow);
	int nGround = rcWindow.Height();

	std::for_each(m_vecObjects.begin(), m_vecObjects.end(), [&](PhysicsObject *pMe) {
		pMe->m_Collide = false;
		std::for_each(m_vecObjects.begin(), m_vecObjects.end(), [&](PhysicsObject *pOther) {
			if (pMe == pOther) return;

			auto rc1 = pMe->GetRect();
			auto rc2 = pOther->GetRect();

			CRect rcOverlap;
			rcOverlap.IntersectRect(rc1, rc2);
			
			if (rcOverlap.Width() > 0 && rcOverlap.Height() > 0)
			{
				// Collide
				pMe->m_Collide = true;

				if (pMe->m_Position.x < pOther->m_Position.x)
					pMe->m_Speed.x += -rcOverlap.Width();
				else if (pMe->m_Position.x > pOther->m_Position.x)
					pMe->m_Speed.x += rcOverlap.Width();

				if (pMe->m_Position.y < pOther->m_Position.y)
					pMe->m_Speed.y += -rcOverlap.Height();
				else if (pMe->m_Position.y > pOther->m_Position.y)
					pMe->m_Speed.y += rcOverlap.Height();

				if (pMe->m_Position.x == pOther->m_Position.x &&
					pMe->m_Position.y == pOther->m_Position.y)
				{
					if (rand() % 2 == 0)
						pMe->m_Speed.x += 1;
					else
						pMe->m_Speed.x -= 1;

					if (rand() % 2 == 0)
						pMe->m_Speed.y += 1;
					else
						pMe->m_Speed.y -= 1;
				}
			}
		});

		pMe->m_Speed.y += 27.0f;

		if (pMe->m_Position.y >= nGround)
		{
			pMe->m_Position.y = nGround;
			pMe->m_Speed.y = -27.0f;
		}
	});

	std::for_each(m_vecObjects.begin(), m_vecObjects.end(), [&](PhysicsObject *pMe) {
		pMe->m_Position.x += pMe->m_Speed.x / 30.0f;
		pMe->m_Position.y += pMe->m_Speed.y / 30.0f;

		pMe->m_Speed = { 0,0 };
	});

	Render();
}

void CNaPhysicsDlg2::Render()
{
	auto wnd = GetDlgItem(IDC_STATIC_CANVAS);
	auto dc = wnd->GetWindowDC();
	if (dc == nullptr)
		return;

	CRect rcCanvas;
	wnd->GetClientRect(&rcCanvas);

	CBrush br;
	br.CreateSolidBrush(RGB(255, 255, 255));
	dc->FillRect(rcCanvas, &br);

	std::for_each(m_vecObjects.begin(), m_vecObjects.end(), [&](PhysicsObject *pObj) {
		auto position = pObj->m_Position;
		
		/*
		CRect rc;
		rc.bottom = rcCanvas.Height() - position.y;
		rc.top = rc.bottom - pObj->m_Size.cy;
		rc.left = position.x - pObj->m_Size.cx / 2;
		rc.right = rc.left + pObj->m_Size.cx;
		*/

		auto rc = pObj->GetRect();
		if (pObj->m_Collide)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			auto oldPen = dc->SelectObject(pen);

			dc->Rectangle(rc);

			dc->SelectObject(oldPen);
			pen.DeleteObject();
		}
		else
			dc->Rectangle(rc);
	});

	br.DeleteObject();
	dc->DeleteDC();
}


void CNaPhysicsDlg2::OnStnClickedStaticCanvas()
{
	CPoint ptMouse;
	GetCursorPos(&ptMouse);

	CRect rcWindow;
	auto wnd = GetDlgItem(IDC_STATIC_CANVAS);
	wnd->GetWindowRect(&rcWindow);

	ptMouse.Offset(-rcWindow.left, -rcWindow.top);

	//TRACE(L"Clicked : %d, %d\n", ptMouse.x, ptMouse.y);

	//AddBox(ptMouse.x, rcWindow.Height() - ptMouse.y);
	AddBox(ptMouse.x, ptMouse.y);
	Render();
}


void CNaPhysicsDlg2::OnTimer(UINT_PTR nIDEvent)
{
	Update();

	CDialogEx::OnTimer(nIDEvent);
}
