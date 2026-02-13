// FienupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Fienup.h"
#include "FienupDlg.h"
#include <math.h>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int timer = 0;


// CFienupDlg dialog
CFienupDlg::CFienupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFienupDlg::IDD, pParent),
	/*
	KolToch(256),
	nevyaz(0),
	Ampl1(2),
	Ampl2(1),
	Ampl3(3),
	Ampl4(1),
	Ampl5(1),
	x1(50),
	x2(110),
	x3(180),
	x4(220),
	x5(20),
	Sigma1(3),
	Sigma2(3),
	Sigma3(3),
	Sigma4(3),
	Sigma5(3),
	fd(1)
	*/

	KolToch(512),
	nevyaz(0),
	Ampl1(1),
	Ampl2(3),
	Ampl3(2),
	Ampl4(4),
	Ampl5(0),
	x1(100),
	x2(300),
	x3(450),
	x4(200),
	x5(40),
	Sigma1(2),
	Sigma2(3),
	Sigma3(4),
	Sigma4(3),
	Sigma5(4),
	fd(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFienupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_KolToch, KolToch);
	DDX_Text(pDX, IDC_nevyaz, nevyaz);

	DDX_Text(pDX, IDC_Ampl1, Ampl1);
	DDX_Text(pDX, IDC_Ampl2, Ampl2);
	DDX_Text(pDX, IDC_Ampl3, Ampl3);
	DDX_Text(pDX, IDC_Ampl4, Ampl4);
	DDX_Text(pDX, IDC_Ampl5, Ampl5);

	DDX_Text(pDX, IDC_x1, x1);
	DDX_Text(pDX, IDC_x2, x2);
	DDX_Text(pDX, IDC_x3, x3);
	DDX_Text(pDX, IDC_x4, x4);
	DDX_Text(pDX, IDC_x5, x5);

	DDX_Text(pDX, IDC_Sigma1, Sigma1);
	DDX_Text(pDX, IDC_Sigma2, Sigma2);
	DDX_Text(pDX, IDC_Sigma3, Sigma3);
	DDX_Text(pDX, IDC_Sigma4, Sigma4);
	DDX_Text(pDX, IDC_Sigma5, Sigma5);

	DDX_Text(pDX, IDC_KolToch2, fd);
}

BEGIN_MESSAGE_MAP(CFienupDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DrawSignalButton, &CFienupDlg::OnBnClickedDrawsignalbutton)
	ON_BN_CLICKED(IDC_DrawVosstSignal, &CFienupDlg::OnBnClickedDrawvosstsignal)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFienupDlg message handlers

BOOL CFienupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SignalWnd = GetDlgItem(IDC_Signal);
	SignalDc = SignalWnd->GetDC();
	SignalWnd->GetClientRect(&SignalPic);

	SpectrWnd = GetDlgItem(IDC_Spectr);
	SpectrDc = SpectrWnd->GetDC();
	SpectrWnd->GetClientRect(&SpectrPic);

	SignalColorWnd = GetDlgItem(IDC_SignalColor);
	SignalColorDc = SignalColorWnd->GetDC();
	SignalColorWnd->GetClientRect(&SignalColorPic);

	SpectrColorWnd = GetDlgItem(IDC_SpectrColor);
	SpectrColorDc = SpectrColorWnd->GetDC();
	SpectrColorWnd->GetClientRect(&SpectrColorPic);

	SignalAndVSignalWnd = GetDlgItem(IDC_SignalAndVSignal);
	SignalAndVSignalDc = SignalAndVSignalWnd->GetDC();
	SignalAndVSignalWnd->GetClientRect(&SignalAndVSignalPic);

	NSignalColorWnd = GetDlgItem(IDC_NSignalColor);
	NSignalColorDc = NSignalColorWnd->GetDC();
	NSignalColorWnd->GetClientRect(&NSignalColorPic);

	NVosstColorWnd = GetDlgItem(IDC_NVosstColor);
	NVosstColorDc = NVosstColorWnd->GetDC();
	NVosstColorWnd->GetClientRect(&NVosstColorPic);

	DrawSignalKoordFlag = true;
	DrawSignalFlag = false;
	DrawSpectrKoordFlag = true;
	DrawSpectrFlag = false;
	DrawSAndVSKoord = true;
	DrawSAndVSFlag = false;

	// создание кисти для осей координат
	const COLORREF koordcolor = RGB(75, 75, 75);
	koordpen.CreatePen(PS_SOLID, 2, koordcolor);
	// создание кисти для сетки координат
	const COLORREF netkoordcolor = RGB(225, 225, 225);
	netkoordpen.CreatePen(PS_DOT, 1, netkoordcolor);
	// создание кисти для сигнала
	const COLORREF signalcolor = RGB(105, 89, 205);
	signalpen.CreatePen(PS_SOLID, 2, signalcolor);
	// создание кисти для спектра
	const COLORREF spectrcolor = RGB(255, 69, 0);
	spectrpen.CreatePen(PS_SOLID, 2, spectrcolor);
	// создание кисти для восстановленного сигнала
	const COLORREF vosstsignalcolor = RGB(0, 205, 102);
	vsignalpen.CreatePen(PS_SOLID, 2, vosstsignalcolor);

	// создание шрифта для графиков
	LOGFONT logfont = {
		14,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_NORMAL,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
		_T("Arial") };				// lpszFacename
	fontgraph.CreateFontIndirect(&logfont);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFienupDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFienupDlg::OnPaint()
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
		CDialog::OnPaint();
		if (DrawSignalKoordFlag == true)
			DrawKoord(SignalDc, SignalPic, CString("x"), CString("A"));
		if (DrawSpectrKoordFlag == true)
			DrawKoord(SpectrDc, SpectrPic, CString("f"), CString("U"));
		if (DrawSAndVSKoord == true)
			DrawKoord(SignalAndVSignalDc, SignalAndVSignalPic, CString("x"), CString("A"));
		if (DrawSignalFlag == true)
		{
			Draw1Graph(SignalMass, SignalDc, SignalPic, &signalpen, KolToch, KolToch, CString("x"), CString("A"));
		}
		if (DrawSpectrFlag == true)
		{
			Draw1Graph(SpectrMass, SpectrDc, SpectrPic, &spectrpen, KolToch, KolToch, CString("f"), CString("U"));
		}
		if (DrawVosstSignalFlag == true)
		{
			Draw1Graph(VosstSignalMass, SignalAndVSignalDc, SignalAndVSignalPic, &vsignalpen, KolToch, KolToch, CString("x"), CString("A"));
		}
		if (DrawSAndVSFlag == true)
		{
			Draw2Graph(SignalMass, &signalpen, VosstSignalMass, &vsignalpen, SignalAndVSignalDc, SignalAndVSignalPic, KolToch, CString("x"), CString("A"));
		}
		SignalColorDc->FillSolidRect(&SignalColorPic, RGB(105, 89, 205));
		SpectrColorDc->FillSolidRect(&SpectrColorPic, RGB(255, 69, 0));
		NSignalColorDc->FillSolidRect(&NSignalColorPic, RGB(105, 89, 205));
		NVosstColorDc->FillSolidRect(&NVosstColorPic, RGB(0, 205, 102));
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFienupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//-----------------------------------Отрисовка_Графиков----------------------------------------------



//отрисовка система координат
void CFienupDlg::DrawKoord(CDC* WinDc, CRect WinPic, CString Abs, CString Ord)
{
	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);
	bmp.CreateCompatibleBitmap(
		WinDc,
		WinPic.Width(),
		WinPic.Height());
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
	// заливка фона графика белым цветом 
	MemDc->FillSolidRect(
		WinPic,
		RGB(255, 255, 255));
	// отрисовка сетки координат 
	pen = MemDc->SelectObject(&netkoordpen);
	// вертикальные линии сетки координат
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width(); i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, 0);
		MemDc->LineTo(i, WinPic.Height());
	}
	// горизонтальные линии сетки координат
	for (float i = (float)WinPic.Height() / 10; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(0, i);
		MemDc->LineTo(WinPic.Width(), i);
	}
	// отрисовка осей 
	pen = MemDc->SelectObject(&koordpen);
	// отрисовка оси X
	MemDc->MoveTo(2, (float)WinPic.Height() * 9 / 10);	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	/*
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 + 2);	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 - 2);	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	*/
	// деления на оси X
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width() * 24 / 25; i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, WinPic.Height() * 9 / 10 + 2);
		MemDc->LineTo(i, WinPic.Height() * 9 / 10 - 3);
	}
	// отрисовка оси Y
	MemDc->MoveTo(WinPic.Width() * 2 / 25, WinPic.Height() - 2);	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	/*
	MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, 15);	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 + 2, 15);	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	*/
	// деления на оси Y
	for (float i = (float)WinPic.Height() / 5; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, i);
		MemDc->LineTo(WinPic.Width() * 2 / 25 + 3, i);
	}

	// подписи осей 
	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	// установка шрифта
	MemDc->SelectObject(&fontgraph);
	// подпись оси X
	MemDc->TextOut((float)WinPic.Width() * 24 / 25 + 4, (float)WinPic.Height() * 9 / 10 + 2, Abs);
	// подпись оси Y
	MemDc->TextOut((float)WinPic.Width() * 2 / 25 + 5, 0, Ord);
	// вывод на экран
	WinDc->BitBlt(0, 0, WinPic.Width(), WinPic.Height(), MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}



// 1-ый и 2-ой графики
void CFienupDlg::Draw1Graph(double* Mass, CDC* WinDc, CRect WinPic, CPen* graphpen, int KolToch, int AbsMax, CString Abs, CString Ord)
{
	// поиск максимального и минимального значения
	Min = Mass[0];
	Max = Mass[0];
	for (int i = 1; i < KolToch; i++)
	{
		if (Mass[i] < Min)
		{
			Min = Mass[i];
		}
		if (Mass[i] > Max)
		{
			Max = Mass[i];
		}
	}
	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);
	bmp.CreateCompatibleBitmap(
		WinDc,
		WinPic.Width(),
		WinPic.Height());
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
	// заливка фона графика белым цветом 
	MemDc->FillSolidRect(
		WinPic,
		RGB(255, 255, 255));
	// отрисовка сетки координат 
	pen = MemDc->SelectObject(&netkoordpen);
	// вертикальные линии сетки координат
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width(); i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, 0);
		MemDc->LineTo(i, WinPic.Height());
	}
	// горизонтальные линии сетки координат
	for (float i = (float)WinPic.Height() / 10; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(0, i);
		MemDc->LineTo(WinPic.Width(), i);
	}
	// отрисовка осей 
	pen = MemDc->SelectObject(&koordpen);
	// отрисовка оси X
	MemDc->MoveTo(2, (float)WinPic.Height() * 9 / 10);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 + 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 - 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	// деления на оси X
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width() * 24 / 25; i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, WinPic.Height() * 9 / 10 + 2);
		MemDc->LineTo(i, WinPic.Height() * 9 / 10 - 3);
	}
	// отрисовка оси Y
	MemDc->MoveTo(WinPic.Width() * 2 / 25, WinPic.Height() - 2);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 + 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	// деления на оси Y
	for (float i = (float)WinPic.Height() / 5; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, i);
		MemDc->LineTo(WinPic.Width() * 2 / 25 + 3, i);
	}
	// подписи осей 
	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	// установка шрифта
	MemDc->SelectObject(&fontgraph);
	// подпись оси X
	MemDc->TextOut((float)WinPic.Width() * 24 / 25 + 4, (float)WinPic.Height() * 9 / 10 + 2, Abs);
	// подпись оси Y
	MemDc->TextOut((float)WinPic.Width() * 2 / 25 + 5, 0, Ord);
	// выбор области для рисования
	xx0 = WinPic.Width() * 2 / 25;
	xxmax = WinPic.Width() * 24 / 25;
	yy0 = WinPic.Height() / 10;
	yymax = WinPic.Height() * 9 / 10;
	// отрисовка
	pen = MemDc->SelectObject(graphpen);
	MemDc->MoveTo(xx0, yymax + (Mass[0] - Min) / (Max - Min) * (yy0 - yymax));
	for (int i = 0; i < KolToch; i++)
	{
		xxi = xx0 + (xxmax - xx0) * i / (KolToch - 1);
		yyi = yymax + (Mass[i] - Min) / (Max - Min) * (yy0 - yymax);
		MemDc->LineTo(xxi, yyi);
	}
	// вывод числовых значений
	// по оси абсцисс
	for (int i = 4; i < 25; i += 3)
	{
		sprintf(znach, "%5.1f", (i - 1) * (float)AbsMax / 22);
		MemDc->TextOut(i * WinPic.Width() / 25 + 2, WinPic.Height() * 9 / 10 + 2, CString(znach));
	}
	// по оси ординат
	sprintf(znach, "%5.1f", Max);	        MemDc->TextOut(32, WinPic.Height() / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.75 * Max);	MemDc->TextOut(32, WinPic.Height() * 5 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.5 * Max); 	MemDc->TextOut(32, WinPic.Height() * 9 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.25 * Max);	MemDc->TextOut(32, WinPic.Height() * 13 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.0);	        MemDc->TextOutW(32, WinPic.Height() * 9 / 10 + 2, CString(znach));
	// вывод на экран
	WinDc->BitBlt(0, 0, WinPic.Width(), WinPic.Height(), MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}

// 3-ий грфик
void CFienupDlg::Draw2Graph(double* Mass1, CPen* graph1pen, double* Mass2, CPen* graph2pen, CDC* WinDc, CRect WinPic, float AbsMax, CString Abs, CString Ord)
{
	// поиск максимального и минимального значения
	Mass1Min = Mass1[0];	Mass1Max = Mass1[0];
	Mass2Min = Mass2[0];	Mass2Max = Mass2[0];
	for (int i = 1; i < KolToch; i++)
	{
		if (Mass1[i] < Mass1Min)
		{
			Mass1Min = Mass1[i];
		}
		if (Mass1[i] > Mass1Max)
		{
			Mass1Max = Mass1[i];
		}
		if (Mass2[i] < Mass2Min)
		{
			Mass2Min = Mass2[i];
		}
		if (Mass2[i] > Mass2Max)
		{
			Mass2Max = Mass2[i];
		}
	}
	if (Mass2Max > Mass1Max)
	{
		Max = Mass2Max;
	}
	else
	{
		Max = Mass1Max;
	}
	if (Mass2Min < Mass1Min)
	{
		Min = Mass2Min;
	}
	else
	{
		Min = Mass1Min;
	}
	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);
	bmp.CreateCompatibleBitmap(
		WinDc,
		WinPic.Width(),
		WinPic.Height());
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
	// заливка фона графика белым цветом 
	MemDc->FillSolidRect(
		WinPic,
		RGB(255, 255, 255));
	// отрисовка сетки координат 
	pen = MemDc->SelectObject(&netkoordpen);
	// вертикальные линии сетки координат
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width(); i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, 0);
		MemDc->LineTo(i, WinPic.Height());
	}
	// горизонтальные линии сетки координат
	for (float i = (float)WinPic.Height() / 10; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(0, i);
		MemDc->LineTo(WinPic.Width(), i);
	}
	// отрисовка осей 
	pen = MemDc->SelectObject(&koordpen);
	// отрисовка оси X
	MemDc->MoveTo(2, (float)WinPic.Height() * 9 / 10);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 + 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	MemDc->MoveTo(WinPic.Width() - 15, (float)WinPic.Height() * 9 / 10 - 2);
	MemDc->LineTo(WinPic.Width() - 2, (float)WinPic.Height() * 9 / 10);
	// деления на оси X
	for (float i = (float)WinPic.Width() / 25; i < WinPic.Width() * 24 / 25; i += (float)WinPic.Width() / 25)
	{
		MemDc->MoveTo(i, WinPic.Height() * 9 / 10 + 2);
		MemDc->LineTo(i, WinPic.Height() * 9 / 10 - 3);
	}
	// отрисовка оси Y
	MemDc->MoveTo(WinPic.Width() * 2 / 25, WinPic.Height() - 2);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	MemDc->MoveTo(WinPic.Width() * 2 / 25 + 2, 15);
	MemDc->LineTo(WinPic.Width() * 2 / 25, 2);
	// деления на оси Y
	for (float i = (float)WinPic.Height() / 5; i < WinPic.Height(); i += (float)WinPic.Height() / 10)
	{
		MemDc->MoveTo(WinPic.Width() * 2 / 25 - 2, i);
		MemDc->LineTo(WinPic.Width() * 2 / 25 + 3, i);
	}
	// подписи осей 
	// установка прозрачного фона текста
	MemDc->SetBkMode(TRANSPARENT);
	// установка шрифта
	MemDc->SelectObject(&fontgraph);
	// подпись оси X
	MemDc->TextOut((float)WinPic.Width() * 24 / 25 + 4, (float)WinPic.Height() * 9 / 10 + 2, Abs);
	// подпись оси Y
	MemDc->TextOut((float)WinPic.Width() * 2 / 25 + 5, 0, Ord);
	// выбор области для рисования
	xx0 = WinPic.Width() * 2 / 25;
	xxmax = WinPic.Width() * 24 / 25;
	yy0 = WinPic.Height() / 10;
	yymax = WinPic.Height() * 9 / 10;
	// отрисовка первого графика
	pen = MemDc->SelectObject(graph1pen);
	MemDc->MoveTo(xx0, yymax + (Mass1[0] - Min) / (Max - Min) * (yy0 - yymax));
	for (int i = 0; i < KolToch; i++)
	{
		xxi = xx0 + (xxmax - xx0) * i / (KolToch - 1);
		yyi = yymax + (Mass1[i] - Min) / (Max - Min) * (yy0 - yymax);
		MemDc->LineTo(xxi, yyi);
	}
	// отрисовка второго графика
	pen = MemDc->SelectObject(graph2pen);
	MemDc->MoveTo(xx0, yymax + (Mass2[0] - Min) / (Max - Min) * (yy0 - yymax));
	for (int i = 0; i < KolToch; i++)
	{
		xxi = xx0 + (xxmax - xx0) * i / (KolToch - 1);
		yyi = yymax + (Mass2[i] - Min) / (Max - Min) * (yy0 - yymax);
		MemDc->LineTo(xxi, yyi);
	}
	// вывод числовых значений
	// по оси абсцисс
	for (int i = 4; i < 25; i += 3)
	{
		sprintf(znach, "%5.1f", (i - 1) * (float)AbsMax / 22);
		MemDc->TextOut(i * WinPic.Width() / 25 + 2, WinPic.Height() * 9 / 10 + 2, CString(znach));
	}
	// по оси ординат
	sprintf(znach, "%5.1f", Max);       	MemDc->TextOut(32, WinPic.Height() / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.75 * Max);	MemDc->TextOut(32, WinPic.Height() * 5 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.5 * Max);	    MemDc->TextOut(32, WinPic.Height() * 9 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0.25 * Max);	MemDc->TextOut(32, WinPic.Height() * 13 / 20 + 1, CString(znach));
	sprintf(znach, "%5.1f", 0 * Max);	    MemDc->TextOutW(32, WinPic.Height() * 9 / 10 + 2, CString(znach));
	// вывод на экран
	WinDc->BitBlt(0, 0, WinPic.Width(), WinPic.Height(), MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}



//-----------------------------------Рассчёты--------------------------------------------------------



// функция потока
DWORD dwThread;
HANDLE hThread;
DWORD WINAPI MyProc(PVOID pv)
{
	CFienupDlg* p = (CFienupDlg*)pv;
	p->CalculateVosstSignal();
	return 0;
}

// вычисление сигнала
void CFienupDlg::CalculateSignal()
{
	// создание массива исходного сигнала
	UpdateData(true);
	SignalMass = new double[KolToch];
	// заполнение массива исходного сигнала
	for (int i = 0; i < KolToch; i++)
	{
		SignalMass[i] = Ampl1 * exp(-(x1 - i) * (x1 - i) / (Sigma1 * Sigma1)) + Ampl2 * exp(-(x2 - i) * (x2 - i) / (Sigma2 * Sigma2)) + Ampl3 * exp(-(x3 - i) * (x3 - i) / (Sigma3 * Sigma3)) + Ampl4 * exp(-(x4 - i) * (x4 - i) / (Sigma4 * Sigma4)) + Ampl5 * exp(-(x5 - i) * (x5 - i) / (Sigma5 * Sigma5));
	}
}



// преобразование фурье
void CFienupDlg::fourea(struct cmplx* data, int n, int is)
{
	int i, j, istep;
	int m, mmax;
	float r, r1, theta, w_r, w_i, temp_r, temp_i;
	float pi = 3.1415926f;

	r = pi * is;
	j = 0;
	for (i = 0; i < n; i++)
	{
		if (i < j)
		{
			temp_r = data[j].real;
			temp_i = data[j].image;
			data[j].real = data[i].real;
			data[j].image = data[i].image;
			data[i].real = temp_r;
			data[i].image = temp_i;
		}
		m = n >> 1;
		while (j >= m)
		{
			j -= m;
			m = (m + 1) / 2;
		}
		j += m;
	}
	mmax = 1;
	while (mmax < n)
	{
		istep = mmax << 1;
		r1 = r / (float)mmax;
		for (m = 0; m < mmax; m++)
		{
			theta = r1 * m;
			w_r = (float)cos((double)theta);
			w_i = (float)sin((double)theta);
			for (i = m; i < n; i += istep)
			{
				j = i + mmax;
				temp_r = w_r * data[j].real - w_i * data[j].image;
				temp_i = w_r * data[j].image + w_i * data[j].real;
				data[j].real = data[i].real - temp_r;
				data[j].image = data[i].image - temp_i;
				data[i].real += temp_r;
				data[i].image += temp_i;
			}
		}
		mmax = istep;
	}
	if (is > 0)
		for (i = 0; i < n; i++)
		{
			data[i].real /= (float)n;
			data[i].image /= (float)n;
		}
}



// вычисление спектра
void CFienupDlg::CalculateSpectr()
{
	// создание массива спектра 
	UpdateData(true);
	Cmplx = new cmplx[KolToch];
	for (int i = 0; i < KolToch; i++)
	{
		Cmplx[i].real = (float)SignalMass[i];
		Cmplx[i].image = 0;
	}
	// прямое фурье-преобразование
	fourea(Cmplx, KolToch, 1);
	// заполнение массива спектра
	SpectrMass = new double[KolToch];
	for (int i = 0; i < KolToch; i++)
	{
		SpectrMass[i] = sqrt(Cmplx[i].real * Cmplx[i].real + Cmplx[i].image * Cmplx[i].image);
	}
}



void CFienupDlg::inverse()
{
	double tempmas1[512];
	for (int i = 0; i < KolToch; i++)
		tempmas1[i] = VosstSignalMass[i];
	for (int i = 0; i < KolToch / 2; i++)
		VosstSignalMass[i] = tempmas1[KolToch / 2 - i - 1];
	for (int i = 0; i < KolToch / 2; i++)
		VosstSignalMass[KolToch / 2 + i] = tempmas1[KolToch - i - 1];
}



// вычисление восстановленного сигнала
void CFienupDlg::CalculateVosstSignal()
{
	killtimer = false;
	//srand (time(NULL)); // изменение затравки
	j = 0;
	cmplx* HelpMass = new cmplx[KolToch];
	VosstSignalMass = new double[KolToch];
	double* PhaseMass = new double[KolToch];
	IterSpMass = new double[KolToch];
	double Eps = 1; // точность

	while (/*Eps>1.e-5*//*Eps>1.9093652847907559e-011*/j < 1000)
	{
		j++;
		if (j == 1) // если первая итерация
		{
			/*float *f0 = new float[KolToch];
			f0[0] = 0;
			f0[KolToch - 1] = 0;
			for(int i = 1; i < KolToch / 2; i++)
			{
			f0[i] = 2 * 3.14159265 * (double)rand()/(double)RAND_MAX;
			}
			for(int i = 1; i < KolToch / 2; i++)
			{
			f0[KolToch - 1 - i]= - f0[i];
			}
			//3.141592/2;
			//2*3.14159265*((double) rand())/((double) RAND_MAX);
			for(int i = 1; i < KolToch; i++)
			{
			HelpMass[i].real = SpectrMass[i] * cos(f0[i]);
			HelpMass[i].image = SpectrMass[i] * sin(f0[i]);
			}
			}*/
			float f0 = 0;
			// случайное распределение фазы
			for (int i = 0; i < KolToch; i++)
			{
				f0 = 2 * 3.14159265 * (double)rand() / (double)RAND_MAX;
				HelpMass[i].real = SpectrMass[i] * cos(f0);
				HelpMass[i].image = SpectrMass[i] * sin(f0);
			}
		}
		// для остальных итераций
		fourea(HelpMass, KolToch, -1);

		for (int i = 0; i < KolToch; i++)
		{
			if (HelpMass[i].real < 0)
				HelpMass[i].real = 0;
			VosstSignalMass[i] = HelpMass[i].real;
			HelpMass[i].image = 0;
		}
		Sleep(10);

		fourea(HelpMass, KolToch, 1);
		/*double
		sum_nev1=0,
		sum_nev2=0;
		for(int i = 0; i < KolToch; i++)
		{
		IterSpMass[i] = sqrt(HelpMass[i].real * HelpMass[i].real + HelpMass[i].image * HelpMass[i].image);
		sum_nev1 += (sqrt(HelpMass[i].real * HelpMass[i].real + HelpMass[i].image * HelpMass[i].image) - SpectrMass[i]) * (sqrt(HelpMass[i].real * HelpMass[i].real + HelpMass[i].image * HelpMass[i].image) - SpectrMass[i]);
		sum_nev2 += SpectrMass[i] * SpectrMass[i];
		}*/
		/*
		Eps = sum_nev1 / sum_nev2;
		epsilon = Eps;
		*/
		for (int i = 0; i < KolToch; i++)
		{
			/*
			if(sqrt(HeplMass[i].real * HeplMass[i].real + HeplMass[i].image * HeplMass[i].image) < 1e-6)
			{
			HeplMass[i].real = 0;
			HeplMass[i].image = 0;
			}
			else
			{
			cos = HeplMass[i].real / sqrt(HeplMass[i].real * HeplMass[i].real + HeplMass[i].image * HeplMass[i].image);
			sin = HeplMass[i].image / sqrt(HeplMass[i].real * HeplMass[i].real + HeplMass[i].image * HeplMass[i].image);

			HeplMass[i].real = SpectrMass[i] * cos;//cos(PhaseMass[i]);
			HeplMass[i].image = SpectrMass[i] * sin;//sin(PhaseMass[i]);
			}
			*/

			// через атан
			PhaseMass[i] = atan2(HelpMass[i].image, HelpMass[i].real);
			HelpMass[i].real = SpectrMass[i] * cos(PhaseMass[i]);
			HelpMass[i].image = SpectrMass[i] * sin(PhaseMass[i]);

		}
	}
	// вост масс
	/*fourea(HelpMass, KolToch, -1);
	for(int i = 0; i < KolToch; i++)
	{
	if(HelpMass[i].real < 0)
	HelpMass[i].real = 0;
	VosstSignalMass[i] = HelpMass[i].real;
	HelpMass[i].image = 0;
	}*/





	// для сдвига  восст сигнала
	double* tempmas = new double[KolToch];
	double* xnewt = new double[KolToch];
	double* mas_nev = new double[KolToch];
	double* mas_inv_nev = new double[KolToch];

	for (int i = 0; i < KolToch; i++)
	{
		xnewt[i] = VosstSignalMass[i];
		tempmas[i] = VosstSignalMass[i];
		mas_nev[i] = 0;
		mas_inv_nev[i] = 0;
	}
	for (int j = 0; j < KolToch; j++)
	{
		for (int i = 0; i < KolToch; i++)
		{
			if ((i + j) >= KolToch)
				xnewt[i] = tempmas[i + j - KolToch];
			else
				xnewt[i] = tempmas[i + j];
		}
		for (int k = 0; k < KolToch; k++)
			mas_nev[j] += sqrt((SignalMass[k] - xnewt[k]) * (SignalMass[k] - xnewt[k]));
	}
	inverse();
	for (int i = 0; i < KolToch; i++)
	{
		xnewt[i] = VosstSignalMass[i];
		tempmas[i] = VosstSignalMass[i];
	}
	for (int j = 0; j < KolToch; j++)
	{
		for (int i = 0; i < KolToch; i++)
		{
			if ((i + j) >= KolToch)
				xnewt[i] = tempmas[i + j - KolToch];
			else
				xnewt[i] = tempmas[i + j];
		}
		for (int k = 0; k < KolToch; k++)
			mas_inv_nev[j] += sqrt((SignalMass[k] - xnewt[k]) * (SignalMass[k] - xnewt[k]));
	}
	inverse();

	double min = mas_nev[0];
	int min1 = 0;
	double mininv = mas_inv_nev[0];
	int min2 = 0;
	for (int i = 1; i < KolToch; i++)
	{
		if (mas_nev[i] < min)
		{
			min = mas_nev[i];
			min1 = i;
		}
		if (mas_inv_nev[i] < mininv)
		{
			mininv = mas_inv_nev[i];
			min2 = i;
		}
	}

	if (mininv < min)
	{
		inverse();
		for (int i = 0; i < KolToch; i++)
			tempmas[i] = VosstSignalMass[i];
		for (int i = 0; i < KolToch; i++)
		{
			if ((i + min2) >= KolToch)
				VosstSignalMass[i] = tempmas[i + min2 - KolToch];
			else
				VosstSignalMass[i] = tempmas[i + min2];
		}
	}
	else
	{
		for (int i = 0; i < KolToch; i++)
			tempmas[i] = VosstSignalMass[i];
		for (int i = 0; i < KolToch; i++)
		{
			if ((i + min1) >= KolToch)
				VosstSignalMass[i] = tempmas[i + min1 - KolToch];
			else
				VosstSignalMass[i] = tempmas[i + min1];
		}
	}
	nevyaz = 0;
	for (int i = 0; i < KolToch; i++)
		nevyaz += sqrt((SignalMass[i] - VosstSignalMass[i]) * (SignalMass[i] - VosstSignalMass[i]));
	killtimer = true;
}



//-----------------------------------Кнопки----------------------------------------------------------



// кнопка сигнал + спектр
void CFienupDlg::OnBnClickedDrawsignalbutton()
{
	DrawSignalKoordFlag = false;
	DrawSignalFlag = true;
	CalculateSignal();
	Draw1Graph(SignalMass, SignalDc, SignalPic, &signalpen, KolToch, KolToch, CString("x"), CString("A"));
	DrawSpectrKoordFlag = false;
	DrawSpectrFlag = true;
	CalculateSpectr();
	Draw1Graph(SpectrMass, SpectrDc, SpectrPic, &spectrpen, KolToch, KolToch, CString("f"), CString("U"));

}



// кнопка для восстановления
void CFienupDlg::OnBnClickedDrawvosstsignal()
{
	DrawVosstSignalKoordFlag = false;
	DrawVosstSignalFlag = true;
	hThread = CreateThread(NULL, 0, MyProc, this, 0, &dwThread);
	timer = SetTimer(1, 1, NULL);
}



void CFienupDlg::OnTimer(UINT_PTR nIDEvent)
{
	DrawSAndVSFlag = false;
	Invalidate(0);
	if (killtimer)
	{
		KillTimer(timer);
		DrawSAndVSFlag = true;
		UpdateData(false);
	}
	CDialog::OnTimer(nIDEvent);
}



