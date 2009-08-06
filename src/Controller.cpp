#include "Controller.h"

const QString Controller::vocabTreeFilename = QString( "tree" ); 

Controller::Controller() : vocabTree( NULL ), quiz( NULL ) {
    applicationDirName = QDir::homeDirPath() + QString( "/.toMOTko" );
    markedXmlFilename = applicationDirName + QString( "/marked.xml" );
    markedFilename = applicationDirName + QString( "/marked.dat.z" );
    prefs.setApplicationDirName( applicationDirName );
}

bool Controller::init() {
    QDir applDir( applicationDirName );
    if( !applDir.exists() ) {
        if( !applDir.mkdir( applicationDirName ) )
            return( false );
    }
    getPreferences().load();
    return( true ); 
}

const QString& Controller::getApplicationDirName() const {
    return( applicationDirName );
}

Preferences& Controller::getPreferences() {
    return( prefs );
}

Folder* Controller::getVocabTree() {
    return( vocabTree );
}

void Controller::startQuiz() {
    switch( getPreferences().getQuizAlgorithm() ) {
        case Preferences::ORIGINAL   : quiz = new OriginalQuiz( applicationDirName, getPreferences().getQuizLength() ); break;
        case Preferences::SUPERMEMO2 : quiz = new SuperMemo2Quiz( applicationDirName ); break;
    }
    if( quiz )
        quiz->init( getPreferences().getFirstLanguage(), getPreferences().getTestLanguage(), vocabTree );
}

void Controller::restartQuiz() {
    if( quiz )
        quiz->reinit();
}

bool Controller::isResumableQuizAvailable() {
    return( quiz ? quiz->isResumable() : false );
}

bool Controller::resumeQuiz() {
    return( quiz ? quiz->load() : false );
}

void Controller::prepareQuiz() {
    emit( progressChanged( getProgress() ) );
}

int Controller::getQuizAnswerCount() const {
    return( quiz ? quiz->getAnswerCount() : 0 );
}

void Controller::showQuizProgressData( QWidget* parent ) const {
    if( quiz )
        quiz->showProgressData( parent );
}

void Controller::concludeQuiz() {
    if( quiz )
        quiz->conclude();
}

Term* Controller::getCurrentTerm() {
    if( !quiz )
        return( NULL );

    TermKey key = quiz->getCurrentTerm();
    Vocabulary* vocab = vocabTree->getVocabulary( key.getVocabUid() );
    if( vocab && !vocab->isMarkedForDeletion() && vocab->isTermExists( key.getTermUid() ) ) {
        Term& term = vocab->getTerm( key.getTermUid() );
        if( !term.isMarkedForDeletion() )
            return( &term );
    }
    // The term is invalid.  Either the container vocabulary or the term itself have been deleted.
    // Let's try to fetch the next term.
    quiz->discardCurrentTerm();
    return( getNextTerm() );
}

Term* Controller::getTerm( const TermKey& termKey, const QString& firstLang /* = QString::null */, const QString& testLang /* = QString::null */ ) {
    Vocabulary* vocab = vocabTree->getVocabulary( termKey.getVocabUid() );
    if( vocab && !vocab->isMarkedForDeletion() && vocab->isTermExists( termKey.getTermUid() ) ) {
        Term& term = vocab->getTerm( termKey.getTermUid() );
        if( !term.isMarkedForDeletion() ) {
            // If we don't care about specific languages, return the term right away.
            if( firstLang.isNull() && testLang.isNull() )
                return( &term );

            // For specific languages, make sure that translations are valid.
            if( !firstLang.isNull() && !testLang.isNull() && term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) )
                return( &term );
        }
    }
    return( NULL );
}

Term* Controller::getNextTerm() {
    if( !quiz )
        return( NULL );

    TermKey nextTerm = quiz->getNextTerm();
    if( nextTerm.isNull() )
        return( NULL );

    TermKey currTerm = quiz->getCurrentTerm();
    Term* term = getTerm( currTerm, getQuizFirstLanguage(), getQuizTestLanguage() );
    if( term )
        return( term );

    // The term is invalid.  Either the container vocabulary or the term itself have been deleted.
    // Let's try to fetch the next term.
    quiz->discardCurrentTerm();
    return( quiz->hasNextTerm() ? getNextTerm() : NULL );
}

bool Controller::hasNextTerm() const {
    return( quiz ? quiz->hasNextTerm() : false );
}

QString Controller::getQuizFirstLanguage() const {
    return( quiz ? quiz->getFirstLanguage() : QString::null );
}

QString Controller::getQuizTestLanguage() const {
    return( quiz ? quiz->getTestLanguage() : QString::null );
}

Preferences::QuizAlgorithm Controller::getQuizAlgorithm() const {
    if( quiz ) {
        if( strcmp( quiz->className(), "OriginalQuiz" ) == 0 )
            return( Preferences::ORIGINAL );
        else if( strcmp( quiz->className(), "SuperMemo2Quiz" ) == 0 )
            return( Preferences::SUPERMEMO2 );
    }
    return( Preferences::ORIGINAL ); // Should never happen but just in case.
}

bool Controller::isQuizInProgress() const {
    return( quiz ? quiz->isInProgress() : false );
}

int Controller::getProgress() const {
    return( quiz ? quiz->getProgress() : 0 );
}

int Controller::getInitialTermCount() const {
    return( quiz ? quiz->getInitialTermCount() : 0 );
}

Sequence& Controller::getRevealingSequence() {
    return( currRevealingSeq );
}

int Controller::getRevealingSequenceStep() const {
    return( currRevealingSeqStep );
}

void Controller::incrementRevealingSequenceStep() {
    currRevealingSeqStep++;
}

Folder* Controller::addFolder( Folder* parentFolder, Folder* folder = NULL ) {
    QUuid newUid = Util::createUuid();

    QString folderLocation = applicationDirName + "/" + parentFolder->getPath();
    QDir folderDir( folderLocation );
    if( !folderDir.exists() ) {
        if( !Util::makeDirectory( folderDir.path() ) ) {
            cerr << "Cannot create directory " << folderLocation << endl;
            return( NULL );
        }
    }

    QString newTitle = ( folder ? folder->getTitle() : QObject::tr( "NewFolder" ) );
    Folder* newFolder = new Folder( -1, newTitle, newUid );
    newFolder->setAuthor( folder ? folder->getAuthor() : parentFolder->getAuthor() );
    if( folder ) {
        newFolder->setDescription( folder->getDescription() );
        newFolder->setCreationDate( folder->getCreationDate() );
        newFolder->setModificationDate( folder->getModificationDate() );
    }
    newFolder->setDirty( true ); // Force saving data.
    if( !saveFolder( newFolder, folderLocation ) ) {
        cerr << "Could not save the new folder." << endl;
        return( NULL );
    }
    parentFolder->add( newFolder );
    return( newFolder );
}

Vocabulary* Controller::addVocabulary( Folder* parentFolder, Vocabulary* vocab = NULL ) {
    QUuid newUid = Util::createUuid();

    QString vocabLocation = applicationDirName + "/" + parentFolder->getPath() + "/v-" + newUid.toString();
    QDir vocabDir( vocabLocation );
    if( !vocabDir.exists() ) {
        if( !Util::makeDirectory( vocabDir.path() ) ) {
            cerr << "Cannot create directory " << vocabLocation << endl;
            return( NULL );
        }
    }

    QString newTitle = ( vocab ? vocab->getTitle() : QObject::tr( "NewGlossary" ) );
    Vocabulary* newVocab = new Vocabulary( -1, newTitle, newUid );
    newVocab->setAuthor( vocab ? vocab->getAuthor() : parentFolder->getAuthor() );
    if( vocab ) {
        newVocab->setDescription( vocab->getDescription() );
        newVocab->setCreationDate( vocab->getCreationDate() );
        newVocab->setModificationDate( vocab->getModificationDate() );
        for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
            const Term& term = *it;
            Term newTerm( 0, newVocab->getUid(), Util::createUuid() );
            for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
                const Translation& trans = it2.data();
                newTerm.addTranslation( trans );
            }
            for( Term::CommentMap::ConstIterator it3 = term.commentsBegin(); it3 != term.commentsEnd(); it3++ ) {
                const BilingualKey& key = it3.key();
                const QString& comment = it3.data();
                newTerm.addComment( key, comment );
            }
            if( !term.getImagePath().isNull() ) {
                // If the path refers to a file outside toMOTko's directory, we just copy the path.
                // Otherwise, we copy the image as well as the container vocabulary may be moved or deleted ulteriorly.
                if( term.getImagePath().left( getApplicationDirName().length() ) == getApplicationDirName() ) {
                    QFileInfo imageToCopyInfo( term.getImagePath() );
                    QString imageFilename = imageToCopyInfo.fileName();
                    if( Util::copy( term.getImagePath(), vocabLocation + "/" + imageFilename ) )
                        newTerm.setImagePath( imageFilename ); 
                    else
                        cerr << "Could not copy " << term.getImagePath() << " to " << ( vocabLocation + "/" + imageFilename ) << endl;
                }
                else
                    newTerm.setImagePath( term.getImagePath() ); 
            }
            newVocab->addTerm( newTerm );
        }
        newVocab->setDirty( true ); // Force saving data.
    }
    if( !saveVocabulary( newVocab, vocabLocation ) ) {
        cerr << "Could not save the new vocabulary." << endl;
        return( NULL );
    }
    parentFolder->add( newVocab );
    return( newVocab );
}

void Controller::copy( Vocabulary* vocab ) {
    QString firstLang( getPreferences().getFirstLanguage() );
    QString testLang( getPreferences().getTestLanguage() );
    Vocabulary* vocabCopy = makeCopy( vocab, firstLang, testLang );

    QByteArray data;
    QDataStream out( data, IO_WriteOnly );
    out << *vocabCopy;
    setClipboardData( QString( "vocabulary" ), Util::qCompress( data ) );
}

void Controller::copy( Folder* folder ) {
    QString firstLang( getPreferences().getFirstLanguage() );
    QString testLang( getPreferences().getTestLanguage() );
    Folder* folderCopy = makeCopy( folder, firstLang, testLang );

    QByteArray data;
    QDataStream out( data, IO_WriteOnly );
    out << *folderCopy;

    // The Folder::operator<< will only write references for vocabularies.
    // We build a map containing copies of the vocabularies.
    
    QMap<QString,Vocabulary> vocabularies;
    folderCopy->buildVocabCopiesMap( vocabularies );
    out << vocabularies;

    setClipboardData( QString( "folder" ), Util::qCompress( data ) );
}

Vocabulary* Controller::makeCopy( Vocabulary* vocab, const QString& firstLang, const QString& testLang ) const {
    QStringList languages;
    languages << firstLang << testLang;

    Vocabulary* vocabCopy = new Vocabulary( -1, vocab->getTitle(), Util::createUuid() );
    vocabCopy->setDescription( vocab->getDescription() );
    vocabCopy->setAuthor( vocab->getAuthor() );
    vocabCopy->setCreationDate( vocab->getCreationDate() );
    vocabCopy->setModificationDate( vocab->getModificationDate() );
    for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
        const Term& term = it.data();
        Term* termCopy = new Term( 0, vocabCopy->getUid(), Util::createUuid() );

        for( QStringList::ConstIterator it = languages.begin(); it != languages.end(); it++ ) {
            const QString& lang = *it;
            if( term.isTranslationExists( lang ) ) {
                Translation transCopy( term.getTranslation( lang ) );
                termCopy->addTranslation( transCopy );
            }
        }
        BilingualKey commentKey( firstLang, testLang );
        if( term.isCommentExists( commentKey ) )
            termCopy->addComment( commentKey, term.getComment( commentKey ) );

        termCopy->setImagePath( getResolvedImagePath( term.getImagePath(), *vocab ) );

        vocabCopy->addTerm( *termCopy );
    }
    return( vocabCopy );
}

Folder* Controller::makeCopy( Folder* folder, const QString& firstLang, const QString& testLang ) const {
    QStringList languages;
    languages << firstLang << testLang;

    Folder* folderCopy = new Folder( -1, folder->getTitle(), Util::createUuid() );
    folderCopy->setDescription( folder->getDescription() );
    folderCopy->setAuthor( folder->getAuthor() );
    folderCopy->setCreationDate( folder->getCreationDate() );
    folderCopy->setModificationDate( folder->getModificationDate() );
    for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) { 
        if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)folderChild;
            if( childFolder->containsTermWithTranslations( firstLang, testLang ) ) {
                Folder* childFolderCopy = makeCopy( childFolder, firstLang, testLang );
                folderCopy->add( childFolderCopy );
            }
        }
        else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)folderChild;
            if( childVocab->containsTermWithTranslations( firstLang, testLang ) ) {
                Vocabulary* childVocabCopy = makeCopy( childVocab, firstLang, testLang );
                folderCopy->add( childVocabCopy );
            }
        }
    }
    return( folderCopy );
}

Vocabulary* Controller::loadVocabulary( const QString& parentDir ) {
    QString filename; 
    Vocabulary* vocab = NULL;
    QDir dir( parentDir );
    QString vocabDirName = dir.dirName();
    if( vocabDirName.left( 3 ) != "v-{" ) {
        int indexOfDash = vocabDirName.find( "-" );
        if( indexOfDash > 0 ) {
            bool isOk;
            QString strVocabId = vocabDirName.right( vocabDirName.length() - indexOfDash - 1 );
            int vocabId = strVocabId.toInt( &isOk );

            if( isOk ) // isOk means that the old format (without uid) is used.
                filename = QString( parentDir + QString( "/vocab-" ) + QString::number( vocabId ) + QString( ".gz" ) );

        }
    }
    else
        filename = parentDir + "/data.gz";

    if( !filename.isNull() ) {
        QFile compressedBinFile( filename );
        if( compressedBinFile.exists() ) {
            vocab = new Vocabulary();
            if( !vocab->load( filename ) ) {
                delete( vocab );
                vocab = NULL;
            }
        }
    }
    if( vocab == NULL )
        cerr << "Could not load vocab file in directory " << parentDir << endl;
    return( vocab );
}

Base* Controller::importData( Folder* folder, const QString& filename, const QStringList& languages ) {
    int status;
    Base* newItem = NULL;

    zipFile inputFile = unzOpen( filename.latin1() );
    if( inputFile == NULL )
        return( NULL );

    unz_global_info gi;
    status = unzGetGlobalInfo( inputFile, &gi );

    if( status == UNZ_OK ) {
        QMap<QString,Vocabulary*> newVocabs;
        QMap<QString,Folder*> newFolders;
        Folder* newFolder = NULL;
        Vocabulary* newVocab = NULL;
        QUuid newVocabUid;
        QUuid newFolderUid;
            
        // Handle all entries.
        for( uLong i = 0; i < gi.number_entry; i++ ) {
            char filename_inzip[ 256 ];
            unz_file_info file_info;
            status = unzGetCurrentFileInfo( inputFile, &file_info, filename_inzip, sizeof( filename_inzip ), NULL, 0, NULL, 0 );
            if( status != UNZ_OK ) 
                break;

            QString filenameInZip( filename_inzip );
            QFileInfo fileInfo( filenameInZip );
            cout << "f=" << filename_inzip << " dp=" << fileInfo.dirPath() << " dpa=" << fileInfo.dirPath( true ) << " fn=" << fileInfo.fileName() << endl;

            if( fileInfo.extension() == "gif" || fileInfo.extension() == "png" ) {
                QString importedVocabUid = findVocabUid( fileInfo.dirPath() );
                if( newVocabs.contains( importedVocabUid ) )
                    newVocab = newVocabs[ importedVocabUid ];
                else{
                    newVocabUid = Util::createUuid();
                    newVocab = new Vocabulary( -1, QString::null, newVocabUid );
                    newVocabs.insert( importedVocabUid, newVocab );
                    if( !newItem )
                        newItem = newVocab;
                    else {
                        QString parentFolderUid = findParentFolderUid( fileInfo.dirPath() );
                        if( !parentFolderUid.isNull() && newFolders.contains( parentFolderUid ) )
                            newFolders[ parentFolderUid ]->add( newVocab ); 
                    }
                }

                QString imageLocation = applicationDirName + "/" + folder->getPath() + convertPath( fileInfo.dirPath(), newFolders ) +
                    "/v-" + newVocab->getUid().toString() + "/" + fileInfo.fileName();
                importImageFromZip( imageLocation, inputFile );
            }
            else if( fileInfo.extension() == "xml" ) {
                if( fileInfo.fileName() == "metadata.xml" ) {
                    QString importedFolderUid = findFolderUid( fileInfo.dirPath() );
                   
                    if( newFolders.contains( importedFolderUid ) ) 
                        newFolder = newFolders[ importedFolderUid ];
                    else {
                        newFolderUid = Util::createUuid();
                        newFolder = new Folder( -1, QString::null, newFolderUid );
                        newFolders.insert( importedFolderUid, newFolder );
                        if( !newItem )
                            newItem = newFolder;
                        else {
                            QString parentFolderUid = findParentFolderUid( fileInfo.dirPath() );
                            if( parentFolderUid != -1 && newFolders.contains( parentFolderUid ) )
                                newFolders[ parentFolderUid ]->add( newFolder ); 
                        }
                    }
                
                    QString folderLocation = applicationDirName + "/" + folder->getPath() + convertPath( fileInfo.dirPath(), newFolders );
                    importFolderFromZip( newFolder, folderLocation, inputFile ); // Should we handle error here?
                }
                else if( fileInfo.fileName() == "data.xml" ) {
                    QString importedVocabUid = findVocabUid( fileInfo.dirPath() );
                    if( newVocabs.contains( importedVocabUid ) ) 
                        newVocab = newVocabs[ importedVocabUid ];
                    else {
                        newVocabUid = Util::createUuid();
                        newVocab = new Vocabulary( -1, QString::null, newVocabUid );
                        newVocabs.insert( importedVocabUid, newVocab );
                        if( !newItem )
                            newItem = newVocab;
                        else {
                            QString parentFolderUid = findParentFolderUid( fileInfo.dirPath() );
                            //cerr << "parentFolderUid=" << parentFolderUid << endl;
                            //for( QMap<QString,Folder*>::Iterator it = newFolders.begin(); it != newFolders.end(); it++ ) {
                            //    cerr << "map key=" << it.key() << endl;
                            //}
                            if( !parentFolderUid.isNull() && newFolders.contains( parentFolderUid ) )
                                newFolders[ parentFolderUid ]->add( newVocab ); 
                        }
                    }

                    QString vocabLocation = applicationDirName + "/" + folder->getPath() + convertPath( fileInfo.dirPath(), newFolders ) +
                        "/v-" + newVocab->getUid().toString();
                    importVocabularyFromZip( newVocab, vocabLocation, languages, inputFile ); // Should we handle error here?
                    cerr << "handling data.xml ok" << endl;
                }
            }

            status = unzGoToNextFile( inputFile );
            if( status == UNZ_END_OF_LIST_OF_FILE ) {
                status = UNZ_OK;
                break;
            }
            else if( status != UNZ_OK ) {
                // A problem occurred so exit loop.
                break;
            }
        }
    }

    // If an error occurred, we recover memory if necessary.
    if( status != UNZ_OK ) {
        if( newItem ) {
            delete( newItem );
            newItem = NULL;
        }
    }

    if( unzClose( inputFile ) != UNZ_OK )
        return( NULL );

    return( newItem );
}

QStringList Controller::getTranslationLanguagesFromFile( const QString& filename ) const {
    QStringList languages;

    zipFile inputFile = unzOpen( filename.latin1() );
    if( inputFile ) {
        unz_global_info gi;
        int status = unzGetGlobalInfo( inputFile, &gi );

        if( status == UNZ_OK ) {
            for( uLong i = 0; i < gi.number_entry; i++ ) {
                char filename_inzip[ 256 ];
                unz_file_info file_info;
                status = unzGetCurrentFileInfo( inputFile, &file_info, filename_inzip, sizeof( filename_inzip ), NULL, 0, NULL, 0 );
                if( status != UNZ_OK ) 
                    break;

                QString filenameInZip( filename_inzip );
                QFileInfo fileInfo( filenameInZip );
                if( fileInfo.fileName().left( 6 ) == "vocab-" && fileInfo.extension() == "xml" ) {
                    QStringList vocabLanguages = getVocabularyTranslationLanguagesFromZip( inputFile );
                    for( QStringList::Iterator it = vocabLanguages.begin(); it != vocabLanguages.end(); it++ ) {
                        const QString& lang = *it;
                        if( !languages.contains( lang ) )
                            languages.append( lang );
                    }
                }

                status = unzGoToNextFile( inputFile );
                if( status == UNZ_END_OF_LIST_OF_FILE ) {
                    status = UNZ_OK;
                    break;
                }
                else if( status != UNZ_OK ) {
                    cerr << "Cannot get next entry in Zip file: " << filename.latin1() << endl;
                    break; // An error has occured so exit the loop.
                }
            }
        }

        if( unzClose( inputFile ) != UNZ_OK )
            cerr << "Cannot close file: " << filename.latin1() << endl;
    }
    else
        cerr << "Cannot open file: " << filename.latin1() << endl;
    
    return( languages );
}

QStringList Controller::getVocabularyTranslationLanguagesFromZip( zipFile inputFile ) const {
    QStringList languages;

    int status = unzOpenCurrentFile( inputFile );
    if( status == UNZ_OK ) {
        bool isOk = true;

        char* buf = NULL;
        uInt size_buf = 8192; // Arbitrary size for now.
        buf = (char*)malloc( size_buf );
        if( buf == NULL ) {
            cerr << "Cannot allocate memory for unzip buffer." << endl;
            status = UNZ_INTERNALERROR;
        }

        if( status == UNZ_OK ) {
            QByteArray ba;
            QTextStream ts( ba, IO_WriteOnly );

            int totalByteCount = 0;
            int readStatus;
            for( ;; ) {
                readStatus = unzReadCurrentFile( inputFile, buf, size_buf );
                if( readStatus > 0 ) {
                    ts.writeRawBytes( buf, readStatus );
                    totalByteCount += readStatus;
                }
                else
                    break;
            }

            if( readStatus == 0 ) {
                Vocabulary* vocab = new Vocabulary();
                QTextStream ts2( ba, IO_ReadOnly );
                VocabParser parser( *vocab, languages );
                QXmlInputSource source( ts2 );

                QXmlSimpleReader reader;
                reader.setContentHandler( &parser );
                isOk = reader.parse( source );

                languages = vocab->getTranslationLanguages();

                delete( vocab );
                vocab = NULL;
            }
        }

        free( buf );

        if( unzCloseCurrentFile( inputFile ) != UNZ_OK )
            cerr << "Cannot close zip entry." << endl; 
    }
    else
        cerr << "Cannot open zip entry." << endl;

    return( languages );
}

bool Controller::importFolderFromZip( Folder* folder, const QString& folderLocation, zipFile inputFile ) {
    int status = unzOpenCurrentFile( inputFile );
    if( status != UNZ_OK )
        return( false );

    bool isOk = true;

    char* buf = NULL;
    uInt size_buf = 8192; // Arbitrary size for now.
    buf = (char*)malloc( size_buf );
    if( buf == NULL ) {
        cerr << "Cannot allocate memory for unzip buffer." << endl;
        status = UNZ_INTERNALERROR;
    }

    if( status == UNZ_OK ) {
        QByteArray ba;
        QTextStream ts( ba, IO_WriteOnly );

        int totalByteCount = 0;
        int readStatus;
        for( ;; ) {
            readStatus = unzReadCurrentFile( inputFile, buf, size_buf );
            if( readStatus > 0 ) {
                ts.writeRawBytes( buf, readStatus );
                totalByteCount += readStatus;
            }
            else
                break;
        }

        if( readStatus == 0 ) {
            QTextStream ts2( ba, IO_ReadOnly );
            FolderParser parser( *folder, folderLocation );
            QXmlInputSource source( ts2 );

            QXmlSimpleReader reader;
            reader.setContentHandler( &parser );
            isOk = reader.parse( source );

            if( isOk ) {
                // Create the containing folder if needed.
                isOk = Util::makeDirectory( folderLocation );
                if( isOk ) {
                    const QString& folderDataFilename( folderLocation + "/metadata.gz" );
                    isOk = folder->saveMetadata( folderDataFilename );
                    if( isOk )
                        folder->setDirty( false );
                }
                else {
                    cerr << "Could not create directory " << folderLocation << endl;
                    isOk = false;
                }
            }

            //if( !isOk || !parser.isVocabularyFile() ) {
            //    delete( vocab );
            //    vocab = NULL;
            //}
        }
    }

    free( buf );

    /*status =*/ unzCloseCurrentFile( inputFile );
    //if( readStatus != UNZ_OK || status != UNZ_OK ) 
    //    return( NULL );
    return( isOk );
}

bool Controller::importVocabularyFromZip( Vocabulary* vocab, const QString& vocabLocation, const QStringList& languages, zipFile inputFile ) {
    int status = unzOpenCurrentFile( inputFile );
    if( status != UNZ_OK )
        return( false );

    bool isOk = true;

    char* buf = NULL;
    uInt size_buf = 8192; // Arbitrary size for now.
    buf = (char*)malloc( size_buf );
    if( buf == NULL ) {
        cerr << "Cannot allocate memory for unzip buffer." << endl;
        status = UNZ_INTERNALERROR;
    }

    if( status == UNZ_OK ) {
        QByteArray ba;
        QTextStream ts( ba, IO_WriteOnly );

        int totalByteCount = 0;
        int readStatus;
        for( ;; ) {
            readStatus = unzReadCurrentFile( inputFile, buf, size_buf );
            if( readStatus > 0 ) {
                ts.writeRawBytes( buf, readStatus );
                totalByteCount += readStatus;
            }
            else
                break;
        }

        if( readStatus == 0 ) {
            QTextStream ts2( ba, IO_ReadOnly );
            VocabParser parser( *vocab, languages );
            QXmlInputSource source( ts2 );

            QXmlSimpleReader reader;
            reader.setContentHandler( &parser );
            isOk = reader.parse( source );
            if( isOk ) {
                vocab->setDirty( true ); // Force saving data.
                isOk = saveVocabulary( vocab, vocabLocation );
            }
        }
    }

    free( buf );

    /*status =*/ unzCloseCurrentFile( inputFile );
    //if( readStatus != UNZ_OK || status != UNZ_OK ) 
    //    return( NULL );
    return( isOk );
}

bool Controller::importImageFromZip( const QString& imagePath, zipFile inputFile ) {
    int status = unzOpenCurrentFile( inputFile );
    if( status != UNZ_OK )
        return( false );

    bool isOk = true;

    char* buf = NULL;
    uInt size_buf = 8192; // Arbitrary size for now.
    buf = (char*)malloc( size_buf );
    if( buf == NULL ) {
        cerr << "Cannot allocate memory for unzip buffer." << endl;
        status = UNZ_INTERNALERROR;
    }

    if( status == UNZ_OK ) {
        QFile imageFile( imagePath );

        const QString imageDir = QFileInfo( imageFile ).dirPath( true );
        isOk = Util::makeDirectory( imageDir );
        if( isOk ) {
            isOk = imageFile.open( IO_WriteOnly );
            if( isOk ) {
                QTextStream ts( &imageFile );

                int totalByteCount = 0;
                int readStatus;
                for( ;; ) {
                    readStatus = unzReadCurrentFile( inputFile, buf, size_buf );
                    if( readStatus > 0 ) {
                        ts.writeRawBytes( buf, readStatus );
                        totalByteCount += readStatus;
                    }
                    else
                        break;
                }
                imageFile.close();
            }
            else 
                cerr << "Could not create file " << imagePath << endl;
        }
        else
            cerr << "Could not create directory " << imageDir << endl;
    }

    free( buf );

    /*status =*/ unzCloseCurrentFile( inputFile );
    //if( readStatus != UNZ_OK || status != UNZ_OK ) 
    //    return( NULL );
    return( isOk );
}

void Controller::loadData() {
    vocabTree = loadFolder( applicationDirName );

    if( !vocabTree ) {
        Folder* folder = new Folder( -1, QObject::tr( "MyGlossaries" ), Util::createUuid() );
        folder->setMarkedForStudy( true );
        folder->setDirty( true );
        vocabTree = folder;
    }
    loadMarkedItems( vocabTree );
}

void Controller::rightAnswer() {
    if( quiz ) {
        quiz->gradeAnswer( 5 );
        emit( progressChanged( getProgress() ) );
    }
}

void Controller::wrongAnswer() {
    if( quiz ) {
        quiz->gradeAnswer( 0 );
        emit( progressChanged( getProgress() ) );
    }
}

void Controller::gradeAnswer( int grade ) {
    if( quiz ) {
        quiz->gradeAnswer( grade );
        emit( progressChanged( getProgress() ) );
    }
}

void Controller::reveal() {
}

Folder* Controller::loadFolder( const QString& parentDir ) {
    QString obsoleteDataFolderToDelete;
    Folder* newFolder = NULL;

    bool isTopLevelFolder = ( parentDir == applicationDirName );
    QDir dir( parentDir );
    if( !isTopLevelFolder ) {
        bool isOk;
        dir.dirName().toInt( &isOk );

        QString folderMetadataFile;
        if( isOk ) { // isOk means that the old format (without uid) is used.
            folderMetadataFile = QString( parentDir + "/folder-" + dir.dirName() + ".gz" );
            obsoleteDataFolderToDelete = parentDir;
        }
        else 
            folderMetadataFile = QString( parentDir + "/metadata.gz" );

        newFolder = new Folder();
        if( !newFolder->loadMetadata( folderMetadataFile ) ) {
            cerr << "Could not load metadata file " << folderMetadataFile << endl;
            delete( newFolder );
            return( NULL );
        }
    }

    QStringList entries = dir.entryList();
    for( QStringList::Iterator it = entries.begin(); it != entries.end(); it++ ) {
        QString entry = *it;
        QString entryPath = parentDir + "/" + entry;
        QFileInfo info( entryPath );
        if( info.isDir() && entry != "." && entry != ".." ) {
            cerr << "entry=" << entry << " isTop=" << isTopLevelFolder << endl;
            if( isTopLevelFolder ) 
                newFolder = loadFolder( entryPath );
            else { 
                if( entry.left( 2 ) == QString( "v-" ) ) {
                    Vocabulary* childVocab = loadVocabulary( entryPath );
                    if( childVocab )
                        newFolder->add( childVocab );
                }
                else {
                    Folder* childFolder = loadFolder( entryPath );
                    if( childFolder )
                        newFolder->add( childFolder ); 
                }
            }
        }
    }

    // Temporary code for data conversion between 0.11.x and 0.12.x. 
    if( !obsoleteDataFolderToDelete.isNull() ) {
        if( !Util::deleteDirectory( obsoleteDataFolderToDelete ) )
            cerr << "Cannot delete directory " << obsoleteDataFolderToDelete << endl;
    }

    return( newFolder );
}

bool Controller::saveFolder( Folder* folder, const QString& parentDir ) const {
    cerr << "saveFolder folder=" << folder << " location=" << parentDir << " dirty? " << folder->isDirty() << endl;
    // Create the folder.
    QString folderPath( parentDir + QString( "/" ) + folder->getUid().toString() );
    QDir folderDir( folderPath );
    if( folder->isDirty() && !folderDir.exists() ) {
        if( !folderDir.mkdir( folderDir.path() ) ) {
            cerr << "Could not make directory " << folderPath << endl;
            return( false );
        }
    }

    // Write the folder data.
    if( folder->isDirty() ) {
        QString folderDataFilename( QString( folderDir.path() + QString( "/metadata.gz" ) ) );
        if( !folder->saveMetadata( folderDataFilename ) ) {
            cerr << "Could not write folder metadata " << folderDataFilename << endl;
            return( false );
        }
        folder->setDirty( false );
    }

    // Handle children recursively.
    if( !folder->isEmpty() ) {
        for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) {
            if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
                Folder* childFolder = (Folder*)folderChild;
                if( !childFolder->isMarkedForDeletion() ) 
                    saveFolder( childFolder, folderDir.path() );
            }
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
                Vocabulary* childVocab = (Vocabulary*)folderChild;
                if( !childVocab->isMarkedForDeletion() )
                    saveVocabulary( childVocab, folderDir.path() );
            }
        }
    }

    return( true );
}

void Controller::writeVocabulariesInXml( Folder* folder, int depth, QTextStream& ts, QStringList* languages ) {
    if( !folder->isEmpty() ) {
        for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) {
            if( strcmp( folderChild->className(), "Folder" ) == 0 )
                writeVocabulariesInXml( (Folder*)folderChild, depth, ts, languages );
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
                Vocabulary* vocab = (Vocabulary*)folderChild;
                writeVocabularyInXml( ts, *vocab, languages, false, depth );
            }
        }
    }
}

bool Controller::deleteItemsMarkedForDeletion( Folder* folder ) {
    cerr << "deleteItemsMarkedForDeletion folder uid=" << folder->getUid().toString() << endl;
    for( Base* childItem = folder->first(); childItem; childItem = folder->next() ) {
        if( strcmp( childItem->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)childItem;
            deleteItemsMarkedForDeletion( childFolder );
        }
        else if( strcmp( childItem->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)childItem; 
            if( childVocab->isMarkedForDeletion() ) {
                QString vocabDir( applicationDirName + "/" + folder->getPath() + "/v-" + childVocab->getUid().toString() );
                if( !Util::deleteDirectory( vocabDir ) ) {
                    cerr << "Cannot delete glossary directory " << vocabDir << endl;
                    return( false );
                }
            }
            else {
                QValueList<QString> termsToRemove;
                for( Vocabulary::TermMap::Iterator it = childVocab->begin(); it != childVocab->end(); it++ ) {
                    Term& term = it.data();
                    cerr << "term id=" << term.getUid().toString() << term.getTranslation( "en" ).getWord() << " isMarked=" << term.isMarkedForDeletion() << endl;

                    if( term.isMarkedForDeletion() ) {
                        if( !term.getImagePath().isNull() && term.getImagePath().left( 1 ) != "/" ) {
                            const QString& imagePath = getApplicationDirName() + "/" + childVocab->getParent()->getPath() +
                                "/v-" + childVocab->getUid().toString() + "/" + term.getImagePath();
                            QFile imageFile( imagePath );
                            if( imageFile.exists() ) {
                                if( !imageFile.remove() )
                                    cerr << "Could not remove image " << imagePath << endl;
                            }
                        }
                        // We cannot remove the term directly because we have a reference on it.
                        termsToRemove.append( term.getUid().toString() );
                    }
                }
                for( uint i = 0; i < termsToRemove.count(); i++ ) {
                    cerr << "removing term=" << termsToRemove[ i ] << endl;
                    childVocab->removeTerm( termsToRemove[ i ] );
                }
            }
        }
    }

    if( folder->isMarkedForDeletion() ) {
        // To remove the reference to the deleted folder in preferences, we set it as opened.
        getPreferences().setFolderOpen( folder->getUid(), true );
        QString folderDir( applicationDirName + "/" + folder->getPath() );
        if( !Util::deleteDirectory( folderDir ) ) {
            cerr << "Cannot delete folder directory " << folderDir << endl;
            return( false );
        }
    }

    return( true );
}

int Controller::findFolderId( const QString& filename ) const {
    int indexOfDash = filename.find( "-" );
    int indexOfDot = filename.find( "." );
    const QString& strFolderId = filename.mid( indexOfDash + 1, indexOfDot - indexOfDash - 1 );
    bool isOk;
    int folderId = strFolderId.toInt( &isOk );
    return( isOk ? folderId : -1 );
}

QString Controller::findFolderUid( const QString& dirPath ) const {
    int lastSlashPos = dirPath.findRev( "/" );
    return( dirPath.right( dirPath.length() - ( lastSlashPos + 1 ) ) );
}

int Controller::findParentFolderId( const QString& dirPath ) const {
    int lastSlashPos = dirPath.findRev( "/" );
    if( lastSlashPos > 0 ) {
        int beforeLastSlashPos = dirPath.findRev( "/", lastSlashPos - 1 );
        if( beforeLastSlashPos > 0 ) {
            QString strParentFolderId = dirPath.mid( beforeLastSlashPos + 1, lastSlashPos - beforeLastSlashPos - 1 );
            bool isOk;
            int parentFolderId = strParentFolderId.toInt( &isOk );
            return( isOk ? parentFolderId : -1 );
        }
        else {
            QString strParentFolderId = dirPath.left( lastSlashPos );
            bool isOk;
            int parentFolderId = strParentFolderId.toInt( &isOk );
            return( isOk ? parentFolderId : -1 );
        }
    }
    return( -1 );
}

QString Controller::findParentFolderUid( const QString& dirPath ) const {
    int lastSlashPos = dirPath.findRev( "/" );
    if( lastSlashPos > 0 ) {
        int beforeLastSlashPos = dirPath.findRev( "/", lastSlashPos - 1 );
        if( beforeLastSlashPos > 0 )
            return( dirPath.mid( beforeLastSlashPos + 1, lastSlashPos - beforeLastSlashPos - 1 ) );
        else
            return( dirPath.left( lastSlashPos ) );
    }
    return( QString::null );
}

int Controller::findVocabId( const QString& dirPath ) const {
    int posOfLeftDelim = dirPath.find( "v-" );
    int posOfRightDelim = dirPath.findRev( "/" );
    QString strImportedVocabId = dirPath.mid( posOfLeftDelim + 1, posOfRightDelim - posOfLeftDelim - 1 );
    bool isOk;
    int importedVocabId = strImportedVocabId.toInt( &isOk );
    return( isOk ? importedVocabId : -1 );
}

QString Controller::findVocabUid( const QString& dirPath ) const {
    int posOfLeftDelim = dirPath.find( "v-" );
    int posOfRightDelim = dirPath.findRev( "/" );
    return( dirPath.mid( posOfLeftDelim + 1, posOfRightDelim - posOfLeftDelim - 1 ) );
}

QString Controller::convertPath( const QString& path, QMap<int,Folder*>& newFolders ) const {
    QString realPath;
    QStringList subDirs = QStringList::split( "/", path );
    for( QStringList::Iterator it = subDirs.begin(); it != subDirs.end(); it++ ) {
        const QString& strFolderId = (*it);
        bool isOk;
        int subDirFolderId = strFolderId.toInt( &isOk );
        if( isOk && newFolders.contains( subDirFolderId ) ) {
            Folder* assocFolder = newFolders[ subDirFolderId ];
            realPath += "/" + QString::number( assocFolder->getId() );
        }
    }
    return( realPath );
}

QString Controller::convertPath( const QString& path, QMap<QString,Folder*>& newFolders ) const {
    QString realPath;
    QStringList subDirs = QStringList::split( "/", path );
    for( QStringList::Iterator it = subDirs.begin(); it != subDirs.end(); it++ ) {
        const QString& strFolderUid = (*it);
        if( newFolders.contains( strFolderUid ) ) {
            Folder* assocFolder = newFolders[ strFolderUid ];
            realPath += "/" + assocFolder->getUid().toString();
        }
    }
    return( realPath );
}

bool Controller::saveData() {
    cerr << "saveData begin" << endl;
    if( !deleteItemsMarkedForDeletion( vocabTree ) ) {
        // Just write a warning message.  We don't return( false ) here
        // because we want to try to save the new data, at least.
        cerr << "Could not delete all items marked for deletion." << endl;
    }
    cerr << "marked items has been deleted." << endl;

    if( !saveFolder( vocabTree, applicationDirName ) )
        return( false );
    cerr << "term data has been saved ok." << endl;
    
    // Temporary code for data conversion between 0.11.x and 0.12.x. 
    QString tempImageCopyDir( applicationDirName + "/tmp" );
    QFileInfo tempImageCopyDirInfo( tempImageCopyDir );
    if( tempImageCopyDirInfo.exists() ) {
        if( !Util::deleteDirectory( tempImageCopyDir ) )
            cerr << "Cannot remove directory " << tempImageCopyDir << endl;
    }

    if( !saveMarkedItems( vocabTree ) )
        return( false );
    return( prefs.save() );
}

bool Controller::saveMarkedItems( Folder* folder ) {
    QByteArray data;

    QDataStream out( data, IO_WriteOnly );
    out.setVersion( 3 );

    out << Q_UINT32( Preferences::magicNumber ) << Q_UINT16( 0x0012 );

    UidList folderUids;
    UidList vocabUids;
    UidListMap termUids;

    saveMarkedItemsRec( folder, folderUids, vocabUids, termUids );

    out << folderUids << vocabUids << termUids;

    QByteArray compressedData( Util::qCompress( data ) ); 

    QFile dataFile( markedFilename );
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

void Controller::saveMarkedItemsRec( Folder* folder, UidList& folderUids, UidList& vocabUids, UidListMap& termUids ) {
    if( folder->isMarkedForStudy() )
        folderUids.append( folder->getUid().toString() );
    if( !folder->isEmpty() ) {
        for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) {
            if( strcmp( folderChild->className(), "Folder" ) == 0 )
                saveMarkedItemsRec( (Folder*)folderChild, folderUids, vocabUids, termUids );
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 )
                saveMarkedItemsRec( (Vocabulary*)folderChild, vocabUids, termUids );
        }
    }
}

void Controller::saveMarkedItemsRec( Vocabulary* vocab, UidList& vocabUids, UidListMap& termUids ) {
    if( vocab->isMarkedForStudy() )
        vocabUids.append( vocab->getUid().toString() );
    UidList termUidList;
    for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
        const Term& term = it.data();
        if( term.isMarkedForStudy() )
            termUidList.append( term.getUid().toString() );
    }
    termUids.insert( vocab->getUid().toString(), termUidList );
}

void Controller::loadMarkedItems( Folder* folder ) {
    QFile markedXmlFile( markedXmlFilename );
    if( markedXmlFile.exists() ) {
        if( !markedXmlFile.remove() )
            cerr << "Coul not remove file " << markedXmlFilename << ". Status=" << markedXmlFile.status() << endl;
    }

    QFile markedFile( markedFilename );
    if( markedFile.exists() ) {
        if( !markedFile.open( IO_ReadOnly ) )
            return;

        QByteArray compressedData( markedFile.readAll() );
        markedFile.close();
        
        QByteArray data( Util::qUncompress( compressedData ) );

        QDataStream in( data, IO_ReadOnly );

        Q_UINT32 tempMagicNumber;
        Q_UINT16 tempVersion;

        UidList tempFolderUids;
        UidList tempVocabUids;
        UidListMap tempTermUids;

        in >> tempMagicNumber >> tempVersion;

        if( tempMagicNumber != Preferences::magicNumber ) {
            cerr << "Wrong magic number: Incompatible data file for marked file." << endl;
            return;
        }
        if( tempVersion > 0x0012 ) {
            cerr << "Marked data file is from a more recent version.  Upgrade toMOTko." << endl;
            return;
        }
        if( tempVersion < 0x0011 ) {
            cerr << "Marked data file format too old.  You must use an anterior version of toMOTko." << endl;
            return;
        }

        in.setVersion( 3 );
        if( tempVersion == 0x0012 ) {
            in >> tempFolderUids >> tempVocabUids >> tempTermUids; 
            initMarkedForStudyRec( folder, tempFolderUids, tempVocabUids, tempTermUids );
        }
        else if( tempVersion == 0x0011 ) {
            IdList tempFolderIds;
            IdList tempVocabIds;
            IdListMap tempTermIds;

            in >> tempFolderIds >> tempVocabIds >> tempTermIds; 
        
            initMarkedForStudyRec( folder, tempFolderIds, tempVocabIds, tempTermIds );
        }
    }
}

void Controller::initMarkedForStudyRec( Folder* folder, UidList& folderUids, UidList& vocabUids, UidListMap& termUids ) {
    if( folderUids.contains( folder->getUid().toString() ) )
        folder->setMarkedForStudy( true );
    if( !folder->isEmpty() ) {
        for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) {
            if( strcmp( folderChild->className(), "Folder" ) == 0 )
                initMarkedForStudyRec( (Folder*)folderChild, folderUids, vocabUids, termUids );
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 )
                initMarkedForStudyRec( (Vocabulary*)folderChild, vocabUids, termUids );
        }
    }
}

void Controller::initMarkedForStudyRec( Vocabulary* vocab, UidList& vocabUids, UidListMap& termUids ) {
    if( vocabUids.contains( vocab->getUid().toString() ) )
        vocab->setMarkedForStudy( true );
    UidList termUidList = termUids[ vocab->getUid().toString() ];
    for( UidList::ConstIterator it = termUidList.begin(); it != termUidList.end(); it++ ) {
        QString termUidStr = *it;
        QUuid termUid = QUuid( termUidStr );
        if( vocab->isTermExists( termUid ) ) {
            Term& term = vocab->getTerm( termUid );
            term.setMarkedForStudy( true );
        }
    }
}

void Controller::initMarkedForStudyRec( Folder* folder, IdList& folderIds, IdList& vocabIds, IdListMap& termIds ) {
    if( folderIds.contains( folder->getId() ) )
        folder->setMarkedForStudy( true );
    if( !folder->isEmpty() ) {
        for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) {
            if( strcmp( folderChild->className(), "Folder" ) == 0 )
                initMarkedForStudyRec( (Folder*)folderChild, folderIds, vocabIds, termIds );
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 )
                initMarkedForStudyRec( (Vocabulary*)folderChild, vocabIds, termIds );
        }
    }
}

void Controller::initMarkedForStudyRec( Vocabulary* vocab, IdList& vocabIds, IdListMap& termIds ) {
    if( vocabIds.contains( vocab->getId() ) )
        vocab->setMarkedForStudy( true );
    IdList termIdList = termIds[ vocab->getId() ];
    for( IdList::ConstIterator it = termIdList.begin(); it != termIdList.end(); it++ ) {
        int termId = *it;
        if( vocab->isTermExists( termId ) ) {
            Term& term = vocab->getTerm( termId );
            term.setMarkedForStudy( true );
        }
    }
}

bool Controller::exportData( Vocabulary* vocab, const QString& file, QStringList* languages ) const {
    zipFile outputFile = zipOpen( file.latin1(), APPEND_STATUS_CREATE );
    if( outputFile == NULL )
        return( false );

    bool isOk = exportVocabularyIntoZip( vocab, outputFile, QString::null, languages );

    if( zipClose( outputFile, "Closing comment" ) != 0 )
        return( false );

    return( isOk );
}

bool Controller::exportVocabularyIntoZip( Vocabulary* vocab, zipFile outputFile, QString path, QStringList* languages ) const {
    QString vocabPath = ( path == QString::null ? QString( "v-" + vocab->getUid().toString() ) : path + "/v-" + vocab->getUid().toString() );

    // Copy the referred images first.
    for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
        const Term& term = it.data();
        if( term.getImagePath() != QString::null ) {
            QString fileExtension = term.getImagePath().right( 4 );
            QCString imageDataFilename = QString( vocabPath + "/" + term.getUid().toString() + fileExtension ).latin1();
            const char* filenameInZip = (const char*)imageDataFilename.data();
            QString absPath = getResolvedImagePath( term.getImagePath(), *vocab );
            QFile imageFile( absPath );
            if( imageFile.exists() ) {
                if( !imageFile.open( IO_ReadOnly ) )
                    return( false );
                QByteArray buffer = imageFile.readAll();
                imageFile.close();

                int err = writeFileIntoZipFile( outputFile, filenameInZip, buffer.data(), buffer.size() );
                if( err != ZIP_OK )
                    return( false );
            }
            else {
                cerr << "Image " << imageFile.name() << " referenced by glossary " << vocab->getTitle() << 
                    " was not found, and therefore, was discarded during export." << endl;
            }
        }
    }

    // Copy the vocabulary itself in XML.
    QCString dataFilename = QString( vocabPath + "/data.xml" ).latin1();
    const char* filenameInZip = (const char*)dataFilename.data();

    QByteArray buffer;
    QTextStream ts( buffer, IO_WriteOnly );
    ts.setEncoding( QTextStream::UnicodeUTF8 );
    writeVocabularyInXml( ts, *vocab, languages );
    int err = writeFileIntoZipFile( outputFile, filenameInZip, buffer.data(), buffer.size() );

    return( err == ZIP_OK );
}

bool Controller::exportData( Folder* folder, const QString& file, QStringList* languages ) const {
    zipFile outputFile = zipOpen( file.latin1(), APPEND_STATUS_CREATE );
    if( outputFile == NULL )
        return( false );

    bool isOk = exportFolderRecIntoZip( folder, outputFile, QString::null, languages );

    if( zipClose( outputFile, "Closing comment" ) != 0 )
        return( false );

    return( isOk );
}

bool Controller::exportFolderRecIntoZip( Folder* folder, zipFile outputFile, QString path, QStringList* languages ) const {
    QString folderPath = ( path == QString::null ? folder->getUid().toString() : path + QString( "/" ) + folder->getUid().toString() );
    if( !folder->isEmpty() ) {
        QCString folderDataFilename = QString( folderPath + "/metadata.xml" ).latin1();
        const char* filenameInZip = (const char*) ( folderDataFilename.data() );

        QByteArray buffer;
        QTextStream ts( buffer, IO_WriteOnly );
        ts.setEncoding( QTextStream::UnicodeUTF8 );
        writeFolderDataInXml( ts, *folder );

        int err = writeFileIntoZipFile( outputFile, filenameInZip, buffer.data(), buffer.size() );
        if( err != ZIP_OK )
            return( false );
    }

    for( Base* child = folder->first(); child; child = folder->next() ) { 
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)child;
            if( !exportVocabularyIntoZip( childVocab, outputFile, folderPath, languages ) )
                return( false );
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)child;
            if( !exportFolderRecIntoZip( childFolder, outputFile, folderPath, languages ) )
                return( false );
        }
    }

    return( true );
}

int Controller::writeFileIntoZipFile( zipFile outputFile, const char* filename, const char* data, int length ) const {
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

void Controller::writeFolderDataInXml( QTextStream& ts, const Folder& folder ) const {
    ts << QString( "<?xml version=\"1.0\"?>" ) << endl;
    ts << QString( "<folder uid=\"" ) << folder.getUid().toString() << QString( "\" name=\"" ) << Util::escapeXml( folder.getTitle() ) << QString( "\"" );

    if( !folder.getAuthor().isNull() )
        ts << QString( " author=\"" ) << Util::escapeXml( folder.getAuthor() ) << QString( "\"" );
    ts << QString( ">" ) << endl;

    if( !folder.getDescription().isNull() )
        ts << QString( "\t<desc>" ) << Util::escapeXml( folder.getDescription() ) << QString( "</desc>" ) << endl;
    ts << QString( "</folder>" ) << endl;
}

void Controller::writeVocabularyInXml( QTextStream& ts, const Vocabulary& vocab, QStringList* languages, bool writeXmlDirective = true, int depth = 0 ) const {
    if( writeXmlDirective ) {
        for( int i = 0; i < depth; i++ )
            ts << "\t";
        ts << QString( "<?xml version=\"1.0\"?>" ) << endl;
    }

    for( int i = 0; i < depth; i++ )
        ts << "\t";
    ts << QString( "<glossary uid=\"" ) << vocab.getUid().toString() << "\" name=\"" << Util::escapeXml( vocab.getTitle() ) << "\" ";
    ts << QString( "author=\"" ) << Util::escapeXml( vocab.getAuthor() ) << "\">" << endl;

    for( int i = 0; i < depth; i++ )
        ts << "\t";
    ts << QString( "\t<desc>" ) << Util::escapeXml( vocab.getDescription() ) << QString( "</desc>" ) << endl;
    for( Vocabulary::TermMap::ConstIterator it = vocab.begin(); it != vocab.end(); it++ ) {
        const Term& term = it.data();
        for( int i = 0; i < depth; i++ )
            ts << "\t";
        ts << Util::term2Xml( term, languages, depth + 1 );
    }
    for( int i = 0; i < depth; i++ )
        ts << "\t";
    ts << QString( "</glossary>" ) << endl;
}

bool Controller::saveVocabulary( Vocabulary* vocab, const QString& location ) const {
    cerr << "saveVocabulary vocab=" << vocab << " title=" << vocab->getTitle() << " location=" << location << " isDirty? " << vocab->isDirty() << endl;
    // Create the containing folder if needed.
    QString folderPath( location.find( "v-" ) == -1 ? location + QString( "/v-" ) + vocab->getUid().toString() : location );
    QDir folderDir( folderPath );
    if( vocab->isDirty() && !folderDir.exists() ) {
        if( !folderDir.mkdir( folderDir.path() ) ) {
            cerr << "Cannot create directory " << folderPath << endl;
            return( false );
        }
    }

    // Write the vocab data.
    if( vocab->isDirty() ) {
        QString dataFilename( QString( folderDir.path() + QString( "/" ) + QString( "data.gz" ) ) );
        if( !vocab->save( dataFilename ) ) {
            cerr << "Could not write vocab data " << dataFilename << endl;
            return( false );
        }
        vocab->setDirty( false );
    }

    return( true );
}

void Controller::initRevealingSequence() {
    int index = ( rand() % prefs.getActiveRevealingSequenceCount() );
    int sequenceCount = prefs.getRevealingSequenceCount();
    for( int i = 0, j = 0; i < sequenceCount; i++ ) {
        Sequence seq = prefs.getRevealingSequenceAt( i );
        if( seq.isEnabled() ) {
            if( j == index ) {
                currRevealingSeq = seq;
                break;
            }
            else 
                j++;
        }
    }
    currRevealingSeqStep = 0;
}

void Controller::setClipboardData( const QString& type, const QByteArray& data ) {
    clipboardDataType = type;
    clipboard = data;
}

QByteArray Controller::getClipboardData() const {
    return( clipboard );
}

QString Controller::getClipboardDataType() const {
    return( clipboardDataType );
}

bool Controller::isImagePathValid( const QString& path, const Vocabulary& vocab ) const {
    if( !path.isNull() ) {
        QString absPath = getResolvedImagePath( path, vocab );
        QFileInfo info( absPath );
        if( info.exists() ) {
            QString format = QPixmap::imageFormat( absPath );
            if( format == "GIF" || format == "PNG" ) 
                return( true );
        }
    }
    return( false );
}

QString Controller::getResolvedImagePath( const QString& path, const Vocabulary& vocab ) const {
    if( path.isNull() )
        return( QString::null );
    else if( path.left( 1 ) == "/" )
        return( path );
    else {
        QString absPath = getApplicationDirName() + "/" + vocab.getParent()->getPath() + "/v-" + vocab.getUid().toString() + "/" + path;
        return( absPath );
    }
}

void Controller::clearSearch() {
    searchQuery = QString::null;
    searchResults.clear();
}

QValueList<TermKey> Controller::search( const QString& query ) {
    searchQuery = query;
    searchResults.clear();
    QString strippedQuery = query.stripWhiteSpace();
    if( !strippedQuery.isEmpty() )
        searchRec( strippedQuery, vocabTree, searchResults );  
    return( searchResults );
}

void Controller::searchRec( const QString& query, Folder* folder, QValueList<TermKey>& results ) {
    if( !folder || folder->isMarkedForDeletion() )
        return;

    for( Base* child = folder->first(); child; child = folder->next() ) { 
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)child;
            searchRec( query, childVocab, results );
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)child;
            searchRec( query, childFolder, results );
        }
    }

}

void Controller::searchRec( const QString& query, Vocabulary* vocab, QValueList<TermKey>& results ) {
    if( !vocab || vocab->isMarkedForDeletion() )
        return;

    for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
        const Term& term = *it;

        if( term.isMarkedForDeletion() )
            continue;

        bool isStringFound = false;

        if( prefs.isLanguageFilterEnabled() ) {
            const QString& firstLang = prefs.getFirstLanguage();
            const QString& testLang = prefs.getTestLanguage();
            if( term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) ) {
                const Translation& firstLangTrans = term.getTranslation( firstLang );
                const Translation& testLangTrans = term.getTranslation( testLang );
                if( firstLangTrans.getWord().find( query ) != -1 || firstLangTrans.getAlt().find( query ) != -1 ||
                    testLangTrans.getWord().find( query ) != -1 || testLangTrans.getAlt().find( query ) != -1 ) {
                    isStringFound = true;
                }
            }
        }
        else {
            for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
                const Translation& trans = it2.data();
                if( trans.getWord().find( query ) != -1 || trans.getAlt().find( query ) != -1 ) {
                    isStringFound = true;
                    break;
                }
            }
        }

        if( !isStringFound ) {
            if( prefs.isLanguageFilterEnabled() ) {
                const QString& firstLang = prefs.getFirstLanguage();
                const QString& testLang = prefs.getTestLanguage();
                BilingualKey key( firstLang, testLang );
                if( term.isCommentExists( key ) && term.getComment( key ).find( query ) != -1 )
                    isStringFound = true;
            }
            else {
                for( Term::CommentMap::ConstIterator it = term.commentsBegin(); it != term.commentsEnd(); it++ ) {
                    const QString& comment = it.data();
                    if( comment.find( query ) != -1 ) {
                        isStringFound = true;
                        break;
                    }
                }
            }
        }

        if( isStringFound ) {
            TermKey termKey( term.getUid(), vocab->getUid() );
            results.append( termKey );
        }
    }
}

QValueList<TermKey> Controller::getSearchResults() const {
    return( searchResults ); 
}

int Controller::getSearchResultsCount() const {
    return( searchResults.count() );
}
