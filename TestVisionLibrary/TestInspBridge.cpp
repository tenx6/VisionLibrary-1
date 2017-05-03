#include "stdafx.h"
#include "../VisionLibrary/VisionAPI.h"
#include "opencv2/highgui.hpp"
#include <iostream>
#include "TestSub.h"

using namespace AOI::Vision;

void TestInspBridge() {
    PR_INSP_BRIDGE_CMD stCmd;
    PR_INSP_BRIDGE_RPY stRpy;

    stCmd.matInputImg = cv::imread("./data/TestInspBridge.png", cv::IMREAD_GRAYSCALE);
    PR_INSP_BRIDGE_CMD::INSP_ITEM inspItem;
    inspItem.enMode = PR_INSP_BRIDGE_MODE::OUTER;
    inspItem.rectInnerWindow = cv::Rect(221, 195, 28, 82);
    inspItem.rectOuterWindow = cv::Rect(212, 187, 43, 102);
    for ( int i = 0; i < 4; ++ i ) {
        PR_INSP_BRIDGE_CMD::INSP_CRITERIA inspCriteria;
        inspCriteria.enDirection = PR_INSP_BRIDGE_DIRECTION(i);
        inspItem.vecInspCriteria.push_back ( inspCriteria );
    }
    stCmd.vecInspItems.push_back ( inspItem );
    
    PR_InspBridge(&stCmd, &stRpy);
    if ( VisionStatus::OK != stRpy.enStatus ) {
        std::cout << "Failed to inspect bridge. Error Status: " << ToInt32(stRpy.enStatus) << std::endl;
    }
    cv::imwrite("./data/TestInspBridge_Result.png", stRpy.matResultImg );
}