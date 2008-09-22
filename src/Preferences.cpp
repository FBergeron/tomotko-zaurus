#include "Preferences.h"

const uint Preferences::fontSizeList[] = { 10, 12, 16, 18, 24, 32, 48, 72 };

Preferences::Preferences() 
    : quizLength( MEDIUM ), interfaceLanguage( QString( "en" ) ), digraphEnabled( false ), quizButtonsHidden( false ), altInTermListShown( false ),
        firstLanguage( QString( "en" ) ), testLanguage( QString( "ja" ) ),
            labelsFontFamily( Util::getDefaultLabelsFontFamily() ), labelsFontSizeModifier( Util::getDefaultLabelsFontSizeModifier() ),
                fontFamily( Util::getDefaultFontFamily() ), fontSizeModifier( Util::getDefaultFontSizeModifier() ),
                    languageFilterEnabled( true ) {
    initDefaultKeyboardAccelerators();

    sequences.clear();
    sequences.append( Sequence( "0+4-2-1+3:off" ) );
    sequences.append( Sequence( "0-2+4-1+3:on" ) );
    sequences.append( Sequence( "2+4-1-0+3:off" ) );

    studyLanguages.clear();
    studyLanguages.append( QString( "en" ) );
    studyLanguages.append( QString( "ja" ) );
}

Preferences::~Preferences() {
}

bool Preferences::load() {
    QFile prefsFile( prefsFilename );
    if( prefsFile.exists() ) {
        if( !prefsFile.open( IO_ReadOnly ) ) {
            cerr << "Cannot open prefs data file: " << prefsFilename << endl;
            return( false );
        }

        QByteArray compressedData( prefsFile.readAll() );
        QByteArray data( Util::qUncompress( compressedData ) );


        QDataStream in( data, IO_ReadOnly );

        Q_UINT32                tempMagicNumber;
        Q_UINT16                tempVersion;

        uint                    tempQuizLength;
        SequenceList            tempSequences;
        int                     tempLanguageFilterEnabledAsInt;
        QString                 tempInterfaceLanguage;
        int                     tempDigraphEnabledAsInt;
        int                     tempQuizButtonsHiddenAsInt;
        int                     tempAltInTermListShownAsInt;
        QString                 tempFirstLanguage;
        QString                 tempTestLanguage;
        QValueList<QString>     tempStudyLanguages;
        QValueList<int>         tempClosedFolders;
        QString                 tempLabelsFontFamily;
        int                     tempLabelsFontSizeModifier;
        QString                 tempFontFamily;
        int                     tempFontSizeModifier;
        QMap<QString,QString>   tempFontOverrideFamilies;
        QMap<QString,int>       tempFontOverrideSizes;
        QMap<int,int>           tempAccel;

        in >> tempMagicNumber >> tempVersion;

        if( tempMagicNumber != Preferences::magicNumber )
            cerr << "Wrong magic number: Incompatible data file for prefs file." << endl;
        if( tempVersion > 0x0011 )
            cerr << "Prefs data file is from a more recent version.  Upgrade toMOTko." << endl;

        in.setVersion( 3 );
        in >> tempQuizLength >> tempSequences;
        in >> tempLanguageFilterEnabledAsInt >> tempInterfaceLanguage >> tempDigraphEnabledAsInt >> tempQuizButtonsHiddenAsInt >> tempAltInTermListShownAsInt;
        in >> tempFirstLanguage >> tempTestLanguage;
        in >> tempStudyLanguages;
        in >> tempClosedFolders; 
        in >> tempLabelsFontFamily >> tempLabelsFontSizeModifier >> tempFontFamily >> tempFontSizeModifier >> tempFontOverrideFamilies >> tempFontOverrideSizes;
        in >> tempAccel;

        prefsFile.close();
    
        quizLength = tempQuizLength;
        sequences = tempSequences;
        languageFilterEnabled = ( tempLanguageFilterEnabledAsInt == 1 );
        interfaceLanguage = tempInterfaceLanguage;
        digraphEnabled = ( tempDigraphEnabledAsInt == 1 );
        quizButtonsHidden = ( tempQuizButtonsHiddenAsInt == 1 );
        altInTermListShown = ( tempAltInTermListShownAsInt == 1 );
        firstLanguage = tempFirstLanguage;
        testLanguage = tempTestLanguage;
        studyLanguages = tempStudyLanguages;
        closedFolders = tempClosedFolders;
        labelsFontFamily = tempLabelsFontFamily;
        labelsFontSizeModifier = tempLabelsFontSizeModifier;
        fontFamily = tempFontFamily;
        fontSizeModifier = tempFontSizeModifier;
        fontOverrideFamilies = tempFontOverrideFamilies;
        fontOverrideSizes = tempFontOverrideSizes;
        accel = tempAccel;
    }
    else {
        QFile prefsXmlFile( prefsXmlFilename );
        if( prefsXmlFile.exists() ) {
            PreferencesParser parser;
            QXmlInputSource source( prefsXmlFile );
            QXmlSimpleReader reader;
            reader.setContentHandler( &parser );
            reader.parse( source );
            setQuizLength( parser.getQuizLength() );
            int seqCount = parser.getActiveRevealingSequenceCount();
            if( seqCount > 0 ) {
                clearRevealingSequences();
                for( int i = 0; i < seqCount; i++ )
                    addRevealingSequence( parser.getRevealingSequenceAt( i ) ); 
            }
            setInterfaceLanguage( parser.getInterfaceLanguage() );
            setDigraphEnabled( parser.isDigraphEnabled() );
            setQuizButtonsHidden( parser.areQuizButtonsHidden() );
            setAltInTermListShown( parser.isAltInTermListShown() );
            setFirstLanguage( parser.getFirstLanguage() );
            QValueList<QString> tempStudyLanguages( parser.getStudyLanguages() );
            if( !tempStudyLanguages.isEmpty() ) {
                studyLanguages.clear();
                for( QValueList<QString>::ConstIterator it = tempStudyLanguages.begin(); it != tempStudyLanguages.end(); it++ ) {
                    QString language(*it);
                    addStudyLanguage( language );
                    if( parser.isFontOverrideFamilyDefined( language ) )
                        setFontOverrideFamily( language, parser.getFontOverrideFamily( language ) );
                    if( parser.isFontOverrideSizeDefined( language ) )
                        setFontOverrideSize( language, parser.getFontOverrideSize( language ) );
                }
            }
            setTestLanguage( parser.getTestLanguage() );
            setLabelsFontFamily( parser.getLabelsFontFamily() );
            setLabelsFontSizeModifier( parser.getLabelsFontSizeModifier() );
            setFontFamily( parser.getFontFamily() );
            setFontSizeModifier( parser.getFontSizeModifier() );
            setLanguageFilterEnabled( parser.isLanguageFilterEnabled() );
            QValueList<int> closedFolders = parser.getClosedFolders();
            for( QValueList<int>::ConstIterator it = closedFolders.begin(); it != closedFolders.end(); it++ )
                setFolderOpen( *it, false );
            QMap<int,int> accels = parser.getAccelerators();
            for( QMap<int,int>::ConstIterator it = accels.begin(); it != accels.end(); it++ )
                setAccelerator( (Action)it.key(), it.data() );
        }
    }
    return( true );
}

bool Preferences::save() {
    QByteArray data;

    QDataStream out( data, IO_WriteOnly );
    out.setVersion( 3 /* QDataStream::Qt_3 ? */ );

    // 0x0011 means 0.11.x version.
    out << Q_UINT32( Preferences::magicNumber ) << Q_UINT16( 0x0011 );

    out << quizLength << sequences;
    int languageFilterEnabledAsInt = ( languageFilterEnabled ? 1 : 0 );
    int digraphEnabledAsInt = ( digraphEnabled ? 1 : 0 ); 
    int quizButtonsHiddenAsInt = ( quizButtonsHidden ? 1 : 0 );
    int altInTermListShownAsInt = ( altInTermListShown ? 1 : 0 );
    out << languageFilterEnabledAsInt << interfaceLanguage << digraphEnabledAsInt << quizButtonsHiddenAsInt << altInTermListShownAsInt;
    out << firstLanguage << testLanguage;
    out << studyLanguages;
    out << closedFolders;
    out << labelsFontFamily << labelsFontSizeModifier << fontFamily << fontSizeModifier << fontOverrideFamilies << fontOverrideSizes;
    out << accel;

    QByteArray compressedData( Util::qCompress( data ) ); 

    QFile dataFile( prefsFilename );
    QFileInfo dataFileInfo( dataFile );

    if( !Util::makeDirectory( dataFileInfo.dirPath() ) )
        return( false );

    if( !dataFile.open( IO_WriteOnly ) )
        return( false );

    int ret = dataFile.writeBlock( compressedData );
    dataFile.close();

    if( ret == -1 || dataFile.status() != IO_Ok ) {
        dataFile.resetStatus();
        return( false );
    }

    return( true );
}

uint Preferences::getQuizLength() const {
    return( quizLength );
}

void Preferences::setQuizLength( uint quizLength ) {
    this->quizLength = quizLength;
}

void Preferences::clearRevealingSequences() {
    sequences.clear();
}

void Preferences::addRevealingSequence( Sequence sequence ) {
    sequences.append( sequence );
}

int Preferences::getRevealingSequence( const QString& seqStr ) {
    int i = 0; 
    for( SequenceList::ConstIterator it = sequences.begin(); it != sequences.end(); it++, i++ ) {
        Sequence seq = *it;
        if( seq.toHumanReadableString() == seqStr )
            return( i );
    }
    return( -1 );
}

void Preferences::removeRevealingSequence( int index ) {
    sequences.remove( sequences.at( index ) );
}

void Preferences::removeRevealingSequence( const QString& seqStr ) {
    for( SequenceList::Iterator it = sequences.begin(); it != sequences.end(); it++ ) {
        Sequence seq = *it;
        if( seq.toHumanReadableString() == seqStr )
            sequences.remove( it );
    }
}

int Preferences::getActiveRevealingSequenceCount() {
    int count = 0;
    for( SequenceList::Iterator it = sequences.begin(); it != sequences.end(); it++ ) {
        Sequence seq = *it;
        if( seq.isEnabled() )
            count++;
    }
    return( count );
}

int Preferences::getRevealingSequenceCount() const {
    return( sequences.count() );
}

Sequence Preferences::getRevealingSequenceAt( int index ) {
    return( sequences[ index ] );
}

QString Preferences::getInterfaceLanguage() const {
    return( interfaceLanguage );
}

void Preferences::setInterfaceLanguage( const QString& interfaceLanguage ) {
    this->interfaceLanguage = interfaceLanguage;
}

bool Preferences::isDigraphEnabled() const {
    return( digraphEnabled );
}

void Preferences::setDigraphEnabled( bool isEnabled ) {
    this->digraphEnabled = isEnabled;
}

bool Preferences::areQuizButtonsHidden() const {
    return( quizButtonsHidden );
}

void Preferences::setQuizButtonsHidden( bool areHidden ) {
    this->quizButtonsHidden = areHidden;
}

bool Preferences::isAltInTermListShown() const {
    return( altInTermListShown );
}

void Preferences::setAltInTermListShown( bool isShown ) {
    this->altInTermListShown = isShown;
}

QString Preferences::getFirstLanguage() const {
    return( firstLanguage );
}

void Preferences::setFirstLanguage( const QString& firstLanguage ) {
    this->firstLanguage = firstLanguage;
}

QValueList<QString> Preferences::getStudyLanguages() {
    return( studyLanguages );
}

bool Preferences::isStudyLanguage( const QString& language ) {
    return( studyLanguages.contains( language ) );
}

void Preferences::addStudyLanguage( const QString& language ) {
    studyLanguages.append( language );
}

void Preferences::removeStudyLanguage( const QString& language ) {
    studyLanguages.remove( language );
}

void Preferences::clearStudyLanguages() {
    studyLanguages.clear();
}

QString Preferences::getTestLanguage() const {
    return( testLanguage );
}

void Preferences::setTestLanguage( const QString& testLanguage ) {
    this->testLanguage = testLanguage;
}

QString Preferences::getFontFamily() const {
    return( fontFamily );
}

void Preferences::setFontFamily( const QString& fontFamily ) {
    this->fontFamily = fontFamily;
}

QString Preferences::getDefaultFontFamily() const {
    return( Util::getDefaultFontFamily() );
}

int Preferences::getDefaultFontSizeModifier() const {
    return( Util::getDefaultFontSizeModifier() );
}

int Preferences::getFontSizeModifier() const {
    return( fontSizeModifier );
}

void Preferences::setFontSizeModifier( int fontSizeModifier ) {
    this->fontSizeModifier = fontSizeModifier;
}

QFont Preferences::getLargeFont( const QString& language = QString::null ) const {
    QString family( isFontOverrideFamilyDefined( language ) ? getFontOverrideFamily( language ) : fontFamily );
    int sizeModif( isFontOverrideSizeDefined( language ) ? getFontOverrideSize( language ) : getFontSizeModifier() );
    return( getFont( family, fontSizeList[ 4 + sizeModif ] ) );
}

QFont Preferences::getMediumFont( const QString& language = QString::null ) const {
    QString family( isFontOverrideFamilyDefined( language ) ? getFontOverrideFamily( language ) : fontFamily );
    int sizeModif( isFontOverrideSizeDefined( language ) ? getFontOverrideSize( language ) : getFontSizeModifier() );
    return( getFont( family, fontSizeList[ 3 + sizeModif ] ) );
}

QFont Preferences::getSmallFont( const QString& language = QString::null ) const {
    QString family( isFontOverrideFamilyDefined( language ) ? getFontOverrideFamily( language ) : fontFamily );
    int sizeModif( isFontOverrideSizeDefined( language ) ? getFontOverrideSize( language ) : getFontSizeModifier() );
    return( getFont( family, fontSizeList[ 2 + sizeModif ] ) );
}

QFont Preferences::getBestFont( const QString& lang1, const QString& lang2 ) const {
    if( lang1 == QString( "en" ) )
        return( getMediumFont( lang2 ) );
    else if( lang2 == QString( "en" ) )
        return( getMediumFont( lang1 ) );
    else {
        QFont font1( getMediumFont( lang1 ) );
        QFont font2( getMediumFont( lang2 ) );
        if( font1.family() == font2.family() ) {
            int pointSize = font1.pointSize() > font2.pointSize() ? font1.pointSize() : font2.pointSize();
            return( QFont( font1.family(), pointSize ) );
        }
        else
            return( getMediumFont() );
    }
}

QString Preferences::getLabelsFontFamily() const {
    return( labelsFontFamily );
}

void Preferences::setLabelsFontFamily( const QString& labelsFontFamily ) {
    this->labelsFontFamily = labelsFontFamily;
}

QString Preferences::getDefaultLabelsFontFamily() const {
    return( Util::getDefaultLabelsFontFamily() );
}

int Preferences::getDefaultLabelsFontSizeModifier() const {
    return( Util::getDefaultLabelsFontSizeModifier() );
}

int Preferences::getLabelsFontSizeModifier() const {
    return( labelsFontSizeModifier );
}

void Preferences::setLabelsFontSizeModifier( int labelsFontSizeModifier ) {
    this->labelsFontSizeModifier = labelsFontSizeModifier;
}

QFont Preferences::getLabelsFont() const {
    return( getFont( labelsFontFamily, fontSizeList[ 3 + getLabelsFontSizeModifier() ] ) );
}

QString Preferences::getFontOverrideFamily( const QString& language ) const {
    return( fontOverrideFamilies[ language ] );
}

int Preferences::getFontOverrideSize( const QString& language ) const {
    return( fontOverrideSizes[ language ] );
}

void Preferences::setFontOverrideFamily( const QString& language, const QString& fontFamily ) {
    fontOverrideFamilies[ language ] = fontFamily;
}

void Preferences::setFontOverrideSize( const QString& language, int fontSize ) {
    fontOverrideSizes[ language ] = fontSize;
}

bool Preferences::isFontOverrideFamilyDefined( const QString& language ) const {
    return( fontOverrideFamilies.contains( language ) );
}

bool Preferences::isFontOverrideSizeDefined( const QString& language ) const {
    return( fontOverrideSizes.contains( language ) );
}

void Preferences::clearFontOverrideFamilies() {
    fontOverrideFamilies.clear();
}

void Preferences::clearFontOverrideSizes() {
    fontOverrideSizes.clear();
}

void Preferences::setLanguageFilterEnabled( bool isEnabled ) {
    languageFilterEnabled = isEnabled;
}

bool Preferences::isLanguageFilterEnabled() const {
    return( languageFilterEnabled );
}

bool Preferences::isFolderOpen( int folderId ) const {
    return( !closedFolders.contains( folderId ) );
}

void Preferences::setFolderOpen( int folderId, bool isOpen ) {
    if( isOpen )
        closedFolders.remove( folderId );
    else {
        if( !closedFolders.contains( folderId ) ) 
            closedFolders.append( folderId );
    }
}

void Preferences::setApplicationDirName( const QString& applDir ) {
    prefsXmlFilename = applDir + QString( "/prefs.xml" );
    prefsFilename = applDir + QString( "/prefs.dat.z" );
}

int Preferences::getDefaultAccelerator( Action action ) {
    return( defaultAccel[ action ] );
}

void Preferences::setAccelerator( Action action, int newAccel ) {
    if( defaultAccel[ action ] == newAccel ) {
        if( accel.contains( action ) )
            accel.remove( action );
    }
    else
        accel[ action ] = newAccel;
}

int Preferences::getAccelerator( Action action ) {
    if( accel.contains( action ) )
        return( accel[ action ] );
    else
        return( getDefaultAccelerator( action ) );
}

void Preferences::initDefaultKeyboardAccelerators() {
    defaultAccel[ ACTION_REVEAL ] = Key_Space;
    defaultAccel[ ACTION_RIGHT_ANSWER ] = Key_Return;
    defaultAccel[ ACTION_WRONG_ANSWER ] = Key_Backspace;
    defaultAccel[ ACTION_EDIT_QUIZ_TERM ] = CTRL + Key_Return;
    defaultAccel[ ACTION_START_QUIZ ] = CTRL + Key_S;
    defaultAccel[ ACTION_MANAGE_GLOSSARIES ] = CTRL + Key_M;
    defaultAccel[ ACTION_IMPORT ] = 0;
    defaultAccel[ ACTION_EXPORT ] = 0;
    defaultAccel[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ] = 0;
    defaultAccel[ ACTION_PREFERENCES ] = CTRL + Key_P;
    defaultAccel[ ACTION_QUIT ] = CTRL + Key_Q;
    defaultAccel[ ACTION_ADD_FOLDER ] = CTRL + Key_F;
    defaultAccel[ ACTION_ADD_GLOSSARY ] = CTRL + Key_G;
    defaultAccel[ ACTION_REMOVE_ITEM ] = CTRL + Key_BackSpace;
    defaultAccel[ ACTION_ADD_TERM ] = CTRL + Key_W;
    defaultAccel[ ACTION_EDIT_TERM ] = CTRL + Key_E;
    defaultAccel[ ACTION_REMOVE_TERMS ] = CTRL + Key_D;
    defaultAccel[ ACTION_CHECK_ALL_TERMS ] = CTRL + Key_A;
    defaultAccel[ ACTION_INVERSE_CHECKED_TERMS ] = CTRL + Key_I;
    defaultAccel[ ACTION_MAXIMIZE ] = CTRL + Key_Space;
    defaultAccel[ ACTION_SEARCH ] = CTRL + Key_Comma;
}

QFont Preferences::getFont( const QString& fontFamily, uint size ) const {
    return( QFont( fontFamily, size, QFont::Normal, false, QFont::Unicode ) );
}

