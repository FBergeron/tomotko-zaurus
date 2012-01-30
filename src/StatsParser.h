#ifndef STATS_PARSER_H
#define STATS_PARSER_H

#include <iostream.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qxml.h>
#include <stdlib.h>
#include <string.h>
#include "Statistics.h"

class StatsParser : public QXmlDefaultHandler {

public:

    StatsParser( Statistics& stats );
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& attribs );
    bool characters( const QString& characters );
    bool endElement( const QString&, const QString&, const QString& qname );
    bool endDocument();

    bool fatalError( const QXmlParseException& exception );

    bool isStatisticsFile();

private:

    QString desc;

    QString tempCh;
    bool mustKeepText;

    Statistics& stats;
    bool isStatsFile;

};

#endif

