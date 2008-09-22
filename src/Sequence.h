#ifndef SEQUENCE_H
#define SEQUENCE_H 

#include <iostream.h>
#include <qtl.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qvaluelist.h>

class Sequence {

public:

    static const int MAX_ITEM;
    static const QString itemString[];
    enum Item { INVALID = -1, FIRST_LANG_TERM, TEST_LANG_ALT, TEST_LANG_TERM, COMMENT, IMAGE };

    typedef QValueList<Item> ItemList;

    Sequence( const QString& seqStr = QString::null );
    Sequence( const Sequence& seq );
    ~Sequence(); 

    bool isEnabled() const;
    void setEnabled( bool isEnabled );

    bool contains( const Item& item ) const;
    bool isEmpty() const;

    void addGroup( ItemList group );
    void removeLastGroup();
    ItemList getGroupAt( int index ) const;
    int getGroupCount() const;

    static Item stringToItem( const QString& itemStr );

    QString toString() const;
    QString toHumanReadableString() const;

    friend QDataStream& operator<<( QDataStream& out, const Sequence& sequence );
    friend QDataStream& operator>>( QDataStream& in, Sequence& sequence );

private:

    bool                    enabled;
    QValueList<ItemList>    groups;

};

QDataStream& operator<<( QDataStream& out, const Sequence& sequence );
QDataStream& operator>>( QDataStream& in, Sequence& sequence );

#endif

