//////////////////////////////////////////////////////////////////////
// FastFIR.h: interface for the CFastFIR class.
//
//  This class implements a FIR Bandpass filter using a FFT convolution algorithm
//The filter is complex and is specified with 3 parameters:
// sample frequency, Hicut and Lowcut frequency
//
// History:
//	2010-09-15  Initial creation MSW
//	2011-03-27  Initial release
//////////////////////////////////////////////////////////////////////
#ifndef FASTFIR_H
#define FASTFIR_H

#include "dsp/datatypes.h"
#include "dsp/fft.h"
#include <QMutex>

class CFastFIR  
{
public:
	CFastFIR();
	virtual ~CFastFIR();

	void SetupParameters( TYPEREAL FLoCut,TYPEREAL FHiCut,TYPEREAL Offset, TYPEREAL SampleRate);
	int ProcessData(int InLength, TYPECPX* InBuf, TYPECPX* OutBuf);

private:
	void CpxMpy(int N, TYPECPX* m, TYPECPX* src, TYPECPX* dest);
	void FreeMemory();

	TYPEREAL m_FLoCut;
	TYPEREAL m_FHiCut;
	TYPEREAL m_Offset;
	TYPEREAL m_SampleRate;

	int m_InBufInPos;
	TYPEREAL* m_pWindowTbl;
	TYPECPX* m_pFFTOverlapBuf;
	TYPECPX* m_pFilterCoef;
	TYPECPX* m_pFFTBuf;
	QMutex m_Mutex;		//for keeping threads from stomping on each other
	CFft m_Fft;
};
#endif // FASTFIR_H
