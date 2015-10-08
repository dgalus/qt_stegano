#include "Stegano.h"

/* CONSTRUCTORS */
Stegano::Stegano(){
}

/* PUBLIC METHODS */
void Stegano::setTargetFileContent(QByteArray targetFileContent)
{
    this->targetFileContent = targetFileContent;
}

void Stegano::setSourceFileContent(QByteArray sourceFileContent)
{
    this->sourceFileContent = sourceFileContent;
}

bool Stegano::checkIfSomethingEncoded(QByteArray inputFileContent)
{
    if(QString::fromLocal8Bit(inputFileContent.data()+inputFileContent.length()-12).startsWith("#AV3"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QByteArray Stegano::hide(int rval, int gval, int bval, bool onSelectedBits)
{
    if(targetFileContent.length() == 0 || targetFileContent == NULL || sourceFileContent == NULL || sourceFileContent.length() == 0 || (rval == 0 && gval == 0 && bval == 0))
    {
        return NULL;
    }
    if(fileType == Enums::NoFile || fileType == Enums::Unknown)
    {
        return NULL;
    }
    if(fileType == Enums::BMP24bit && onSelectedBits)
    {
        return hideIn24BitBitmapOnSelectedBits(rval, gval, bval);
    }
    if(fileType == Enums::BMP24bit && !onSelectedBits)
    {
        return hideIn24BitBitmap(rval, gval, bval);
    }
    if(fileType == Enums::BMP32bit && onSelectedBits)
    {
        return hideIn32BitBitmapOnSelectedBits(rval, gval, bval);
    }
    if(fileType == Enums::BMP32bit && !onSelectedBits)
    {
        return hideIn32BitBitmap(rval, gval, bval);
    }
    return NULL;
}

QByteArray Stegano::hide(int val, bool onSelectedBits)
{
    if(targetFileContent.length() == 0 || targetFileContent == NULL || sourceFileContent == NULL || sourceFileContent.length() == 0 || val == 0)
    {
        return NULL;
    }
    if (fileType == Enums::NoFile || fileType == Enums::Unknown)
    {
        return NULL;
    }
    if (fileType == Enums::BMP24bit || fileType == Enums::BMP32bit)
    {
        return hide(val, val, val, onSelectedBits);
    }
    return NULL;
}

QByteArray Stegano::decode(QByteArray inputFileContent)
{
    if(!checkIfSomethingEncoded(inputFileContent))
    {
        return NULL;
    }

    fileType = getFileType(inputFileContent);

    if(fileType == Enums::NoFile || fileType == Enums::Unknown)
    {
        return NULL;
    }

    getColorValues(inputFileContent);

    if(fileType == Enums::BMP24bit && hideOnSelectedBits)
    {
        return decodeSelectedBitsFrom24BitBitmap(inputFileContent);
    }
    if(fileType == Enums::BMP24bit && !hideOnSelectedBits)
    {
        return decode24BitBitmap(inputFileContent);
    }
    if(fileType == Enums::BMP32bit && hideOnSelectedBits)
    {
        return decodeSelectedBitsFrom32BitBitmap(inputFileContent);
    }
    if(fileType == Enums::BMP32bit && !hideOnSelectedBits)
    {
        return decode32BitBitmap(inputFileContent);
    }

    return NULL;
}

Enums::FileFormat Stegano::getFileType()
{
     if(targetFileContent.length() == 0 || targetFileContent == NULL)
     {
         fileType = Enums::NoFile;
     }
     else
     {
         if(targetFileContent.startsWith("BM"))
         {
             switch(checkAmountOfBitsPerPixel())
             {
                 case 24:
                     fileType = Enums::BMP24bit;
                     break;
                 case 32:
                     fileType = Enums::BMP32bit;
                     break;
                 default:
                     fileType = Enums::Unknown;
                     break;
             }
         }
         else
         {
             fileType = Enums::Unknown;
         }
     }
 return fileType;
}

/* PRIVATE METHODS */
Enums::FileFormat Stegano::getFileType(QByteArray inputFileContent)
{
    if(inputFileContent.length() == 0 || inputFileContent == NULL)
    {
        fileType = Enums::NoFile;
    }
    else
    {
        if(inputFileContent.startsWith("BM"))
        {
            switch(checkAmountOfBitsPerPixel(inputFileContent))
            {
                case 24:
                    fileType = Enums::BMP24bit;
                    break;
                case 32:
                    fileType = Enums::BMP32bit;
                    break;
                default:
                    fileType = Enums::Unknown;
                    break;
            }
        }
        else
        {
            fileType = Enums::Unknown;
        }
    }
    return fileType;
}

int Stegano::checkAmountOfBitsPerPixel()
{
    if((targetFileContent != NULL) && (targetFileContent.length() > 54))
    {
        return (int) targetFileContent.at(28);
    }
    else
    {
        return -1;
    }
}

int Stegano::checkAmountOfBitsPerPixel(QByteArray inputFileContent)
{
    if((inputFileContent != NULL) && (inputFileContent.length() > 54))
    {
        return (int) inputFileContent.at(28);
    }
    else
    {
        return -1;
    }
}

QByteArray Stegano::convertSourceFileContentToBitArray()
{
    QByteArray sourceFileContentBitArray(sourceFileContent.length()*8, 0);

    for(int i=0; i<sourceFileContent.length(); i++)
    {
        for(int j=7; j>=0; j--)
        {
            sourceFileContentBitArray.data()[i*8+j] = ((sourceFileContent[i] >> (7-j)) & 0x01);
        }
    }
    return sourceFileContentBitArray;
}

void Stegano::getColorValues(QByteArray inputFileContent)
{
    sourceFileLength = (inputFileContent.data()[inputFileContent.length()-4] << 24) & 0xff000000;
    sourceFileLength += ((inputFileContent.data()[inputFileContent.length()-3] << 16) & 0x00ff0000);
    sourceFileLength += ((inputFileContent.data()[inputFileContent.length()-2] << 8) & 0x0000ff00);
    sourceFileLength += ((inputFileContent.data()[inputFileContent.length()-1]) & 0x000000ff);

    rval = (int) inputFileContent[inputFileContent.length()-8];
    gval = (int) inputFileContent[inputFileContent.length()-7];
    bval = (int) inputFileContent[inputFileContent.length()-6];
    if(inputFileContent.data()[inputFileContent.length()-5] == 0)
    {
        hideOnSelectedBits = false;
    }
    else
    {
        hideOnSelectedBits = true;
    }
}

QByteArray Stegano::decode24BitBitmap(QByteArray inputFileContent)
{
    QByteArray outputArray(sourceFileLength, 0);
    QByteArray bits(sourceFileLength*8, 0);

    unsigned long long iter;
    unsigned long long iter2;
    unsigned long long iter4 = 0;
    long long iter3;
    char c;
    unsigned long long bytesToDecode = (24*sourceFileLength)/(bval+gval+rval);

    for(iter=54, iter2=0; iter<bytesToDecode+54; iter++, iter2++)
    {
        switch(iter2 % 3)
        {
            case 0:
                for(iter3=bval-1; iter3>=0; iter3--)
                {
                    c = inputFileContent.data()[iter];
                    c = (c >> iter3);
                    c &= 0x01;
                    bits.data()[iter4++] = c;
                }
                break;
            case 1:
                for(iter3=gval-1; iter3>=0; iter3--)
                {
                    c = inputFileContent.data()[iter];
                    c = (c >> iter3);
                    c &= 0x01;
                    bits.data()[iter4++] = c;
                }
                break;
            case 2:
                for(iter3=rval-1; iter3>=0; iter3--)
                {
                    c = inputFileContent.data()[iter];
                    c = (c >> iter3);
                    c &= 0x01;
                    bits.data()[iter4++] = c;
                }
                break;
        }
    }

    for(iter=0, iter3 = 0; iter <sourceFileLength*8; iter+=8, iter3++)
    {
        c = 0;
        for(iter2=0; iter2<8; iter2++)
        {
            c = c << 1;
            c += bits.data()[iter+iter2];
        }
        outputArray.data()[iter3] = c;
    }
    return outputArray;
}

QByteArray Stegano::decodeSelectedBitsFrom24BitBitmap(QByteArray inputFileContent)
{
    QByteArray sourceFileBits(sourceFileLength*8, 0);

    char rShift = (1 << (rval-1));
    char gShift = (1 << (gval-1));
    char bShift = (1 << (bval-1));

    for(int i=54; i<sourceFileLength*8+54; i++)
    {
        switch((i-54) % 3)
        {
            case 0:
                sourceFileBits.data()[i-54] = (inputFileContent.data()[i] & bShift);
                break;
            case 1:
                sourceFileBits.data()[i-54] = (inputFileContent.data()[i] & gShift);
                break;
            case 2:
                sourceFileBits.data()[i-54] = (inputFileContent.data()[i] & rShift);
                break;
        }
    }

    for(int i=0; i<sourceFileLength*8; i++)
    {
        sourceFileBits.data()[i] = (sourceFileBits.data()[i] != 0 ? 1 : 0);
    }

    QByteArray outputArray(sourceFileLength, 0);

    for(int i=0; i<sourceFileLength; i++)
    {
        for(int j=0; j<8; j++)
        {
            outputArray.data()[i] = (outputArray.data()[i] << 1);
            outputArray.data()[i] += sourceFileBits.data()[i*8+j];
        }
    }
    return outputArray;
}

QByteArray Stegano::decode32BitBitmap(QByteArray inputFileContent)
{
    QByteArray outputArray(sourceFileLength, 0);
    QByteArray bits(sourceFileLength*8, 0);

    unsigned long long iter;
    unsigned long long iter2;
    unsigned long long iter4 = 0;
    long long iter3;
    char c;
    unsigned long long bytesToDecode = (24*sourceFileLength)/(bval+gval+rval);

    for(iter=54, iter2=0; iter<bytesToDecode+54; iter++, iter2++)
    {
        switch(iter2 % 4)
        {
            case 0:
                break;
            case 1:
                for(iter3=bval-1; iter3>=0; iter3--)
                {
                    c = inputFileContent.data()[iter];
                    c = (c >> iter3);
                    c &= 0x01;
                    bits.data()[iter4++] = c;
                }
                break;
            case 2:
                for(iter3=gval-1; iter3>=0; iter3--)
                {
                    c = inputFileContent.data()[iter];
                    c = (c >> iter3);
                    c &= 0x01;
                    bits.data()[iter4++] = c;
                }
                break;
            case 3:
                for(iter3=rval-1; iter3>=0; iter3--)
                {
                    c = inputFileContent.data()[iter];
                    c = (c >> iter3);
                    c &= 0x01;
                    bits.data()[iter4++] = c;
                }
                break;
        }
    }

    for(iter=0, iter3 = 0; iter <sourceFileLength*8; iter+=8, iter3++)
    {
        c = 0;
        for(iter2=0; iter2<8; iter2++)
        {
            c = c << 1;
            c += bits.data()[iter+iter2];
        }
        outputArray.data()[iter3] = c;
    }
    return outputArray;
}

QByteArray Stegano::decodeSelectedBitsFrom32BitBitmap(QByteArray inputFileContent)
{
    QByteArray sourceFileBits(sourceFileLength*8, 0);

    char rShift = (1 << (rval-1));
    char gShift = (1 << (gval-1));
    char bShift = (1 << (bval-1));

    for(int i=54; i<sourceFileLength*8+54; i++)
    {
        switch((i-54) % 4)
        {
            case 0:
                break;
            case 1:
                sourceFileBits.data()[i-54] = (inputFileContent.data()[i] & bShift);
                break;
            case 2:
                sourceFileBits.data()[i-54] = (inputFileContent.data()[i] & gShift);
                break;
            case 3:
                sourceFileBits.data()[i-54] = (inputFileContent.data()[i] & rShift);
                break;
        }
    }

    for(int i=0; i<sourceFileLength*8; i++)
    {
        sourceFileBits.data()[i] = (sourceFileBits.data()[i] != 0 ? 1 : 0);
    }

    QByteArray outputArray(sourceFileLength, 0);

    for(int i=0; i<sourceFileLength; i++)
    {
        for(int j=0; j<8; j++)
        {
            outputArray.data()[i] = (outputArray.data()[i] << 1);
            outputArray.data()[i] += sourceFileBits.data()[i*8+j];
        }
    }
    return outputArray;
}

QByteArray Stegano::hideIn24BitBitmap(int rval, int gval, int bval)
{
    if(rval != 0 || gval != 0 || bval != 0)
    {
        QByteArray outputArray(targetFileContent.length()+12, 0);

        for(int i=0; i<targetFileContent.length(); i++)
        {
            outputArray.data()[i] = targetFileContent.data()[i];
        }

        outputArray[outputArray.length()-12] = '#';
        outputArray[outputArray.length()-11] = 'A';
        outputArray[outputArray.length()-10] = 'V';
        outputArray[outputArray.length()-9] = '3';
        outputArray[outputArray.length()-8] = (char) rval;
        outputArray[outputArray.length()-7] = (char) gval;
        outputArray[outputArray.length()-6] = (char) bval;
        outputArray[outputArray.length()-5] = 0;

        int sourceFileContentLength = sourceFileContent.length();

        outputArray.data()[outputArray.length()-4] = (sourceFileContentLength & 0xff000000) >> 24;
        outputArray.data()[outputArray.length()-3] = (sourceFileContentLength & 0x00ff0000) >> 16;
        outputArray.data()[outputArray.length()-2] = (sourceFileContentLength & 0x0000ff00) >> 8;
        outputArray.data()[outputArray.length()-1] = (sourceFileContentLength & 0x000000ff);

        QByteArray sourceFileContentBitArray = convertSourceFileContentToBitArray();

        int bitArrayIterator = 0;

        for(int i=54; bitArrayIterator < sourceFileContentBitArray.length() && i < targetFileContent.length(); i++)
        {
            switch((i-54)%3)
            {
                case 0:
                    for(int j=bval-1; j>=0; j--)
                    {
                        if(bitArrayIterator < sourceFileContentBitArray.length())
                        {
                            if(sourceFileContentBitArray.data()[bitArrayIterator] == 0)
                            {
                                outputArray.data()[i] &= ~(1 << j);
                            }
                            if(sourceFileContentBitArray.data()[bitArrayIterator] == 1)
                            {
                                outputArray.data()[i] |= (1 << j);
                            }
                            bitArrayIterator++;
                        }
                    }
                    break;
                case 1:
                    for(int j=gval-1; j>=0; j--)
                    {
                        if(bitArrayIterator < sourceFileContentBitArray.length())
                        {
                            if(sourceFileContentBitArray.data()[bitArrayIterator] == 0)
                            {
                                outputArray.data()[i] &= ~(1 << j);
                            }
                            if(sourceFileContentBitArray.data()[bitArrayIterator] == 1)
                            {
                                outputArray.data()[i] |= (1 << j);
                            }
                            bitArrayIterator++;
                        }
                    }
                    break;
                case 2:
                    for(int j=rval-1; j>=0; j--)
                    {
                        if(bitArrayIterator < sourceFileContentBitArray.length())
                        {
                            if(sourceFileContentBitArray.data()[bitArrayIterator] == 0)
                            {
                                outputArray.data()[i] &= ~(1 << j);
                            }
                            if(sourceFileContentBitArray.data()[bitArrayIterator] == 1)
                            {
                                outputArray.data()[i] |= (1 << j);
                            }
                            bitArrayIterator++;
                        }
                    }
                    break;
            }
        }
        return outputArray;
    }
    else
    {
        return targetFileContent;
    }
}

QByteArray Stegano::hideIn24BitBitmapOnSelectedBits(int rval, int gval, int bval)
{
    QByteArray outputArray(targetFileContent.length()+12, 0);

    for(int i=0; i<targetFileContent.length(); i++)
    {
        outputArray.data()[i] = targetFileContent.data()[i];
    }

    outputArray[outputArray.length()-12] = '#';
    outputArray[outputArray.length()-11] = 'A';
    outputArray[outputArray.length()-10] = 'V';
    outputArray[outputArray.length()-9] = '3';
    outputArray[outputArray.length()-8] = (char) rval;
    outputArray[outputArray.length()-7] = (char) gval;
    outputArray[outputArray.length()-6] = (char) bval;
    outputArray[outputArray.length()-5] = 1;

    int sourceFileContentLength = sourceFileContent.length();
    outputArray.data()[outputArray.length()-4] = (sourceFileContentLength & 0xff000000) >> 24;
    outputArray.data()[outputArray.length()-3] = (sourceFileContentLength & 0x00ff0000) >> 16;
    outputArray.data()[outputArray.length()-2] = (sourceFileContentLength & 0x0000ff00) >> 8;
    outputArray.data()[outputArray.length()-1] = (sourceFileContentLength & 0x000000ff);

    QByteArray sourceFileContentBitArray = convertSourceFileContentToBitArray();

    for(int i=54; i<sourceFileContentBitArray.length()+54; i++)
    {
        switch((i-54) % 3)
        {
            case 0:
                if(sourceFileContentBitArray.data()[i-54] == 0)
                {
                    outputArray.data()[i] &= ~(1 << (bval-1));
                }
                if(sourceFileContentBitArray.data()[i-54] == 1)
                {
                    outputArray.data()[i] |= (1 << (bval-1));
                }
                break;
            case 1:
                if(sourceFileContentBitArray.data()[i-54] == 0)
                {
                    outputArray.data()[i] &= ~(1 << (gval-1));
                }
                if(sourceFileContentBitArray.data()[i-54] == 1)
                {
                    outputArray.data()[i] |= (1 << (gval-1));
                }
                break;
            case 2:
                if(sourceFileContentBitArray.data()[i-54] == 0)
                {
                    outputArray.data()[i] &= ~(1 << (rval-1));
                }
                if(sourceFileContentBitArray.data()[i-54] == 1)
                {
                    outputArray.data()[i] |= (1 << (rval-1));
                }
                break;
        }
    }
    return outputArray;
}

QByteArray Stegano::hideIn32BitBitmap(int rval, int gval, int bval)
{
    QByteArray outputArray(targetFileContent.length()+12, 0);

    for(int i=0; i<targetFileContent.length(); i++)
    {
        outputArray[i] = targetFileContent[i];
    }

    outputArray[outputArray.length()-12] = '#';
    outputArray[outputArray.length()-11] = 'A';
    outputArray[outputArray.length()-10] = 'V';
    outputArray[outputArray.length()-9] = '3';
    outputArray[outputArray.length()-8] = (char) rval;
    outputArray[outputArray.length()-7] = (char) gval;
    outputArray[outputArray.length()-6] = (char) bval;
    outputArray[outputArray.length()-5] = 0;

    int sourceFileContentLength = sourceFileContent.length();
    outputArray.data()[outputArray.length()-4] = (sourceFileContentLength & 0xff000000) >> 24;
    outputArray.data()[outputArray.length()-3] = (sourceFileContentLength & 0x00ff0000) >> 16;
    outputArray.data()[outputArray.length()-2] = (sourceFileContentLength & 0x0000ff00) >> 8;
    outputArray.data()[outputArray.length()-1] = (sourceFileContentLength & 0x000000ff);

    QByteArray sourceFileContentBitArray = convertSourceFileContentToBitArray();

    int bitArrayIterator = 0;

    for(int i=54; bitArrayIterator < sourceFileContentBitArray.length() && i < targetFileContent.length(); i++)
    {
        switch((i-54) % 4)
        {
            case 0:
                break;
            case 1:
                for(int j=bval-1; j>=0; j--)
                {
                    if(bitArrayIterator < sourceFileContentBitArray.length())
                    {
                        if(sourceFileContentBitArray.data()[bitArrayIterator] == 0)
                        {
                            outputArray.data()[i] &= ~(1 << j);
                        }
                        if(sourceFileContentBitArray.data()[bitArrayIterator] == 1)
                        {
                            outputArray.data()[i] |= (1 << j);
                        }
                        bitArrayIterator++;
                    }
                }
                break;
            case 2:
                for(int j=gval-1; j>=0; j--)
                {
                    if(bitArrayIterator < sourceFileContentBitArray.length())
                    {
                        if(sourceFileContentBitArray.data()[bitArrayIterator] == 0)
                        {
                            outputArray.data()[i] &= ~(1 << j);
                        }
                        if(sourceFileContentBitArray.data()[bitArrayIterator] == 1)
                        {
                            outputArray.data()[i] |= (1 << j);
                        }
                        bitArrayIterator++;
                    }
                }
                break;
            case 3:
                for(int j=rval-1; j>=0; j--)
                {
                    if(bitArrayIterator < sourceFileContentBitArray.length())
                    {
                        if(sourceFileContentBitArray.data()[bitArrayIterator] == 0)
                        {
                            outputArray.data()[i] &= ~(1 << j);
                        }
                        if(sourceFileContentBitArray.data()[bitArrayIterator] == 1)
                        {
                            outputArray.data()[i] |= (1 << j);
                        }
                        bitArrayIterator++;
                    }
                }
                break;
        }
    }

    return outputArray;
}

QByteArray Stegano::hideIn32BitBitmapOnSelectedBits(int rval, int gval, int bval)
{
    QByteArray outputArray(targetFileContent.length()+12, 0);

    for(int i=0; i<targetFileContent.length(); i++)
    {
        outputArray[i] = targetFileContent[i];
    }

    outputArray[outputArray.length()-12] = '#';
    outputArray[outputArray.length()-11] = 'A';
    outputArray[outputArray.length()-10] = 'V';
    outputArray[outputArray.length()-9] = '3';
    outputArray[outputArray.length()-8] = (char) rval;
    outputArray[outputArray.length()-7] = (char) gval;
    outputArray[outputArray.length()-6] = (char) bval;
    outputArray[outputArray.length()-5] = 1;

    int sourceFileContentLength = sourceFileContent.length();
    outputArray.data()[outputArray.length()-4] = (sourceFileContentLength & 0xff000000) >> 24;
    outputArray.data()[outputArray.length()-3] = (sourceFileContentLength & 0x00ff0000) >> 16;
    outputArray.data()[outputArray.length()-2] = (sourceFileContentLength & 0x0000ff00) >> 8;
    outputArray.data()[outputArray.length()-1] = (sourceFileContentLength & 0x000000ff);

    QByteArray sourceFileContentBitArray = convertSourceFileContentToBitArray();

    for(int i=54; i<sourceFileContentBitArray.length()+54; i++)
    {
        switch((i-54) % 4)
        {
            case 0:
                break;
            case 1:
                if(sourceFileContentBitArray.data()[i-54] == 0)
                {
                    outputArray.data()[i] &= ~(1 << (bval-1));
                }
                if(sourceFileContentBitArray.data()[i-54] == 1)
                {
                    outputArray.data()[i] |= (1 << (bval-1));
                }
                break;
            case 2:
                if(sourceFileContentBitArray.data()[i-54] == 0)
                {
                    outputArray.data()[i] &= ~(1 << (gval-1));
                }
                if(sourceFileContentBitArray.data()[i-54] == 1)
                {
                    outputArray.data()[i] |= (1 << (gval-1));
                }
                break;
            case 3:
                if(sourceFileContentBitArray.data()[i-54] == 0)
                {
                    outputArray.data()[i] &= ~(1 << (rval-1));
                }
                if(sourceFileContentBitArray.data()[i-54] == 1)
                {
                    outputArray.data()[i] |= (1 << (rval-1));
                }
                break;
        }
    }
    return outputArray;
}
