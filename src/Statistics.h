#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream.h>
#include <qdatastream.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmap.h>
#include <qobject.h>
#include <qstring.h>
#include "BilingualKey.h"
#include "Progress.h"
#include "Util.h"

class Statistics : public QObject {

    Q_OBJECT

    static const Q_UINT32 magicNumber( Q_UINT32( 0x22446680 ) );
    static uint termDataEntrySize;

public:

    static void makeInstance( const QString& applDirName );
    static Statistics* instance();

    TermData getTermData( const QString& termUid );
    void setTermData( const QString& termUid, const TermData& termData );
    bool loadTermData( const QString& firstLang, const QString& testLang );
    TermData loadTermData( const QString& termUid, const QString& firstLang, const QString& testLang );

    /*
     * Save term data for an existing record.  This function overwrite the file and should not be used
     * to insert new record.
     * Returns true if the record was written, false otherwise.
     */
    bool saveTermData( const QString& termUid, const QString& firstLang, const QString& testLang, const TermData& termData );
    bool saveTermData( const QString& firstLang, const QString& testLang ) const;
    bool saveUncompressedTermData( const QString& firstLang, const QString& testLang ) const;

    QString getTermDataFilename( const QString& firstLang, const QString& testLang ) const;

private:

    static Statistics* singleton;

    Statistics( const QString& applDirName );

    /*
     * Search for a term data dichotomically and position the stream at the beginning of the record, if found.
     * Returns true when the record is found, false otherwise.
     */
    bool seekTermData( const QString& termUid, QFile& dataFile, const uint headerSize, const uint entryCount, QDataStream& in );

    QString                 applicationDirName;

    BilingualKey            currentLanguages;
    QMap<QString, TermData> allTermData;

};

#endif

