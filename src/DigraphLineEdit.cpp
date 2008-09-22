#include "DigraphLineEdit.h"

DigraphLineEdit::DigraphLineEdit( QWidget* parent, const char* name = 0 )
    : QLineEdit( parent, name ), digraphEnabled( false ) {
}

DigraphLineEdit::DigraphLineEdit( const QString& content, QWidget* parent, const char* name = 0 )
    : QLineEdit( content, parent, name ), digraphEnabled( false ) {
}

DigraphLineEdit::~DigraphLineEdit() {
}

void DigraphLineEdit::keyPressEvent( QKeyEvent* event ) {
    //cout << "txt='" << event->text() << "' asc=" << event->ascii() << "; k=" << event->key() << " count=" << event->count() << endl;
    if( digraphEnabled ) {
        // We consider only printable keys.  Control keys are processed normally.
        // I'm not sure if this test covers all the cases though.
        if( event->count() > 0 ) {
            if( event->key() == Key_Backspace ) {
                if( cursorPosition() > 0 ) {
                    if( buffer.isNull() ) {
                        QString charToDelete( text().mid( cursorPosition() - 1, 1 ) );
                        buffer = charToDelete;
                    }
                    else 
                        buffer = QString::null;
                }
            }
            else {
                if( !buffer.isNull() ) {
                    buffer += event->text();
                    const QString newChar( Util::getDigraph( buffer ) );
                    if( newChar == QString::null )
                        buffer = QString::null;
                    else {
                        QKeyEvent* digraphEvent = new QKeyEvent( QEvent::KeyPress, 0, 0, 0, newChar, event->isAutoRepeat(), 0 );
                        QLineEdit::keyPressEvent( digraphEvent );
                        return;
                    }
                }
            }
        }
    }
    QLineEdit::keyPressEvent( event );
}

void DigraphLineEdit::setDigraphEnabled( bool isEnabled ) {
    this->digraphEnabled = isEnabled;
}

bool DigraphLineEdit::isDigraphEnabled() const {
    return( digraphEnabled );
}
