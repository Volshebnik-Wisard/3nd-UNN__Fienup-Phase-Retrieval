// FienupDlg.h : header file
//

#pragma once


// CFienupDlg dialog
class CFienupDlg : public CDialog
{
	// Construction
public:
	CFienupDlg(CWnd* pParent = NULL);	// standard constructor

	struct cmplx
	{
		float real;
		float image;
	};
	cmplx* Cmplx;
	void fourea(struct cmplx* data, int n, int is);

	void DrawKoord(CDC*, CRect, CString, CString);
	void Draw1Graph(double*, CDC*, CRect, CPen*, int, int, CString, CString);
	void Draw2Graph(double*, CPen*, double*, CPen*, CDC*, CRect, float, CString, CString);
	void CalculateSignal();
	void CalculateSpectr();
	void inverse();
	void CalculateVosstSignal();

	CWnd* SignalWnd;
	CWnd* SpectrWnd;
	CWnd* SignalColorWnd;
	CWnd* SpectrColorWnd;
	CWnd* VosstSignalWnd;
	CWnd* VosstSignalColorWnd;
	CWnd* SignalAndVSignalWnd;
	CWnd* NSignalColorWnd;
	CWnd* NVosstColorWnd;
	CDC* SignalDc;
	CDC* SpectrDc;
	CDC* SignalColorDc;
	CDC* SpectrColorDc;
	CDC* VosstSignalDc;
	CDC* VosstSignalColorDc;
	CDC* SignalAndVSignalDc;
	CDC* NSignalColorDc;
	CDC* NVosstColorDc;
	CRect SignalPic;
	CRect SpectrPic;
	CRect SignalColorPic;
	CRect SpectrColorPic;
	CRect VosstSignalPic;
	CRect VosstSignalColorPic;
	CRect SignalAndVSignalPic;
	CRect NSignalColorPic;
	CRect NVosstColorPic;

	CPen koordpen, netkoordpen, signalpen, spectrpen, vsignalpen;
	CPen* pen;
	CFont fontgraph;
	CFont* font;


	// Dialog Data
	enum { IDD = IDD_IT_Z5_VOSSTFAZINF_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;

	int
		KolToch,
		j;
	float
		nevyaz;
	double
		Ampl1,
		Ampl2,
		Ampl3,
		Ampl4,
		Ampl5,
		x1,
		x2,
		x3,
		x4,
		x5,
		Sigma1,
		Sigma2,
		Sigma3,
		Sigma4,
		Sigma5,
		fd;
	double
		* SignalMass,
		* SpectrMass,
		* HelpMass,
		* PhaseMass,
		* VosstSignalMass,
		* IterSpMass;
	double
		Max,
		Min,
		Mass1Min,
		Mass1Max,
		Mass2Min,
		Mass2Max,
		SignalMin,
		SignalMax,
		SpectrMin,
		SpectrMax;
	double
		xx0,
		xxmax,
		yy0,
		yymax,
		xxi,
		yyi,
		iter;
	char
		znach[1000];
	bool
		DrawSignalKoordFlag,
		DrawSignalFlag,
		DrawSpectrKoordFlag,
		DrawSpectrFlag,
		DrawVosstSignalKoordFlag,
		DrawVosstSignalFlag,
		DrawSAndVSKoord,
		DrawSAndVSFlag,
		killtimer;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDrawsignalbutton();
	afx_msg void OnBnClickedDrawvosstsignal();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
