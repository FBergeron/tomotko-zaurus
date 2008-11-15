#include "PreferencesParser.h"

PreferencesParser::PreferencesParser() :
    quizLength( Util::getDefaultQuizLength() ), interfaceLanguage( QString( "en" ) ), digraphEnabled( false ), 
        quizButtonsHidden( false ), altInTermListShown( false ),
            firstLanguage( QString( "en" ) ), testLanguage( QString( "ja" ) ), 
                labelsFontFamily( Util::getDefaultLabelsFontFamily() ), labelsFontSizeModifier( Util::getDefaultLabelsFontSizeModifier() ),
                    fontFamily( Util::getDefaultFontFamily() ), fontSizeModifier( Util::getDefaultFontSizeModifier() ), 
                        languageFilterEnabled( true ) {
}

bool PreferencesParser::startElement( const QString&, const QString&, const QString& qname, const QXmlAttributes& attribs ) {
    if( qname == QString( "preferences" ) ) {
        bool isQuizLengthValid;
        int tempQuizLength = attribs.value( QString( "quizLength" ) ).toInt( &isQuizLengthValid, 10 );
        if( isQuizLengthValid )
            quizLength = tempQuizLength;

        // We ignore the revealSeq attribute voluntarily.  This way, the revealing sequences will contain the intial values.

        QString tempLanguageFilterEnabled = attribs.value( QString( "languageFilteredEnabled" ) );
        if( QString( "false" ) == tempLanguageFilterEnabled ) 
            languageFilterEnabled = false;

        QString tempInterfaceLanguage = attribs.value( QString( "interfaceLanguage" ) );
        if( tempInterfaceLanguage )
            interfaceLanguage = tempInterfaceLanguage;

        QString tempDigraphEnabled = attribs.value( QString( "digraph" ) );
        if( QString( "true" ) == tempDigraphEnabled )
            digraphEnabled = true;

        QString tempQuizButtonsHidden = attribs.value( QString( "quizButtonsHidden" ) );
        if( QString( "true" ) == tempQuizButtonsHidden )
            quizButtonsHidden = true;

        QString tempAltInTermListShown = attribs.value( QString( "altInTermListShown" ) );
        if( QString( "true" ) == tempAltInTermListShown )
            altInTermListShown = true;

        QString tempFirstLanguage = attribs.value( QString( "firstLanguage" ) );
        if( tempFirstLanguage )
            firstLanguage = ( tempFirstLanguage.length() == 0 ? QString::null : tempFirstLanguage );

        QString tempTestLanguage = attribs.value( QString( "testLanguage" ) );
        if( tempTestLanguage )
            testLanguage = ( tempTestLanguage.length() == 0 ? QString::null : tempTestLanguage );

        QStringList studyLanguageList( QStringList::split( QString( "," ), attribs.value( QString( "studyLanguages" ) ) ) );
        for( QStringList::ConstIterator it = studyLanguageList.begin(); it != studyLanguageList.end(); it++ ) {
            QString language( *it );
            studyLanguages.append( language );
        }

        QStringList closedFolderList( QStringList::split( QString( "," ), attribs.value( QString( "closedFolders" ) ) ) );
        for( QStringList::ConstIterator it = closedFolderList.begin(); it != closedFolderList.end(); it++ ) {
            bool isFolderIdValid;
            int tempFolderId = (*it).toInt( &isFolderIdValid, 10 );
            if( isFolderIdValid )
                closedFolders.append( tempFolderId );
        }

        QString tempLabelsFontFamily = attribs.value( QString( "labelsFontName" ) );
        if( tempLabelsFontFamily )
            labelsFontFamily = tempLabelsFontFamily;

        bool isLabelsFontSizeModifierValid;
        int tempLabelsFontSizeModifier = attribs.value( QString( "labelsFontSize" ) ).toInt( &isLabelsFontSizeModifierValid, 10 );
        if( isLabelsFontSizeModifierValid )
            labelsFontSizeModifier = tempLabelsFontSizeModifier;

        QString tempFontFamily = attribs.value( QString( "fontName" ) );
        if( tempFontFamily )
            fontFamily = tempFontFamily;

        bool isFontSizeModifierValid;
        int tempFontSizeModifier = attribs.value( QString( "fontSize" ) ).toInt( &isFontSizeModifierValid, 10 );
        if( isFontSizeModifierValid )
            fontSizeModifier = tempFontSizeModifier;
    }
    else if( qname == QString( "fontOverride" ) ) {
        QString tempLang( attribs.value( QString( "language" ) ) );

        if( tempLang ) {
            QString tempFontFamily = attribs.value( QString( "fontName" ) );
            if( tempFontFamily )
                fontOverrideFamilies[ tempLang ] = tempFontFamily;

            bool isFontSizeModifierValid;
            int tempFontSizeModifier = attribs.value( QString( "fontSize" ) ).toInt( &isFontSizeModifierValid, 10 );
            if( isFontSizeModifierValid )
                fontOverrideSizes[ tempLang ] = tempFontSizeModifier; 
        }
    }
    else if( qname == QString( "accel" ) ) {
        QString tempActionId = attribs.value( QString( "action" ) );

        bool isActionValid = false;
        int tempAction = -1;
        for( int i = 0; i < ACTION_COUNT; i++ ) {
            if( tempActionId == actionId[ i ] ) {
                isActionValid = true;
                tempAction = i;
                break;
            }
        }
        if( isActionValid ) {
            bool isKeyValid;
            int tempKey = attribs.value( QString( "key" ) ).toInt( &isKeyValid, 10 );

            if( isKeyValid )
                accel[ tempAction ] = tempKey;
        }
    }
    return( true );
}

int PreferencesParser::getQuizLength() const {
    return( quizLength );
}

int PreferencesParser::getRevealingSequenceCount() const {
    return( sequences.count() );
}

Sequence PreferencesParser::getRevealingSequenceAt( int index ) {
    return( sequences[ index ] );
}

QString PreferencesParser::getInterfaceLanguage() const {
    return( interfaceLanguage );
}

bool PreferencesParser::isDigraphEnabled() const {
    return( digraphEnabled );
}

bool PreferencesParser::areQuizButtonsHidden() const {
    return( quizButtonsHidden );
}

bool PreferencesParser::isAltInTermListShown() const {
    return( altInTermListShown );
}

QString PreferencesParser::getFirstLanguage() const {
    return( firstLanguage );
}

QValueList<QString> PreferencesParser::getStudyLanguages() const {
    return( studyLanguages );
}

QString PreferencesParser::getTestLanguage() const {
    return( testLanguage );
}

QString PreferencesParser::getLabelsFontFamily() const {
    return( labelsFontFamily );
}

int PreferencesParser::getLabelsFontSizeModifier() const {
    return( labelsFontSizeModifier );
}

QString PreferencesParser::getFontFamily() const {
    return( fontFamily );
}

int PreferencesParser::getFontSizeModifier() const {
    return( fontSizeModifier );
}

QString PreferencesParser::getFontOverrideFamily( const QString& language ) const {
    return( fontOverrideFamilies[ language ] );
}

int PreferencesParser::getFontOverrideSize( const QString& language ) const {
    return( fontOverrideSizes[ language ] );
}

bool PreferencesParser::isFontOverrideFamilyDefined( const QString& language ) const {
    return( fontOverrideFamilies.contains( language ) );
}

bool PreferencesParser::isFontOverrideSizeDefined( const QString& language ) const {
    return( fontOverrideSizes.contains( language ) );
}

bool PreferencesParser::isLanguageFilterEnabled() const {
    return( languageFilterEnabled );
}

QValueList<int> PreferencesParser::getClosedFolders() const {
    return( closedFolders );
}

QMap<int,int> PreferencesParser::getAccelerators() const {
    return( accel );
}
