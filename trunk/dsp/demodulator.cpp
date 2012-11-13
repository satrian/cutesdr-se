/////////////////////////////////////////////////////////////////////
// demodulator.cpp: implementation of the Cdemodulator class.
//
//	This class implements the demodulation DSP functionality to take
//raw I/Q data from the radio, shift to baseband, decimate, demodulate,
//perform AGC, and send the audio to the sound card.
//
// History:
//	2010-09-15  Initial creation MSW
//	2011-03-27  Initial release
/////////////////////////////////////////////////////////////////////
//==========================================================================================
// + + +   This Software is released under the "Simplified BSD License"  + + +
//Copyright 2010 Moe Wheatley. All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, are
//permitted provided that the following conditions are met:
//
//   1. Redistributions of source code must retain the above copyright notice, this list of
//	  conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright notice, this list
//	  of conditions and the following disclaimer in the documentation and/or other materials
//	  provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY Moe Wheatley ``AS IS'' AND ANY EXPRESS OR IMPLIED
//WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Moe Wheatley OR
//CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
//ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//The views and conclusions contained in the software and documentation are those of the
//authors and should not be interpreted as representing official policies, either expressed
//or implied, of Moe Wheatley.
//==========================================================================================
#include "dsp/demodulator.h"
#include "gui/testbench.h"
#include <QDebug>

//////////////////////////////////////////////////////////////////
//	Constructor/Destructor
//////////////////////////////////////////////////////////////////
CDemodulator::CDemodulator()
{
	m_DesiredMaxOutputBandwidth = 48000.0;
	m_DownConverterOutputRate = 48000.0;
	m_DemodOutputRate = 48000.0;
	m_pDemodInBuf = new TYPECPX[MAX_INBUFSIZE];
	m_pDemodTmpBuf = new TYPECPX[MAX_INBUFSIZE];
	m_InBufPos = 0;
	m_InBufLimit = 1000;
	m_DemodMode = -1;
	m_pAmDemod = NULL;
	m_pSamDemod = NULL;
	m_pSsbDemod = NULL;
	m_pFmDemod = NULL;
	m_pWFmDemod = NULL;
	m_USFm = true;
	SetDemodFreq(0.0);
}

CDemodulator::~CDemodulator()
{
	DeleteAllDemods();
	if(m_pDemodInBuf)
		delete m_pDemodInBuf;
	if(m_pDemodTmpBuf)
		delete m_pDemodTmpBuf;
}

//////////////////////////////////////////////////////////////////
//	Deletes all demod objects
//////////////////////////////////////////////////////////////////
void CDemodulator::DeleteAllDemods()
{
	if(m_pAmDemod)
		delete m_pAmDemod;
	if(m_pSamDemod)
		delete m_pSamDemod;
	if(m_pFmDemod)
		delete m_pFmDemod;
	if(m_pWFmDemod)
		delete m_pWFmDemod;
	if(m_pSsbDemod)
		delete m_pSsbDemod;
	m_pAmDemod = NULL;
	m_pSamDemod = NULL;
	m_pFmDemod = NULL;
	m_pWFmDemod = NULL;
	m_pSsbDemod = NULL;
}


//////////////////////////////////////////////////////////////////
//	Called to set/change the demodulator input sample rate
//////////////////////////////////////////////////////////////////
void CDemodulator::SetInputSampleRate(TYPEREAL InputRate)
{
	if(m_InputRate != InputRate)
	{
		m_InputRate = InputRate;
		//change any demod parameters that may occur with sample rate change
		switch(m_DemodMode)
		{
			case DEMOD_FM:
				m_DownConverterOutputRate = m_DownConvert.SetDataRate(m_InputRate, m_DesiredMaxOutputBandwidth);
				m_DemodOutputRate = m_DownConverterOutputRate;
				if(m_pFmDemod)
					m_pFmDemod->SetSampleRate(m_DownConverterOutputRate);
				break;
			case DEMOD_WFM:
				m_DownConverterOutputRate = m_DownConvert.SetWfmDataRate(m_InputRate, 100000);
				if(m_pWFmDemod)
					m_DemodOutputRate = m_pWFmDemod->SetSampleRate(m_DownConverterOutputRate, m_USFm);
				break;
			case DEMOD_AM:
			case DEMOD_SAM:
			case DEMOD_USB:
			case DEMOD_LSB:
			case DEMOD_CWU:
			case DEMOD_CWL:
				m_DownConverterOutputRate = m_DownConvert.SetDataRate(m_InputRate, m_DesiredMaxOutputBandwidth);
				m_DemodOutputRate = m_DownConverterOutputRate;
				break;
		}
	}
}

//////////////////////////////////////////////////////////////////
//	Called to set/change the active Demod object
//or if a demod parameter or filter parameter changes
//////////////////////////////////////////////////////////////////
void CDemodulator::SetDemod(int Mode, tDemodInfo CurrentDemodInfo)
{
	m_Mutex.lock();
	m_DemodInfo = CurrentDemodInfo;
	if(m_DemodMode != Mode)	//do only if changes
	{
		DeleteAllDemods();		//remove current demod object
		m_DemodMode = Mode;
		//create decimation chain and get output sample rate
		if((DEMOD_LSB == m_DemodMode) || (DEMOD_CWL == m_DemodMode) )
			m_DesiredMaxOutputBandwidth = -m_DemodInfo.LowCutmin;
		else
			m_DesiredMaxOutputBandwidth = m_DemodInfo.HiCutmax;

		//now create correct demodulator
		switch(m_DemodMode)
		{
			case DEMOD_AM:
				m_DownConverterOutputRate = m_DownConvert.SetDataRate(m_InputRate, m_DesiredMaxOutputBandwidth);
				m_pAmDemod = new CAmDemod(m_DownConverterOutputRate);
				m_DemodOutputRate = m_DownConverterOutputRate;
				break;
			case DEMOD_SAM:
				m_DownConverterOutputRate = m_DownConvert.SetDataRate(m_InputRate, m_DesiredMaxOutputBandwidth);
				m_pSamDemod = new CSamDemod(m_DownConverterOutputRate);
				m_DemodOutputRate = m_DownConverterOutputRate;
				break;
			case DEMOD_FM:
				m_DownConverterOutputRate = m_DownConvert.SetDataRate(m_InputRate, m_DesiredMaxOutputBandwidth);
				m_pFmDemod = new CFmDemod(m_DownConverterOutputRate);
				m_DemodOutputRate = m_DownConverterOutputRate;
				break;
			case DEMOD_WFM:
				m_DownConverterOutputRate = m_DownConvert.SetWfmDataRate(m_InputRate, 100000);
				m_pWFmDemod = new CWFmDemod(m_DownConverterOutputRate);
				m_DemodOutputRate = m_pWFmDemod->GetDemodRate();
				break;
			case DEMOD_USB:
			case DEMOD_LSB:
			case DEMOD_CWU:
			case DEMOD_CWL:
				m_DownConverterOutputRate = m_DownConvert.SetDataRate(m_InputRate, m_DesiredMaxOutputBandwidth);
				m_pSsbDemod = new CSsbDemod();
				m_DemodOutputRate = m_DownConverterOutputRate;
				break;
		}
	}
	m_CW_Offset = m_DemodInfo.Offset;
	m_DownConvert.SetCwOffset(m_CW_Offset);
	if(m_DemodMode != DEMOD_WFM)
		m_FastFIR.SetupParameters(m_DemodInfo.LowCut, m_DemodInfo.HiCut,m_CW_Offset,m_DownConverterOutputRate);
	m_Agc.SetParameters(m_DemodInfo.AgcOn, m_DemodInfo.AgcHangOn, m_DemodInfo.AgcThresh,
						m_DemodInfo.AgcManualGain, m_DemodInfo.AgcSlope, m_DemodInfo.AgcDecay, m_DownConverterOutputRate);
	if(	m_pFmDemod != NULL)
		m_pFmDemod->SetSquelch(m_DemodInfo.SquelchValue);
	if(m_pAmDemod != NULL)
		m_pAmDemod->SetBandwidth( (m_DemodInfo.HiCut-m_DemodInfo.LowCut)/2.0);
	//set input buffer limit so that decimated output is abt 10mSec or more of data
	m_InBufLimit = (m_DemodOutputRate/100.0) * m_InputRate/m_DemodOutputRate;	//process abt .01sec of output samples at a time
	m_InBufLimit &= 0xFFFFFF00;	//keep modulo 256 since decimation is only in power of 2
	m_Mutex.unlock();
qDebug()<<"m_InputRate="<<m_InputRate<<" DesiredMaxOutputBandwidth=="<<m_DesiredMaxOutputBandwidth <<"DemodOutputRate="<<m_DemodOutputRate;
//qDebug()<<"m_InBufLimit="<<m_InBufLimit;
}

//////////////////////////////////////////////////////////////////
//	Called with complex data from radio and performs the demodulation
// with MONO audio output
//////////////////////////////////////////////////////////////////
int CDemodulator::ProcessData(int InLength, TYPECPX* pInData, TYPEREAL* pOutData)
{
int ret = 0;
	m_Mutex.lock();
	for(int i=0; i<InLength; i++)
	{	//place in demod buffer
		m_pDemodInBuf[m_InBufPos++] = pInData[i];
		if(m_InBufPos >= m_InBufLimit)
		{	//when have enough samples, call demod routine sequence

			//perform baseband tuning and decimation
			int n = m_DownConvert.ProcessData(m_InBufPos, m_pDemodInBuf, m_pDemodInBuf);
			g_pTestBench->DisplayData(n, m_pDemodInBuf, m_DownConverterOutputRate,PROFILE_1);

			if(m_DemodMode != DEMOD_WFM)
			{	//if not wideband FM mode do filtering and AGC
				//perform main bandpass filtering
				n = m_FastFIR.ProcessData(n, m_pDemodInBuf, m_pDemodTmpBuf);
				g_pTestBench->DisplayData(n, m_pDemodTmpBuf, m_DemodOutputRate,PROFILE_2);

				//perform S-Meter processing
				m_SMeter.ProcessData(n, m_pDemodTmpBuf, m_DemodOutputRate);

				//perform AGC
				m_Agc.ProcessData(n, m_pDemodTmpBuf, m_pDemodTmpBuf );
//				g_pTestBench->DisplayData(n, m_pDemodTmpBuf, m_DemodOutputRate, PROFILE_3);
			}
			else
			{
				//perform S-Meter processing for wideband FM
				m_SMeter.ProcessData(n, m_pDemodInBuf, m_DownConverterOutputRate);
			}

			//perform the desired demod action
			switch(m_DemodMode)
			{
				case DEMOD_AM:
					n = m_pAmDemod->ProcessData(n, m_pDemodTmpBuf, pOutData );
					break;
				case DEMOD_SAM:
					n = m_pSamDemod->ProcessData(n, m_pDemodTmpBuf, pOutData );
					break;
				case DEMOD_FM:
					n = m_pFmDemod->ProcessData(n, m_DemodInfo.HiCut, m_pDemodTmpBuf, pOutData );
					break;
				case DEMOD_WFM:
					n = m_pWFmDemod->ProcessData(n, m_pDemodInBuf, pOutData );
					break;
				case DEMOD_USB:
				case DEMOD_LSB:
				case DEMOD_CWU:
				case DEMOD_CWL:
					n = m_pSsbDemod->ProcessData(n, m_pDemodTmpBuf, pOutData);
					break;
			}
			g_pTestBench->DisplayData(n, pOutData, m_DemodOutputRate,PROFILE_4);
			m_InBufPos = 0;
			ret += n;
		}
	}
	m_Mutex.unlock();
	return ret;
}

//////////////////////////////////////////////////////////////////
//	Called with complex data from radio and performs the demodulation
// with STEREO audio output
//////////////////////////////////////////////////////////////////
int CDemodulator::ProcessData(int InLength, TYPECPX* pInData, TYPECPX* pOutData)
{
int ret = 0;
	m_Mutex.lock();
	for(int i=0; i<InLength; i++)
	{	//place in demod buffer
		m_pDemodInBuf[m_InBufPos++] = pInData[i];
		if(m_InBufPos >= m_InBufLimit)
		{	//when have enough samples, call demod routine sequence

			//perform baseband tuning and decimation
			int n = m_DownConvert.ProcessData(m_InBufPos, m_pDemodInBuf, m_pDemodInBuf);
g_pTestBench->DisplayData(n, m_pDemodInBuf, m_DownConverterOutputRate,PROFILE_1);


			if(m_DemodMode != DEMOD_WFM)
			{	//if not wideband FM mode do filtering and AGC
				//perform main bandpass filtering
				n = m_FastFIR.ProcessData(n, m_pDemodInBuf, m_pDemodTmpBuf);
				g_pTestBench->DisplayData(n, m_pDemodTmpBuf, m_DemodOutputRate,PROFILE_2);

				//perform S-Meter processing
				m_SMeter.ProcessData(n, m_pDemodTmpBuf, m_DemodOutputRate);

				//perform AGC
				m_Agc.ProcessData(n, m_pDemodTmpBuf, m_pDemodTmpBuf );
//				g_pTestBench->DisplayData(n, m_pDemodTmpBuf, m_DemodOutputRate, PROFILE_3);
			}
			else
			{
				//perform S-Meter processing for wideband FM
				m_SMeter.ProcessData(n, m_pDemodInBuf, m_DownConverterOutputRate);
			}
			//perform the desired demod action
			switch(m_DemodMode)
			{
				case DEMOD_AM:
					n = m_pAmDemod->ProcessData(n, m_pDemodTmpBuf, pOutData );
					break;
				case DEMOD_SAM:
					n = m_pSamDemod->ProcessData(n, m_pDemodTmpBuf, pOutData );
					break;
				case DEMOD_FM:
					n = m_pFmDemod->ProcessData(n, m_DemodInfo.HiCut, m_pDemodTmpBuf, pOutData );
					break;
				case DEMOD_WFM:
					n = m_pWFmDemod->ProcessData(n, m_pDemodInBuf, pOutData );
					break;
				case DEMOD_USB:
				case DEMOD_LSB:
				case DEMOD_CWU:
				case DEMOD_CWL:
					n = m_pSsbDemod->ProcessData(n, m_pDemodTmpBuf, pOutData);
					break;
			}
		g_pTestBench->DisplayData(n, pOutData, m_DemodOutputRate,PROFILE_4);
			m_InBufPos = 0;
			ret += n;
		}
	}
	m_Mutex.unlock();
	return ret;
}
