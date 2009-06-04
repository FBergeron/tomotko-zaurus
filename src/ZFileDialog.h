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
#include <qpixmap.h>
#include <iostream.h>
#include <qvgroupbox.h>

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
    ZFileDialog( const QString title, const QString &path, Mode mode, QWidget *parent );
  
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
    QListView   *fileLV;
    QComboBox   *fstabCB;
    QCheckBox   *showImagePreview;
    QVGroupBox  *imagePreviewBox;
    QLabel      *imagePreview;
    QHBox       *fileLVHBox;

    QStringList dir_list;
    QStringList filters;

    QLineEdit *selection;

    Mode mode;

    ZFileCustomFilter *customFilter;

private:

    void insertDirEntry( const QString &label, const QString &path, QPixmap *pixmap = NULL );

protected:

    void setDir( QString path );
    void refreshFileList();
    int insertDirTree( const QString &label, QString basePath, const QString &path, bool subCheck,
    QPixmap *pixmap = NULL );

protected slots:

    void dirSelected( int idx );
    void fileClicked( QListViewItem *it );
    void parentDirClicked();
    void itemSelected();
    void toggleImagePreview( bool isOn );

};

#endif
