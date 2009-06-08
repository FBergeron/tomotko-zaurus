/****************************************************************************
* ZFileDialog.h
* 
* The code of this class is borrowed from zfiledialog.h (rev. 1.5) found in 
* ZBEDic project.
* Copyright (C) 2004 Rafal Mantiuk <rafm@users.sourceforge.org>
****************************************************************************/

#ifndef ZFILE_DIALOG_H
#define ZFILE_DIALOG_H

#include <qcheckbox.h>
#include <qdialog.h>
#include <qdir.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qmovie.h>
#include <qpixmap.h>
#include <iostream.h>
#include <qvbox.h>
#include <qhgroupbox.h>

#include "PixMap.h"

class QListView;
class QComboBox;
class QListViewItem;
class QLineEdit;
class QVGroupBox;

class ZFileCustomFilter {

public:

    virtual void filter( QStringList &fileList ) = 0;

};


class ZFileDialog : public QDialog {

    Q_OBJECT

public:

    enum Mode { AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly };
    ZFileDialog( const QString title, const QString &path, Mode mode, bool useImagePreview, QWidget *parent );
  
    const QDir* dir() const {
      return &currentDir;
    }

    void setFilters( const QStringList &filters );
  
    QStringList selectedFiles() const;
    QString selectedFile() const;

    void setSelection( const QString &name );

    void setCustomFilter( ZFileCustomFilter *filter ) {
        customFilter = filter;
    }

    virtual void show();

protected:

    QDir currentDir;
    QHBox       *hbox;
    QVBox       *vbox;
    QListView   *fileLV;
    QComboBox   *fstabCB;
    QCheckBox   *showImagePreview;
    QHGroupBox  *imagePreviewBox;
    QVBox       *imagePreviewWrapper;
    QLabel      *imagePreview;
    QVBox       *fileLVBox;

    QWidget     *mainPanel;
    QBoxLayout  *mainPanelLayout;

    QStringList dir_list;
    QStringList filters;

    QLineEdit *selection;

    Mode mode;
    bool previewImage;

    ZFileCustomFilter *customFilter;

    bool imagePreviewEnabled;
    bool imagePreviewDirty;
    QString imagePreviewPath;

private:

    void insertDirEntry( const QString &label, const QString &path, QPixmap *pixmap = NULL );
    void clearImagePreview();
    void initImagePreview( const QString& imagePath ); 
    void resizeImagePreview();
    bool eventFilter( QObject* target, QEvent* evt );

protected:

    void setDir( QString path );
    void refreshFileList();
    int insertDirTree( const QString &label, QString basePath, const QString &path, bool subCheck,
    QPixmap *pixmap = NULL );

public slots:

    void updateGeometry();

protected slots:

    void dirSelected( int idx );
    void fileClicked( QListViewItem *it );
    void parentDirClicked();
    void itemSelected();
    void setImagePreviewEnabled( bool isOn );
    bool isImagePreviewEnabled() const;
    void resizeEvent( QResizeEvent* );

};

#endif
