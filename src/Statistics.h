#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream.h>
#include <qdatastream.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmap.h>
#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include "BilingualKey.h"
#include "BiUidKey.h"
#include "Folder.h"
#include "Progress.h"
#include "Util.h"
#include "minizip/zip.h"

class Statistics : public QObject {

    Q_OBJECT

    static const Q_UINT32 magicNumber( Q_UINT32( 0x22446680 ) );
    static uint termDataEntrySize;

public:

    static void makeInstance( const QString& applDirName );
    static Statistics* instance();

    TermData getTermData( const BiUidKey& key );
    void setTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& termData );

    /**
     * Load term data for all the specified language pair.
     * @param firstLang First language of the language pair.
     * @param testLang Test language of the language pair.
     * @returns True if the data has been successfully read. Falseotherwise.
     */
    bool loadTermData( const QString& firstLang, const QString& testLang );

    TermData loadTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang );

    /**
     * Save term data.  Existing record will be overwritten.  Otherwise, new record will be inserted.
     * @returns True if the record was saved, false otherwise.
     */
    bool saveTermData( const BiUidKey& key, const QString& firstLang, const QString& testLang, const TermData& termData );
    bool saveTermData( const QString& firstLang, const QString& testLang ) const;
    bool saveUncompressedTermData( const QString& firstLang, const QString& testLang ) const;

    QString getTermDataFilename( const QString& firstLang, const QString& testLang ) const;

    void addDeletedTranslation( const QUuid& uid, const QString& lang );
    void removeDeletedTranslation( const QUuid& uid, const QString& lang );
    bool purgeObsoleteData();

    QStringList toString() const; // For debugging.

    void convertTermData( const QString& firstLang, const QString& testLang, Folder* topFolder );

    /**
     * Reset internal data structures.
     */
    void clear();

    bool exportIntoZip( zipFile outputFile, QStringList* languages, QStringList& exportedTransUidList ) const;

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

    bool removeTermData( QValueList<QString>& transUidList, const QString& filename );

    void writeTermDataInXml( QTextStream& ts, QStringList* languages, QStringList& exportedTransUidList ) const; 

    static Statistics* singleton;

    Statistics( const QString& applDirName );

    /**
     * Search for a term data dichotomically and position the stream at the beginning of the record, if found.
     * @returns True when the record is found, false otherwise.
     */
    bool seekTermData( const BiUidKey& key, QFile& dataFile, const uint headerSize, const uint entryCount, QDataStream& in );

    QString                 applicationDirName;

    BilingualKey            currentLanguages;
    QMap<BiUidKey,TermData> termData;

    QMap<QString,QValueList<QString> > deletedTranslations; 

};

#endif

