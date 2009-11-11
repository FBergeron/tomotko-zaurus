#include "Statistics.h"

uint Statistics::termDataEntrySize = 0;

Statistics* Statistics::singleton = 0;

void Statistics::makeInstance( const QString& applDirName ) {
    Statistics::singleton = new Statistics( applDirName );
}

Statistics* Statistics::instance() {
    return( Statistics::singleton );
}

Statistics::Statistics( const QString& applDirName ) : applicationDirName( applDirName ) {
}

TermData Statistics::getTermData( const QString& termUid ) {
    if( allTermData.contains( termUid ) )
        return( allTermData[ termUid ] );
    else {
        TermData termData;
        termData.interval = 0;
        termData.repetition = 0;
        termData.easinessFactor = 2.5;
        termData.nextRepetitionDate = QDate();
        allTermData[ termUid ] = termData;
        return( termData );
    }
}

void Statistics::setTermData( const QString& termUid, const QString& firstLang, const QString& testLang, const TermData& data ) {
    allTermData[ termUid ] = data;
    bool isSaved = saveTermData( termUid, firstLang, testLang, data ); 
    cerr << "isSaved=" << isSaved << endl;
}

bool Statistics::loadTermData( const QString& firstLang, const QString& testLang ) {
    BilingualKey key( firstLang, testLang );
    if( key == currentLanguages ) 
        return( true );

    QFile dataFile( getTermDataFilename( firstLang, testLang ) );
    if( !dataFile.exists() )
        return( true );

    if( !dataFile.open( IO_ReadOnly ) ) {
        cerr << "Cannot open metadata file: " << dataFile.name() << endl;
        return( false );
    }
    
    //QByteArray compressedData( dataFile.readAll() );
    QByteArray data( dataFile.readAll() );
    dataFile.close();

    //QByteArray data( Util::qUncompress( compressedData ) );

    QDataStream in( data, IO_ReadOnly );

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
    QString tempTermUid;
    int tempInterval;
    uint tempRepetition;
    float tempEasinessFactor;
    QDate tempNextRepetitionDate;
   
    in >> tempMagicNumber >> tempVersion;
    if( tempMagicNumber != Statistics::magicNumber ) {
        cerr << "Wrong magic number: Incompatible statistics data file." << endl;
        return( false );
    }
    if( tempVersion > 0x0001 ) {
        cerr << "Statistics data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( false );
    }
    if( tempVersion < 0x0001 ) {
        cerr << "Statistics data format too old.  You must use an anterior version of toMOTko." << endl;
        return( false );
    }

    allTermData.clear();

    in.setVersion( 3 );
    while( !in.atEnd() ) {
        in >> tempTermUid >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;

        TermData termData;
        termData.interval = tempInterval;
        termData.repetition = tempRepetition;
        termData.easinessFactor = tempEasinessFactor;
        termData.nextRepetitionDate = tempNextRepetitionDate;

        allTermData[ tempTermUid ] = termData;

        cerr << tempTermUid << ": interval=" << tempInterval << " repetition=" << tempRepetition << " EF=" << tempEasinessFactor << " nextRepDate=" << tempNextRepetitionDate.toString() << endl;
    }
cerr << "allTermData.size=" << allTermData.count() << endl;
    currentLanguages = key;

    return( true );
}

/*
 * Search for a term data dichotomically and position the stream at the beginning of the record, if found.
 * Returns true when the record is found, false otherwise.
 */
bool Statistics::seekTermData( const QString& termUid, QFile& dataFile, const uint headerSize, const uint entryCount, QDataStream& in ) {
    QString tempTermUid;
    int min = 0;
    int max = entryCount - 1;
    for( ;; ) {
        int jump = (int)( ( max - min ) / 2 );
        
        int entryIndex = min + jump;
        int pos = headerSize + entryIndex * Statistics::termDataEntrySize;

        dataFile.at( pos );

        in >> tempTermUid;

        int keyTest = tempTermUid.compare( termUid );
        if( keyTest == 0 )
            return( true );
        else {
            if( keyTest < 0 ) {
                if( entryIndex == max )
                    break;
                min = entryIndex + 1;
            }
            else {
                if( entryIndex == min )
                    break;
                max = entryIndex - 1;
            }
        }
    }
    return( false );
}

TermData Statistics::loadTermData( const QString& termUid, const QString& firstLang, const QString& testLang ) {
    TermData termData;

    QFile dataFile( getTermDataFilename( firstLang, testLang ) );
    if( !dataFile.exists() )
        return( termData );

    if( !dataFile.open( IO_ReadOnly ) ) {
        cerr << "Cannot open metadata file: " << dataFile.name() << endl;
        return( termData );
    }

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
    QString tempTermUid;
    int tempInterval;
    uint tempRepetition;
    float tempEasinessFactor;
    QDate tempNextRepetitionDate;
   
    QDataStream in( &dataFile );

    in >> tempMagicNumber >> tempVersion;
    if( tempMagicNumber != Statistics::magicNumber ) {
        cerr << "Wrong magic number: Incompatible statistics data file." << endl;
        return( termData );
    }
    if( tempVersion > 0x0001 ) {
        cerr << "Statistics data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( termData );
    }
    if( tempVersion < 0x0001 ) {
        cerr << "Statistics data format too old.  You must use an anterior version of toMOTko." << endl;
        return( termData );
    }

    uint headerSize = dataFile.at();
    uint fileSize = QFileInfo( dataFile ).size();
    in.setVersion( 3 );

    // Read the first entry to determine its size.
    if( Statistics::termDataEntrySize == 0 && !in.atEnd() ) {
        in >> tempTermUid >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;

        Statistics::termDataEntrySize = dataFile.at() - headerSize;
    }

    uint entryCount = ( fileSize - headerSize ) / Statistics::termDataEntrySize;

    bool isTermDataFound = seekTermData( termUid, dataFile, headerSize, entryCount, in );
    cerr << "isTermDataFound=" << isTermDataFound << endl;
    if( isTermDataFound ) {
        in >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;
cerr << "tempNextRepetitionDate=" << tempNextRepetitionDate.toString() << endl;

        termData.interval = tempInterval;
        termData.repetition = tempRepetition;
        termData.easinessFactor = tempEasinessFactor;
        termData.nextRepetitionDate = tempNextRepetitionDate;
    }
    
    dataFile.close();

    return( termData );
}

bool Statistics::saveTermData( const QString& firstLang, const QString& testLang ) const {
    QByteArray data;

    QDataStream out( data, IO_WriteOnly );
    out.setVersion( 3 /* QDataStream::Qt_3 ? */ );

    out << Q_UINT32( Statistics::magicNumber ) << Q_UINT16( 0x0001 );
    for( QMap<QString, TermData>::ConstIterator it = allTermData.begin(); it != allTermData.end(); it++ ) {
        QString termUid = it.key();
        TermData termData = it.data();
        out << termUid << termData.interval << termData.repetition << termData.easinessFactor << termData.nextRepetitionDate;
    }

    //QByteArray compressedData( Util::qCompress( data ) ); 

    QFile dataFile( getTermDataFilename( firstLang, testLang ) );
    QFileInfo dataFileInfo( dataFile );

    if( !Util::makeDirectory( dataFileInfo.dirPath() ) )
        return( false );

    if( !dataFile.open( IO_WriteOnly ) )
        return( false );

    //dataFile.writeBlock( compressedData );
    dataFile.writeBlock( data );
    dataFile.close();

    return( true );
}

bool Statistics::saveTermData( const QString& termUid, const QString& firstLang, const QString& testLang, const TermData& termData ) {
    if( overwriteTermData( termUid, firstLang, testLang, termData ) )
        return( true );

    return( insertTermData( termUid, firstLang, testLang, termData ) );
}

bool Statistics::overwriteTermData( const QString& termUid, const QString& firstLang, const QString& testLang, const TermData& termData ) {
    bool isNewRecordWritten = false;

cerr << "overwriteTermData uid=" << termUid << " nr date=" << termData.nextRepetitionDate.toString() << endl;
    QFile dataFile( getTermDataFilename( firstLang, testLang ) );
    if( !dataFile.exists() )
        return( false );

    if( !dataFile.open( IO_ReadWrite ) ) {
        cerr << "Cannot open metadata file: " << dataFile.name() << endl;
        return( false );
    }

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
    QString tempTermUid;
   
    QDataStream in( &dataFile );

    in >> tempMagicNumber >> tempVersion;
    if( tempMagicNumber != Statistics::magicNumber ) {
        cerr << "Wrong magic number: Incompatible statistics data file." << endl;
        return( false );
    }
    if( tempVersion > 0x0001 ) {
        cerr << "Statistics data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( false );
    }
    if( tempVersion < 0x0001 ) {
        cerr << "Statistics data format too old.  You must use an anterior version of toMOTko." << endl;
        return( false );
    }

    uint headerSize = dataFile.at();
    uint fileSize = QFileInfo( dataFile ).size();
    in.setVersion( 3 );

    // Read the first entry to determine its size.
    if( Statistics::termDataEntrySize == 0 && !in.atEnd() ) {
        int tempInterval;
        uint tempRepetition;
        float tempEasinessFactor;
        QDate tempNextRepetitionDate;

        in >> tempTermUid >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;

        Statistics::termDataEntrySize = dataFile.at() - headerSize;
    }

    uint entryCount = ( fileSize - headerSize ) / Statistics::termDataEntrySize;

    bool isTermDataFound = seekTermData( termUid, dataFile, headerSize, entryCount, in );
    if( isTermDataFound ) {
        QDataStream out( &dataFile );

        out << termData.interval << termData.repetition << termData.easinessFactor << termData.nextRepetitionDate;

        isNewRecordWritten = true;
    }
    
    dataFile.close();

    return( isNewRecordWritten );
}

bool Statistics::insertTermData( const QString& termUid, const QString& firstLang, const QString& testLang, const TermData& newTermData ) {
cerr << "insertTermData uid=" << termUid << " nr date=" << newTermData.nextRepetitionDate.toString() << endl;
    bool isNewRecordWritten = false;

    QFile dataFile( getTermDataFilename( firstLang, testLang ) );
    bool dataFileExists = dataFile.exists();

    if( dataFileExists ) {
        if( !dataFile.open( IO_ReadOnly ) ) {
            cerr << "Cannot open metadata file: " << dataFile.name() << endl;
            return( false );
        }
    }

    QFile newDataFile( getTermDataFilename( firstLang, testLang ) + ( dataFileExists ? ".tmp" : "" ) );
    if( newDataFile.exists() && !newDataFile.remove() ) {
        dataFile.close();
        return( false );
    }

    if( !newDataFile.open( IO_WriteOnly ) ) {
        cerr << "Cannot open metadata file: " << newDataFile.name() << endl;
        dataFile.close();
        return( false );
    }

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
   
    QDataStream in;
    QDataStream out( &newDataFile );

    if( dataFileExists ) {
        in.setDevice( &dataFile );
        in >> tempMagicNumber >> tempVersion;
        if( tempMagicNumber != Statistics::magicNumber ) {
            cerr << "Wrong magic number: Incompatible statistics data file." << endl;
            dataFile.close();
            newDataFile.close();
            return( false );
        }
        if( tempVersion > 0x0001 ) {
            cerr << "Statistics data file is from a more recent version.  Upgrade toMOTko." << endl;
            dataFile.close();
            newDataFile.close();
            return( false );
        }
        if( tempVersion < 0x0001 ) {
            cerr << "Statistics data format too old.  You must use an anterior version of toMOTko." << endl;
            dataFile.close();
            newDataFile.close();
            return( false );
        }

        in.setVersion( 3 );
    }

    out.setVersion( 3 );
    out << Q_UINT32( Statistics::magicNumber ) << Q_UINT16( 0x0001 );

    if( dataFileExists ) {
        while( !in.atEnd() ) {
            QString tempTermUid;
            int tempInterval;
            uint tempRepetition;
            float tempEasinessFactor;
            QDate tempNextRepetitionDate;

            in >> tempTermUid >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;

            if( !isNewRecordWritten && termUid.compare( tempTermUid ) < 0 ) {
                out << termUid << newTermData.interval << newTermData.repetition << newTermData.easinessFactor << newTermData.nextRepetitionDate;

                isNewRecordWritten = true;
            }

            out << tempTermUid << tempInterval << tempRepetition << tempEasinessFactor << tempNextRepetitionDate;
        }
        // The new record is the last one.
        if( !isNewRecordWritten ) {
            out << termUid << newTermData.interval << newTermData.repetition << newTermData.easinessFactor << newTermData.nextRepetitionDate;
            
            isNewRecordWritten = true;
        }
        dataFile.close();
    }
    else { 
        out << termUid << newTermData.interval << newTermData.repetition << newTermData.easinessFactor << newTermData.nextRepetitionDate;

        isNewRecordWritten = true;
    }
    
    newDataFile.close();

    if( dataFileExists ) {
        if( !dataFile.remove() ) {
            cerr << "Cannot remove file: " << dataFile.name() << endl;
            return( false );
        }

        if( !Util::copy( newDataFile.name(), dataFile.name() ) ) {
            cerr << "Cannot copy file: " << newDataFile.name() << " to " << dataFile.name() << endl;
            return( false );
        }

        // If we cannot remove the temporay file, it's not a fatal error at this point so we just display a warning.
        if( !newDataFile.remove() )
            cerr << "Cannot remove file: " << dataFile.name() << endl;
    }

    return( isNewRecordWritten );
}

// This method is temporary.  It's used to uncompress the term data files.
// It should be removed once the new implementation of term data files is over.
bool Statistics::saveUncompressedTermData( const QString& firstLang, const QString& testLang ) const {
    QByteArray data;

    QDataStream out( data, IO_WriteOnly );
    out.setVersion( 3 /* QDataStream::Qt_3 ? */ );

    out << Q_UINT32( Statistics::magicNumber ) << Q_UINT16( 0x0001 );
    for( QMap<QString, TermData>::ConstIterator it = allTermData.begin(); it != allTermData.end(); it++ ) {
        QString termUid = it.key();
        TermData termData = it.data();
        out << termUid << termData.interval << termData.repetition << termData.easinessFactor << termData.nextRepetitionDate;
    }

    //QByteArray compressedData( Util::qCompress( data ) ); 

    QString filename = getTermDataFilename( firstLang, testLang );
    QFile dataFile( filename.left( filename.length() - 2 ) );
    QFileInfo dataFileInfo( dataFile );

    if( !Util::makeDirectory( dataFileInfo.dirPath() ) )
        return( false );

    if( !dataFile.open( IO_WriteOnly ) )
        return( false );

    //dataFile.writeBlock( compressedData );
    dataFile.writeBlock( data );
    dataFile.close();

    return( true );
}

QString Statistics::getTermDataFilename( const QString& firstLang, const QString& testLang ) const {
    //QString firstLang = prefs.getFirstLanguage();
    //QString testLang = prefs.getTestLanguage();
    if( !firstLang.isEmpty() && !testLang.isEmpty() ) {
        BilingualKey key( firstLang, testLang );
        return( applicationDirName + "/termData_" + key.toString() + ".dat" );
    }
    else
        return( QString::null );
    //return( applicationDirName + "/termData_de-fr.dat.z" );

    //return( applicationDirName + "/termData_de-fr.dat" );
}
