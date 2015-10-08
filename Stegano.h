#ifndef STEGANO_H
#define STEGANO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Enums.h"
#include <QtCore>
#include <QByteArray>
#include <QtGui>

class Stegano
{
public:
    Stegano();
    void setTargetFileContent(QByteArray targetFileContent);
    void setSourceFileContent(QByteArray sourceFileContent);
    bool checkIfSomethingEncoded(QByteArray inputFileContent);
    QByteArray hide(int rval, int gval, int bval, bool onSelectedBits);
    QByteArray hide(int val, bool onSelectedBits);
    QByteArray decode(QByteArray inputFileContent);
    Enums::FileFormat getFileType();

private:
    // Properties
    Enums::FileFormat fileType = Enums::NoFile;
    int rval;
    int gval;
    int bval;
    bool hideOnSelectedBits;
    QByteArray targetFileContent = NULL;
    QByteArray sourceFileContent = NULL;
    unsigned int sourceFileLength = 0;

    //methods
    Enums::FileFormat getFileType(QByteArray inputFileContent);
    int checkAmountOfBitsPerPixel();
    int checkAmountOfBitsPerPixel(QByteArray inputFileContent);
    QByteArray convertSourceFileContentToBitArray();
    void getColorValues(QByteArray inputFileContent);

    QByteArray decode24BitBitmap(QByteArray inputFileContent);
    QByteArray decodeSelectedBitsFrom24BitBitmap(QByteArray inputFileContent);
    QByteArray decode32BitBitmap(QByteArray inputFileContent);
    QByteArray decodeSelectedBitsFrom32BitBitmap(QByteArray inputFileContent);

    QByteArray hideIn24BitBitmap(int rval, int gval, int bval);
    QByteArray hideIn24BitBitmapOnSelectedBits(int rval, int gval, int bval);
    QByteArray hideIn32BitBitmap(int rval, int gval, int bval);
    QByteArray hideIn32BitBitmapOnSelectedBits(int rval, int gval, int bval);
};

#endif // STEGANO_H
