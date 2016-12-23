#include "FitParallelLineProcedure.h"
#include "messageboxdialog.h"
#include "constants.h"
#include <QMessageBox>
#include <QApplication>

using namespace AOI::Vision;

FitParallelLineProcedure::FitParallelLineProcedure(VisionView *pVisionView) : Procedure(pVisionView)
{
}

FitParallelLineProcedure::~FitParallelLineProcedure()
{
}

int FitParallelLineProcedure::run(const std::string &imagePath)
{
    QRect rect = qApp->activeWindow()->geometry();
	std::unique_ptr<MessageBoxDialog> pMessageBox = std::make_unique<MessageBoxDialog>();
    pMessageBox->setGeometry(rect.x() + POS_X, rect.y() + POS_Y, pMessageBox->size().width(), pMessageBox->size().height());
    pMessageBox->setWindowTitle("Fit Parallel Line");
	pMessageBox->SetMessageText1("Please input the line 1 search window");
    pMessageBox->SetMessageText2("Press and drag the left mouse buttont to input");
	pMessageBox->setWindowFlags(Qt::WindowStaysOnTopHint);
	pMessageBox->show();
	pMessageBox->raise();
	pMessageBox->activateWindow();
    _pVisionView->setTestVisionState(VisionView::TEST_VISION_STATE::SET_RECT_SRCH_WINDOW);
    _pVisionView->setCurrentSrchWindowIndex(0);
	int iReturn = pMessageBox->exec();
    if ( iReturn != QDialog::Accepted ) {
        _pVisionView->setTestVisionState(VisionView::TEST_VISION_STATE::UNDEFINED);
        return ToInt(STATUS::NOK);
    }

    pMessageBox->SetMessageText1("Please input the line 2 search window");
    pMessageBox->SetMessageText2("Press and drag the left mouse buttont to input");
	pMessageBox->setWindowFlags(Qt::WindowStaysOnTopHint);
	pMessageBox->show();
	pMessageBox->raise();
	pMessageBox->activateWindow();
    _pVisionView->setTestVisionState(VisionView::TEST_VISION_STATE::SET_RECT_SRCH_WINDOW);
    _pVisionView->setCurrentSrchWindowIndex(1);

	iReturn = pMessageBox->exec();
    if ( iReturn != QDialog::Accepted ) {
        _pVisionView->setTestVisionState(VisionView::TEST_VISION_STATE::UNDEFINED);
        return ToInt(STATUS::NOK);
    }
   
    _vecSrchWindow = _pVisionView->getVecSrchWindow();
    if ( _vecSrchWindow.size() <= 1 )   {
        pMessageBox->SetMessageText1("The input is invalid");
        pMessageBox->SetMessageText2("");
        pMessageBox->exec();
        _pVisionView->setTestVisionState(VisionView::TEST_VISION_STATE::UNDEFINED);
        return ToInt(STATUS::NOK);
    }

    int nStatus = fitParallelLine(imagePath);

    _pVisionView->setTestVisionState(VisionView::TEST_VISION_STATE::UNDEFINED);
    return nStatus;
}

int FitParallelLineProcedure::fitParallelLine(const std::string &imagePath)
{
    PR_FIT_PARALLEL_LINE_CMD stCmd;
	stCmd.matInput = cv::imread(imagePath);
	stCmd.enRmNoiseMethod = PR_RM_FIT_NOISE_METHOD::ABSOLUTE_ERR;
	stCmd.fErrTol = _fErrorTol;
	stCmd.rectArrROI[0] = _vecSrchWindow[0];
    stCmd.rectArrROI[1] = _vecSrchWindow[1];
	stCmd.nThreshold = _nThreshold;

	PR_FIT_PARALLEL_LINE_RPY stRpy;
	VisionStatus visionStatus = PR_FitParallelLine(&stCmd, &stRpy);
	if (VisionStatus::OK != visionStatus)	{
		std::cout << "Failed to fit parallel line, VisionStatus = " << stRpy.nStatus << std::endl;
		return static_cast<int> ( visionStatus );
	}
	_matResult = stRpy.matResult;	
    return ToInt(STATUS::OK);
}

void FitParallelLineProcedure::setErrTol(float fErrTol)
{
    _fErrorTol = fErrTol;
}

void FitParallelLineProcedure::setThreshold(int nThreshold)
{
    _nThreshold = nThreshold;
}

void FitParallelLineProcedure::setAlgorithm(int nAlgorithm)
{
    _nAlgorithm = nAlgorithm;
}

cv::Mat FitParallelLineProcedure::getResultMat() const
{
    return _matResult;
}