//////////////////////////////////////////////////////////////////////
// wfmmod.h: interface for the CWFmMod class.
//
// History:
//	2011-08-18  Initial creation MSW
//	2011-08-18  Initial release
/////////////////////////////////////////////////////////////////////
#ifndef WFMMOD_H
#define WFMMOD_H
#include "dsp/datatypes.h"
#include "dsp/fir.h"
#include "dsp/iir.h"

#define RDSBUF_SIZE 16384
#define MAX_RDS_DATA 2000	//max number of blocks that can be stored


class CWFmMod
{
public:
	CWFmMod();
	void GenerateData(int InLength,TYPEREAL Amplitude, TYPECPX* pOutData);
	void SetSampleRate(TYPEREAL SampleRate);
	void SetSweep(TYPEREAL SweepFreqNorm, TYPEREAL SweepFrequency, TYPEREAL SweepStopFrequency, TYPEREAL SweepRateInc);

private:
	void InitRDS();
	void CreateRdsGroup(quint16 Blk1, quint16 Blk2, quint16 Blk3, quint16 Blk4);
	quint32 CreateBlockWithCheckword(quint16 Data, quint32 BlockOffset);
	void CreateRdsSamples(int InLength , TYPEREAL* pBuf);
	double CreateNextRdsBit();

	double m_DeviationRate;
	double m_ModAcc;
	double m_PilotAcc;
	double m_PilotInc;
	double m_LeftAcc;
	double m_LeftInc;
	double m_RightAcc;
	double m_RightInc;
	double m_LeftAmp;
	double m_RightAmp;

	double m_SweepFrequency;
	double m_SweepFreqNorm;
	double m_SweepStopFrequency;
	double m_SweepRateInc;



	double m_SampleRate;
	int StateTimer;
	int TimerPeriod;
	int ModState;

	TYPEREAL m_RdsPulseCoef[RDSBUF_SIZE];
	TYPEREAL m_RdsOut[RDSBUF_SIZE];
	double m_RdsPulseLength;
	double m_RdsTime;
	double m_RdsSamplePeriod;
	double m_RdsTimeToIdx;
	int m_RdsD1;
	int m_RdsD2;
	int m_RdsBufPos;
	int m_RdsBufLength;
	int m_RdsLastBit;
	quint32 m_RdsBitPtr;
	quint32 m_RdsDataBuf[MAX_RDS_DATA];

};

#endif // WFMMOD_H
