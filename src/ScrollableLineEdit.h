/*
 * The source code of this class comes from Qt (qt-x11/qt-2.3.2/src/widgets/qlineedit.h/cpp).
 * It's been slightly modified to allow scrollRight() and scrollLeft() features.
 * This was needed because the API doesn't allow to retrieve the offset.
 * If there is a better way to implement that, suggestions are welcome.
 *
 */


/**********************************************************************
** $Id: qt/src/widgets/qlineedit.h   2.3.3   edited 2001-04-04 $
**
** Definition of ScrollableLineEdit widget class
**
** Created : 941011
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef SCROLLABLE_LINEEDIT_H
#define SCROLLABLE_LINEEDIT_H

struct ScrollableLineEditPrivate;

class QComboBox;
class QValidator;


#ifndef QT_H
#include "qwidget.h"
#include "qstring.h"
#include <iostream.h>
#endif // QT_H

#ifndef QT_NO_LINEEDIT

class Q_EXPORT ScrollableLineEdit : public QWidget
{
    Q_OBJECT
    Q_ENUMS( EchoMode )
    Q_PROPERTY( QString text READ text WRITE setText )
    Q_PROPERTY( int maxLength READ maxLength WRITE setMaxLength )
    Q_PROPERTY( bool frame READ frame WRITE setFrame )
    Q_PROPERTY( EchoMode echoMode READ echoMode WRITE setEchoMode )
    Q_PROPERTY( QString displayText READ displayText )
    Q_PROPERTY( int cursorPosition READ cursorPosition WRITE setCursorPosition )
    Q_PROPERTY( Alignment alignment READ alignment WRITE setAlignment )
    Q_PROPERTY( bool edited READ edited WRITE setEdited )
    Q_PROPERTY( bool hasMarkedText READ hasMarkedText )
    Q_PROPERTY( QString markedText READ markedText )
    
public:
    ScrollableLineEdit( QWidget *parent, const char *name=0 );
    ScrollableLineEdit( const QString &, QWidget *parent, const char *name=0 );
   ~ScrollableLineEdit();

    QString text() const;

    QString displayText() const;

    int     maxLength() const;
    virtual void setMaxLength( int );

    virtual void setFrame( bool );
    bool    frame() const;

    enum    EchoMode { Normal, NoEcho, Password };
    virtual void setEchoMode( EchoMode );
    EchoMode    echoMode() const;
#if QT_VERSION >= 300
#error "Make setReadOnly virtual"
#endif
    void setReadOnly( bool );
    bool isReadOnly() const;

    virtual void setValidator( const QValidator * );
    const QValidator * validator() const;

    QSize   sizeHint() const;
    QSize   minimumSizeHint() const;
    QSizePolicy sizePolicy() const;

    virtual void setEnabled( bool );
    virtual void setFont( const QFont & );
    virtual void setPalette( const QPalette & );
    virtual void setSelection( int, int );

    virtual void setCursorPosition( int );
    int     cursorPosition() const;

    bool    validateAndSet( const QString &, int, int, int );

#ifndef QT_NO_CLIPBOARD
    void    cut();
    void    copy() const;
    void    paste();
#endif

    void setAlignment( int flag );
    int alignment() const;

    void    cursorLeft( bool mark, int steps = 1 );
    void    cursorRight( bool mark, int steps = 1 );
    void    cursorWordForward( bool mark );
    void    cursorWordBackward( bool mark );
    void    backspace();
    void    del();
    void    home( bool mark );
    void    end( bool mark );

    void    setEdited( bool );
    bool    edited() const;

    bool    hasMarkedText() const;
    QString markedText() const;
#if defined(Q_INCOMPATIBLE_3_0_ADDONS)
    bool    getSelection( int *start, int *end );
    void    setPasswordChar( QChar c );
    QChar   passwordChar() const;
#endif    

public slots:
    virtual void setText( const QString &);

    void    selectAll();
    void    deselect();

    void    clearValidator();

    void    insert( const QString &);

    void    clear();

    void    scrollLeft(); // For toMOTko.
    void    scrollRight(); // For toMOTko.

signals:
    void    textChanged( const QString &);
    void    returnPressed();
    void    characterClicked( const QChar&, const QPoint& );

protected:
    bool    event( QEvent * );
    void    mousePressEvent( QMouseEvent * );
    void    mouseMoveEvent( QMouseEvent * );
    void    mouseReleaseEvent( QMouseEvent * );
    void    mouseDoubleClickEvent( QMouseEvent * );
    void    keyPressEvent( QKeyEvent * );
    void    focusInEvent( QFocusEvent * );
    void    focusOutEvent( QFocusEvent * );
    void    paintEvent( QPaintEvent * );
    void    resizeEvent( QResizeEvent * );
    void    leaveEvent( QEvent * );
#ifndef QT_NO_DRAGANDDROP
    void    dragEnterEvent( QDragEnterEvent * );
    void    dropEvent( QDropEvent * );
#endif

    void    repaintArea( int, int );

    void    keyReleaseEvent( QKeyEvent * ); // For toMOTko.

private slots:
    void    clipboardChanged();
    void    blinkSlot();
    void    dragScrollSlot();

//#ifndef QT_NO_DRAGANDDROP
//    void    doDrag();
//#endif

private:
    // kept
    void    newMark( int pos, bool copy=TRUE );
    void    markWord( int pos );
    int     lastCharVisible() const;
    int     minMark() const;
    int     maxMark() const;
    int     frameW() const;

    void    init();

    QString tbuf;
    ScrollableLineEditPrivate * d;
    int     cursorPos;
    int     offset;
    int     maxLen;
    int     markAnchor;
    int     markDrag;
    bool    cursorOn;
    bool    dragScrolling;
    bool    scrollingLeft;
    int     alignmentFlag;
    bool    ed;

    void updateOffset();
    int xPosToCursorPos( int ) const;
    void blinkOn();
    void makePixmap() const;
    void undoInternal();
    void redoInternal();

private:    // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    ScrollableLineEdit( const ScrollableLineEdit & );
    ScrollableLineEdit &operator=( const ScrollableLineEdit & );
#endif
};

#endif // QT_NO_LINEEDIT

#endif // QLINEEDIT_H
