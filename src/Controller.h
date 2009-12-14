#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream.h>
#include <qdatastream.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qpe/applnk.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qobject.h>
#include <qcstring.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qxml.h>
#include <math.h>
#include <stdlib.h>
#include "FolderParser.h"
#include "OriginalQuiz.h"
#include "Preferences.h"
#include "Progress.h"
#include "Sequence.h"
#include "Statistics.h"
#include "SuperMemo2Quiz.h"
#include "Term.h"
#include "TermKey.h"
#include "VocabParser.h"
#include "Util.h"
#include "Vocabulary.h"
#include "minizip/unzip.h"
#include "minizip/zip.h"

class Controller : public QObject {

    Q_OBJECT

    static const Q_UINT32 magicNumber( Q_UINT32( 0x22446680 ) );

public:

    Controller();

    bool init();

    const QString& getApplicationDirName() const;
    Preferences& getPreferences();

    Folder* getVocabTree();

    void initQuiz();
    void restartQuiz();
    bool isResumableQuizAvailable();
    bool loadQuiz();
    void prepareQuiz();
    int getQuizAnswerCount() const;
    ProgressData getProgressData();

    Term* getCurrentTerm();
    bool hasNextTerm() const;
    Term* getTerm( const TermKey& termKey, const QString& firstLang = QString::null, const QString& testLang = QString::null );
    Term* getNextTerm();
 
    QString getQuizFirstLanguage() const;
    QString getQuizTestLanguage() const;
    Preferences::QuizAlgorithm getQuizAlgorithm() const;

    bool isQuizInProgress() const;
    int getProgress() const;
    Sequence& getRevealingSequence();
    int getRevealingSequenceStep() const;
    void incrementRevealingSequenceStep();
  
    void rightAnswer();
    void wrongAnswer();
    void gradeAnswer( int grade );
    void reveal();

    void getSchedule( int* schedule );
    void getEFDistribution( QMap<int,int>& efDist, float& efSum, int& efCount );
    float getEFStandardDeviation( const float& efAverage );

    bool saveData();
    void loadData();
   
    bool saveMarkedItems( Folder* folder );
    void loadMarkedItems( Folder* folder );

    Vocabulary* addVocabulary( Folder* parentFolder, Vocabulary* vocab = NULL ); 
    Folder* addFolder( Folder* parentFolder, Folder* folder = NULL ); 

    void copy( Vocabulary* vocab );
    void copy( Folder* folder );

    Vocabulary* loadVocabulary( const QString& parentDir );
    Folder* loadFolder( const QString& parentDir );
    bool saveFolder( Folder* folder, const QString& parentDir ) const;

    bool saveVocabulary( Vocabulary* vocab, const QString& location ) const;
    bool exportData( Vocabulary* vocab, const QString& file, QStringList* languages = NULL ) const;
    bool exportData( Folder* folder, const QString& file, QStringList* languages = NULL ) const;
    void writeFolderDataInXml( QTextStream& ts, const Folder& folder ) const;
    void writeVocabularyInXml( QTextStream& ts, const Vocabulary& vocab, QStringList* languages, bool writeXmlDirective = true, int depth = 0 ) const;

    void loadLegacyVocabulary( Folder* folder );

    QStringList getTranslationLanguagesFromFile( const QString& filename ) const;
    Base* importData( Folder* rootFolder, const QString& filename, const QStringList& languages );

    int getInitialTermCount() const;

    void initRevealingSequence();

    void writeVocabulariesInXml( Folder* folder, int depth, QTextStream& ts, QStringList* languages );

    void setClipboardData( const QString& type, const QByteArray& data );
    QByteArray getClipboardData() const;
    QString getClipboardDataType() const;

    bool isImagePathValid( const QString& path, const Vocabulary& vocab ) const;
    QString getResolvedImagePath( const QString& path, const Vocabulary& vocab ) const;

    void clearSearch();
    QValueList<TermKey> search( const QString& query );
    QValueList<TermKey> getSearchResults() const;
    int getSearchResultsCount() const;

public slots:
    
    void concludeQuiz();

signals:

    void progressReset( int termCount );
    void progressChanged( int progress );

private:

    void getScheduleRec( Folder* folder, int* schedule );
    void getScheduleRec( Vocabulary* vocab, int* schedule );
    void getEFDistributionRec( Folder* folder, QMap<int,int>& efDist, float& efSum, int& efCount );
    void getEFDistributionRec( Vocabulary* vocab, QMap<int,int>& efDist, float& efSum, int& efCount );
    float getEFStandardDeviationRec( Folder* folder, const float& efAverage, int& efCount );
    float getEFStandardDeviationRec( Vocabulary* vocab, const float& efAverage, int& efCount );

    int writeFileIntoZipFile( zipFile outputFile, const char* filename, const char* data, int length ) const; 

    void initMarkedForStudyRec( Folder* folder, UidList& folderUids, UidList& vocabUids, UidListMap& termUids );
    void initMarkedForStudyRec( Vocabulary* vocab, UidList& vocabUids, UidListMap& termUids );
    void initMarkedForStudyRec( Folder* folder, IdList& folderIds, IdList& vocabIds, IdListMap& termIds );
    void initMarkedForStudyRec( Vocabulary* vocab, IdList& vocabIds, IdListMap& termIds );
    
    void saveMarkedItemsRec( Folder* folder, UidList& folderUids, UidList& vocabUids, UidListMap& termUids );
    void saveMarkedItemsRec( Vocabulary* vocab, UidList& vocabUids, UidListMap& termUids );

    QStringList getVocabularyTranslationLanguagesFromZip( zipFile inputFile ) const;

    bool importFolderFromZip( Folder* folder, const QString& folderLocation, zipFile inputFile );
    bool importVocabularyFromZip( Vocabulary* vocab, const QString& vocabLocation, const QStringList& languages, zipFile inputFile );
    bool importImageFromZip( const QString& imgPath, zipFile inputFile );
    bool exportFolderRecIntoZip( Folder* folder, zipFile outputFile, QString path, QStringList* languages = NULL ) const;
    bool exportVocabularyIntoZip( Vocabulary* vocab, zipFile outputFile, QString path, QStringList* languages = NULL ) const;

    Vocabulary* makeCopy( Vocabulary* vocab, const QString& firstLang, const QString& testLang ) const;
    Folder* makeCopy( Folder* folder, const QString& firstLang, const QString& testLang ) const;

    bool deleteItemsMarkedForDeletion( Folder* folder );

    int findFolderId( const QString& filename ) const;
    QString findFolderUid( const QString& dirPath ) const;
    int findParentFolderId( const QString& dirPath ) const;
    QString findParentFolderUid( const QString& dirPath ) const;
    int findVocabId( const QString& filename ) const;
    QString findVocabUid( const QString& filename ) const;
    QString convertPath( const QString& path, QMap<int,Folder*>& newFolders ) const;
    QString convertPath( const QString& path, QMap<QString,Folder*>& newFolders ) const;

    void searchRec( const QString& query, Folder* folder, QValueList<TermKey>& results );
    void searchRec( const QString& query, Vocabulary* folder, QValueList<TermKey>& results );

    Preferences         prefs;

    Folder*             vocabTree; 

    Sequence            currRevealingSeq;
    int                 currRevealingSeqStep;
    Quiz*               quiz;

    QString             markedXmlFilename;
    QString             markedFilename;
    QString             applicationDirName; 

    QString             searchQuery;
    QValueList<TermKey> searchResults;

    // Custom clipboard because data property is disabled for QMimeSource in Zaurus' Qt.
    QByteArray          clipboard; 
    QString             clipboardDataType;

};

#endif
