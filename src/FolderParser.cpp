#include "FolderParser.h"

FolderParser::FolderParser( Folder& folder, const QString& location ) : folder( folder ), isFolderFile( false ), location( location ) {
}

bool FolderParser::startDocument() {
    return( true );
}

bool FolderParser::startElement( const QString&, const QString&, const QString& qname, const QXmlAttributes& attribs ) {
    if( qname == QString( "folder" ) ) {
        folder.setTitle( attribs.value( QString( "name" ) ) );
        folder.setAuthor( attribs.value( QString( "author" ) ) );
        desc = QString();
        isFolderFile = true;
    }
    else if( qname == QString( "desc" ) ) {
        mustKeepText = true;
        tempCh = QString();
    }
    return( true );
}

bool FolderParser::characters( const QString& characters ) {
    if( mustKeepText ) {
        tempCh = characters.stripWhiteSpace();
        mustKeepText = false;
    }
    return( true );
}

bool FolderParser::endElement( const QString&, const QString&, const QString& qname ) {
    if( qname == QString( "desc" ) ) {
        desc = tempCh;
        folder.setDescription( desc );
    }
    
    return( true );
}

bool FolderParser::endDocument() {
    return( true );
}

bool FolderParser::fatalError( const QXmlParseException& exception ) {
    QMessageBox::warning( 0, QObject::tr( "Error" ), 
        QObject::tr( "ParseError" )
            .arg( exception.lineNumber() )
            .arg( exception.columnNumber() )
            .arg( exception.message() ) );
    return( false );
}

bool FolderParser::isVocabularyFile() {
    return( isFolderFile );
}
