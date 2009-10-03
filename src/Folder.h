#ifndef FOLDER_H
#define FOLDER_H

#include <qpe/quuid.h>
#include <iostream.h>
#include <qdatetime.h>
#include <qlist.h>
#include <qstring.h>
#include <qstringlist.h>
#include "Base.h"
#include "Vocabulary.h"

class Folder : public Base {

public:

    static const Q_UINT32 magicNumber( Q_UINT32( 0x77446688 ) );

    Folder( int id = -1, const QString& title = QString::null, const QUuid& uid = QUuid() );
    Folder( const Folder& folder );
    virtual ~Folder();

    const char* className() const { return "Folder"; }

    QUuid getUid() const;
    int getId() const;
    const QString getTitle() const;
    void setTitle( const QString& title );
    const QString getDescription() const;
    void setDescription( const QString& desc );
    const QString getAuthor() const;
    void setAuthor( const QString& author );
    const QDateTime getCreationDate() const;
    void setCreationDate( const QDateTime& date );
    const QDateTime getModificationDate() const;
    void setModificationDate( const QDateTime& date );

    bool isDirty() const;
    void setDirty( bool isDirty, bool recursive = false );

    bool isMarkedForStudy() const;
    void setMarkedForStudy( bool isMarkedForStudy );
    bool isMarkedForDeletion() const;
    void setMarkedForDeletion( bool isMarkedForDeletion );
    bool containsTermWithTranslations( const QString& lang1, const QString& lang2 );
    bool isReachableFromRoot() const;

    QStringList getTranslationLanguages();
    void removeTranslations( const QStringList& languages );

    void add( Vocabulary* child );
    void add( Folder* child );
    void remove( Vocabulary* child, bool deleteChild = true );
    void remove( Folder* child, bool deleteChild = true );
    bool isEmpty() const;
    int getChildrenCount() const;
    Folder* getFolder( const QUuid& uid );
    Folder* getFolder( int id );
    Vocabulary* getVocabulary( const QUuid& uid );
    Vocabulary* getVocabulary( int id );
    Folder* getRoot() const;
    Folder* getParent() const;
    QString getOldPath() const;
    QString getPath() const;
    QString getHumanReadablePath() const;
    void setParent( Folder* parent );
    //uint getTermCount();
    //uint getVocabularyCount();
    //uint getFolderCount();
    void getItemsCount( uint* termCount, uint* vocabCount, uint* folderCount, 
        uint* checkedTermCount, uint* checkedVocabCount, uint* checkedFolderCount, 
            uint* selectedTermCount, uint* selectedVocabCount, uint* selectedFolderCount, 
                bool isReachableFromRoot, 
                    const QString& firstLang = QString::null, const QString& testLang = QString::null );

    Base* first();
    Base* next();

    bool loadMetadata( const QString& filename );
    bool saveMetadata( const QString& filename ) const;

    void buildVocabCopiesMap( QMap<QString,Vocabulary>& vocabularies ) const;

    friend QDataStream& operator<<( QDataStream& out, const Folder& folder );
    friend QDataStream& operator>>( QDataStream& in, Folder& folder );

private:

    Vocabulary* getVocabularyRec( const QUuid& uid );
    Vocabulary* getVocabularyRec( int id );

    QUuid       uid;
    int         id;
    QString     title;
    QString     description;
    QString     author;
    QDateTime   creationDate;
    QDateTime   modificationDate;
    bool        markedForStudy;
    bool        markedForDeletion;

    bool        dirty;

    Folder*     parent;
    QList<Base> children;

};

QDataStream& operator<<( QDataStream& out, const Folder& folder );
QDataStream& operator>>( QDataStream& in, Folder& folder );

#endif
