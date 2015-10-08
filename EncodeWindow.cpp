#include "EncodeWindow.h"
#include "ui_EncodeWindow.h"

/* CONSTRUCTORS */

EncodeWindow::EncodeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EncodeWindow)
{
    ui->setupUi(this);
    hideSliders();
    stegano = new Stegano();
    ui->showGraphWindowButton->setEnabled(false);
    ui->viewPicturesButton->setEnabled(false);
}

EncodeWindow::~EncodeWindow()
{
    delete ui;
}

/* PRIVATE SLOTS */

void EncodeWindow::on_selectTargetButton_clicked()
{
    QFile file(
        QFileDialog::getOpenFileName(this, textConstants.ConstOpenFile, "",
        textConstants.ConstBitmapBMP + " (*.bmp);;")
    );
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstCannotOpenSelectedFile);
        return;
    }
    targetFileName = file.fileName();
    targetFileContent = file.readAll();
    targetFileSize = targetFileContent.length();
    file.close();
    if(stegano->checkIfSomethingEncoded(targetFileContent))
    {
        QMessageBox::warning(this, textConstants.ConstWarning, textConstants.ConstSelectedFileAlreadyContainsHiddenData + "\n" + textConstants.ConstContinuingWillResultInTheLostOfThatData);
    }
    stegano->setTargetFileContent(targetFileContent);
    fileFormat = stegano->getFileType();
    switch(fileFormat){
        case Enums::BMP24bit:
        case Enums::BMP32bit:
            ui->targetFilenameLabel->setText(file.fileName().split("/").last());
            showSliders();
            updateLabels();
            break;
        case Enums::Unknown:
            targetFileContent = "";
            targetFileSize = 0;
            stegano->setTargetFileContent(targetFileContent);
            QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstCannotRecognizeFileFormat);
            hideSliders();
            break;
        case Enums::NoFile:
            targetFileContent = "";
            targetFileSize = 0;
            stegano->setTargetFileContent(targetFileContent);
            QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstNoOrEmptyFileSelected);
            hideSliders();
            break;
    }
}

void EncodeWindow::on_selectSourceButton_clicked()
{
    if(targetFileContent.isEmpty() || targetFileSize == 0)
    {
        QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstYouMustSelectTargetFileFirst);
        return;
    }

    QFile file(QFileDialog::getOpenFileName(this, textConstants.ConstOpenFile));
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstCannotOpenSelectedFile);
        return;
    }

    ui->sourceFilenameLabel->setText(file.fileName().split("/").last());
    sourceFileContent = file.readAll();
    sourceFileSize = sourceFileContent.length();
    file.close();
    stegano->setSourceFileContent(sourceFileContent);
    ui->amountLabel->setText(textConstants.ConstSourceFileSize + QString::number(sourceFileSize) + " B [" + QString::number(sourceFileSize/1024) +" kB]");
    ui->hideButton->setEnabled(true);
    buttonShowed = true;
}

void EncodeWindow::on_hideButton_clicked()
{
    stegano->setSourceFileContent(sourceFileContent);
    QByteArray outputArray = NULL;

    if(sourceFileSize > calcMaximumSpaceToUse())
    {
        QMessageBox::warning(this, textConstants.ConstWarning, textConstants.ConstSourceFileIsBiggerAsYouCanHideInTargetPictureWithSelectedParametersXSomeDataWillBeLost);
    }

    switch(fileFormat)
    {
        case Enums::BMP24bit:
        case Enums::BMP32bit:
            outputArray = stegano->hide(ui->rSlider->value(), ui->gSlider->value(), ui->bSlider->value(), ui->bitNumbersCheckBox->isChecked());
            break;
        case Enums::NoFile:
        case Enums::Unknown:
            QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstCannotRecognizeFileFormat);
            break;
    }

    QFile file(QFileDialog::getSaveFileName(this, textConstants.ConstSaveFile));
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstCannotSaveToSelectedFile);
        return;
    }
    outputFileName = file.fileName();
    file.write(outputArray);
    file.close();

    QMessageBox::information(this, textConstants.ConstInformation, textConstants.ConstSuccessfullySaved);
    setButtonsAfterEncryption();
}

void EncodeWindow::on_rSlider_valueChanged(int value)
{
    ui->rValLabel->setText(QString::number(value));
    updateLabels();
}

void EncodeWindow::on_gSlider_valueChanged(int value)
{
    ui->gValLabel->setText(QString::number(value));
    updateLabels();
}

void EncodeWindow::on_bSlider_valueChanged(int value)
{
    ui->bValLabel->setText(QString::number(value));
    updateLabels();
}

void EncodeWindow::on_bitNumbersCheckBox_clicked()
{
    updateLabels();
}

void EncodeWindow::on_showGraphWindowButton_clicked()
{
    GraphWindow *gw = new GraphWindow(sourceFileSize*360./calcMaximumSpaceToUse());
    gw->show();
}

/* PRIVATE METHODS */

void EncodeWindow::showSliders()
{
    ui->rSlider->setMaximum(8);
    ui->gSlider->setMaximum(8);
    ui->bSlider->setMaximum(8);
    ui->rSlider->show();
    ui->gSlider->show();
    ui->bSlider->show();
    ui->rLabel->show();
    ui->gLabel->show();
    ui->bLabel->show();
    ui->rValLabel->show();
    ui->gValLabel->show();
    ui->bValLabel->show();
    ui->bitNumbersCheckBox->show();
}

void EncodeWindow::hideSliders()
{
    ui->rLabel->hide();
    ui->gLabel->hide();
    ui->bLabel->hide();
    ui->rSlider->hide();
    ui->gSlider->hide();
    ui->bSlider->hide();
    ui->rValLabel->hide();
    ui->gValLabel->hide();
    ui->bValLabel->hide();
    ui->bitNumbersCheckBox->hide();
}

void EncodeWindow::showHideButton()
{
    ui->hideButton->show();
}

void EncodeWindow::hideHideButton()
{
    ui->hideButton->hide();
}

void EncodeWindow::on_viewPicturesButton_clicked()
{
    if(outputFileName != NULL && outputFileName.length() > 0)
    {
        ViewImageWindow* before = new ViewImageWindow(targetFileName, QString::fromUtf8("Przed zaszyfrowaniem"));
        before->show();
        ViewImageWindow* after = new ViewImageWindow(outputFileName, QString::fromUtf8("Po zaszyfrowaniu"));
        after->show();
    }
}

void EncodeWindow::updateLabels()
{
    int rval;
    int gval;
    int bval;

    if(buttonShowed)
    {
        ui->hideButton->setEnabled(true);
    }

    if(targetFileSize != 0)
    {
        if(ui->bitNumbersCheckBox->isChecked())
        {
            if(ui->rSlider->value() != 0)
            {
                rval = 1;
            }
            else
            {
                rval = 0;
            }
            if(ui->gSlider->value() != 0)
            {
                gval = 1;
            }
            else
            {
                gval = 0;
            }
            if(ui->bSlider->value() != 0)
            {
                bval = 1;
            }
            else
            {
                bval = 0;
            }
        }
        else
        {
            rval = ui->rSlider->value();
            gval = ui->gSlider->value();
            bval = ui->bSlider->value();
        }
        if(rval != 0 || gval != 0 || bval != 0)
        {
            if(fileFormat == Enums::BMP24bit)
            {
                int var = (targetFileSize-54)*(rval+gval+bval)/24;
                ui->usedSpaceLabel->setText(textConstants.ConstCapacityOfMedia + QString::number(var) + " B [" + QString::number(var/1024) + " kB] (" + QString::number(var*100./targetFileSize, 0, 2) + "%)");
            }
            else if(fileFormat == Enums::BMP32bit)
            {
                int var = (targetFileSize-54)*(rval+gval+bval)/32;
                ui->usedSpaceLabel->setText(textConstants.ConstCapacityOfMedia + QString::number(var) + " B [" + QString::number(var/1024) + " kB] (" + QString::number(var*100./targetFileSize, 0, 2) + "%)");
            }
            else
            {
            }
        }
        else
        {
            ui->usedSpaceLabel->setText(textConstants.ConstCapacityOfMedia + "0 B [0 kB] (0%)");
            ui->hideButton->setEnabled(false);
        }
    }
    else
    {
        ui->usedSpaceLabel->setText("");
    }
}

void EncodeWindow::setButtonsAfterEncryption()
{
    hideSliders();
    ui->selectTargetButton->setEnabled(false);
    ui->selectSourceButton->setEnabled(false);
    ui->hideButton->setEnabled(false);
    ui->showGraphWindowButton->setEnabled(true);
    ui->viewPicturesButton->setEnabled(true);
}

int EncodeWindow::calcMaximumSpaceToUse()
{
    int rval;
    int gval;
    int bval;

    if(targetFileSize != 0)
    {
        if(ui->bitNumbersCheckBox->isChecked())
        {
            if(ui->rSlider->value() != 0)
            {
                rval = 1;
            }
            else
            {
                rval = 0;
            }
            if(ui->gSlider->value() != 0)
            {
                gval = 1;
            }
            else
            {
                gval = 0;
            }
            if(ui->bSlider->value() != 0)
            {
                bval = 1;
            }
            else
            {
                bval = 0;
            }
        }
        else
        {
            rval = ui->rSlider->value();
            gval = ui->gSlider->value();
            bval = ui->bSlider->value();
        }
        if(rval != 0 || gval != 0 || bval != 0)
        {
            if(fileFormat == Enums::BMP24bit)
            {
                return (targetFileSize-54)*(rval+gval+bval)/24;
            }
            else if(fileFormat == Enums::BMP32bit)
            {
                return (targetFileSize-54)*(rval+gval+bval)/32;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}
