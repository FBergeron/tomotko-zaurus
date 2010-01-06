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
#include "BiUidKey.h"
#include "Progress.h"
#include "Util.h"

class Statistics : public QObject {

    Q_OBJECT

    static const Q_UINT32 magicNumber( Q_UINT32( 0x22446680 ) );
    static uint termDataEntrySize;

public:

    static void makeInstance( const QString& applDirName );
    static Statistics* instance();

    TermData getTermData( const BiUidKey& key );
    void setTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& termData );
    bool loadTermData( const QString& firstLang, const QString& testLang );
    TermData loadTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang );

    /**
     * Save term data.  Existing record will be overwritten.  Otherwise, new record will be inserted.
     * @returns True if the record was saved, false otherwise.
     */
    bool saveTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& termData );
    bool saveTermData( const QString& firstLang, const QString& testLang ) const;
    bool saveUncompressedTermData( const QString& firstLang, const QString& testLang ) const;

    /**
     * Remove data associated to the given translation in parameter.
     * @parameter transLang Language of the translation.
     * @parameter transUid Identifier of the translation.
     * @returns True if the data has been successfully removed.
     */
    bool removeTermData( const QString& transLang, const QUuid& transUid );

    QString getTermDataFilename( const QString& firstLang, const QString& testLang ) const;

private:

    /**
     * Save term data for an existing record.  This function overwrite the file and should not be used
     * to insert new record.
     * @returns True if the record was written, false otherwise.
     */
    bool overwriteTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& termData );

    /**
     * Insert new term data record.
     * @returns True if the record was written, false otherwise.
     */
    bool insertTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& newTermData );

    bool removeTermDataFromFile( const QUuid& transUid, const QString& transLang );

    static Statistics* singleton;

    Statistics( const QString& applDirName );

    /**
     * Search for a term data dichotomically and position the stream at the beginning of the record, if found.
     * @returns True when the record is found, false otherwise.
     */
    bool seekTermData( const BiUidKey& key, QFile& dataFile, const uint headerSize, const uint entryCount, QDataStream& in );

    QString                 applicationDirName;

    BilingualKey            currentLanguages;
    QMap<BiUidKey, TermData> termData;

};

#endif

