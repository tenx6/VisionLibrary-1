#include "visionwidget.h"
#include "FitParallelLineProcedure.h"
#include "FitRectProcedure.h"
#include "OcrProcedure.h"
#include "SrchFiducialProcedure.h"
#include "constants.h"
#include "VisionAPI.h"
#include <QFileDialog>
#include <QMessageBox>

using namespace AOI::Vision;

VisionWidget::VisionWidget(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowIcon(QIcon(":/VisionWidget/Image/VisionWidget.png"));

    ui.visionViewToolBox->SetVisionView(ui.visionView);
    _ptrIntValidator    = std::make_unique<QIntValidator>(1,100);
    ui.lineEditFitCircleErrTol->setValidator(_ptrIntValidator.get() );
    ui.visionView->setState(VisionView::VISION_VIEW_STATE::TEST_VISION_LIBRARY);

    _ptrGrayScaleWidget = std::make_unique<GrayScaleWidget>(this);
    _ptrGrayScaleWidget->setVisionView ( ui.visionView );
    ui.verticalLayout->addWidget(_ptrGrayScaleWidget.get());

    _ptrFilterWidget = std::make_unique<FilterWidget>(this);
    _ptrFilterWidget->setVisionView ( ui.visionView );
    ui.verticalLayout->addWidget(_ptrFilterWidget.get());

    _ptrThresholdWidget = std::make_unique<ThresholdWidget>(this);
    _ptrThresholdWidget->setVisionView ( ui.visionView );
    ui.verticalLayout->addWidget(_ptrThresholdWidget.get());

    _ptrFillHoleWidget = std::make_unique<FillHoleWidget>(this);
    _ptrFillHoleWidget->setVisionView( ui.visionView );
    ui.verticalLayout->addWidget(_ptrFillHoleWidget.get());

    _ptrEdgeDetectWidget = std::make_unique<EdgeDetectWidget>(this);
    _ptrEdgeDetectWidget->setVisionView ( ui.visionView );
    ui.verticalLayout->addWidget(_ptrEdgeDetectWidget.get());

    _ptrCcWidget = std::make_unique<CCWidget>(this);
    _ptrCcWidget->setVisionView ( ui.visionView );
    ui.verticalLayout->addWidget(_ptrCcWidget.get());
}

VisionWidget::~VisionWidget()
{
}

bool VisionWidget::checkDisplayImage()
{
    if ( _sourceImagePath.empty() ) {
        QMessageBox::information(this, "Vision Widget", "Please select an image first!", "Quit");
        return false;
    }

    if ( ui.visionView->isDisplayResultImage()) {
        QMessageBox::information(this, "Vision Widget", "Please swap to the original image first!", "Quit");
        return false;
    }
    return true;
}

void VisionWidget::on_selectImageBtn_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("Image Files (*.png *.jpg *.bmp)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec())  {
        fileNames = dialog.selectedFiles();
    }else
        return;

    _sourceImagePath = fileNames[0].toStdString();
    ui.imagePathEdit->setText(fileNames[0]);

    on_checkBoxByerFormat_clicked ( ui.checkBoxByerFormat->isChecked() );
}

void VisionWidget::on_checkBoxByerFormat_clicked(bool checked)
{
    if ( _sourceImagePath.empty() )
        return;

    cv::Mat mat;
    if ( checked )    {
        mat = cv::imread( _sourceImagePath, IMREAD_GRAYSCALE );
        cv::Mat matColor;
        cv::cvtColor (mat, matColor, CV_BayerGR2BGR );
        _matOriginal = matColor;
    }
    else
        _matOriginal = cv::imread( _sourceImagePath );

    ui.visionView->setMat( VisionView::DISPLAY_SOURCE::ORIGINAL, _matOriginal );
    ui.visionView->clearMat ( VisionView::DISPLAY_SOURCE::INTERMEDIATE );
    ui.visionView->clearMat ( VisionView::DISPLAY_SOURCE::RESULT );
}

void VisionWidget::on_fitCircleBtn_clicked()
{
    if ( _sourceImagePath.empty() ) {
        QMessageBox::information(this, "Vision Widget", "Please select an image first!", "Quit");
        return;
    }

    ui.visionView->setState ( VisionView::VISION_VIEW_STATE::TEST_VISION_LIBRARY );
    ui.visionView->applyIntermediateResult();

    PR_FIT_CIRCLE_CMD stCmd;
	stCmd.matInput = ui.visionView->getMat();
    stCmd.matMask = ui.visionView->getMask();
    stCmd.rectROI = ui.visionView->getSelectedWindow();    
	stCmd.enRmNoiseMethod = static_cast<PR_RM_FIT_NOISE_METHOD>(ui.comboBoxFitCircleDirection->currentIndex());
	stCmd.fErrTol = ui.lineEditFitCircleErrTol->text().toFloat();
    stCmd.bPreprocessed = true;
	stCmd.bAutoThreshold = false;
	//stCmd.nThreshold = _nThreshold;
    //stCmd.enAttribute = static_cast<PR_OBJECT_ATTRIBUTE>(_nAttribute);
	stCmd.enMethod = static_cast<PR_FIT_CIRCLE_METHOD>(ui.comboBoxFitCircleAlgorithm->currentIndex());
	stCmd.nMaxRansacTime = 20;

	PR_FIT_CIRCLE_RPY stRpy;
	VisionStatus enStatus = PR_FitCircle(&stCmd, &stRpy);
    if ( VisionStatus::OK == enStatus )
    {
        ui.visionView->setMat(VisionView::DISPLAY_SOURCE::RESULT, stRpy.matResult );
    }else {
        PR_GET_ERROR_STR_RPY stErrStrRpy;
        PR_GetErrorStr(enStatus, &stErrStrRpy);
        QMessageBox::critical(nullptr, "Fit Line", stErrStrRpy.achErrorStr, "Quit");
    }
}

void VisionWidget::on_calcRoundnessBtn_clicked()
{
    if ( _sourceImagePath.empty() ) {
        QMessageBox::information(this, "Vision Widget", "Please select an image first!", "Quit");
        return;
    }

    ui.visionView->setState ( VisionView::VISION_VIEW_STATE::TEST_VISION_LIBRARY );
    ui.visionView->applyIntermediateResult();

    PR_CIRCLE_ROUNDNESS_CMD stCmd;
	stCmd.matInput = ui.visionView->getMat();
    stCmd.matMask = ui.visionView->getMask();
    stCmd.rectROI = ui.visionView->getSelectedWindow();

	PR_CIRCLE_ROUNDNESS_RPY stRpy;
	VisionStatus enStatus = PR_CircleRoundness(&stCmd, &stRpy);
	if (VisionStatus::OK == enStatus)	{
		ui.visionView->setMat ( VisionView::DISPLAY_SOURCE::RESULT, stRpy.matResult );
        ui.lineEditRoundnessResult->setText( std::to_string(stRpy.fRoundness).c_str() );
    }else {
        PR_GET_ERROR_STR_RPY stErrStrRpy;
        PR_GetErrorStr(enStatus, &stErrStrRpy);
        QMessageBox::critical(nullptr, "Fit Line", stErrStrRpy.achErrorStr, "Quit");
    }
}

void VisionWidget::on_fitLineBtn_clicked()
{
    if ( _sourceImagePath.empty() ) {
        QMessageBox::information(this, "Vision Widget", "Please select an image first!", "Quit");
        return;
    }

    ui.visionView->setState ( VisionView::VISION_VIEW_STATE::TEST_VISION_LIBRARY );
    ui.visionView->applyIntermediateResult();

    PR_FIT_LINE_CMD stCmd;
	stCmd.matInput = ui.visionView->getMat();
    stCmd.matMask = ui.visionView->getMask();
	stCmd.enRmNoiseMethod = PR_RM_FIT_NOISE_METHOD::ABSOLUTE_ERR;
	stCmd.fErrTol = ui.lineEditFitLineErrTol->text().toFloat();
	stCmd.rectROI = ui.visionView->getSelectedWindow();
    stCmd.bPreprocessed = false;
	stCmd.nThreshold = ui.lineEditFitLineThreshold->text().toInt();
    stCmd.enAttribute = static_cast<PR_OBJECT_ATTRIBUTE> ( ui.comboBoxLineAttribute->currentIndex() );

	PR_FIT_LINE_RPY stRpy;
	VisionStatus enStatus = PR_FitLine(&stCmd, &stRpy);
    if ( VisionStatus::OK == enStatus )
    {
        ui.visionView->setMat ( VisionView::DISPLAY_SOURCE::RESULT, stRpy.matResult );
    }else {
        PR_GET_ERROR_STR_RPY stErrStrRpy;
        PR_GetErrorStr(enStatus, &stErrStrRpy);
        QMessageBox::critical(nullptr, "Fit Line", stErrStrRpy.achErrorStr, "Quit");
    }
}

void VisionWidget::on_detectLineBtn_clicked()
{
    if ( _sourceImagePath.empty() ) {
        QMessageBox::information(this, "Vision Widget", "Please select an image first!", "Quit");
        return;
    }

    ui.visionView->setState ( VisionView::VISION_VIEW_STATE::TEST_VISION_LIBRARY );
    ui.visionView->applyIntermediateResult();

    PR_DETECT_LINE_CMD stCmd;
    stCmd.matInput = ui.visionView->getMat();
    stCmd.matMask = ui.visionView->getMask();
    stCmd.rectROI = ui.visionView->getSelectedWindow();
    stCmd.enDetectDir = static_cast<PR_DETECT_LINE_DIR> ( ui.comboBoxDetectLineDirection->currentIndex() );

    PR_DETECT_LINE_RPY stRpy;
	VisionStatus enStatus = PR_DetectLine(&stCmd, &stRpy);
	if (VisionStatus::OK == enStatus)	{
		ui.visionView->setMat ( VisionView::DISPLAY_SOURCE::RESULT, stRpy.matResult );
    }
}

void VisionWidget::on_fitParallelLineBtn_clicked()
{
    if ( ! checkDisplayImage() )
        return;

    FitParallelLineProcedure procedure(ui.visionView);
    procedure.setErrTol ( ui.lineEditFitParallelLineErrTol->text().toFloat());
    procedure.setThreshold ( ui.lineEditFitParallelLineThreshold->text().toInt());
    procedure.setAttribute ( ui.comboBoxParallelLineAttribute->currentIndex());
	int nStatus = procedure.run(_sourceImagePath);
    if ( ToInt(VisionStatus::OK) == nStatus )
    {
        ui.visionView->setMat(VisionView::DISPLAY_SOURCE::RESULT, procedure.getResultMat());
    }
}

void VisionWidget::on_fitRectBtn_clicked()
{
    if ( ! checkDisplayImage() )
        return;

    FitRectProcedure procedure(ui.visionView);
    procedure.setErrTol ( ui.lineEditFitRectErrTol->text().toFloat());
    procedure.setThreshold ( ui.lineEditFitRectThreshold->text().toInt());
    procedure.setAttribute ( ui.comboBoxRectAttribute->currentIndex());
	int nStatus = procedure.run(_sourceImagePath);
    if ( ToInt(VisionStatus::OK) == nStatus )
    {
        ui.visionView->setMat(VisionView::DISPLAY_SOURCE::RESULT, procedure.getResultMat());
    }
}

void VisionWidget::on_ocrBtn_clicked()
{
    if ( ! checkDisplayImage() )
        return;

	OcrProcedure procedure(ui.visionView);
    procedure.setTextDirection(ui.comboBoxOcrTextDirection->currentIndex());
	int nStatus = procedure.run(_sourceImagePath);
    if ( ToInt(VisionStatus::OK) == nStatus )
    {
        ui.textEditOcrResult->setText( procedure.getResult().c_str() );
    }
}

void VisionWidget::on_srchFiducialBtn_clicked()
{
    if ( ! checkDisplayImage() )
        return;

    SrchFiducialProcedure procedure(ui.visionView);
    procedure.setFiducialSize ( ui.lineEditFiducialMarkSize->text().toFloat());
    procedure.setFiducialMarge ( ui.lineEditFiducialMarkMargin->text().toFloat());
    procedure.setFicucialType ( ui.comboBoxFiducialMarkShape->currentIndex() );
    int nStatus = procedure.run(_sourceImagePath);
    if ( ToInt(VisionStatus::OK) == nStatus )
    {
        ui.visionView->setMat ( VisionView::DISPLAY_SOURCE::RESULT, procedure.getResultMat() );
    }
}

void VisionWidget::on_selectTemplate_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("Image Files (*.png *.jpg *.bmp)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec())  {
        fileNames = dialog.selectedFiles();
    }else
        return;

    std::string strFilePath = fileNames[0].toStdString();
    _matTmpl = cv::imread ( strFilePath , cv::IMREAD_GRAYSCALE );
    if ( _matTmpl.empty() )
        return;

    cv::Mat matDisplay;
    cv::Size size( ui.labelTmplView->size().height(), ui.labelTmplView->size().width() );
    cv::resize ( _matTmpl, matDisplay, size );
    cvtColor ( matDisplay, matDisplay, CV_GRAY2RGB );
    QImage image = QImage((uchar*) matDisplay.data, matDisplay.cols, matDisplay.rows, ToInt32(matDisplay.step), QImage::Format_RGB888);
    ui.labelTmplView->setPixmap(QPixmap::fromImage(image));
}

void VisionWidget::on_matchTmplBtn_clicked()
{
    if ( _sourceImagePath.empty() ) {
        QMessageBox::information(this, "Vision Widget", "Please select an image first!", "Quit");
        return;
    }

    ui.visionView->setState ( VisionView::VISION_VIEW_STATE::TEST_VISION_LIBRARY );
    ui.visionView->applyIntermediateResult();

    PR_MATCH_TEMPLATE_CMD stCmd;
    stCmd.matInput = ui.visionView->getMat();
    stCmd.matTmpl = _matTmpl;
    stCmd.rectSrchWindow = ui.visionView->getSelectedWindow();
    stCmd.enMotion = static_cast<PR_OBJECT_MOTION>( ui.cbMotion->currentIndex() );

    PR_MATCH_TEMPLATE_RPY stRpy;
    VisionStatus enStatus = PR_MatchTmpl( &stCmd, &stRpy );
    if ( VisionStatus::OK == enStatus )
    {
        ui.visionView->setMat ( VisionView::DISPLAY_SOURCE::RESULT, stRpy.matResult );
        char chArrCenter[100];
        _snprintf( chArrCenter, sizeof ( chArrCenter ), "%.3f, %.3f", stRpy.ptObjPos.x, stRpy.ptObjPos.y);
        ui.lineEditObjCenter->setText( chArrCenter );
        std::string strRotation = std::to_string ( stRpy.fRotation );
        ui.lineEditObjRotation->setText(strRotation.c_str());
    }else {
        PR_GET_ERROR_STR_RPY stErrStrRpy;
        PR_GetErrorStr(enStatus, &stErrStrRpy);
        QMessageBox::critical(nullptr, "Match Template", stErrStrRpy.achErrorStr, "Quit");
        ui.lineEditObjCenter->clear();
        ui.lineEditObjRotation->clear();
    }
}