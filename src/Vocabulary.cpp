#include "Vocabulary.h"

QString Vocabulary::parentPath = QString::null; // Temporary variable for data conversion from 0.11.x to 0.12.x.

Vocabulary::Vocabulary( int id, const QString& title /* = QString::null */, const QUuid& uid ) 
    : Base(), uid( uid ), id( id ), markedForStudy( false ), markedForDeletion( false ), 
        title( title ), description( QString::null ), author( QString::null ), 
            dirty( false ) {
    QDateTime now( QDateTime::currentDateTime() );
    creationDate = modificationDate = now;
}

Vocabulary::Vocabulary( const Vocabulary& voc ) 
    : Base(), uid( voc.uid ), id( voc.id ), markedForStudy( voc.markedForStudy ), markedForDeletion( voc.markedForDeletion ), 
        title( voc.title ), description( voc.description ), author( voc.author ), 
            creationDate( voc.creationDate ), modificationDate( voc.modificationDate ), dirty( voc.dirty ), parent( voc.parent ) {
    for( TermMap::ConstIterator it = voc.terms.begin(); it != voc.terms.end(); it++ ) {
        const Term& term = it.data();
        addTerm( term );
    }
}

Vocabulary::~Vocabulary() {
}

QUuid Vocabulary::getUid() const {
    return( uid );
}

int Vocabulary::getId() const {
    return( id );
}

bool Vocabulary::isMarkedForStudy() const {
    return( markedForStudy );
}

void Vocabulary::setMarkedForStudy( bool isMarkedForStudy ) {
    markedForStudy = isMarkedForStudy;
}

bool Vocabulary::isMarkedForDeletion() const {
    return( markedForDeletion );
}

void Vocabulary::setMarkedForDeletion( bool isMarkedForDeletion ) {
    markedForDeletion = isMarkedForDeletion;
}

const QString Vocabulary::getTitle() const {
    return( title );
    //return( title + QString( "(" ) + getUid().toString() + QString( ")" ) ); // For debugging.
}

void Vocabulary::setTitle( const QString& title ) {
    this->title = title;
}

const QString Vocabulary::getDescription() const {
    return( description );
}

void Vocabulary::setDescription( const QString& desc ) {
    this->description = desc;
}

const QString Vocabulary::getAuthor() const {
    return( author );
}

void Vocabulary::setAuthor( const QString& author ) {
    this->author = author;
}

const QDateTime Vocabulary::getCreationDate() const {
    return( creationDate );
}

void Vocabulary::setCreationDate( const QDateTime& creationDate ) {
    this->creationDate = creationDate;
}

const QDateTime Vocabulary::getModificationDate() const {
    return( modificationDate );
}

void Vocabulary::setModificationDate( const QDateTime& modificationDate ) {
    this->modificationDate = modificationDate;
}

void Vocabulary::addTerm( const Term& term ) {
    terms.insert( term.getUid().toString(), term );
}

void Vocabulary::removeTerm( const QUuid& uid ) {
    terms.remove( uid.toString() );
}

bool Vocabulary::isTermExists( const QUuid& uid ) const {
    return( terms.contains( uid.toString() ) );
}

bool Vocabulary::isTermExists( const int& id ) const {
    for( TermMap::ConstIterator it = terms.begin(); it != terms.end(); it++ ) {
        const Term& term = it.data();
        if( term.getId() == id )
            return( true );
    }
    return( false );
}

Term& Vocabulary::getTerm( const QUuid& uid ) {
    return( terms[ uid.toString() ] );
}

Term& Vocabulary::getTerm( const int& id ) {
    for( TermMap::Iterator it = terms.begin(); it != terms.end(); it++ ) {
        Term& term = it.data();
        if( term.getId() == id )
            return( term );
    }
}

bool Vocabulary::isEmpty() const {
    return( terms.isEmpty() );
}

const uint Vocabulary::getSize() const {
    return( terms.count() );
}

void Vocabulary::getItemsCount( uint* termCount, uint* checkedTermCount, uint* selectedTermCount, bool isReachableFromRoot, const QString& firstLang = QString::null, const QString& testLang = QString::null ) const {
    if( !isMarkedForDeletion() ) {
        for( TermMap::ConstIterator it = terms.begin(); it != terms.end(); it++ ) {
            const Term& term = it.data();
            if( !term.isMarkedForDeletion() ) {
                if( !firstLang.isNull() && !testLang.isNull() ) {
                    if( term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) ) {
                        *termCount += 1;
                        if( term.isMarkedForStudy() ) {
                            *checkedTermCount += 1;
                            if( isReachableFromRoot )
                                *selectedTermCount += 1;
                        }
                    }
                }
                else {
                    *termCount += 1;
                    if( term.isMarkedForStudy() ) {
                        *checkedTermCount += 1;
                        if( isReachableFromRoot )
                            *selectedTermCount += 1;
                    }
                }
            }
        }
    }
}

bool Vocabulary::containsTermWithTranslations( const QString& lang1, const QString& lang2 ) const {
    if( !isMarkedForDeletion() ) {
        for( Vocabulary::TermMap::ConstIterator it = terms.begin(); it != terms.end(); it++ ) {
            const Term& term = it.data();
            if( !term.isMarkedForDeletion() && term.isTranslationExists( lang1 ) && term.isTranslationExists( lang2 ) )
                return( true );
        }
    }
    return( false );
}

QStringList Vocabulary::getTranslationLanguages() const {
    QStringList languages;
    if( !isMarkedForDeletion() ) {
        for( TermMap::ConstIterator it = begin(); it != end(); it++ ) {
            const Term& term = it.data();
            if( !term.isMarkedForDeletion() ) {
                for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
                    const Translation& trans = it2.data();
                    if( !languages.contains( trans.getLanguage() ) )
                        languages.append( trans.getLanguage() );
                }
            }
        }
    }
    return( languages );
}

void Vocabulary::removeTranslations( const QStringList& languages ) {
    for( TermMap::Iterator it = begin(); it != end(); it++ ) {
        Term& term = it.data();
        if( term.isMarkedForDeletion() ) {
            for( QStringList::ConstIterator it2 = languages.begin(); it2 != languages.end(); it2++ ) {
                const QString& lang = *it2;
                term.removeTranslation( lang );
            }
        }
    }
}

Vocabulary::TermMap::ConstIterator Vocabulary::begin() const {
    return( terms.begin() );
}

Vocabulary::TermMap::ConstIterator Vocabulary::end() const {
    return( terms.end() );
}

Vocabulary::TermMap::Iterator Vocabulary::begin() {
    return( terms.begin() );
}

Vocabulary::TermMap::Iterator Vocabulary::end() {
    return( terms.end() );
}

bool Vocabulary::isDirty() const {
    return( dirty );
}

void Vocabulary::setDirty( bool isDirty ) {
    dirty = isDirty;
}

Folder* Vocabulary::getParent() const {
    return( parent );
}

void Vocabulary::setParent( Folder* parent ) {
    this->parent = parent;
}

bool Vocabulary::load( const QString& filename ) {
    QFile dataFile( filename );
    if( !dataFile.open( IO_ReadOnly ) )
        return( false );
    
    QByteArray compressedData( dataFile.readAll() );
    dataFile.close();
    QByteArray data( Util::qUncompress( compressedData ) );

    QDataStream in( data, IO_ReadOnly );
    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
    Vocabulary tempVocab;

    in >> tempMagicNumber >> tempVersion;
    if( tempMagicNumber != Vocabulary::magicNumber ) {
        cerr << "Wrong magic number: Incompatible vocabulary data file." << endl;
        return( false );
    }
    if( tempVersion > 0x0011 ) {
        cerr << "Vocabulary data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( false );
    }
    if( tempVersion < 0x0010 ) {
        cerr << "Vocabulary format too old.  You must use an anterior version of toMOTko." << endl;
        return( false );
    }

    in.setVersion( 3 );
    if( tempVersion == 0x0011 ) // With UID.
        in >> tempVocab;
    else if( tempVersion < 0x0011 )
        readOldFormat( in, tempVocab, tempVersion ); 

    uid = tempVocab.getUid();
    id = tempVocab.getId();
    markedForStudy = tempVocab.isMarkedForStudy();
    title = tempVocab.getTitle();
    description = tempVocab.getDescription();
    author = tempVocab.getAuthor();
    creationDate = tempVocab.getCreationDate();
    modificationDate = tempVocab.getModificationDate();
    dirty = tempVocab.isDirty();
    for( TermMap::Iterator it = tempVocab.begin(); it != tempVocab.end(); it++ ) {
        Term& term = it.data();
        term.setVocabUid( uid );
for( QMap<QString,Translation>::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
    const Translation& trans = it2.data();
    cerr << "termUid=" << term.getUid().toString() << " transUid=" << trans.getUid().toString() << endl;
}
        addTerm( term );
    }
    return( true );
}

bool Vocabulary::save( const QString& filename ) const {
    QByteArray data;

    QDataStream out( data, IO_WriteOnly );
    out.setVersion( 3 /* QDataStream::Qt_3 ? */ );
//cerr << "Size of saved Vocab=" << getSize() << " this=" << this << endl;
//for( TermMap::ConstIterator it = terms.begin(); it != terms.end(); it++ ) {
//    const Term& term = it.data();
//    cerr << "term uid=" << term.getUid().toString() << endl;
//}
    out << Q_UINT32( Vocabulary::magicNumber ) << Q_UINT16( 0x0011 ) << *this;

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
    for( Vocabulary::TermMap::ConstIterator it = terms.begin(); it != terms.end(); it++ ) {
        const Term& term = it.data();
        if( !term.getImagePath().isNull() ) {
            QFileInfo imageFileInfo( term.getImagePath() );
            if( term.getImagePath() == term.getUid().toString() + "." + imageFileInfo.extension( false ) ) {
                QFileInfo parentPathInfo( Vocabulary::parentPath );
                QString absImagePath( parentPathInfo.dirPath() + "/v-" + getUid().toString() + "/" + term.getImagePath() );
                QFileInfo absImagePathInfo( absImagePath );
                if( !absImagePathInfo.exists() ) {
                    QString applDir( parentPathInfo.dirPath().left( parentPathInfo.dirPath().find( ".toMOTko" ) + 8 ) );
                    QString tempDirPath( applDir + "/tmp" );
                    QString tempImageCopyPath( tempDirPath + "/" + term.getUid().toString() + "." + imageFileInfo.extension( false ) );
                    QFileInfo tempImageCopyPathInfo( tempImageCopyPath );
                    if( tempImageCopyPathInfo.exists() ) {
                        if( !Util::copy( tempImageCopyPath, absImagePath ) )
                            cerr << "Cannot copy image " << tempImageCopyPath << " to " << absImagePath << endl;
                    }
                }
            }
        }
    }

    if( ret == -1 || dataFile.status() != IO_Ok ) {
        dataFile.resetStatus();
        return( false );
    }

    return( true );
}

QDataStream& operator<<( QDataStream& out, const Vocabulary& vocab ) {
    out << vocab.uid.toString() << vocab.title << vocab.terms;
    out << vocab.description << vocab.author << vocab.creationDate << vocab.modificationDate;

    return( out );
}

QDataStream& operator>>( QDataStream& in, Vocabulary& vocab ) {
    QString tempUidStr;
    QUuid tempUid;
    QString tempTitle;
    Vocabulary::TermMap tempTerms;
    QString tempDescription;
    QString tempAuthor;
    QDateTime tempCreationDate;
    QDateTime tempModificationDate;

    in >> tempUidStr;
    tempUid = QUuid( tempUidStr );
    in >> tempTitle >> tempTerms;

    in >> tempDescription >> tempAuthor >> tempCreationDate >> tempModificationDate;

    vocab = Vocabulary( -1, tempTitle, tempUid );
    for( Vocabulary::TermMap::ConstIterator it = tempTerms.begin(); it != tempTerms.end(); it++ ) {
        const Term& term = it.data();
        vocab.addTerm( term );
    }
   
    vocab.setDescription( tempDescription );
    vocab.setAuthor( tempAuthor );
    vocab.setCreationDate( tempCreationDate );
    vocab.setModificationDate( tempModificationDate );

    return( in );
}

QDataStream& readOldFormat( QDataStream& in, Vocabulary& vocab, Q_UINT16 version ) {
    QUuid tempUid;
    int tempId;
    QString tempTitle;
    Vocabulary::TermMap tempTerms;
    Vocabulary::OldTermMap tempOldTerms;
    QString tempDescription;
    QString tempAuthor;
    QDateTime tempCreationDate;
    QDateTime tempModificationDate;
    if( version == 0x0010 )
        tempUid = Util::createUuid();

    Term::isOldFormat = true; // Set the conversion flag to make Term's operator>> behave correctly. 
    Translation::isOldFormat = true; // Set the conversion flag to make Translation's operator>> behave correctly.
    in >> tempId >> tempTitle >> tempOldTerms;

    Term::isOldFormat = false; // Reset the conversion flag.
    Translation::isOldFormat = false; // Reset the conversion flag.

    in >> tempDescription >> tempAuthor >> tempCreationDate >> tempModificationDate;

    vocab = Vocabulary( tempId, tempTitle, tempUid );
    for( Vocabulary::OldTermMap::Iterator it = tempOldTerms.begin(); it != tempOldTerms.end(); it++ ) {
        Term& term = it.data();
        term.setUid( Util::createUuid() );
        term.setVocabUid( tempUid );
        // We need to generate uid for translation instances for this version.
        if( version == 0x0010 ) {
            QValueList<Translation> convertedTranslations;
            for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
                const Translation& trans = it2.data();
                Translation convertedTrans( Util::createUuid(), trans.getLanguage(), trans.getWord(), trans.getAlt() );
                convertedTranslations.append( convertedTrans );
            }
            // Replace the old translation instances by converted ones.
            for( QValueList<Translation>::Iterator it2 = convertedTranslations.begin(); it2 != convertedTranslations.end(); it2++ ) {
                Translation trans = *it2;
                term.removeTranslation( trans.getLanguage() );
                term.addTranslation( trans );
            }
        }
        if( !term.getImagePath().isNull() ) {
            QFileInfo imageFileInfo( term.getImagePath() );
            if( term.getImagePath() == QString::number( term.getId() ) + "." + imageFileInfo.extension( false ) ) {
                // Temporary code for data conversion between 0.11.x and 0.12.x. 
                // Copy the image into a temporary directory with its uid filename.
                // When saving data, the image will be copied in its vocabulary directory and the temporary directory will be removed.
                QFileInfo parentPathInfo( Vocabulary::parentPath );
                QString applDir( parentPathInfo.dirPath().left( parentPathInfo.dirPath().find( ".toMOTko" ) + 8 ) );
                QString tempDirPath( applDir + "/tmp" );
                QString absImagePath( parentPathInfo.dirPath() + "/v-" + QString::number( tempId ) + "/" + term.getImagePath() );
                QString tempImageCopyPath( tempDirPath + "/" + term.getUid().toString() + "." + imageFileInfo.extension( false ) );
                if( !Util::makeDirectory( tempDirPath ) )
                    cerr << "Cannot create directory " << tempDirPath << endl;
                else {
                    if( !Util::copy( absImagePath, tempImageCopyPath ) )
                        cerr << "Cannot copy image " << absImagePath << " to directory " << tempImageCopyPath << endl;
                }
                term.setImagePath( term.getUid().toString() + "." + imageFileInfo.extension( false ) );
            }
        }

        vocab.addTerm( term );
    }
   
    vocab.setDescription( tempDescription );
    vocab.setAuthor( tempAuthor );
    vocab.setCreationDate( tempCreationDate );
    vocab.setModificationDate( tempModificationDate );
    vocab.setDirty( true ); // Force saving data.

    return( in );
}
