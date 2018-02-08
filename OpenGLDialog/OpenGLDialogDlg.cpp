
// OpenGLDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OpenGLDialog.h"
#include "OpenGLDialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WIDTH 640
#define HEIGHT 480

#define SPEED 1
// COpenGLDialogDlg dialog

static CDC* pDC;

COpenGLDialogDlg::COpenGLDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENGLDIALOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenGLDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenGLDialogDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COpenGLDialogDlg message handlers

BOOL COpenGLDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon

	SetWindowPos(&wndTop, 0, 0, WIDTH, HEIGHT, SWP_NOMOVE);
	pDC = GetDC();
	CenterWindow();
	Init();
	SetTimer(1, SPEED, NULL);

	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void COpenGLDialogDlg::Init()
{
	// ���������� �������
	CRect rect;
	/*	
		HGLRC - Render Context
		�������� ��������� ���������� � ����� �������� ���������� ����� ������������ ������� OpenGL. ��������, ���� � ���������
		���� ��������� ���� OpenGL, �� ����� ������� ������ OpenGL ���������� ������� ����, � ������� ����� ���������� ��� �������.
		� ���������� ��������� ������������� ������� ��������� OpenGL, ��������, ���������� ������ � �.�. 
	*/
	HGLRC hrc;

	if (!bSetupPixelFormat())
		return;

	// �������� ��������� opengl - ������� windows - ��� �-��� ����� ������� "wgl"
	// �������� hendle ���������
	hrc = wglCreateContext(pDC->GetSafeHdc());

	// ������, ���� ������� ������� �� �������
	ASSERT(hrc != NULL);

	// ������� �������� OpenGL �������� ��� ��������� ���� Windows
	wglMakeCurrent(pDC->GetSafeHdc(), hrc);

	// �������� ������ ���������� �������
	GetClientRect(&rect);
	
	// ������� ����� ����� � ����� �������
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);

	glDepthFunc(GL_LESS);
	/*
		glEnable ���������� ��������� � ���������� ���� ��� ����� ������ ������ ��������� OpenGL. 
		��� ������� ����������� ���������� �����, � �� ��������� ��������� ����� ��������������� � ������ ���������� ������.
	*/
	glEnable(GL_DEPTH_TEST);
	/*
		�������� ����� ��������� ��������� �����, �, ���� ������� ��������������� �����, �� ����� ����������� �������� ������������ ������ �� ����������� ���������.
		��� ���������� ������� ������������ ������������ �������
			void glShadeModel (GLenum  mode)
		����� ������� � ���������� GL_SMOOTH �������� ������������ (��������� �� ���������), � � GL_FLAT � ���������.
	*/
	glShadeModel(GL_SMOOTH);
	/*
		��� ������� ��������� �������������� �������� ����� � OpenGL ������������ �������� ��� ���������,
		��� ���� ��������� ��� ���� ������: ��������-�������, ������� �������� � ������� �������� (GL_MODELVIEW, GL_PROJECTION, ��� GL_TEXTURE). 
		��� ��� ����� ������ 4x4. ������� ������� ���������� �������������� ������� � ������� �����������, 
		����� ��� ������������ �������, ��������� �������� � �������. ������� �������� ����������, ��� ����� �������������� ���������� ������� �� ��������� ������
		(� ������� ����������), � ������� �������� ���������� ��������� �������� �� ������. 
	*/
	glMatrixMode(GL_PROJECTION);

	// �������� ������� ������� �� ���������
	glLoadIdentity();
	/*
		������������� ��������
		������� ������ ��������� ����� ��������� � ������������� ������� ���������. 
		�������� angley ���������� ���� ��������� � �������� �� ��� � � ������ ���������� � ��������� �� 0 �� 180. 
		���� ��������� ����� ��� x �������� ���������� aspect, ������� ������ �������� ��� ��������� ������ ������� ������ 
		(��� �������, �������� ����) ��������� zfar � znear ������ ���������� �� ����������� �� ���������� ��������� �� ������� � 
		������ ���� ��������������. ��� ������ ��������� zfar/znear, ��� ���� � ������ ������� ����� ����������� �������������
		����� �����������, ��� ��� �� ��������� � ���� ����� ������������ �������� ������� � ��������� �� 0 �� 1 (��. �. 0.).
	*/
	gluPerspective(45.0f, (GLfloat)rect.right /	(GLfloat)rect.bottom, 0.1f, 100.0f);
	// ��������� ���� ������� ��� ��������-�������
	glMatrixMode(GL_MODELVIEW);

}

BOOL COpenGLDialogDlg::bSetupPixelFormat()
{
	/* Pixel format
		����� ��������� ��������� �������� ����� ���������� ������ �������� (pixel format) ���������.
		��� ����� ��� ����, ����� �������� ������������ �������, ����� ������� ���������� �������� ��� ������� ���������.
		������ �������� ���������, ����� ����������� OpenGL �� ����� ������������: ������� �����, ����� �����, ����� �������, ������ ����� � �.�.
		����� ���������� ������ �������, ����� ��������� ��������� PIXELFORMATDESCRIPTOR � �������� �� � ������� ��������
	*/
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
		1, // version number
		PFD_DRAW_TO_WINDOW | // support window
		PFD_SUPPORT_OPENGL | // support OpenGL
		PFD_DOUBLEBUFFER, // double buffered
		PFD_TYPE_RGBA, // RGBA type
		24, // 24-bit color depth
		0, 0, 0, 0, 0, 0, // color bits ignored
		0, // no alpha buffer
		0, // shift bit ignored
		0, // no accumulation buffer
		0, 0, 0, 0, // accum bits ignored
		32, // 32-bit z-buffer
		0, // no stencil buffer
		0, // no auxiliary buffer
		PFD_MAIN_PLANE, // main layer
		0, // reserved
		0, 0, 0 // layer masks ignored
	};

	/*
		Now that we have a PIXELFORMATDESCRIPTOR, we need to convert this into a pixel format number. 
		We do this with the function ChoosePixelFormat. 
		This function takes a device context and PFD struct and returns a pixel format number. 
		If it returns 0, then it could not find a pixel format that matches the description, or the PDF was not filled out correctly.

		Once you have the pixel format number, you can set it into the DC with SetPixelFormat.
		This function takes the DC, the pixel format number, and a PFD struct pointer. 
		Don't get excited about being able to supply the PFD struct; 
		it doesn't read any important information out of it to set the pixel format into the context.
	*/
	int pixelformat;
	if ((pixelformat = ChoosePixelFormat(pDC->GetSafeHdc(), &pfd)) == 0)
	{
		MessageBox(L"ChoosePixelFormat failed");
		return FALSE;
	}
	// �������� ������� ������� � ������� ��������
	if (SetPixelFormat(pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox(L"SetPixelFormat failed");
		return FALSE;
	}
	return TRUE;
}

// ���������� ��� ������ ������ �������
void COpenGLDialogDlg::DrawScene()
{
	static GLfloat angle = 0;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -2.0f);
	
	glRotatef(angle, 1.0f, 1.0f, 0.0f);
	
	glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
	glEnd();

	glFinish();
	// ������ ����������� ���������� ������� ������ ���� � �������� �����
	SwapBuffers(wglGetCurrentDC());
	
	angle += 0.5f;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COpenGLDialogDlg::OnPaint()
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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COpenGLDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// -------------------------------------------------------------------------------------

BOOL COpenGLDialogDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CDialogEx::PreCreateWindow(cs);
}


void COpenGLDialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	DrawScene();

	CDialogEx::OnTimer(nIDEvent);
}


void COpenGLDialogDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	HGLRC hrc;
	// ������� ������ �������
	KillTimer(1);
	// �������� ������� �������� OpenGL
	hrc = ::wglGetCurrentContext();
	// ������� ��������
	::wglMakeCurrent(NULL, NULL);
	// ����������� ������ ��� ���������
	if (hrc)
		::wglDeleteContext(hrc);

	CDialogEx::OnClose();
}


void COpenGLDialogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	HGLRC hrc;
	KillTimer(1);
	hrc = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL, NULL);
	if (hrc)
		::wglDeleteContext(hrc);
}
