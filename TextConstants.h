#ifndef TEXTCONSTANTS_H
#define TEXTCONSTANTS_H

#include <QString>

class TextConstants
{
public:
    const QString ConstCapacityOfMedia = QString::fromUtf8("Pojemność nośnika: ");
    const QString ConstUsedSpace = QString::fromUtf8("Użyte miejsce: ");
    const QString ConstError = QString::fromUtf8("Błąd");
    const QString ConstYouMustSelectTargetFileFirst = QString::fromUtf8("Musisz najpierw wybrać plik docelowy!");
    const QString ConstOpenFile = QString::fromUtf8("Otwórz plik");
    const QString ConstBitmapBMP = QString::fromUtf8("Bitmapa BMP");
    const QString ConstCannotOpenSelectedFile = QString::fromUtf8("Nie można otworzyć wybranego pliku!");
    const QString ConstCannotRecognizeFileFormat = QString::fromUtf8("Nie można rozpoznać formatu pliku!");
    const QString ConstNoOrEmptyFileSelected = QString::fromUtf8("Nie wybrano pliku lub wybrany plik jest pusty!");
    const QString ConstUsedSpaceGraph = QString::fromUtf8("Użyte miejsce");
    const QString ConstFreeSpaceGraph = QString::fromUtf8("Wolne miejsce");
    const QString ConstSaveFile = QString::fromUtf8("Zapisz plik");
    const QString ConstCannotSaveToSelectedFile = QString::fromUtf8("Nie można zapisać we wskazanym pliku!");
    const QString ConstInformation = QString::fromUtf8("Informacja");
    const QString ConstSuccessfullySaved = QString::fromUtf8("Pomyślnie zapisano plik");
    const QString ConstSomeUnknownErrorOccured = QString::fromUtf8("Wystąpił nieznany błąd");
    const QString ConstSizeOfFile = QString::fromUtf8("Rozmiar pliku: ");
    const QString ConstFileName = QString::fromUtf8("Nazwa pliku: ");
    const QString ConstSelectedFileDoesNotContainAnyEncryptedData = QString::fromUtf8("Wybrany plik nie zawiera żadnej ukrytych danych!");
    const QString ConstWarning = QString::fromUtf8("Ostrzeżenie");
    const QString ConstSelectedFileAlreadyContainsHiddenData = QString::fromUtf8("Wybrany plik zawiera już ukryte dane.");
    const QString ConstContinuingWillResultInTheLostOfThatData = QString::fromUtf8("Kontynuowanie z tym plikiem zniszczy ukryte dane.");
    const QString ConstSourceFileSize = QString::fromUtf8("Rozmiar pliku źródłowego: ");
    const QString ConstSourceFileIsBiggerAsYouCanHideInTargetPictureWithSelectedParametersXSomeDataWillBeLost = QString::fromUtf8("Nie można ukryć tak wiele informacji w pliku docelowym przy wybranych parametrach. \nCzęść danych zostanie utracona.");
};

#endif // TEXTCONSTANTS_H
