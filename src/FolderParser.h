#ifndef FOLDER_PARSER_H
#define FOLDER_PARSER_H

#include <iostream.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qxml.h>
#include <stdlib.h>
#include <string.h>
#include "Folder.h"

class FolderParser : public QXmlDefaultHandler {

public:

    FolderParser( Folder& folder );
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString&, const QXmlAttributes& attribs );
    bool characters( const QString& characters );
    bool endElement( const QString&, const QString&, const QString& qname );
    bool endDocument();

    bool fatalError( const QXmlParseException& exception );

    bool isFolderFile();

private:

    QString desc;

    QString tempCh;
    bool mustKeepText;

    Folder& folder;
    bool folderFile;

};

#endif
