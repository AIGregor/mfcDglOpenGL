
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
	// Клиентская область
	CRect rect;
	/*	
		HGLRC - Render Context
		Контекст рисования определяет в какой контекст устройства будут направляться команды OpenGL. Например, если в программе
		есть несколько окон OpenGL, то перед вызовом команд OpenGL необходимо указать окно, в которое будут направлены эти команды.
		С контекстом рисования ассоциировано текущее состояние OpenGL, текстуры, дисплейные списки и т.п. 
	*/
	HGLRC hrc;

	if (!bSetupPixelFormat())
		return;

	// Создание контекста opengl - функция windows - все ф-ции имеют префикс "wgl"
	// Получаем hendle контекста
	hrc = wglCreateContext(pDC->GetSafeHdc());

	// Ошибка, есть контекс создать не удалось
	ASSERT(hrc != NULL);

	// Сделать контекст OpenGL активным для контекста окна Windows
	wglMakeCurrent(pDC->GetSafeHdc(), hrc);

	// Получаем размер клиентской области
	GetClientRect(&rect);
	
	// Очищаем буфер цвета и буфер глубины
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);

	glDepthFunc(GL_LESS);
	/*
		glEnable производят включение и отключение того или иного режима работы конвейера OpenGL. 
		Эти команды применяются достаточно часто, и их возможные параметры будут рассматриваться в каждом конкретном случае.
	*/
	glEnable(GL_DEPTH_TEST);
	/*
		Вершинам можно назначать различные цвета, и, если включен соответствующий режим, то будет проводиться линейная интерполяция цветов по поверхности примитива.
		Для управления режимом интерполяции используется команда
			void glShadeModel (GLenum  mode)
		вызов которой с параметром GL_SMOOTH включает интерполяцию (установка по умолчанию), а с GL_FLAT – отключает.
	*/
	glShadeModel(GL_SMOOTH);
	/*
		Для задания различных преобразований объектов сцены в OpenGL используются операции над матрицами,
		при этом различают три типа матриц: модельно-видовая, матрица проекций и матрица текстуры (GL_MODELVIEW, GL_PROJECTION, или GL_TEXTURE). 
		Все они имеют размер 4x4. Видовая матрица определяет преобразования объекта в мировых координатах, 
		такие как параллельный перенос, изменение масштаба и поворот. Матрица проекций определяет, как будут проецироваться трехмерные объекты на плоскость экрана
		(в оконные координаты), а матрица текстуры определяет наложение текстуры на объект. 
	*/
	glMatrixMode(GL_PROJECTION);

	// Заменяет текущую матрицу на единичную
	glLoadIdentity();
	/*
		Перспективная проекция
		которая задает усеченный конус видимости в левосторонней системе координат. 
		Параметр angley определяет угол видимости в градусах по оси у и должен находиться в диапазоне от 0 до 180. 
		Угол видимости вдоль оси x задается параметром aspect, который обычно задается как отношение сторон области вывода 
		(как правило, размеров окна) Параметры zfar и znear задают расстояние от наблюдателя до плоскостей отсечения по глубине и 
		должны быть положительными. Чем больше отношение zfar/znear, тем хуже в буфере глубины будут различаться расположенные
		рядом поверхности, так как по умолчанию в него будет записываться ‘сжатая’ глубина в диапазоне от 0 до 1 (см. п. 0.).
	*/
	gluPerspective(45.0f, (GLfloat)rect.right /	(GLfloat)rect.bottom, 0.1f, 100.0f);
	// Изменение типа метрицы как модельно-видовая
	glMatrixMode(GL_MODELVIEW);

}

BOOL COpenGLDialogDlg::bSetupPixelFormat()
{
	/* Pixel format
		После получения контекста устойств нужно установить формат пикселей (pixel format) контекста.
		Это нужно для того, чтобы сообщить операционной системе, какие ресурсы необходимо выделить для данного контекста.
		Формат пикселей указывает, какие возможности OpenGL мы будем использовать: двойной буфер, буфер маски, буфер глубины, формат цвета и т.д.
		Чтобы установить формат пикселя, нужно заполнить структуру PIXELFORMATDESCRIPTOR и передать ее в текущий контекст
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
	// Передача формата пикселя в текущий контекст
	if (SetPixelFormat(pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox(L"SetPixelFormat failed");
		return FALSE;
	}
	return TRUE;
}

// Прорисовка при каждом вызове таймера
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
	// Быстро скопировать содержимое заднего буфера окна в передний буфер
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
	// Очищаем память таймера
	KillTimer(1);
	// Получаем текущей контекст OpenGL
	hrc = ::wglGetCurrentContext();
	// Очищаем контекст
	::wglMakeCurrent(NULL, NULL);
	// Освобождаем память под котекстом
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
