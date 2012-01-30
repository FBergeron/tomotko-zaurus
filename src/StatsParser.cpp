#include "StatsParser.h"

StatsParser::StatsParser( Statistics& stats ) : stats( stats ), isStatsFile( false ) {
}

bool StatsParser::startDocument() {
    return( true );
}

bool StatsParser::startElement( const QString&, const QString&, const QString& qname, const QXmlAttributes& attribs ) {
    //if( qname == QString( "folder" ) ) {
    //    folder.setTitle( attribs.value( QString( "name" ) ) );
    //    folder.setAuthor( attribs.value( QString( "author" ) ) );
    //    desc = QString();
    //    isStatsFile = true;
    //}
    //else if( qname == QString( "desc" ) ) {
    //    mustKeepText = true;
    //    tempCh = QString();
    //}
    return( true );
}

bool StatsParser::characters( const QString& characters ) {
    //if( mustKeepText ) {
    //    tempCh = characters.stripWhiteSpace();
    //    mustKeepText = false;
    //}
    return( true );
}

bool StatsParser::endElement( const QString&, const QString&, const QString& qname ) {
    //if( qname == QString( "desc" ) ) {
    //    desc = tempCh;
    //    folder.setDescription( desc );
    //}
    
    return( true );
}

bool StatsParser::endDocument() {
    return( true );
}

bool StatsParser::fatalError( const QXmlParseException& exception ) {
    QMessageBox::warning( 0, QObject::tr( "Error" ), 
        QObject::tr( "ParseError" )
            .arg( exception.lineNumber() )
            .arg( exception.columnNumber() )
            .arg( exception.message() ) );
    return( false );
}

bool StatsParser::isStatisticsFile() {
    return( isStatsFile );
}

