//////////////////////////////////////////////////////////////////////
// plotter.h: interface for the CPlotter class.
//
// History:
//	2010-09-15  Initial creation MSW
//	2011-03-27  Initial release
/////////////////////////////////////////////////////////////////////
#ifndef PLOTTER_H
#define PLOTTER_H

#include <QtGui>
#include <QFrame>
#include <QImage>
#include "interface/sdrinterface.h"

#define VERT_DIVS 14	//specify grid screen divisions
#define HORZ_DIVS 10

#define MAX_TXT 128

class CPlotter : public QFrame
{
    Q_OBJECT
public:
	explicit CPlotter(QWidget *parent = 0);
	~CPlotter();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void SetSdrInterface(CSdrInterface* ptr){m_pSdrInterface = ptr;}
	void draw();		//call to draw new fft data onto screen plot
	void SetRunningState(bool running){m_Running = running;}
	void SetClickResolution(int clickres){m_ClickResolution=clickres;}
	void SetFilterClickResolution(int clickres){m_FilterClickResolution=clickres;}
	void SetPercent2DScreen(int percent){m_Percent2DScreen=percent;	m_Size = QSize(0,0);resizeEvent(NULL);}

	void SetCenterFreq(quint64 f){m_CenterFreq=f;}
	void SetDemodCenterFreq(quint64 f){m_DemodCenterFreq=f;}
	void SetHiLowCutFrequencies(int LowCut, int HiCut){m_DemodLowCutFreq = LowCut; m_DemodHiCutFreq = HiCut;}
	void SetDemodRanges(int FLowCmin, int FLowCmax, int FHiCmin, int FHiCmax, bool symetric);
	void SetSpanFreq(quint32 s){m_Span=(qint32)s;}
	void SetMaxdB(int max){m_MaxdB=max;}
	void SetADOverload(bool ADOverLoad){m_ADOverLoad = ADOverLoad;m_ADOverloadOneShotCounter=0;}
	void SetdBStepSize(int stepsz){m_dBStepSize=stepsz;}
	void UpdateOverlay(){DrawOverlay();}

	char m_RdsCall[MAX_TXT];
	char m_RdsText[MAX_TXT];

signals:
	void NewCenterFreq(qint64 f);
	void NewDemodFreq(qint64 f);
	void NewLowCutFreq(int f);
	void NewHighCutFreq(int f);

public slots:

protected:
		//re-implemented widget event handlers
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent* event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void wheelEvent( QWheelEvent * event );

private:
	enum eCapturetype {
	   NONE,
	   LEFT,
	   CENTER,
	   RIGHT
	};
	void DrawOverlay();
	void MakeFrequencyStrs();
	int XfromFreq(qint64 freq);
	qint64 FreqfromX(int x);
	qint64 RoundFreq(qint64 freq, int resolution);
	bool IsPointCloseTo(int x, int xr, int delta){return ((x > (xr-delta) ) && ( x<(xr+delta)) );}
	void ClampDemodParameters();
	eCapturetype m_CursorCaptured;
	QPixmap m_2DPixmap;
	QPixmap m_OverlayPixmap;
	QPixmap m_WaterfallPixmap;
	QColor m_ColorTbl[256];
	QSize m_Size;
	QString m_Str;
	QString m_HDivText[HORZ_DIVS+1];
	bool m_Running;
	bool m_ADOverLoad;
	qint64 m_CenterFreq;
	qint64 m_DemodCenterFreq;
	int m_DemodHiCutFreq;
	int m_DemodLowCutFreq;
	int m_DemodFreqX;		//screen coordinate x position
	int m_DemodHiCutFreqX;	//screen coordinate x position
	int m_DemodLowCutFreqX;	//screen coordinate x position
	int m_CursorCaptureDelta;
	int m_GrabPosition;
	int m_Percent2DScreen;
	int m_ADOverloadOneShotCounter;

	int m_FLowCmin;
	int m_FLowCmax;
	int m_FHiCmin;
	int m_FHiCmax;
	bool m_symetric;

	qint32 m_Span;
	qint32 m_MaxdB;
	qint32 m_MindB;
	qint32 m_dBStepSize;
	qint32 m_FreqUnits;
	int m_ClickResolution;
	int m_FilterClickResolution;


	quint32 m_LastSampleRate;
	CSdrInterface* m_pSdrInterface;

};

#endif // PLOTTER_H
