#include "DecodeWindow.h"
#include "ui_DecodeWindow.h"

DecodeWindow::DecodeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DecodeWindow)
{
    ui->setupUi(this);
}

DecodeWindow::~DecodeWindow()
{
    delete ui;
}

void DecodeWindow::on_selectFileButton_clicked()
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

    fileContent = file.readAll();

    if(!stegano.checkIfSomethingEncoded(fileContent))
    {
        free(&fileContent);
        QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstSelectedFileDoesNotContainAnyEncryptedData);
    }

    ui->infoLabel->setText(textConstants.ConstFileName + file.fileName().split("/").last() + "\n" + textConstants.ConstSizeOfFile + QString::number(file.size()) + " B");

    QByteArray outputArray = stegano.decode(fileContent);
    if(outputArray != NULL && outputArray.length() > 0)
    {
        QFile fileWrite(QFileDialog::getSaveFileName(this, textConstants.ConstSaveFile));
        if(!fileWrite.open(QIODevice::WriteOnly)){
            QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstCannotSaveToSelectedFile);
            return;
        }
        fileWrite.write(outputArray);
        fileWrite.close();
        QMessageBox::information(this, textConstants.ConstInformation, textConstants.ConstSuccessfullySaved);
        this->close();
    }
    else
    {
        QMessageBox::critical(this, textConstants.ConstError, textConstants.ConstSomeUnknownErrorOccured);
        this->close();
    }
}
