//////////////////////////////////////////////////////////////////////
// demodulator.h: interface for the CDemodulator class.
//
// History:
//	2010-09-15  Initial creation MSW
//	2011-03-27  Initial release
/////////////////////////////////////////////////////////////////////
#ifndef DEMODULATOR_H
#define DEMODULATOR_H

#include "dsp/downconvert.h"
#include "dsp/fastfir.h"
#include "smeter.h"
#include "dsp/agc.h"
#include "dsp/amdemod.h"
#include "dsp/samdemod.h"
#include "dsp/fmdemod.h"
#include "dsp/wfmdemod.h"
#include "dsp/ssbdemod.h"

#define DEMOD_AM 0		//defines for supported demod modes
#define DEMOD_SAM 1
#define DEMOD_FM 2
#define DEMOD_USB 3
#define DEMOD_LSB 4
#define DEMOD_CWU 5
#define DEMOD_CWL 6
#define DEMOD_WFM 7

#define NUM_DEMODS 8	//manually update if modify number of demod types

#define MAX_INBUFSIZE 250000	//maximum size of demod input buffer
								//pick so that worst case decimation leaves
								//reasonable number of samples to process
#define MAX_MAGBUFSIZE 32000

typedef struct _sdmd
{
	int HiCut;
	int HiCutmin;	//not saved in settings
	int HiCutmax;	//not saved in settings
	int LowCut;
	int LowCutmin;	//not saved in settings
	int LowCutmax;	//not saved in settings
	int FilterClickResolution;
	int Offset;
	int SquelchValue;
	int AgcSlope;
	int AgcThresh;
	int AgcManualGain;
	int AgcDecay;
	bool AgcOn;
	bool AgcHangOn;
	bool Symetric;	//not saved in settings
	QString txt;	//not saved in settings
}tDemodInfo;

class CDemodulator
{
public:
	CDemodulator();
	virtual ~CDemodulator();

	void SetInputSampleRate(TYPEREAL InputRate);
	double GetOutputRate(){return m_DemodOutputRate;}
	double GetSMeterPeak(){return m_SMeter.GetPeak();}
	double GetSMeterAve(){return m_SMeter.GetAve();}

	void SetDemod(int Mode, tDemodInfo CurrentDemodInfo);
	void SetDemodFreq(TYPEREAL Freq){m_DownConvert.SetCwOffset(m_CW_Offset);
										m_DownConvert.SetFrequency(Freq);}

	//overloaded functions to perform demod mono or stereo
	int ProcessData(int InLength, TYPECPX* pInData, TYPEREAL* pOutData);
	int ProcessData(int InLength, TYPECPX* pInData, TYPECPX* pOutData);

	void SetUSFmVersion(bool USFm){m_USFm = USFm;}
	bool GetUSFmVersion(){return m_USFm;}

	//access to WFM mode status
	int GetStereoLock(int* pPilotLock){ if(m_pWFmDemod) return m_pWFmDemod->GetStereoLock(pPilotLock); else return false;}
	int GetNextRdsGroupData(tRDS_GROUPS* pGroupData)
	{
		if(m_pWFmDemod) return m_pWFmDemod->GetNextRdsGroupData(pGroupData); else return false;
	}

private:
	void DeleteAllDemods();
	CDownConvert m_DownConvert;
	CFastFIR m_FastFIR;
	CAgc m_Agc;
	CSMeter m_SMeter;
	QMutex m_Mutex;		//for keeping threads from stomping on each other
	tDemodInfo m_DemodInfo;
	TYPEREAL m_InputRate;
	TYPEREAL m_DownConverterOutputRate;
	TYPEREAL m_DemodOutputRate;
	TYPEREAL m_DesiredMaxOutputBandwidth;
	TYPECPX* m_pDemodInBuf;
	TYPECPX* m_pDemodTmpBuf;
	TYPEREAL m_CW_Offset;
	bool m_USFm;
	int m_DemodMode;
	int m_InBufPos;
	int m_InBufLimit;
	int m_AGClength;
	int m_AGChang;
	int m_AGChangTimer;
	//pointers to all the various implemented demodulator classes
	CAmDemod* m_pAmDemod;
	CSamDemod* m_pSamDemod;
	CFmDemod* m_pFmDemod;
	CWFmDemod* m_pWFmDemod;
	CSsbDemod* m_pSsbDemod;	//includes CW modes
};

#endif // DEMODULATOR_H
