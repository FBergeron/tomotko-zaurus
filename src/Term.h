#ifndef TERM_H
#define TERM_H

#include <qpe/quuid.h>
#include <qfileinfo.h>
#include <iterator.h>
#include <string.h>
#include <qdatastream.h>
#include <qmap.h>
#include <qstring.h>
#include "Base.h"
#include "BilingualKey.h"
#include "Comment.h"
#include "Translation.h"

class Term : public Base {

public:

    static bool isOldFormat; // Temporary flag for data conversion from 0.11.x to 0.12.x.

    Term( int id = 0, const QUuid& vocabUid = QUuid(), const QUuid& uid = QUuid() );
    Term( const Term& term );
    ~Term();
   
    /**
     * @returns Class name = "Term".
     */
    const char* className() const { return "Term"; }

    typedef QMap<QString, Translation> TranslationMap;
    typedef QMap<BilingualKey, QString> OldCommentMap; // Temporary for data conversion from 0.11.x to 0.12.x.
    typedef QMap<BilingualKey, Comment> CommentMap;

    const QUuid getUid() const;
    void setUid( const QUuid& uid );
    const int getId() const;
    const QUuid getVocabUid() const;
    void setVocabUid( const QUuid& vocabUid );
    const int getVocabId() const;
    bool isMarkedForStudy() const;
    void setMarkedForStudy( bool isMarkedForStudy );
    bool isMarkedForDeletion() const;

    /**
     * Indicate that a term must be deleted.
     * All sub-items (like translations) will also be marked for deletion.
     * @parameter isMarkedForDeletion True to indicate that the term must be deleted, false otherwise.
     */
    void setMarkedForDeletion( bool isMarkedForDeletion );

    void addTranslation( const Translation& translation );
    void removeTranslation( const QString& language );
    bool isTranslationExists( const QString& language ) const;
    Translation& getTranslation( const QString& language );
    Translation getTranslation( const QString& language ) const;
    TranslationMap::ConstIterator translationsBegin() const;
    TranslationMap::ConstIterator translationsEnd() const;
    int getTranslationCount() const;
   
    void addComment( const BilingualKey& key, const Comment& comment );
    void removeComment( const BilingualKey& key );
    void removeComments( const QString& language );
    bool isCommentExists( const BilingualKey& key ) const;
    Comment& getComment( const BilingualKey& key );
    Comment getComment( const BilingualKey& key ) const;
    CommentMap::ConstIterator commentsBegin() const;
    CommentMap::ConstIterator commentsEnd() const;
    const QString getImagePath() const;
    void setImagePath( const QString& imagePath );

    Base* getObject( const QUuid& uid );
   
    friend QDataStream& operator<<( QDataStream& out, const Term& term );
    friend QDataStream& operator>>( QDataStream& in, Term& term );

private:
    
    QUuid   uid;
    int     id;
    QUuid   vocabUid;
    bool    markedForStudy;
    bool    markedForDeletion;

    TranslationMap  translations;
    CommentMap      comments;

    QString         imagePath;

};

QDataStream& operator<<( QDataStream& out, const Term& term );
QDataStream& operator>>( QDataStream& in, Term& term );

#endif
