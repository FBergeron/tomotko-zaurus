#include "TermListItem.h"

TermListItem::TermListItem( QListView* parent, Term* term, const QString& firstLanguage, const QString& testLanguage, bool isAltShown = true )
    : QCheckListItem( parent, "TermListItem", CheckBox ), term( term ), firstLanguage( firstLanguage ), testLanguage( testLanguage ), 
        altShown( isAltShown ) {
    updateUi();
}

TermListItem::~TermListItem() {
}

void TermListItem::updateUi() {
    if( term ) {
        setOn( term->isMarkedForStudy() );
        if( term->isTranslationExists( firstLanguage ) ) {
            const Translation& firstLangTranslation = term->getTranslation( firstLanguage );
            setText( 0, firstLangTranslation.getWord() );
        }
        else
            setText( 0, QObject::tr( "Undefined" ) );

        if( term->isTranslationExists( testLanguage ) ) {
            const Translation& testLangTranslation = term->getTranslation( testLanguage );
            QString testLangStr = testLangTranslation.getWord(); 
            if( altShown && testLangTranslation.getAlt().length() > 0 )
                testLangStr += " [" + testLangTranslation.getAlt() + "]";
            setText( 1, testLangStr );
        }
        else
            setText( 1, QObject::tr( "Undefined" ) );
    }
}

void TermListItem::setOn( bool isOn ) {
    QCheckListItem::setOn( isOn );
    term->setMarkedForStudy( isOn );
}

Term* TermListItem::getTerm() {
    return( term );
}

void TermListItem::setFont( int column, const QFont& font ) {
    fonts[ column ] = font;
}

QFont TermListItem::font( int column ) const {
    if( fonts.contains( column ) )
        return( fonts[ column ] );
    else
        return( listView()->font() );
}

void TermListItem::paintCell( QPainter* p, const QColorGroup& cg, int column, int width, int align ) {
    if( !p )
        return;

    QFont oldFont = p->font();
    p->setFont( font( column ) );

    QCheckListItem::paintCell( p, cg, column, width, align );

    QColorGroup colorGroup( cg );
    switch( column ) {
        case 0 : 
            if( term && !term->isTranslationExists( firstLanguage ) )
                colorGroup.setColor( QColorGroup::Text, listView()->palette().color( QPalette::Disabled, QColorGroup::Text ) );
            break;

        case 1 :
            if( term && !term->isTranslationExists( testLanguage ) )
                colorGroup.setColor( QColorGroup::Text, listView()->palette().color( QPalette::Disabled, QColorGroup::Text ) );
            break;
    }

    QListViewItem::paintCell( p, colorGroup, column, width, align );

    p->setFont( oldFont );
}
