#ifndef STATS_PARSER_H
#define STATS_PARSER_H

#include <iostream.h>
#include <qmap.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qxml.h>
#include <stdlib.h>
#include <string.h>
#include "BiUidKey.h"
#include "Progress.h"
#include "Util.h"

class StatsParser : public QXmlDefaultHandler {

public:

    StatsParser( QMap<BiUidKey,TermData>& termData );
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& attribs );
    bool endElement( const QString&, const QString&, const QString& qname );
    bool endDocument();

    bool fatalError( const QXmlParseException& exception );

    bool isStatisticsFile();

private:

    QString desc;

    BiUidKey currTermDataKey;
    TermData* currTermData;
    QMap<BiUidKey,TermData>& termData;
    bool isStatsFile;

};

#endif

