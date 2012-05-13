#ifndef VOCAB_PARSER_H
#define VOCAB_PARSER_H

#include <iostream.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qxml.h>
#include <stdlib.h>
#include <string.h>
#include "BilingualKey.h"
#include "Comment.h"
#include "Term.h"
#include "Translation.h"
#include "Vocabulary.h"

class VocabParser : public QXmlDefaultHandler {

public:

    VocabParser( Vocabulary& vocabulary, const QStringList& languages, QMap<QString,Translation>* importedTransUidTable = NULL /*, const QString& location = QString::null*/ );
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& attribs );
    bool characters( const QString& characters );
    bool endElement( const QString&, const QString&, const QString& qname );
    bool endDocument();

    bool fatalError( const QXmlParseException& exception );

    bool isVocabularyFile();

private:

    QString lang;
    QString word;
    QString commentText;
    BilingualKey commentKey;
    QString alt;
    QString transUid;

    QString desc;

    QString tempCh;
    bool mustKeepText;

    Term term;
    Vocabulary& vocabulary;
    const QStringList& languages;
    bool isVocabFile;
    QMap<QString,Translation>* importedTransUidTable;
    //const QString& location;

};

#endif
