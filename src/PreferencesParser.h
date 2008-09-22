#ifndef PREFERENCES_PARSER_H
#define PREFERENCES_PARSER_H 

#include <iostream.h>
#include <qfile.h>
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qxml.h>
#include <stdlib.h>
#include <string.h>
#include "Sequence.h"
#include "Util.h"

class PreferencesParser : public QXmlDefaultHandler {

public:

    PreferencesParser();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& attribs );

    int getQuizLength() const;
    int getActiveRevealingSequenceCount() const;
    Sequence getRevealingSequenceAt( int index );
    QString getInterfaceLanguage() const;
    bool isDigraphEnabled() const;
    bool areQuizButtonsHidden() const;
    bool isAltInTermListShown() const;
    QString getFirstLanguage() const;
    QValueList<QString> getStudyLanguages() const;
    QString getTestLanguage() const;
    QString getLabelsFontFamily() const;
    int getLabelsFontSizeModifier() const;
    QString getFontFamily() const;
    int getFontSizeModifier() const;
    QString getFontOverrideFamily( const QString& language ) const;
    int getFontOverrideSize( const QString& language ) const;
    bool isFontOverrideFamilyDefined( const QString& language ) const;
    bool isFontOverrideSizeDefined( const QString& language ) const;
    bool isLanguageFilterEnabled() const;
    QValueList<int> getClosedFolders() const;
    QMap<int,int> getAccelerators() const;

private:

    int                     quizLength;
    QValueList<Sequence>    sequences;
    QString                 interfaceLanguage;
    bool                    digraphEnabled;
    bool                    quizButtonsHidden;
    bool                    altInTermListShown;
    QString                 firstLanguage;
    QValueList<QString>     studyLanguages;
    QString                 testLanguage;
    QString                 labelsFontFamily;
    int                     labelsFontSizeModifier;
    QString                 fontFamily;
    int                     fontSizeModifier;
    QMap<QString,QString>   fontOverrideFamilies;
    QMap<QString,int>       fontOverrideSizes;
    bool                    languageFilterEnabled;
    QValueList<int>         closedFolders;
    QMap<int,int>           accel;

};

#endif
