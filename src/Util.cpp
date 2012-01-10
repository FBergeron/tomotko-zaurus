#include "Util.h"

const QString Util::languageCodeList[] = { 
    QObject::tr( "en" ), QObject::tr( "fr" ), QObject::tr( "es" ), QObject::tr( "ja" ), QObject::tr( "de" ), 
    QObject::tr( "ar" ), QObject::tr( "bg" ), QObject::tr( "cs" ), QObject::tr( "da" ), QObject::tr( "el" ), 
    QObject::tr( "eo" ), QObject::tr( "fi" ), QObject::tr( "he" ), QObject::tr( "hi" ), QObject::tr( "it" ), 
    QObject::tr( "ko" ), QObject::tr( "la" ), QObject::tr( "no" ), QObject::tr( "nl" ), QObject::tr( "pt" ), QObject::tr( "rm" ),
    QObject::tr( "ro" ), QObject::tr( "ru" ), QObject::tr( "sv" ), QObject::tr( "sw" ), QObject::tr( "th" ), 
    QObject::tr( "tl" ), QObject::tr( "vi" ), QObject::tr( "zh" )
};

QMap<QString,QString>* Util::digraphMap = 0;

const QString& Util::getLanguageCode( const QString& language ) {
    int languageCodeListLength = sizeof( languageCodeList ) / sizeof( QString );
    for( int i = 0; i < languageCodeListLength; i++ ) {
        if( QObject::tr( languageCodeList[ i ] ) == language ) 
            return( languageCodeList[ i ] );
    }
    return( QString::null ); // Should never happen.
}

QString Util::getWeekday( int weekday ) {
    switch( weekday ) {
        case 1 : return( QObject::tr( "Monday" ) );
        case 2 : return( QObject::tr( "Tuesday" ) );
        case 3 : return( QObject::tr( "Wednesday" ) );
        case 4 : return( QObject::tr( "Thursday" ) );
        case 5 : return( QObject::tr( "Friday" ) );
        case 6 : return( QObject::tr( "Saturday" ) );
        case 7 : return( QObject::tr( "Sunday" ) );
    }
    return( QString::null ); // Should never happen but just in case.
}

QString Util::getDefaultLabelsFontFamily() {
    return( getDefaultFontFamily() );
}

int Util::getDefaultLabelsFontSizeModifier() {
    return( 0 );
}

QString Util::getDefaultFontFamily() {
    QFontDatabase fontDatabase;
    QStringList families = fontDatabase.families( false );
    int unifontCount = families.contains( QString( "unifont" ) );
    int lcfontCount = families.contains( QString( "lcfont" ) );
    if( unifontCount > 0 )
        return( QString( "unifont" ) );
    else if( lcfontCount > 0 )
        return( QString( "lcfontCount" ) );
    else
        return( QFont::defaultFont().family() );
}

int Util::getDefaultFontSizeModifier() {
    return( 0 );
}

QString Util::getDateAsShortString( const QDate& date ) {
    return( QString::number( date.year() ) + QString( "-" ) + 
        Util::padWithLeadingZero( date.month() ) + QString( "-" ) + 
            Util::padWithLeadingZero( date.day() ) );
}

QString Util::padWithLeadingZero( const int& number ) {
    return( number < 10 ? QString( "0" ) + QString::number( number ) : QString::number( number ) );
}

int Util::getDefaultQuizLength() {
    return( 1 );
}

// Taken from Qt-4.4.3's QUuid class and modified a bit.
QUuid Util::createUuid() {
    static const int intbits = sizeof(int)*8;
    static int randbits = 0;
    if (!randbits) {
        int max = RAND_MAX;
        do { ++randbits; } while ((max=max>>1));
        //qsrand((uint)QDateTime::currentDateTime().toTime_t());
        //qrand(); // Skip first
        srand( QDateTime::currentDateTime().time().msec() ); // Not as good as the original but probably good enough. 
        rand(); // Skip first
    }

    QUuid result;
    //uint *data = &(result.data1);
    ulong *data = &(result.data1);
    int chunks = 16 / sizeof(uint);
    while (chunks--) {
        uint randNumber = 0;
        for (int filled = 0; filled < intbits; filled += randbits) {
            // randNumber |= qrand()<<filled;
            randNumber |= rand()<<filled;
        }
        *(data+chunks) = randNumber;
    }

    result.data4[0] = (result.data4[0] & 0x3F) | 0x80;        // UV_DCE
    result.data3 = (result.data3 & 0x0FFF) | 0x4000;        // UV_Random

    return result;
}

QString Util::escapeXml( QString xml ) {
    QString escapedStr;
    for( uint i = 0; i < xml.length(); i++ ) {
        if( xml[ i ] == QChar( '&' ) )
            escapedStr.append( "&amp;" );
        else if( xml[ i ] == QChar( '<' ) )
            escapedStr.append( "&lt;" );
        else if( xml[ i ] == QChar( '>' ) )
            escapedStr.append( "&gt;" );
        else
            escapedStr.append( xml[ i ] );
    }
    return( escapedStr );
}

QString Util::term2Xml( const Term& term, QStringList& exportedTransUidList, QStringList* languages, uint indentLevel = 0 ) {
    QString indent;
    for( uint i = 0; i < indentLevel; i++ )
        indent += QString( "\t" );

    QString xml;
    QTextStream ts( &xml, IO_WriteOnly );
    ts << indent << QString( "<term uid=\"" ) << term.getUid().toString() << QString( "\"" );
    if( term.getImagePath() != QString::null ) {
        QString fileExtension = term.getImagePath().right( 4 );
        QString imageFilename = term.getUid().toString() + fileExtension;
        ts << QString( " imagePath=\"" ) << imageFilename << QString( "\"" );
    }
    ts << QString( ">" ) << endl;
    for( Term::TranslationMap::ConstIterator it = term.translationsBegin(); it != term.translationsEnd(); it++ ) {
        const Translation& translation = it.data();
        if( !languages || languages->contains( translation.getLanguage() ) ) {
            ts << indent << QString( "\t<trans uid=\"" ) << translation.getUid().toString() << QString( "\" lang=\"" ) << translation.getLanguage() << QString( "\">" ) << endl;
            if( !exportedTransUidList.contains( translation.getUid().toString() ) )
                exportedTransUidList.append( translation.getUid().toString() );
            if( !translation.getWord().isEmpty() )
                ts << indent << QString( "\t\t<word>" ) << Util::escapeXml( translation.getWord() ) << QString( "</word>" ) << endl;
            if( !translation.getAlt().isEmpty() )
                ts << indent << QString( "\t\t<alt>" ) << Util::escapeXml( translation.getAlt() ) << QString( "</alt>" ) << endl;
            ts << indent << QString( "\t</trans>" ) << endl;
        }
    }
    for( Term::CommentMap::ConstIterator it = term.commentsBegin(); it != term.commentsEnd(); it++ ) {
        const BilingualKey& key = it.key();
        const Comment& comment = it.data();
        if( !comment.isEmpty() ) {
            if( !languages || ( languages->contains( key.getFirstLanguage() ) && languages->contains( key.getSecondLanguage() ) ) )
                ts << indent << "\t<comment uid=\"" << comment.getUid().toString() << "\" languages=\"" << key.toString() << "\">" << Util::escapeXml( comment.getText() ) << "</comment>" << endl;
        }
    }
    ts << indent << QString( "</term>" ) << endl;
    return( xml );
}

bool Util::makeDirectory( const QString& dir ) {
    QDir d( dir );
    QDir absDir( d.absPath() );
    if( absDir.exists() )
        return( true );

    QStringList subDirs = QStringList::split( QString( "/" ), d.absPath() );
    for( uint i = 0; i < subDirs.count(); i++ ) {
        QString tempDirStr( "" );
        for( uint j = 0; j <= i; j++ ) 
            tempDirStr = tempDirStr + QString( "/" ) + subDirs[ j ];
        QDir tempDir( tempDirStr );
        if( !tempDir.exists() ) {
            if( !tempDir.mkdir( tempDir.path() ) )
                return( false );
        }
    }
    return( true );
}

bool Util::copy( const QString& fromFile, const QString& toFile ) {
    //cerr << "copy " << fromFile << " to " << toFile << endl;
    QFileInfo srcInfo( fromFile );
    QFileInfo destInfo( toFile );

    // If the source and destination are the same, we don't need to perform the copy.
    if( srcInfo.absFilePath() == destInfo.absFilePath() ) 
        return( true );

    QFile src( fromFile );
    if( !src.open( IO_ReadOnly ) ) {
        cerr << "Cannot open src file " << fromFile << " when performing copy." << endl;
        return( false );
    }
    QFile dest( toFile );
    if( !dest.open( IO_WriteOnly ) ) {
        src.close();
        cerr << "Cannot open dest file " << fromFile << " when performing copy." << endl;
        return( false );
    }
    bool isOk = false;
    
    uInt bufferSize = 8192; // Arbitrary size for now.
    char* buffer = (char*)malloc( bufferSize ); 
    int bytesRead;
    while( ( bytesRead = src.readBlock( buffer, bufferSize ) ) > 0 ) {
        int bytesWritten = dest.writeBlock( buffer, bytesRead );
        if( bytesRead != bytesWritten ) {
            cerr << "Read and written bytes count differs (" << bytesRead << " vs " << bytesWritten << ") when performing copy." << endl;
            break;
        }
    }
    if( bytesRead == 0 )
        isOk = true;
    delete( buffer );
    src.close();
    dest.close();
    return( isOk );
}

bool Util::deleteDirectory( const QString& dir ) {
    QDir d( dir );
    QDir absDir( d.absPath() );
    if( !absDir.exists() )
        return( false );

    QStringList entries = absDir.entryList();
    for( QStringList::Iterator it = entries.begin(); it != entries.end(); it++ ) {
        QString entry = *it;
        QString entryPath = absDir.path() + "/" + entry;
        QFileInfo info( entryPath );
        if( info.isDir() ) {
            if( entry != "." && entry != ".." ) {
                if( !deleteDirectory( entryPath ) )
                    return( false );
            }
        }
        else if( info.isFile() ) {
            QFile f( entryPath );
            if( !f.remove() )
                return( false );
        }
    }

    return( absDir.rmdir( d.absPath() ) );
}

QString Util::getDigraph( const QString& buffer ) {
    if( !digraphMap ) 
        initDigraphs();
    return( digraphMap->contains( buffer ) ? (*digraphMap)[ buffer ] : QString::null );
}

void Util::initDigraphs() {
    cerr << "initDigraphs" << endl;
    digraphMap = new QMap<QString, QString>();

    QString digraphFilename( "/opt/Qtopia/lib/toMOTko/digraphs.conf" );
    QFile digraphFile( digraphFilename );

    if( !digraphFile.open( IO_ReadOnly ) ) {
        cerr << "Could not open /opt/Qtopia/lib/toMOTko/digraphs.conf" << endl;
        return;
    }

    QTextStream ts( &digraphFile );
    while ( !ts.eof() ) {
        QString line = ts.readLine();
        if( line.startsWith( "#" ) || line.length() == 0 )
            continue;
        int indexOfFirstWhitespace = line.find( " " );
        QString digraph = line.left( 2 );
        QString strValue = line.mid( 5, indexOfFirstWhitespace == 9 ? 4 : 2 );
        if( !digraph.isNull() && !strValue.isNull() ) {
            bool isOk;
            uint value = strValue.toUInt( &isOk, 16 );
            if( isOk )
                (*digraphMap)[ digraph ] = QString( QChar( value ) );
        }
    }

    digraphFile.close();
}

QAction* Util::createAction( const QString& name, const char** icon, QObject* receiver, const char* slot, int defaultKey = -1, bool toggle = false ) {
    QAction *a;
    if( icon == NULL )
        a = new QAction( name, name, 0, receiver, 0, toggle );
    else
        a = new QAction( name, QIconSet( ZPIXMAP( icon ) ), name, 0, receiver, 0, toggle );
    if( toggle )
        QObject::connect( a, SIGNAL( toggled( bool ) ), receiver, slot );
    else
        QObject::connect( a, SIGNAL( activated() ), receiver, slot );
    if( defaultKey != -1 )
        a->setAccel( defaultKey );
    return a;
}

QString Util::describeKey( int key ) {
    QString ks;
    if( (key & Qt::CTRL) != 0 )
        ks += "Ctrl+";
    if( (key & Qt::ALT) != 0 )
        ks += "Alt+";
    if( (key & Qt::SHIFT) != 0)
        ks += "Shift+";

    key = key & ~Qt::MODIFIER_MASK;

    if (key == 0x20)
        ks += "Space";
    else if (0x21 <= key && key <= 0xff) {
        char st[2];
        st[0] = key;
        st[1] = 0;
        ks += st;
    }
    else if (0x1030 <= key && key <= 0x1052)
        ks += QString("f") + QChar(key - 0x1030 + 1);
    else {
        switch (key) {
            case Qt::Key_Escape:
                ks += "Esc";
                break;
            case Qt::Key_Tab:
                ks += "Tab";
                break;
            case Qt::Key_Backtab:
                ks += "Backtab";
                break;
            case Qt::Key_Backspace:
                ks += "Backspace";
                break;
            case Qt::Key_Return:
                ks += "Return";
                break;
            case Qt::Key_Enter:
                ks += "Enter";
                break;
            case Qt::Key_Insert:
                ks += "Insert";
                break;
            case Qt::Key_Delete:
                ks += "Delete";
                break;
            case Qt::Key_Pause:
                ks += "Pause";
                break;
            case Qt::Key_Print:
                ks += "Print";
                break;
            case Qt::Key_SysReq:
                ks += "SysReq";
                break;
            case Qt::Key_Home:
                ks += "Home";
                break;
            case Qt::Key_End:
                ks += "End";
                break;
            case Qt::Key_Left:
                ks += "Left";
                break;
            case Qt::Key_Up:
                ks += "Up";
                break;
            case Qt::Key_Right:
                ks += "Right";
                break;
            case Qt::Key_Down:
                ks += "Down";
                break;
            case Qt::Key_PageUp:
                ks += "PageUp";
                break;
            case Qt::Key_PageDown:
                ks += "PageDown";
                break;
            case Qt::Key_Super_L:
                ks += "Super_L";
                break;
            case Qt::Key_Super_R:
                ks += "Super_R";
                break;
            case Qt::Key_Menu:
                ks += "Menu";
                break;
            case Qt::Key_Hyper_L:
                ks += "Hyper_L";
                break;
            case Qt::Key_Hyper_R:
                ks += "Hyper_R";
                break;
            case Qt::Key_Help:
                ks += "Help";
                break;
            default:
                ks += "<" + QString::number( key, 16 ) + ">";
                break;
        }
    }
    return ks;
}

QByteArray Util::qCompress( const QByteArray& byteArray, int compressionLevel ) {
    const uchar* data = (const uchar*)(byteArray.data());
    int nbytes = byteArray.size();

    if (nbytes == 0) {
        QByteArray a( 4 );
        a.fill( '\0' );
        return( a );
    }
    if (!data) {
        qWarning("qCompress: Data is null");
        return QByteArray();
    }
    if (compressionLevel < -1 || compressionLevel > 9)
        compressionLevel = -1;

    ulong len = nbytes + nbytes / 100 + 13;
    QByteArray bazip;
    int res;
    do {
        bazip.resize(len + 4);
        res = ::compress2((uchar*)bazip.data()+4, &len, (uchar*)data, nbytes, compressionLevel);

        switch (res) {
        case Z_OK:
            bazip.resize(len + 4);
            bazip[0] = (nbytes & 0xff000000) >> 24;
            bazip[1] = (nbytes & 0x00ff0000) >> 16;
            bazip[2] = (nbytes & 0x0000ff00) >> 8;
            bazip[3] = (nbytes & 0x000000ff);
            break;
        case Z_MEM_ERROR:
            qWarning("qCompress: Z_MEM_ERROR: Not enough memory");
            bazip.resize(0);
            break;
        case Z_BUF_ERROR:
            len *= 2;
            break;
        }
    } while (res == Z_BUF_ERROR);

    return bazip;
}

QByteArray Util::qUncompress( const QByteArray& byteArray ) {
    const uchar* data = (const uchar*)(byteArray.data());
    int nbytes = byteArray.size();

    if (!data) {
        qWarning("qUncompress: Data is null");
        return QByteArray();
    }
    if (nbytes <= 4) {
        if (nbytes < 4 || (data[0]!=0 || data[1]!=0 || data[2]!=0 || data[3]!=0))
            qWarning("qUncompress: Input data is corrupted");
        return QByteArray();
    }
    ulong expectedSize = (data[0] << 24) | (data[1] << 16) | (data[2] <<  8) | (data[3] );
    //qMax(expectedSize, 1ul);
    ulong len = ( expectedSize > 1ul ? expectedSize : 1ul );
    QByteArray baunzip;
    int res;
    do {
        baunzip.resize(len);
        res = ::uncompress((uchar*)baunzip.data(), &len, (uchar*)data+4, nbytes-4);

        switch (res) {
        case Z_OK:
            if ((uint)len != baunzip.size())
                baunzip.resize(len);
            break;
        case Z_MEM_ERROR:
            qWarning("qUncompress: Z_MEM_ERROR: Not enough memory");
            break;
        case Z_BUF_ERROR:
            len *= 2;
            break;
        case Z_DATA_ERROR:
            qWarning("qUncompress: Z_DATA_ERROR: Input data is corrupted");
            break;
        }
    } while (res == Z_BUF_ERROR);

    if (res != Z_OK)
        baunzip = QByteArray();

    return baunzip;
}

QString Util::fromUnicodeToUtf8( ushort unicode ) {
    if( unicode <= 0x7f )
        return( QString::number( unicode, 16 ) );
    else if( unicode <= 0x07ff ) {
        ushort secondByte = 0x80 | ( unicode & 0x3f );
        ushort firstByte = 0xc0 | ( unicode >> 6 );
        return( QString::number( firstByte, 16 ) + QString::number( secondByte, 16 ) );
    }
    else {
        ushort thirdByte = ( unicode & 0x3f ) | 0x80;
        ushort secondByte = ( ( unicode >> 6 ) & 0x3f ) | 0x80;
        ushort firstByte = ( ( unicode >> 12 ) & 0x0f ) | 0xe0;
        return( QString::number( firstByte, 16 ) + QString::number( secondByte, 16 ) + QString::number( thirdByte, 16 ) );
    }
}

int Util::writeFileIntoZipFile( zipFile outputFile, const char* filename, const char* data, int length ) {
    zip_fileinfo zipFileInfo;

    zipFileInfo.tmz_date.tm_sec = zipFileInfo.tmz_date.tm_min = zipFileInfo.tmz_date.tm_hour =
        zipFileInfo.tmz_date.tm_mday = zipFileInfo.tmz_date.tm_mon = zipFileInfo.tmz_date.tm_year = 0;
    zipFileInfo.dosDate = 0;
    zipFileInfo.internal_fa = 0;
    zipFileInfo.external_fa = 0;
    //filetime( filenameInZip, &zipFileInfo.tmz_date, &zipFileInfo.dosDate );

    int err = zipOpenNewFileInZip3( outputFile, filename, &zipFileInfo,
         NULL, 0, NULL, 0, NULL /* comment*/,
         Z_DEFLATED /* method */,
         5 /* level */, 0,
         /* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
         -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
         NULL, 0 );
    if( err )
        return( err );

    int writeErr = zipWriteInFileInZip( outputFile, data, length );

    err = zipCloseFileInZip( outputFile );

    if( writeErr || err )
        return( writeErr ? writeErr : err );

    return( ZIP_OK );
}

