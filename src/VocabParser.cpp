#include "VocabParser.h"

VocabParser::VocabParser( Vocabulary& vocab, const QStringList& languages/*, const QString& location*/ ) : vocabulary( vocab ), languages( languages ), isVocabFile( false )/*, location( location )*/ {
}

bool VocabParser::startDocument() {
    return( true );
}

bool VocabParser::startElement( const QString&, const QString&, const QString& qname, const QXmlAttributes& attribs ) {
    if( qname == QString( "glossary" ) ) {
        vocabulary.setTitle( attribs.value( QString( "name" ) ) );
        vocabulary.setAuthor( attribs.value( QString( "author" ) ) );
        desc = QString();
        isVocabFile = true;
    }
    else if( qname == QString( "word" ) || qname == QString( "alt" ) || qname == QString( "comment" ) ) {
        mustKeepText = true;
        tempCh = QString();
        if( qname == QString( "comment" ) ) {
            commentKey = attribs.value( QString( "languages" ) ); 
            comment = QString();
        }
    }
    else if( qname == QString( "trans" ) ) {
        word = QString();
        alt = QString();
        lang = attribs.value( QString( "lang" ) );
    }
    else if( qname == QString( "term" ) ) {
        QString uid = attribs.value( QString( "uid" ) );
        QString imagePath = attribs.value( QString( "imagePath" ) );
        term = Term( -1, vocabulary.getUid(), Util::createUuid() );
        if( !imagePath.isNull() )
            term.setImagePath( /*location + "/" +*/ imagePath );
    }
    else if( qname == QString( "desc" ) ) {
        mustKeepText = true;
        tempCh = QString();
    }
    return( true );
}

bool VocabParser::characters( const QString& characters ) {
    if( mustKeepText ) {
        tempCh = characters.stripWhiteSpace();
        mustKeepText = false;
    }
    return( true );
}

bool VocabParser::endElement( const QString&, const QString&, const QString& qname ) {
    if( qname == QString( "word" ) )
        word = tempCh;
    else if( qname == QString( "comment" ) ) {
        comment = tempCh;
        if( languages.count() == 0 || ( languages.contains( commentKey.getFirstLanguage() ) && languages.contains( commentKey.getSecondLanguage() ) ) )
            term.addComment( commentKey, comment );
    }
    else if( qname == QString( "alt" ) )
        alt = tempCh;
    else if( qname == QString( "trans" ) ) {
        if( languages.count() == 0 || languages.contains( lang ) ) {
            Translation translation( lang, word, alt );
            term.addTranslation( translation ); 
        }
    }
    else if( qname == QString( "term" ) ) {
        if( term.getTranslationCount() > 0 )
            vocabulary.addTerm( term );
    }
    else if( qname == QString( "desc" ) ) {
        desc = tempCh;
        vocabulary.setDescription( desc );
    }
    
    return( true );
}

bool VocabParser::endDocument() {
    return( true );
}

bool VocabParser::fatalError( const QXmlParseException& exception ) {
    QMessageBox::warning( 0, QObject::tr( "Error" ), 
        QObject::tr( "ParseError" )
            .arg( exception.lineNumber() )
            .arg( exception.columnNumber() )
            .arg( exception.message() ) );
    return( false );
}

bool VocabParser::isVocabularyFile() {
    return( isVocabFile );
}
