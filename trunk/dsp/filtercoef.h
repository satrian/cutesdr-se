// filtercoef.h: filte coefficients for various halfband filters
//
// History:
//	2010-09-15  Initial creation MSW
//	2011-03-27  Initial release
//////////////////////////////////////////////////////////////////////
#ifndef FILTERCOEF_H
#define FILTERCOEF_H

//////////////////////////////////////////////////////////////////////
// Filter -140dB Alias free normalized bandwidth constants for each type of filter
// These values are used to determine what filter to use in the decimation
// chains as a function of desired maximum output bandwidth
//////////////////////////////////////////////////////////////////////

//normalized alias free bandwidths for the filters
#define CIC3_MAX (.5-.4985)
#define HB11TAP_MAX (.5-.475)
#define HB15TAP_MAX (.5-.451)
#define HB19TAP_MAX (.5-.428)
#define HB23TAP_MAX (.5-.409)
#define HB27TAP_MAX (.5-.392)
#define HB31TAP_MAX (.5-.378)
#define HB35TAP_MAX (.5-.366)
#define HB39TAP_MAX (.5-.356)
#define HB43TAP_MAX (.5-.347)
#define HB47TAP_MAX (.5-.340)
#define HB51TAP_MAX (.5-.333)

////////////////////////////////////////////////////////////////////
//  The following coefficients were designed with
// MatLab for best alias rejection at -140dB
////////////////////////////////////////////////////////////////////
#define HB11TAP_LENGTH 11
const double HB11TAP_H[HB11TAP_LENGTH] =
{
   0.0060431029837374152,
   0.0,
   -0.049372515458761493,
   0.0,
   0.29332944952052842,
   0.5,
   0.29332944952052842,
   0.0,
   -0.049372515458761493,
   0.0,
   0.0060431029837374152,
};

#define HB15TAP_LENGTH 15
const double HB15TAP_H[HB15TAP_LENGTH] =
{
	-0.001442203300285281,
	0.0,
	0.013017512802724852,
	0.0,
	-0.061653278604903369,
	0.0,
	0.30007792316024057,
	0.5,
	0.30007792316024057,
	0.0,
	-0.061653278604903369,
	0.0,
	0.013017512802724852,
	0.0,
	-0.001442203300285281
};

#define HB19TAP_LENGTH 19
const double HB19TAP_H[HB19TAP_LENGTH] =
{
	0.00042366527106480427,
	0.0,
	-0.0040717333369021894,
	0.0,
	0.019895653881950692,
	0.0,
	-0.070740034412329067,
	0.0,
	0.30449249772844139,
	0.5,
	0.30449249772844139,
	0.0,
	-0.070740034412329067,
	0.0,
	0.019895653881950692,
	0.0,
	-0.0040717333369021894,
	0.0,
	0.00042366527106480427
};

#define HB23TAP_LENGTH 23
const double HB23TAP_H[HB23TAP_LENGTH] =
{
	-0.00014987651418332164,
	0.0,
	0.0014748633283609852,
	0.0,
	-0.0074416944990005314,
	0.0,
	0.026163522731980929,
	0.0,
	-0.077593699116544707,
	0.0,
	0.30754683719791986,
	0.5,
	0.30754683719791986,
	0.0,
	-0.077593699116544707,
	0.0,
	0.026163522731980929,
	0.0,
	-0.0074416944990005314,
	0.0,
	0.0014748633283609852,
	0.0,
	-0.00014987651418332164
};
#define HB27TAP_LENGTH 27
const double HB27TAP_H[HB27TAP_LENGTH] =
{
	0.000063730426952664685,
	0.0,
	-0.00061985193978569082,
	0.0,
	0.0031512504783365756,
	0.0,
	-0.011173151342856621,
	0.0,
	0.03171888754393197,
	0.0,
	-0.082917863582770729,
	0.0,
	0.3097770473566307,
	0.5,
	0.3097770473566307,
	0.0,
	-0.082917863582770729,
	0.0,
	0.03171888754393197,
	0.0,
	-0.011173151342856621,
	0.0,
	0.0031512504783365756,
	0.0,
	-0.00061985193978569082,
	0.0,
	0.000063730426952664685
};

#define HB31TAP_LENGTH 31
const double HB31TAP_H[HB31TAP_LENGTH] =
{
	-0.000030957335326552226,
	0.0,
	0.00029271992847303054,
	0.0,
	-0.0014770381124258423,
	0.0,
	0.0052539088990950535,
	0.0,
	-0.014856378748476874,
	0.0,
	0.036406651919555999,
	0.0,
	-0.08699862567952929,
	0.0,
	0.31140967076042625,
	0.5,
	0.31140967076042625,
	0.0,
	-0.08699862567952929,
	0.0,
	0.036406651919555999,
	0.0,
	-0.014856378748476874,
	0.0,
	0.0052539088990950535,
	0.0,
	-0.0014770381124258423,
	0.0,
	0.00029271992847303054,
	0.0,
	-0.000030957335326552226
};
#define HB35TAP_LENGTH 35
const double HB35TAP_H[HB35TAP_LENGTH] =
{
	0.000017017718072971716,
	0.0,
	-0.00015425042851962818,
	0.0,
	0.00076219685751140838,
	0.0,
	-0.002691614694785393,
	0.0,
	0.0075927497927344764,
	0.0,
	-0.018325727896057686,
	0.0,
	0.040351004914363969,
	0.0,
	-0.090198224668969554,
	0.0,
	0.31264689763504327,
	0.5,
	0.31264689763504327,
	0.0,
	-0.090198224668969554,
	0.0,
	0.040351004914363969,
	0.0,
	-0.018325727896057686,
	0.0,
	0.0075927497927344764,
	0.0,
	-0.002691614694785393,
	0.0,
	0.00076219685751140838,
	0.0,
	-0.00015425042851962818,
	0.0,
	0.000017017718072971716
};
#define HB39TAP_LENGTH 39
const double HB39TAP_H[HB39TAP_LENGTH] =
{
	-0.000010175082832074367,
	0.0,
	0.000088036416015024345,
	0.0,
	-0.00042370835558387595,
	0.0,
	0.0014772557414459019,
	0.0,
	-0.0041468438954260153,
	0.0,
	0.0099579126901608011,
	0.0,
	-0.021433527104289002,
	0.0,
	0.043598963493432855,
	0.0,
	-0.092695953625928404,
	0.0,
	0.31358799113382152,
	0.5,
	0.31358799113382152,
	0,
	-0.092695953625928404,
	0.0,
	0.043598963493432855,
	0.0,
	-0.021433527104289002,
	0.0,
	0.0099579126901608011,
	0.0,
	-0.0041468438954260153,
	0.0,
	0.0014772557414459019,
	0.0,
	-0.00042370835558387595,
	0.0,
	0.000088036416015024345,
	0.0,
	-0.000010175082832074367
};
#define HB43TAP_LENGTH 43
const double HB43TAP_H[HB43TAP_LENGTH] =
{
	0.0000067666739082756387,
	0.0,
	-0.000055275221547958285,
	0.0,
	0.00025654074579418561,
	0.0,
	-0.0008748125689163153,
	0.0,
	0.0024249876017061502,
	0.0,
	-0.0057775190656021748,
	0.0,
	0.012299834239523121,
	0.0,
	-0.024244050662087069,
	0.0,
	0.046354303503099069,
	0.0,
	-0.094729903598633314,
	0.0,
	0.31433918020123208,
	0.5,
	0.31433918020123208,
	0.0,
	-0.094729903598633314,
	0.0,
	0.046354303503099069,
	0.0,
	-0.024244050662087069,
	0.0,
	0.012299834239523121,
	0.0,
	-0.0057775190656021748,
	0.0,
	0.0024249876017061502,
	0.0,
	-0.0008748125689163153,
	0.0,
	0.00025654074579418561,
	0.0,
	-0.000055275221547958285,
	0.0,
	0.0000067666739082756387
};
#define HB47TAP_LENGTH 47
const double HB47TAP_H[HB47TAP_LENGTH] =
{
	-0.0000045298314172004251,
	0.0,
	0.000035333704512843228,
	0.0,
	-0.00015934776420643447,
	0.0,
	0.0005340788063118928,
	0.0,
	-0.0014667949695500761,
	0.0,
	0.0034792089350833247,
	0.0,
	-0.0073794356720317733,
	0.0,
	0.014393786384683398,
	0.0,
	-0.026586603160193314,
	0.0,
	0.048538673667907428,
	0.0,
	-0.09629115286535718,
	0.0,
	0.31490673428547367,
	0.5,
	0.31490673428547367,
	0.0,
	-0.09629115286535718,
	0.0,
	0.048538673667907428,
	0.0,
	-0.026586603160193314,
	0.0,
	0.014393786384683398,
	0.0,
	-0.0073794356720317733,
	0.0,
	0.0034792089350833247,
	0.0,
	-0.0014667949695500761,
	0.0,
	0.0005340788063118928,
	0.0,
	-0.00015934776420643447,
	0.0,
	0.000035333704512843228,
	0.0,
	-0.0000045298314172004251
};
#define HB51TAP_LENGTH 51
const double HB51TAP_H[HB51TAP_LENGTH] =
{
	0.0000033359253688981639,
	0.0,
	-0.000024584155158361803,
	0.0,
	0.00010677777483317733,
	0.0,
	-0.00034890723143173914,
	0.0,
	0.00094239127078189603,
	0.0,
	-0.0022118302078923137,
	0.0,
	0.0046575030752162277,
	0.0,
	-0.0090130973415220566,
	0.0,
	0.016383673864361164,
	0.0,
	-0.028697281101743237,
	0.0,
	0.05043292242400841,
	0.0,
	-0.097611898315791965,
	0.0,
	0.31538104435015801,
	0.5,
	0.31538104435015801,
	0.0,
	-0.097611898315791965,
	0.0,
	0.05043292242400841,
	0.0,
	-0.028697281101743237,
	0.0,
	0.016383673864361164,
	0.0,
	-0.0090130973415220566,
	0.0,
	0.0046575030752162277,
	0.0,
	-0.0022118302078923137,
	0.0,
	0.00094239127078189603,
	0.0,
	-0.00034890723143173914,
	0.0,
	0.00010677777483317733,
	0.0,
	-0.000024584155158361803,
	0.0,
	0.0000033359253688981639
};

#endif // FILTERCOEF_H
