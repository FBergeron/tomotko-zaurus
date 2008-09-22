#ifndef MARKED_ITEMS_PARSER_H
#define MARKED_ITEMS_PARSER_H 

#include <iostream.h>
#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qxml.h>
#include <stdlib.h>
#include <string.h>
#include "Util.h"

class MarkedItemsParser : public QXmlDefaultHandler {

public:

    MarkedItemsParser();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& attribs );

    IdList*     getMarkedFolders() const;
    IdList*     getMarkedVocabs() const;
    IdListMap*  getMarkedTerms() const;

private:

    IdList*     markedFolderIdList;
    IdList*     markedVocabIdList;
    IdListMap*  markedTermIdListMap;

};

#endif
