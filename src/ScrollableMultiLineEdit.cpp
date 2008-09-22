#include "ScrollableMultiLineEdit.h"

ScrollableMultiLineEdit::ScrollableMultiLineEdit( QWidget *parent, const char *name )
    : QMultiLineEdit( parent, name ) {
}

ScrollableMultiLineEdit::~ScrollableMultiLineEdit() {
}

void ScrollableMultiLineEdit::scrollUp() {
    setCursorPosition( QMAX( topCell() - 1, 0 ), 0 );
}

void ScrollableMultiLineEdit::scrollDown() {
    setCursorPosition( QMIN( lastRowVisible() + 1, numLines() ), 0 );
}

void ScrollableMultiLineEdit::scrollPageUp() {
    pageUp();
}

void ScrollableMultiLineEdit::scrollPageDown() {
    pageDown();
}
