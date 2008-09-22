#include "ResultListItem.h"

ResultListItem::ResultListItem( QListView* parent, Term* term, const QString& firstLanguage, const QString& testLanguage, const QString& vocabTitle, const QString& location, bool isAltShown = true )
    : QListViewItem( parent ), term( term ), firstLanguage( firstLanguage ), testLanguage( testLanguage ), 
        vocabTitle( vocabTitle ), location( location ), altShown( isAltShown ) {
    updateUi();
}

ResultListItem::~ResultListItem() {
}

void ResultListItem::setup() {
    int rowHeight = QFontMetrics( listView()->font() ).height();
    QFontMetrics firstLangFontMetrics( font( 0 ) );
    if( firstLangFontMetrics.height() > rowHeight )
        rowHeight = firstLangFontMetrics.height();
    QFontMetrics testLangFontMetrics( font( 1 ) );
    if( testLangFontMetrics.height() > rowHeight )
        rowHeight = testLangFontMetrics.height();
    setHeight( rowHeight );
}

void ResultListItem::updateUi() {
    if( term ) {
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

        setText( 2, vocabTitle );
        setText( 3, location );
    }
}

Term* ResultListItem::getTerm() {
    return( term );
}

void ResultListItem::setFont( int column, const QFont& font ) {
    fonts[ column ] = font;
}

QFont ResultListItem::font( int column ) const {
    if( fonts.contains( column ) )
        return( fonts[ column ] );
    else
        return( listView()->font() );
}

void ResultListItem::paintCell( QPainter* p, const QColorGroup& cg, int column, int width, int align ) {
    if( !p )
        return;

    QFont oldFont = p->font();
    p->setFont( font( column ) );

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
