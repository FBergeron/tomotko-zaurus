#ifndef TERM_H
#define TERM_H

#include <iterator.h>
#include <string.h>
#include <qdatastream.h>
#include <qmap.h>
#include <qstring.h>
#include "BilingualKey.h"
#include "Translation.h"

class Term {

public:

    Term( int id = 0, int vocabId = 0 );
    Term( const Term& term );
    ~Term();
   
    typedef QMap<QString, Translation> TranslationMap;
    typedef QMap<BilingualKey, QString> CommentMap;

    const int getId() const;
    const int getVocabId() const;
    bool isMarkedForStudy() const;
    void setMarkedForStudy( bool isMarkedForStudy );

    void addTranslation( const Translation& translation );
    void removeTranslation( const QString& language );
    bool isTranslationExists( const QString& language ) const;
    Translation& getTranslation( const QString& language );
    Translation getTranslation( const QString& language ) const;
    TranslationMap::ConstIterator translationsBegin() const;
    TranslationMap::ConstIterator translationsEnd() const;
    int getTranslationCount() const;
   
    void addComment( const BilingualKey& key, const QString& comment );
    void removeComment( const BilingualKey& key );
    void removeComments( const QString& language );
    bool isCommentExists( const BilingualKey& key ) const;
    QString& getComment( const BilingualKey& key );
    QString getComment( const BilingualKey& key ) const;
    CommentMap::ConstIterator commentsBegin() const;
    CommentMap::ConstIterator commentsEnd() const;
    const QString getImagePath() const;
    void setImagePath( const QString& imagePath );

   
    friend QDataStream& operator<<( QDataStream& out, const Term& term );
    friend QDataStream& operator>>( QDataStream& in, Term& term );

private:
    
    int id;
    int vocabId;
    bool markedForStudy;

    TranslationMap translations;
    CommentMap comments;

    QString imagePath;

};

QDataStream& operator<<( QDataStream& out, const Term& term );
QDataStream& operator>>( QDataStream& in, Term& term );

#endif
