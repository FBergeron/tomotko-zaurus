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
#ifdef DEBUG
    cout << "getTermData key=" << key.toString() << " contains=" << termData.contains( key ) << endl;
#endif
    if( termData.contains( key ) ) {
        TermData data( termData[ key ] );
#ifdef DEBUG
        cout << "ef=" << data.easinessFactor << " success=" << data.successCount << " miss=" << data.missCount << endl;
#endif
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
#ifdef DEBUG
    cout << "loadTermData firstLang=" << firstLang << " testLang=" << testLang << endl;
#endif
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
#ifdef DEBUG
    cout << "tempMagicNumber=" << tempMagicNumber << " tempVersion=" << tempVersion << endl;
#endif
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
#ifdef DEBUG
        cout << "tempKey=" << tempKey << " tempInterval=" << tempInterval << " tempRepetition=" << tempRepetition << " tempEasinessFactor=" << tempEasinessFactor;
        cout << " tempSuccessCount=" << tempSuccessCount << " tempMissCount=" << tempMissCount << endl;
#endif

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
//cout << "pos=" << pos << " tempKey=" << tempKey << " key=" << key.toString() << " eq?" << keyTest<< endl;
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
#ifdef DEBUG
    cout << "loadTermData2" << endl; 
#endif
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
#ifdef DEBUG
    cout << "saveTermData firstLang=" << firstLang << " testLang=" << testLang << endl;
#endif
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

//cout << "insert a record key=" << tempKey << endl;
            out << tempKey << tempInterval << tempRepetition << tempEasinessFactor << tempNextRepetitionDate;
            out << tempLastRepetitionDate << tempSuccessCount << tempMissCount;
        }
        // The new record is the last one.
        if( !isNewRecordWritten ) {
//cout << "insert the last record key=" << key.toString() << endl;
            out << key.toString() << newTermData.interval << newTermData.repetition << newTermData.easinessFactor << newTermData.nextRepetitionDate;
            out << newTermData.lastRepetitionDate << newTermData.successCount << newTermData.missCount;
            
            isNewRecordWritten = true;
        }
        dataFile.close();
    }
    else { 
//cout << "insert a record2 key=" << key.toString() << " ef=" << newTermData.easinessFactor << " success=" << newTermData.successCount << " miss=" << newTermData.missCount << endl;
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

bool Statistics::removeTermData( QValueList<QString>& transUidList, const QString& filename ) {
    cout << "removeTermData filename=" << filename << " count=" << transUidList.count() << endl;
    for( QValueList<QString>::ConstIterator it = transUidList.begin(); it != transUidList.end(); it++ ) {
        cout << "uid=" << (*it) << endl;
    }
    cout << "toto" << endl;

    QFile dataFile( filename );
    if( !dataFile.open( IO_ReadOnly ) ) {
        cerr << "Cannot open metadata file: " << dataFile.name() << endl;
        return( false );
    }

    QFile newDataFile( filename + ".tmp" );
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
    
    out.setVersion( 3 );
    out << Q_UINT32( Statistics::magicNumber ) << Q_UINT16( 0x0001 );

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

        BiUidKey key( tempKey );
        if( !transUidList.contains( key.getFirstUid() ) && !transUidList.contains( key.getSecondUid() ) ) {
            out << tempKey << tempInterval << tempRepetition << tempEasinessFactor << tempNextRepetitionDate;
            out << tempLastRepetitionDate << tempSuccessCount << tempMissCount;
        }
        else {
            // We omit writing the record so that it's effectively removed.
            // Also remove the record from memory.
            termData.remove( key );
        }
    }
    dataFile.close();
    newDataFile.close();

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

    return( true );
}

void Statistics::convertTermData( const QString& firstLang, const QString& testLang, Folder* topFolder ) {
#ifdef DEBUG
    cout << "convertTermData firstLang=" << firstLang << " testLang=" << testLang << endl;
#endif
    QValueList<BiUidKey> keysToRemove;
    for( QMap<BiUidKey, TermData>::ConstIterator it = termData.begin(); it != termData.end(); it++ ) {
        BiUidKey key = it.key();
        TermData data = it.data();
#ifdef DEBUG
        cout << "key=" << key.toString() << endl;
#endif
        if( key.getFirstUid() == key.getSecondUid() ) {
            QUuid termKey( key.getFirstUid() );
            Term* term = topFolder->getTerm( termKey );
            if( term && term->isTranslationExists( firstLang ) && term->isTranslationExists( testLang ) ) {
                keysToRemove.append( key );
                BiUidKey newKey( term->getTranslation( firstLang ).getUid().toString(), 
                    term->getTranslation( testLang ).getUid().toString() );
                termData[ newKey ] = data;
            }
            else {
#ifdef DEBUG
                cout << "Not found!" << endl;
#endif
                Base* object = topFolder->getObject( termKey );
                if( object ) {
                    if( strcmp( object->className(), "Term" ) == 0 ) {
                        keysToRemove.append( key );
#ifdef DEBUG
                        cout << "The key points a term without related translations so I remove the key." << endl;
#endif
                    }
                    else {
#ifdef DEBUG
                        cout << "className=" << object->className() << ".  What should I do with that?  Delete the key?" << endl;
#endif
                    }
                }
                else {
#ifdef DEBUG
                    cout << "Really..." << endl;
#endif
                    keysToRemove.append( key );
                }
            }
        }
        else {
            Base* obj1 = topFolder->getObject( key.getFirstUid() );
            if( obj1 ) {
            }
            else {
#ifdef DEBUG
                cout << "Part of the key points to inexisting object so I remove the key." << endl;
#endif
                keysToRemove.append( key );
            }

            Base* obj2 = topFolder->getObject( key.getSecondUid() );
            if( obj2 ) {
            }
            else {
#ifdef DEBUG
                cout << "Part of the key points to inexisting object so I remove the key." << endl;
#endif
                keysToRemove.append( key );
            }
        }
    }

    //QUuid nullUid;
    //Base* objWithNullUid = topFolder->getObject( nullUid );
    //if( objWithNullUid )
    //    cout << "There is still an object with nullUid: " << objWithNullUid->className() << endl;

    for( uint i = 0; i < keysToRemove.count(); i++ )
        termData.remove( keysToRemove[ i ] );
}

void Statistics::clear() {
    currentLanguages = BilingualKey(); 
    termData.clear();
    deletedTranslations.clear();
}

QString Statistics::getTermDataFilename( const QString& firstLang, const QString& testLang ) const {
    if( !firstLang.isEmpty() && !testLang.isEmpty() ) {
        BilingualKey key( firstLang, testLang );
        return( applicationDirName + "/termData_" + key.toString() + ".dat" );
    }
    else
        return( QString::null );
}

void Statistics::addDeletedTranslation( const QUuid& uid, const QString& lang ) {
#ifdef DEBUG
    cout << "addDeletedTranslation uid=" << uid.toString() << " lang=" << lang << endl;
#endif
    if( !deletedTranslations.contains( lang ) ) {
        QValueList<QString> transUidList;
        deletedTranslations.insert( lang, transUidList );
    }
    QValueList<QString>& transUidList = deletedTranslations[ lang ];
    transUidList.append( uid.toString() );
#ifdef DEBUG
    cout << "addDeletedTranslation count=" << transUidList.count() << endl;
    for( QValueList<QString>::ConstIterator it = transUidList.begin(); it != transUidList.end(); it++ )
        cout << (*it) << ",";
    cout << endl;
#endif
}

void Statistics::removeDeletedTranslation( const QUuid& uid, const QString& lang ) {
    if( deletedTranslations.contains( lang ) ) {
        QValueList<QString>& transUidList = deletedTranslations[ lang ];
        if( transUidList.contains( uid.toString() ) )
            transUidList.remove( uid.toString() );
    }
}

bool Statistics::purgeObsoleteData() {
    QDir dataDir( applicationDirName );
    for( uint i = 0; i < dataDir.count(); i++ ) {
        if( dataDir[ i ].left( 9 ) == "termData_" ) {
            QValueList<QString> languages;
            QString firstLang = dataDir[ i ].mid( 9, 2 );
            QString secondLang = dataDir[ i ].mid( 12, 2 );
            languages << firstLang << secondLang;
            QValueList<QString> transUidList;
            for( QValueList<QString>::ConstIterator it = languages.begin(); it != languages.end(); it++ ) {
                QString lang = *it;
                if( deletedTranslations.contains( lang ) ) {
                    const QValueList<QString>& transUidList2 = deletedTranslations[ lang ];
                    for( QValueList<QString>::ConstIterator it2 = transUidList2.begin(); it2 != transUidList2.end(); it2++ ) {
                        QString uid = *it2;
                        transUidList << uid;
                    }
                }
            }
            if( transUidList.count() > 0 ) {
                if( !removeTermData( transUidList, applicationDirName + "/" + dataDir[ i ] ) )
                    return( false );
            }
        }
    }
    deletedTranslations.clear();
    return( true );
}

QStringList Statistics::toString() const {
    QStringList str;

    for( QMap<BiUidKey, TermData>::ConstIterator it = termData.begin(); it != termData.end(); it++ ) {
        BiUidKey key = it.key();
        TermData termData = it.data();
   
        str += key.toString() + " <=> " + termData.toString();
    }

    return( str );
}

bool Statistics::exportIntoZip( zipFile outputFile, QStringList& exportedTransUidList ) const {
#ifdef DEBUG
    cout << "exportIntoZip" << endl;
#endif
    QCString dataFilename = QString( "/stats.xml" ).latin1();
    const char* filenameInZip = (const char*)dataFilename.data();

    QByteArray buffer;
    QTextStream ts( buffer, IO_WriteOnly );
    ts.setEncoding( QTextStream::UnicodeUTF8 );
    //writeStatsInXml( ts, *Statistics::instance(), exportedTransUidList );
    int err = Util::writeFileIntoZipFile( outputFile, filenameInZip, buffer.data(), buffer.size() );

    return( err == ZIP_OK );
}

