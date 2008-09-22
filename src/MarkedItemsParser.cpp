#include "MarkedItemsParser.h"

MarkedItemsParser::MarkedItemsParser() 
    : markedFolderIdList( new IdList() ), markedVocabIdList( new IdList() ), markedTermIdListMap( new IdListMap() ) {
}

bool MarkedItemsParser::startElement( const QString&, const QString&, const QString& qname, const QXmlAttributes& attribs ) {
    if( qname == QString( "folders" ) ) {
        QStringList folderIdList( QStringList::split( QString( "," ), attribs.value( QString( "ids" ) ) ) );
        for( QStringList::ConstIterator it = folderIdList.begin(); it != folderIdList.end(); it++ ) {
            bool isIdValid;
            int folderId = (*it).toInt( &isIdValid, 10 );
            if( isIdValid )        
                markedFolderIdList->append( folderId );
        }
    }
    else if( qname == QString( "glossaries" ) ) {
        QStringList vocabIdList( QStringList::split( QString( "," ), attribs.value( QString( "ids" ) ) ) );
        for( QStringList::ConstIterator it = vocabIdList.begin(); it != vocabIdList.end(); it++ ) {
            bool isIdValid;
            int vocabId = (*it).toInt( &isIdValid, 10 );
            if( isIdValid )        
                markedVocabIdList->append( vocabId );
        }
    }
    else if( qname == QString( "terms" ) ) {
        QStringList termIdList( QStringList::split( QString( "," ), attribs.value( QString( "ids" ) ) ) );
        bool isVocabIdValid;
        int vocabId = attribs.value( QString( "glossId" ) ).toInt( &isVocabIdValid, 10 );
        if( isVocabIdValid ) {
            IdList* markedTermIdList = new IdList();
            for( QStringList::ConstIterator it = termIdList.begin(); it != termIdList.end(); it++ ) {
                bool isIdValid;
                int termId = (*it).toInt( &isIdValid, 10 );
                if( isIdValid )        
                    markedTermIdList->append( termId );
            }
            markedTermIdListMap->insert( vocabId, *markedTermIdList );
        }
    }
    return( TRUE );
}

IdList* MarkedItemsParser::getMarkedFolders() const {
    return( markedFolderIdList );
}

IdList* MarkedItemsParser::getMarkedVocabs() const {
    return( markedVocabIdList );
}

IdListMap* MarkedItemsParser::getMarkedTerms() const {
    return( markedTermIdListMap );
}
