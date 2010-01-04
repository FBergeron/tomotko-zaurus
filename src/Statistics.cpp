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

TermData Statistics::getTermData( const BiUidKey& key ) {
cerr << "getTermData key=" << key.toString() << " contains=" << termData.contains( key ) << endl;
    if( termData.contains( key ) ) {
        TermData data( termData[ key ] );
cerr << "ef=" << data.easinessFactor << " success=" << data.successCount << " miss=" << data.missCount << endl;
        return( termData[ key ] );
    }
    else {
        TermData aTermData;
        aTermData.interval = 0;
        aTermData.repetition = 0;
        aTermData.easinessFactor = 2.5;
        aTermData.nextRepetitionDate = QDate::currentDate();
        aTermData.lastRepetitionDate = QDate();
        aTermData.successCount = 0;
        aTermData.missCount = 0;
        termData[ key ] = aTermData;
        return( aTermData );
    }
}

void Statistics::setTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& data ) {
    termData[ key ] = data;
    saveTermData( key, firstLang, testLang, data ); 
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
    
    QByteArray data( dataFile.readAll() );
    dataFile.close();

    QDataStream in( data, IO_ReadOnly );

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
    QString tempKey;
    int tempInterval;
    uint tempRepetition;
    float tempEasinessFactor;
    QDate tempNextRepetitionDate;
    QDate tempLastRepetitionDate;
    uint tempSuccessCount;
    uint tempMissCount;
   
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

    termData.clear();

    in.setVersion( 3 );
    while( !in.atEnd() ) {
        in >> tempKey >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;
        in >> tempLastRepetitionDate >> tempSuccessCount >> tempMissCount;

        TermData aTermData;
        aTermData.interval = tempInterval;
        aTermData.repetition = tempRepetition;
        aTermData.easinessFactor = tempEasinessFactor;
        aTermData.nextRepetitionDate = tempNextRepetitionDate;
        aTermData.lastRepetitionDate = tempLastRepetitionDate;
        aTermData.successCount = tempSuccessCount;
        aTermData.missCount = tempMissCount;

        termData[ BiUidKey( tempKey ) ] = aTermData;
    }
    currentLanguages = key;

    return( true );
}

/*
 * Search for a term data dichotomically and position the stream at the beginning of the record, if found.
 * Returns true when the record is found, false otherwise.
 */
bool Statistics::seekTermData( const BiUidKey& key, QFile& dataFile, const uint headerSize, const uint entryCount, QDataStream& in ) {
    QString tempKey;
    int min = 0;
    int max = entryCount - 1;
    for( ;; ) {
        int jump = (int)( ( max - min ) / 2 );
        
        int entryIndex = min + jump;
        int pos = headerSize + entryIndex * Statistics::termDataEntrySize;
        dataFile.at( pos );

        in >> tempKey;
        int keyTest = tempKey.compare( key.toString() );
//cerr << "pos=" << pos << " tempKey=" << tempKey << " key=" << key.toString() << " eq?" << keyTest<< endl;
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

TermData Statistics::loadTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang ) {
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
    QString tempKey;
    int tempInterval;
    uint tempRepetition;
    float tempEasinessFactor;
    QDate tempNextRepetitionDate;
    QDate tempLastRepetitionDate;
    uint tempSuccessCount;
    uint tempMissCount;
   
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
        in >> tempKey >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;
        in >> tempLastRepetitionDate >> tempSuccessCount >> tempMissCount;
        Statistics::termDataEntrySize = dataFile.at() - headerSize;
    }

    uint entryCount = ( fileSize - headerSize ) / Statistics::termDataEntrySize;

    bool isTermDataFound = seekTermData( key, dataFile, headerSize, entryCount, in );
    if( isTermDataFound ) {
        in >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;
        in >> tempLastRepetitionDate >> tempSuccessCount >> tempMissCount;

        termData.interval = tempInterval;
        termData.repetition = tempRepetition;
        termData.easinessFactor = tempEasinessFactor;
        termData.nextRepetitionDate = tempNextRepetitionDate;
        termData.lastRepetitionDate = tempLastRepetitionDate;
        termData.successCount = tempSuccessCount;
        termData.missCount = tempMissCount;
    }
    
    dataFile.close();

    return( termData );
}

bool Statistics::saveTermData( const QString& firstLang, const QString& testLang ) const {
    QByteArray data;

    QDataStream out( data, IO_WriteOnly );
    out.setVersion( 3 /* QDataStream::Qt_3 ? */ );

    out << Q_UINT32( Statistics::magicNumber ) << Q_UINT16( 0x0001 );
    for( QMap<BiUidKey, TermData>::ConstIterator it = termData.begin(); it != termData.end(); it++ ) {
        BiUidKey key = it.key();
        TermData termData = it.data();
        out << key.toString() << termData.interval << termData.repetition << termData.easinessFactor << termData.nextRepetitionDate;
        out << termData.lastRepetitionDate << termData.successCount << termData.missCount;
    }

    QFile dataFile( getTermDataFilename( firstLang, testLang ) );
    QFileInfo dataFileInfo( dataFile );

    if( !Util::makeDirectory( dataFileInfo.dirPath() ) )
        return( false );

    if( !dataFile.open( IO_WriteOnly ) )
        return( false );

    dataFile.writeBlock( data );
    dataFile.close();

    return( true );
}

bool Statistics::saveTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& termData ) {
    if( overwriteTermData( key, firstLang, testLang, termData ) )
        return( true );

    return( insertTermData( key, firstLang, testLang, termData ) );
}

bool Statistics::overwriteTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& termData ) {
    bool isNewRecordWritten = false;

    QFile dataFile( getTermDataFilename( firstLang, testLang ) );
    if( !dataFile.exists() )
        return( false );

    if( !dataFile.open( IO_ReadWrite ) ) {
        cerr << "Cannot open metadata file: " << dataFile.name() << endl;
        return( false );
    }

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
    QString tempKey;
   
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
        QDate tempLastRepetitionDate;
        uint tempSuccessCount;
        uint tempMissCount;

        in >> tempKey >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;
        in >> tempLastRepetitionDate >> tempSuccessCount >> tempMissCount;

        Statistics::termDataEntrySize = dataFile.at() - headerSize;
    }

    uint entryCount = ( fileSize - headerSize ) / Statistics::termDataEntrySize;

    bool isTermDataFound = seekTermData( key, dataFile, headerSize, entryCount, in );
    if( isTermDataFound ) {
        QDataStream out( &dataFile );

        out << termData.interval << termData.repetition << termData.easinessFactor << termData.nextRepetitionDate;
        out << termData.lastRepetitionDate << termData.successCount << termData.missCount;

        isNewRecordWritten = true;
    }
    
    dataFile.close();

    return( isNewRecordWritten );
}

bool Statistics::insertTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& newTermData ) {
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
            QString tempKey;
            int tempInterval;
            uint tempRepetition;
            float tempEasinessFactor;
            QDate tempNextRepetitionDate;
            QDate tempLastRepetitionDate;
            uint tempSuccessCount;
            uint tempMissCount;

            in >> tempKey >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;
            in >> tempLastRepetitionDate >> tempSuccessCount >> tempMissCount;

            if( !isNewRecordWritten && key.toString().compare( tempKey ) < 0 ) {
                out << key.toString() << newTermData.interval << newTermData.repetition << newTermData.easinessFactor << newTermData.nextRepetitionDate;
                out << newTermData.lastRepetitionDate << newTermData.successCount << newTermData.missCount;

                isNewRecordWritten = true;
            }

//cerr << "insert a record key=" << tempKey << endl;
            out << tempKey << tempInterval << tempRepetition << tempEasinessFactor << tempNextRepetitionDate;
            out << tempLastRepetitionDate << tempSuccessCount << tempMissCount;
        }
        // The new record is the last one.
        if( !isNewRecordWritten ) {
//cerr << "insert the last record key=" << key.toString() << endl;
            out << key.toString() << newTermData.interval << newTermData.repetition << newTermData.easinessFactor << newTermData.nextRepetitionDate;
            out << newTermData.lastRepetitionDate << newTermData.successCount << newTermData.missCount;
            
            isNewRecordWritten = true;
        }
        dataFile.close();
    }
    else { 
//cerr << "insert a record2 key=" << key.toString() << " ef=" << newTermData.easinessFactor << " success=" << newTermData.successCount << " miss=" << newTermData.missCount << endl;
        out << key.toString() << newTermData.interval << newTermData.repetition << newTermData.easinessFactor << newTermData.nextRepetitionDate;
        out << newTermData.lastRepetitionDate << newTermData.successCount << newTermData.missCount;

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

QString Statistics::getTermDataFilename( const QString& firstLang, const QString& testLang ) const {
    if( !firstLang.isEmpty() && !testLang.isEmpty() ) {
        BilingualKey key( firstLang, testLang );
        return( applicationDirName + "/termData_" + key.toString() + ".dat" );
    }
    else
        return( QString::null );
}
