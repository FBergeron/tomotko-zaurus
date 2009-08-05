#include "Folder.h"

Folder::Folder( int id /* = -1 */, const QString& title /* = QString::null */ , const QUuid& uid /* = QUuid() */ ) 
    : Base(), uid( uid ), id( id ), title( title ), markedForStudy( false ), markedForDeletion( false ), dirty( false ), parent( NULL ) {
    QDateTime now( QDateTime::currentDateTime() );
    creationDate = modificationDate = now;
}

Folder::Folder( const Folder& folder ) 
    : Base(), uid( folder.uid ), id( folder.id ), title( folder.title ), description( folder.description ), author( folder.author ),
        creationDate( folder.creationDate ), modificationDate( folder.modificationDate ), 
            markedForStudy( folder.isMarkedForStudy() ), markedForDeletion( folder.markedForDeletion ), dirty( folder.dirty ), parent( folder.parent ) {
    for( QListIterator<Base> it( folder.children ); it.current(); ++it ) {
        const Base* childItem = it.current();
        if( strcmp( childItem->className(), "Folder" ) == 0 ) {
            Folder* childFolder = new Folder( *((Folder*)childItem) );
            add( childFolder );
        }
        else if( strcmp( childItem->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = new Vocabulary( *((Vocabulary*)childItem) ); 
            add( childVocab );
        }
    }
}

Folder::~Folder() {
}

QUuid Folder::getUid() const {
    return( uid );
}

int Folder::getId() const {
    return( id );
}

const QString Folder::getTitle() const {
    return( title );
    //return( title + QString( "(" ) + getUid().toString() + QString( ")" ) ); // For debugging.
    //return( title + QString( "(" ) + QString::number( getId() ) + QString( ")" ) ); // For debugging.
}

void Folder::setTitle( const QString& title ) {
    this->title = title;
}

const QString Folder::getDescription() const {
    return( description );
}

void Folder::setDescription( const QString& desc ) {
    this->description = desc;
}

const QString Folder::getAuthor() const {
    return( author );
}

void Folder::setAuthor( const QString& author ) {
    this->author = author;
}

const QDateTime Folder::getCreationDate() const {
    return( creationDate );
}

void Folder::setCreationDate( const QDateTime& creationDate ) {
    this->creationDate = creationDate;
}

const QDateTime Folder::getModificationDate() const {
    return( modificationDate );
}

void Folder::setModificationDate( const QDateTime& modificationDate ) {
    this->modificationDate = modificationDate;
}

bool Folder::isMarkedForStudy() const {
    return( markedForStudy );
}

void Folder::setMarkedForStudy( bool isMarkedForStudy ) {
    markedForStudy = isMarkedForStudy;
}

bool Folder::isMarkedForDeletion() const {
    return( markedForDeletion );
}

void Folder::setMarkedForDeletion( bool isMarkedForDeletion ) {
    markedForDeletion = isMarkedForDeletion;
    for( QListIterator<Base> it( children ); it.current(); ++it ) {
        const Base* folderChild = it.current();
        if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)folderChild;
            childFolder->setMarkedForDeletion( isMarkedForDeletion );
        }
        else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)folderChild;
            childVocab->setMarkedForDeletion( isMarkedForDeletion );
        }
    }

}

bool Folder::containsTermWithTranslations( const QString& lang1, const QString& lang2 ) {
    for( Base* child = children.first(); child; child = children.next() ) {
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* vocab = (Vocabulary*)child;
            if( vocab->containsTermWithTranslations( lang1, lang2 ) )
                return( true );
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* folder = (Folder*)child;
            if( folder->containsTermWithTranslations( lang1, lang2 ) )
                return( true );
        }
    }
    return( false );
}

bool Folder::isReachableFromRoot() const {
    if( !isMarkedForStudy() )
        return( false );
    const Folder* folder = this;
    for( ;; ) {
        folder = folder->getParent();
        if( !folder )
            return( true );
        if( !folder->isMarkedForStudy() )
            return( false );
    } 
}

Folder* Folder::getFolder( const QUuid& uid ) {
    if( getUid() == uid )
        return( this );

    for( Base* child = children.first(); child; child = children.next() ) { 
        if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)child;
            Folder* folder = childFolder->getFolder( uid );
            if( folder )
                return( folder );
        }
    }

    return( NULL );
}

Folder* Folder::getFolder( int id ) {
    if( getId() == id )
        return( this );

    for( Base* child = children.first(); child; child = children.next() ) { 
        if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)child;
            Folder* folder = childFolder->getFolder( id );
            if( folder )
                return( folder );
        }
    }

    return( NULL );
}

QStringList Folder::getTranslationLanguages() {
    QStringList languages;
    for( Base* child = children.first(); child; child = children.next() ) {
        QStringList childLanguages;
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* vocab = (Vocabulary*)child;
            childLanguages = vocab->getTranslationLanguages();
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* folder = (Folder*)child;
            childLanguages = folder->getTranslationLanguages();
        }
        for( QStringList::ConstIterator it = childLanguages.begin(); it != childLanguages.end(); it++ ) {
            const QString& lang = *it;
            if( !languages.contains( lang ) )
                languages.append( lang );
        }
    }
    return( languages );
}

void Folder::removeTranslations( const QStringList& languages ) {
    for( Base* child = children.first(); child; child = children.next() ) {
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* vocab = (Vocabulary*)child;
            vocab->removeTranslations( languages );
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* folder = (Folder*)child;
            folder->removeTranslations( languages );
        }
    }
}

void Folder::add( Vocabulary* child ) {
    children.append( child );
    child->setParent( this );
}

void Folder::add( Folder* child ) {
    children.append( child );
    child->setParent( this );
}

void Folder::remove( Vocabulary* child ) {
    children.remove( child );
    delete( child );
}

void Folder::remove( Folder* child ) {
    children.remove( child );
    delete( child );
}

bool Folder::isEmpty() const {
    return( children.isEmpty() );
}

int Folder::getChildrenCount() const {
    return( children.count() );
}

Vocabulary* Folder::getVocabulary( const QUuid& uid ) {
    return( getVocabularyRec( uid ) );
}

Vocabulary* Folder::getVocabulary( int id ) {
    return( getVocabularyRec( id ) );
}

Base* Folder::first() {
    return( children.first() );
}

Base* Folder::next() {
    return( children.next() );
}

bool Folder::loadMetadata( const QString& filename ) {
    QFile dataFile( filename );
    if( !dataFile.open( IO_ReadOnly ) ) {
        cerr << "Cannot open metadata file: " << filename << endl;
        return( false );
    }
    
    QByteArray compressedData( dataFile.readAll() );
    dataFile.close();

    QByteArray data( Util::qUncompress( compressedData ) );

    QDataStream in( data, IO_ReadOnly );

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
    QString tempUidStr;
    QUuid tempUid;
    int tempId;
    QString tempTitle;
    QString tempDescription;
    QString tempAuthor;
    QDateTime tempCreationDate;
    QDateTime tempModificationDate;

    in >> tempMagicNumber >> tempVersion;
    if( tempMagicNumber != Folder::magicNumber ) {
        cerr << "Wrong magic number: Incompatible folder data file." << endl;
        return( false );
    }
    if( tempVersion > 0x0011 ) {
        cerr << "Folder data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( false );
    }
    if( tempVersion < 0x0010 ) {
        cerr << "Folder format too old.  You must use an anterior version of toMOTko." << endl;
        return( false );
    }

    in.setVersion( 3 );
    if( tempVersion == 0x0011 ) {
        in >> tempUidStr;
        tempUid = QUuid( tempUidStr );
    }
    else if( tempVersion == 0x0010 ) {
        Vocabulary::parentPath = filename; // This hack will be used by Term to retrieve the image.
        tempUid = Util::createUuid();
        in >> tempId;
        dirty = true; // To save the folder in the new format.
    }
    in >> tempTitle >> tempDescription >> tempAuthor >> tempCreationDate >> tempModificationDate;

    uid = tempUid;
    id = tempId;
    title = tempTitle;
    description = tempDescription;
    author = tempAuthor;
    creationDate = tempCreationDate;
    modificationDate = tempModificationDate;
//cerr << "tempUid=" << uid.toString() << " id=" << id << " title=" << title << " desc=" << description << " author=" << author << " crD=" << creationDate.toString() << " modD=" << modificationDate.toString() << endl;

    return( true );
}

bool Folder::saveMetadata( const QString& filename ) const {
    QByteArray data;

    QDataStream out( data, IO_WriteOnly );
    out.setVersion( 3 /* QDataStream::Qt_3 ? */ );

    out << Q_UINT32( Folder::magicNumber ) << Q_UINT16( 0x0011 );
    out << getUid().toString() << getTitle() << getDescription() << getAuthor() << getCreationDate() << getModificationDate();  

    QByteArray compressedData( Util::qCompress( data ) ); 

    QFile dataFile( filename );
    QFileInfo dataFileInfo( dataFile );

    if( !Util::makeDirectory( dataFileInfo.dirPath() ) )
        return( false );

    if( !dataFile.open( IO_WriteOnly ) )
        return( false );

    int ret = dataFile.writeBlock( compressedData );
    dataFile.close();

    // Temporary code for data conversion between 0.11.x and 0.12.x. 
    Vocabulary::parentPath = filename; // This hack will be used by Term to copy images from temporary directory to vocab's directory.

    if( ret == -1 || dataFile.status() != IO_Ok ) {
        dataFile.resetStatus();
        return( false );
    }

    return( true );
}

void Folder::buildVocabCopiesMap( QMap<QString,Vocabulary>& vocabularies ) const {
    for( QListIterator<Base> it( children ); it.current(); ++it ) {
        const Base* folderChild = it.current();
        if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
            Folder* childFolder = (Folder*)folderChild;
            childFolder->buildVocabCopiesMap( vocabularies );
        }
        else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* childVocab = (Vocabulary*)folderChild;
            vocabularies.insert( childVocab->getUid().toString(), *childVocab );
        }
    }
}

Folder* Folder::getRoot() const {
    if( getParent() )
        return( getParent()->getRoot() );
    return( (Folder*)this );
}

bool Folder::isDirty() const {
    return( dirty );
}

void Folder::setDirty( bool isDirty, bool recursive = false ) {
    dirty = isDirty;
    if( recursive ) {
        for( Base* child = children.first(); child; child = children.next() ) { 
            if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
                Vocabulary* childVocab = (Vocabulary*)child;
                childVocab->setDirty( isDirty );
            }
            else if( strcmp( child->className(), "Folder" ) == 0 ) {
                Folder* childFolder = ((Folder*)child);
                childFolder->setDirty( isDirty, true );
            }
        }
    }
}

Folder* Folder::getParent() const {
    return( parent );
}

QString Folder::getOldPath() const {
    Folder* parentFolder = getParent();
    if( parentFolder )
        return( QString( parentFolder->getPath() + QString( "/" ) + QString::number( getId() ) ) );
    else
        return( QString::number( getId() ) );
}

QString Folder::getPath() const {
    Folder* parentFolder = getParent();
    if( parentFolder )
        return( QString( parentFolder->getPath() + QString( "/" ) + getUid().toString() ) );
    else
        return( getUid().toString() );
}

QString Folder::getHumanReadablePath() const {
    Folder* rootFolder = getRoot();
    QString str;
    QStringList folders = QStringList::split( "/", getPath() );
    QString delim = "";
    for( QStringList::Iterator it = folders.begin(); it != folders.end(); it++ ) {
        QString strFolderId = *it;
        bool isFolderIdValid;
        int folderId = strFolderId.toInt( &isFolderIdValid, 10 );
        if( isFolderIdValid ) {
            Folder* folder = rootFolder->getFolder( folderId );
            if( folder ) {
                str += delim + folder->getTitle();
                delim = "/";
            }
        }
    }
    return( str ); 
}

void Folder::setParent( Folder* parent ) {
    this->parent = parent;
}

/*
uint Folder::getTermCount() {
    uint termCount = 0;
    for( Base* child = children.first(); child; child = children.next() ) { 
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* vocab = (Vocabulary*)child;
            termCount += vocab->getSize();
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* folder = (Folder*)child;
            termCount += folder->getTermCount();
        }
    }
    return( termCount );
}

uint Folder::getVocabularyCount() {
    uint vocabCount = 0;
    for( Base* child = children.first(); child; child = children.next() ) { 
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            vocabCount += 1; 
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Folder* folder = (Folder*)child;
            vocabCount += folder->getVocabularyCount();
        }
    }
    return( vocabCount );
}

uint Folder::getFolderCount() {
    uint folderCount = 0;
    for( Base* child = children.first(); child; child = children.next() ) { 
        if( strcmp( child->className(), "Folder" ) == 0 ) {
            folderCount += 1;
        }
    }
    return( folderCount );
}
*/

void Folder::getItemsCount( uint* termCount, uint* vocabCount, uint* folderCount, 
        uint* checkedTermCount, uint* checkedVocabCount, uint* checkedFolderCount, 
            uint* selectedTermCount, uint* selectedVocabCount, uint* selectedFolderCount, 
                bool isReachableFromRoot,
                    const QString& firstLang = QString::null, const QString& testLang = QString::null ) {
    if( !isMarkedForDeletion() ) {
        for( Base* child = children.first(); child; child = children.next() ) { 
            if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
                Vocabulary* vocab = (Vocabulary*)child;
                if( !vocab->isMarkedForDeletion() ) {
                    if( !firstLang.isNull() && !testLang.isNull() ) {
                        if( vocab->containsTermWithTranslations( firstLang, testLang ) ) {
                            *vocabCount += 1;
                            if( vocab->isMarkedForStudy() ) {
                                *checkedVocabCount += 1;
                                if( isReachableFromRoot )
                                    *selectedVocabCount += 1;
                            }
                        }
                    }
                    else {
                        *vocabCount += 1;
                        if( vocab->isMarkedForStudy() ) {
                            *checkedVocabCount += 1;
                            if( isReachableFromRoot )
                                *selectedVocabCount += 1;
                        }
                    }
                    vocab->getItemsCount( termCount, checkedTermCount, selectedTermCount, isReachableFromRoot && vocab->isMarkedForStudy(), firstLang, testLang );
                }
            }
            else if( strcmp( child->className(), "Folder" ) == 0 ) {
                Folder* folder = (Folder*)child;
                if( !folder->isMarkedForDeletion() ) {
                    if( !firstLang.isNull() && !testLang.isNull() ) {
                        if( folder->containsTermWithTranslations( firstLang, testLang ) ) {
                            *folderCount += 1;
                            if( folder->isMarkedForStudy() ) {
                                *checkedFolderCount += 1;
                                if( isReachableFromRoot )
                                    *selectedFolderCount += 1;
                            }
                        }
                    }
                    else {
                        *folderCount += 1;
                        if( folder->isMarkedForStudy() ) {
                            *checkedFolderCount += 1;
                            if( isReachableFromRoot )
                                *selectedFolderCount += 1;
                        }
                    }
                    folder->getItemsCount( termCount, vocabCount, folderCount, 
                        checkedTermCount, checkedVocabCount, checkedFolderCount,
                            selectedTermCount, selectedVocabCount, selectedFolderCount, 
                                isReachableFromRoot && folder->isMarkedForStudy(), firstLang, testLang );
                }
            }
        }
    }
}

Vocabulary* Folder::getVocabularyRec( const QUuid& uid ) {
    for( Base* child = children.first(); child; child = children.next() ) { 
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* vocab = (Vocabulary*)child;
            if( vocab->getUid() == uid )
                return( vocab );
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Vocabulary* vocab = ((Folder*)child)->getVocabularyRec( uid );
            if( vocab )
                return( vocab );
        }
    }
    return( NULL );
}

Vocabulary* Folder::getVocabularyRec( int id ) {
    for( Base* child = children.first(); child; child = children.next() ) { 
        if( strcmp( child->className(), "Vocabulary" ) == 0 ) {
            Vocabulary* vocab = (Vocabulary*)child;
            if( vocab->getId() == id )
                return( vocab );
        }
        else if( strcmp( child->className(), "Folder" ) == 0 ) {
            Vocabulary* vocab = ((Folder*)child)->getVocabularyRec( id );
            if( vocab )
                return( vocab );
        }
    }
    return( NULL );
}

QDataStream& operator<<( QDataStream& out, const Folder& folder ) {
    out << folder.getUid().toString() << folder.getTitle() << folder.getDescription() << folder.getAuthor();
    out << folder.getCreationDate() << folder.getModificationDate();
    if( !folder.isEmpty() ) {
        for( QListIterator<Base> it( folder.children ); it.current(); ++it ) {
            const Base* folderChild = it.current();
            if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
                // This deliminer indicates that what follows is a folder.
                // This is required as the structure is recursive and heterogeous (contains folder and vocabularies).
                out << QString( "F" );
                out << *((const Folder*)folderChild);
            }
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
                // This deliminer indicates that what follows is a folder.
                // This is required as the structure is recursive and heterogeous (contains folder and vocabularies).
                out << QString( "V" );

                // We just write the vocabulary's uid as a reference.
                // Vocabularies are stored separately in their own files to reduce i/o operations.
                out << ((Vocabulary*)folderChild)->getUid().toString();
            }
        }
    }

    out << QString( "EOF" ); // End of Folder delimiter.

    return( out );
}

QDataStream& operator>>( QDataStream& in, Folder& folder ) {
    QString tempUidStr;
    QUuid tempUid;
    QString tempTitle;
    QString tempDescription;
    QString tempAuthor;
    QDateTime tempCreationDate;
    QDateTime tempModificationDate;
    QString tempChildType;
    Folder tempChildFolder;
    QString tempVocabUidStr;
    QUuid tempVocabUid;

    in >> tempUidStr;
    tempUid = QUuid( tempUidStr );

    in >> tempTitle >> tempDescription >> tempAuthor;
    in >> tempCreationDate >> tempModificationDate;

    folder = Folder( -1, tempTitle, tempUid );
    folder.setDescription( tempDescription );
    folder.setAuthor( tempAuthor );
    folder.setCreationDate( tempCreationDate );
    folder.setModificationDate( tempModificationDate );

    in >> tempChildType;
    while( tempChildType != "EOF" ) {
        if( tempChildType == "F" ) {
            in >> tempChildFolder;
            Folder* childFolder = new Folder( tempChildFolder );
            folder.add( childFolder );
        }
        else if( tempChildType == "V" ) {
            in >> tempVocabUidStr;
            tempVocabUid = QUuid( tempVocabUidStr );
            Vocabulary* childVocab = new Vocabulary( -1, QString::null, tempVocabUid );
            folder.add( childVocab );
        }
        in >> tempChildType;
    }

    return( in );
}
