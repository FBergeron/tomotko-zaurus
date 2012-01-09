#ifndef UTIL_H
#define UTIL_H

#include <qpe/quuid.h>
#include <stdio.h>
#include <stdlib.h> // For srand/rand
#include <qaction.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qfont.h>
#include <qfontdatabase.h>
#include <qlistview.h>
#include <qmap.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qvaluelist.h>
#include <zlib.h>
#include <stdlib.h>
#include "Base.h"
#include "Comment.h"
#include "PixMap.h"
#include "Term.h"
#include "minizip/zip.h"

typedef QValueList<QString> UidList;
typedef QValueList<int> IdList;
typedef QMap<QString,UidList> UidListMap;
typedef QMap<int,IdList> IdListMap;

enum Action {
    ACTION_REVEAL = 0,
    ACTION_RIGHT_ANSWER,
    ACTION_WRONG_ANSWER,
    ACTION_EDIT_QUIZ_TERM,
    ACTION_START_QUIZ,
    ACTION_MANAGE_GLOSSARIES,
    ACTION_IMPORT,
    ACTION_EXPORT,
    ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS,
    ACTION_PREFERENCES,
    ACTION_QUIT,
    ACTION_ADD_FOLDER,
    ACTION_ADD_GLOSSARY,
    ACTION_REMOVE_ITEM,
    ACTION_ADD_TERM,
    ACTION_EDIT_TERM,
    ACTION_REMOVE_TERMS,
    ACTION_CHECK_ALL_TERMS,
    ACTION_INVERSE_CHECKED_TERMS,
    ACTION_MAXIMIZE,
    ACTION_SEARCH,
    ACTION_GRADE_ANSWER_1,
    ACTION_GRADE_ANSWER_2,
    ACTION_GRADE_ANSWER_3,
    ACTION_GRADE_ANSWER_4,
    ACTION_GRADE_ANSWER_5,
    ACTION_GRADE_ANSWER_6,
    ACTION_SHOW_PROGRESS,
    ACTION_COUNT
};

const QString actionId[] = {
    "reveal",
    "rightAnswer",
    "wrongAnswer",
    "editQuizTerm",
    "startQuiz",
    "manageGlossaries",
    "import",
    "export",
    "showAllGlossariesAndTerms",
    "preferences",
    "quit",
    "addFolder",
    "addGlossary",
    "removeItem",
    "addTerm",
    "editTerm",
    "removeTerms",
    "checkAllTerms",
    "inverseCheckedTerms",
    "maximize",
    "search",
    "grade1",
    "grade2",
    "grade3",
    "grade4",
    "grade5",
    "grade6",
    "progressDetails"
};

class Util {

public:

    static const QString languageCodeList[];
    static const QString& getLanguageCode( const QString& language );

    static QString getWeekday( int weekday );

    static QUuid createUuid(); // Taken from Qt-4.4.3's QUuid class.
    static QString escapeXml( QString xml );
    static bool makeDirectory( const QString& dir );
    static bool copy( const QString& fromFile, const QString& toFile );
    static bool deleteDirectory( const QString& dir );
    static QString term2Xml( const Term& term, QStringList& exportedTransUidList, QStringList* languages = NULL, uint indentLevel = 0 );
    static int getDefaultQuizLength();
    static QString getDefaultLabelsFontFamily();
    static int getDefaultLabelsFontSizeModifier();
    static QString getDefaultFontFamily();
    static int getDefaultFontSizeModifier();

    static QString getDigraph( const QString& buffer );
    static QAction* createAction( const QString& name, const char** icon, QObject* receiver, const char* slot, int defaultKey = -1, bool toggle = false );
    static QString describeKey( int key );

    // Taken from qt-4.3.1's QByteArray.
    static QByteArray qCompress( const QByteArray& byteArray, int compressionLevel = -1 ); 
    static QByteArray qUncompress( const QByteArray& byteArray );

    static QString fromUnicodeToUtf8( ushort unicode );

    static int writeFileIntoZipFile( zipFile outputFile, const char* filename, const char* data, int length ); 

private:

    static QMap<QString, QString>* digraphMap; 

    static void initDigraphs();

};

#endif
