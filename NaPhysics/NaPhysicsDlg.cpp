
// NaPhysicsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NaPhysics.h"
#include "NaPhysicsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNaPhysicsDlg dialog



CNaPhysicsDlg::CNaPhysicsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NAPHYSICS_DIALOG, pParent)
	//, m_Gravity(0.0f, -10.0f)
	, m_Gravity(0.0f, 0.0f)
	, m_World(m_Gravity)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNaPhysicsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNaPhysicsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_UPDATE, &CNaPhysicsDlg::OnBnClickedBtnUpdate)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_CANVAS, &CNaPhysicsDlg::OnStnClickedStaticCanvas)
	ON_BN_CLICKED(IDC_BTN_TIMER, &CNaPhysicsDlg::OnBnClickedBtnTimer)
END_MESSAGE_MAP()


// CNaPhysicsDlg message handlers

BOOL CNaPhysicsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//////////////////////////////////////////////////////////////////////////
	// Define the gravity vector.
	//b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	//b2World world(gravity);
	//m_World.SetGravity(gravity);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = m_World.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(500.0f, 10.0f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	//////////////////////////////////////////////////////////////////////////
	AddBox(50, 50);
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNaPhysicsDlg::OnPaint()
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
HCURSOR CNaPhysicsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNaPhysicsDlg::OnBnClickedBtnUpdate()
{
	Update();
}

void CNaPhysicsDlg::Update()
{
	// It is generally best to keep the time step and iterations fixed.
	//float32 timeStep = 1.0f / 60.0f;
	float32 timeStep = 1.0f / 30.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	m_World.Step(timeStep, velocityIterations, positionIterations);

	// Now print the position and angle of the body.
	std::for_each(m_vecObjects.begin(), m_vecObjects.end(),
		[&](PhysicsObject *pObj) {
		auto body = pObj->m_pBody;
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		//TRACE(L"%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	});

	Render();
}

void CNaPhysicsDlg::Render()
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

	std::for_each(m_vecObjects.begin(), m_vecObjects.end(), 
		[&](const PhysicsObject *pObj) {
		auto position = pObj->m_pBody->GetPosition();
		CRect rc;
		rc.bottom = rcCanvas.Height() - position.y;
		rc.top = rc.bottom - pObj->m_Size.cy;
		rc.left = position.x - pObj->m_Size.cx / 2;
		rc.right = rc.left + pObj->m_Size.cx;

		//auto rcObj = pObj->GetRect();
		dc->Rectangle(rc);
	});

	br.DeleteObject();
	dc->DeleteDC();
}


void CNaPhysicsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	Update();

	CDialogEx::OnTimer(nIDEvent);
}


void CNaPhysicsDlg::OnStnClickedStaticCanvas()
{
	CPoint ptMouse;
	GetCursorPos(&ptMouse);

	CRect rcWindow;
	auto wnd = GetDlgItem(IDC_STATIC_CANVAS);
	wnd->GetWindowRect(&rcWindow);

	ptMouse.Offset(-rcWindow.left, -rcWindow.top);

	//TRACE(L"Clicked : %d, %d\n", ptMouse.x, ptMouse.y);

	AddBox(ptMouse.x, rcWindow.Height() - ptMouse.y);
	Render();
}


void CNaPhysicsDlg::OnBnClickedBtnTimer()
{
	SetTimer(0, 1000 / 60, NULL);
}

void CNaPhysicsDlg::AddBox(int x, int y)
{
	auto pObj = new PhysicsObject();
	m_vecObjects.push_back(pObj);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	//bodyDef.position.Set(0.0f, 4.0f);
	bodyDef.position.Set(x, y);
	pObj->m_pBody = m_World.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	pObj->m_Size = { 30, 30 };
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(pObj->m_Size.cx / 2, pObj->m_Size.cy / 2);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	pObj->m_pBody->CreateFixture(&fixtureDef);
}
