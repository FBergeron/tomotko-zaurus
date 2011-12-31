#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <qpe/quuid.h>
#include <iterator.h>
#include <qcstring.h>
#include <qdatastream.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>
#include "Base.h"
#include "Term.h"
#include "Util.h"

class Folder;

class Vocabulary : public Base {

public:

    /**
     * Number used by vocabulary data file.
     */
    static const Q_UINT32 magicNumber( Q_UINT32( 0x22446688 ) );

    /**
     * Temporary variable for data conversion from 0.11.x to 0.12.x.
     */
    static QString parentPath;

    /**
     * Deprecated Type used for data conversion from 0.11.x to 0.12.x.  Should be removed in 0.13.
     */
    typedef QMap<int, Term> OldTermMap;

    /**
     * Type for hashtable: key is term's UID; value is term.
     */
    typedef QMap<QString, Term> TermMap;

    /**
     * Vocabulary constructor.
     * @param id Numeric identifier (deprecated, should disappear in 0.13).
     * @param title Title of the vocabulary.
     * @param uid Unique identifier.
     */
    Vocabulary( int id = -1, const QString& title = QString::null, const QUuid& uid = QUuid() );

    /**
     * Copy constructor for vocabulary.
     * @param voc Vocabulary from which the copy is built.
     */
    Vocabulary( const Vocabulary& voc );

    /**
     * Destructor.
     */
    virtual ~Vocabulary();
   
    /**
     * @returns Class name = "Vocabulary".
     */
    const char* className() const { return "Vocabulary"; }

    /**
     * @returns Unique identifier of the vocabulary.
     */
    QUuid getUid() const;

    /**
     * @returns Numeric identifier of the vocabulary.
     * @deprecated Has been replaced by getUid(). Should disappear in 0.13.
     */
    int getId() const;

    /**
     * @returns Highest numeric identifier.
     * @deprecated Numeric identifier has been replaced by getUid(). Should disappear in 0.13.
     */ 
    int getMaxId() const;

    /**
     * @returns True if the vocabulary is marked for study, false otherwise.
     */
    bool isMarkedForStudy() const;

    /**
     * @param isMarkedForStudy Boolean value indicating whether the vocabulary is marked for study or not.
     */
    void setMarkedForStudy( bool isMarkedForStudy );

    /**
     * When a vocabulary is removed by a user, it is not immediately deleted.
     * Instead, it's marked for deletion and will be removed when the application exits.
     * @returns True if the vocabulary is marked for deletion, false otherwise.
     */
    bool isMarkedForDeletion() const;

    /**
     * @param isMarkedForDeletion Boolean value indicating whether the vocabulary is marked for deletion or not.
     */
    void setMarkedForDeletion( bool isMarkedForDeletion );

    /**
     * @returns Title of the vocabulary.
     */
    const QString getTitle() const;

    /**
     * Set the title of the vocabulary.
     * @param title New title.
     */
    void setTitle( const QString& title );

    /**
     * @returns Description of the vocabulary.
     */
    const QString getDescription() const;

    /**
     * Set the description of the vocabulary.
     * @param desc New description.
     */
    void setDescription( const QString& desc );

    /**
     * @returns Author of the vocabulary.
     */
    const QString getAuthor() const;

    /**
     * Set the author of the vocabulary.
     * @param author New author.
     */
    void setAuthor( const QString& author );

    /**
     * @returns Creation date of the vocabulary.
     */
    const QDateTime getCreationDate() const;

    /**
     * Set the creation date of the vocabulary.
     * @param date New creation date.
     */
    void setCreationDate( const QDateTime& date );


    /**
     * @returns Modification date of the vocabulary.
     */
    const QDateTime getModificationDate() const;

    /**
     * Set the modification date of the vocabulary.
     * @param date New modification date.
     */
    void setModificationDate( const QDateTime& date );

    /**
     * Insert a term into a vocabulary.
     * @param term Term to insert.
     */
    void addTerm( const Term& term );

    /**
     * Remove a term from the vocabulary.
     * @param uid Identifier of the term to remove.
     */
    void removeTerm( const QUuid& uid );

    /**
     * Even terms that are marked for deletion are considered.
     * @param uid Identifier of the searched term.
     * @returns True if the vocabulary contains the term, otherwise false.
     */
    bool isTermExists( const QUuid& uid ) const;

    /**
     * Even terms that are marked for deletion are considered.
     * @param id Numeric identifier of the searched term.
     * @deprecated Numeric identifier has been replaced by getUid(). Should disappear in 0.13.
     */
    bool isTermExists( const int& id ) const;

    /**
     * Retrieve a term from a vocabulary given its identifier.
     * The method isTermExits() must be called before calling this method to ensure that
     * the term is contained in the vocabulary.
     * Even terms marked for deletion are conѕidered.
     * @param uid Identifier of the searched term.
     * @returns Instance of term.
     */
    Term& getTerm( const QUuid& uid );

    /**
     * Retrieve a term from a vocabulary given its numeric identifier.
     * The method isTermExits() must be called before calling this method to ensure that
     * the term is contained in the vocabulary.
     * Even terms marked for deletion are conѕidered.
     * @param id Numeric identifier of the searched term.
     * @returns Instance of term.
     * @deprecated Numeric identifier has been replaced by getUid(). Should disappear in 0.13.
     */
    Term& getTerm( const int& id );

    /**
     * @returns Number of term instances contained in the vocabulary, including terms marked for deletion.
     */
    const uint getSize() const;

    /**
    * @returns True if the vocabulary contains no term references (including terms marked for deletion).
    */
    bool isEmpty() const;

    /**
     * Compute the number of items contained in the vocabulary.  Terms marked for deletion are not counted.
     * @param termCount Accumulator for term count. 
     * @param checkedTermCount Accumulator for checked term count.
     * @param selectedTermCount Accumulator for selected term count.
     * @param isReachableFromRoot True to indicate that the vocabulary is not reachable from the root node, false otherwise.
     * @param firstLang Selected first language.  When specified, the terms including translations in this language and the test language will be counted.
     * @param testLang Selected test language.  When spedificef, the terms including translations in this language and the first language will be counted.
     */
    void getItemsCount( uint* termCount, uint* checkedTermCount, uint* selectedTermCount, bool isReachableFromRoot, const QString& firstLang = QString::null, const QString& testLang = QString::null ) const;

    /**
     * @param lang1 First language.
     * @param lang2 Second language.
     * @return True when the vocabulary contains at least one term with a translation in the 2 given languages, false otherwise.  The terms
     * marked for deletion are ignored by this method.
     */
    bool containsTermWithTranslations( const QString& lang1, const QString& lang2 ) const;

    /**
     * @returns List of languages of the term translations found in the vocabulary.  Terms marked for deletion are ignored by this method.
     */
    QStringList getTranslationLanguages() const;

    /**
     * Remove translations in the given languages from terms marked for deletion.
     * @param languages List of languages of the translations to remove for terms contained in the vocabulary.  
     */
    void removeTranslations( const QStringList& languages );

    /**
     * @returns Begin iterator to traverse all terms contained in the vocabulary.
     */
    TermMap::ConstIterator begin() const;

    /**
     * @returns End iterator to traverse all terms contained in the vocabulary.
     */
    TermMap::ConstIterator end() const;

    /**
     * @returns Begin iterator to traverse all terms contained in the vocabulary.
     */
    TermMap::Iterator begin();

    /**
     * @returns End iterator to traverse all terms contained in the vocabulary.
     */
    TermMap::Iterator end();

    /**
    * @returns True if the vocabulary needs to be written to disk for update.
     */
    bool isDirty() const;

    /**
     * @param isDirty True to indicate that the vocabulary needs to be written to disk for update.
     */
    void setDirty( bool isDirty );

    /**
     * @returns Pointer to parent folder.
     */
    Folder* getParent() const;

    /**
     * Set parent folder of the vocabulary.
     * @param parent Pointer to parent folder.
     */
    void setParent( Folder* parent );

    /**
     * Load the data from a file to a vocabulary instance.
     * @param filename Filename of the vocabulary data.
     * @returns True if the data could be loaded successfully, false otherwise. 
     */
    bool load( const QString& filename );

    /**
     * Save the vocabulary data into a file.
     * @param filename Filename of the vocabulary data.
     * @returns True if the data could be saved successfully, false otherwise. 
     */
    bool save( const QString& filename ) const;

    friend QDataStream& operator<<( QDataStream& out, const Vocabulary& vocab );
    friend QDataStream& operator>>( QDataStream& in, Vocabulary& vocab );
    friend QDataStream& readOldFormat( QDataStream& in, Vocabulary& vocab, Q_UINT16 version );
    friend QDataStream& readOldFormatTemp( QDataStream& in, Vocabulary& vocab, Q_UINT16 version );

private:

    QUuid       uid;
    int         id;
    bool        markedForStudy;
    bool        markedForDeletion;
    QString     title;
    QString     description;
    QString     author;
    QDateTime   creationDate;
    QDateTime   modificationDate;
    TermMap     terms;

    bool        dirty;

    Folder*     parent;

};

QDataStream& operator<<( QDataStream& out, const Vocabulary& vocab );
QDataStream& operator>>( QDataStream& in, Vocabulary& vocab );

#endif
