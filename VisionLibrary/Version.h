/*****************************************************************************
 * Version.h -- $Id$
 *
 * Purpose
 *   This file includes the version of the library and the change history.
 *
 * Indentation
 *   Four characters. No tabs!
 *
 * Modifications
 *   2017-09-13 (XSG) Add PR_CalcPD function to calculate system pattern distortion.
 *   2016-10-14 (XSG) Created.
 *
 * Copyright (c) 2016-2016, Xiao Shengguang.  All rights reserved.
 ****************************************************************************/
//2018-01-25 (XSG) Add fMinMatchScore for PR_MatchTmpl and PR_SrchFiducialMark function.
#define AOI_VISION_VERSION          "1.00.22"

//2018-01-23 (XSG) Auto calculate the ReverseSeq in PR_Calib3DBase function, the bReverseSeq changed from input to output.
//#define AOI_VISION_VERSION          "1.00.21"

//2018-01-21 (XSG) Add PR_MotorCalib3D function.
//#define AOI_VISION_VERSION          "1.00.20"

//2017-12-28 (XSG) Change PR_Caliper to PR_FindLine function.
//#define AOI_VISION_VERSION          "1.00.19"

//2017-12-20 (XSG) Add PR_FindCircle function.
//2017-12-23 (XSG) Add PR_FitLineByPoint and PR_FitCircleByPoint functions.
//#define AOI_VISION_VERSION          "1.00.18"

//2017-12-08 (XSG) Fix interval didn't return correct range size problem.
//#define AOI_VISION_VERSION          "1.00.17"

//2017-12-02 (XSG) Auto offset the phase measure range.
//#define AOI_VISION_VERSION          "1.00.16"

//2017-11-15 (XSG) Fix PR_Calc3DHeight use gamma not correct problem.
//2017-11-19 (XSG) Add PR_TwoLineAngle function to calculate angle of two lines.
//#define AOI_VISION_VERSION          "1.00.15"

//2017-11-13 (XSG) Add fRemoveLowerNoiseRatio in PR_MERGE_3D_HEIGHT_CMD to remove lower noise.
//2017-11-13 (XSG) Move median filter to the end of PR_Merge3DHeight.
//2017-11-13 (XSG) Remove nan value in the output height.
//#define AOI_VISION_VERSION          "1.00.14"

//2017-11-11 (XSG) Update the method to calculate 3D height. Change the phase range from -pi~pi to -1~1.
//2017-11-11 (XSG) Remove PR_Comb3DCalib and PR_FastCalc3DHeight function.
//2017-11-09 (XSG) Support use thinnest pattern.
//#define AOI_VISION_VERSION          "1.00.13"

//2017-11-05 (XSG) Add PR_Merge3DHeight function.
//#define AOI_VISION_VERSION          "1.00.12"

//2017-11-01 (XSG) Speed up PR_FastCalc3DHeight function by using atan2 lookup table.
//#define AOI_VISION_VERSION          "1.00.11"

//2017-10-27 (XSG) Add PR_FastCalc3DHeight function.
//#define AOI_VISION_VERSION          "1.00.10"

//2017-10-09 (XSG) Add PR_Integrate3DCalib function.
//#define AOI_VISION_VERSION          "1.00.09"

//2017-09-29 (XSG) Add PR_CalcCameraMTF function.
//2017-09-28 (XSG) Fix PR_CalibrateCamera fail problem on new optics image.
//#define AOI_VISION_VERSION          "1.00.08"

//2017-09-23 (XSG) Use Nelder-Mead algorithm to do auto threshold to speed up.
//2017-09-23 (XSG) Add PR_Calc3DBase function to avoid repeatly calculate base surface.
//#define AOI_VISION_VERSION          "1.00.07"

//2017-09-17 (XSG) Add PR_Comb3DCalib function.
//2017-09-16 (XSG) Add bSubPixelRefine in PR_MATCH_TEMPLATE_CMD. Default is false.
//#define AOI_VISION_VERSION          "1.00.06"

//2017-09-16 (XSG) Support 5 steps calibration.
//2017-09-16 (XSG) Use new method to do 3D calibrate, remove the top error points to fit surface.
//#define AOI_VISION_VERSION          "1.00.05"

//2017-09-15 (XSG) Add PR_Calc3DHeightDiff function.
//2017-09-14 (XSG) Enhance the Unwrap::_phaseUnwrapSurfaceTrk, add procedure unwrap from bottom to top.
//#define AOI_VISION_VERSION          "1.00.04"

//2017-09-14 (XSG) Add CALIB_3D_HEIGHT_SURFACE_TOO_SMALL status for calib 3D height.
//2017-09-14 (XSG) Remove 4 times harmonic wave.
//2017-09-14 (XSG) Fix Unwrap::_getIntegralTree not finish yet problem. Handle standalone pole points.
//#define AOI_VISION_VERSION          "1.00.03"

//2017-09-13 (XSG) Add PR_CalcPD function to calculate system pattern distortion.
//#define AOI_VISION_VERSION          "1.00.01"