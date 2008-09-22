#ifndef LANGUAGE_UPDATE_PROXY_H
#define LANGUAGE_UPDATE_PROXY_H 

#include <qobject.h>
#include <qstring.h>

/*
 * The sole purpose of this class is to emit a signal
 * when a study language is selected or deselected.
 * This class is used by StudyLanguageItem.
 * I was forced to do that because QCheckListItem doesn't
 * inherit from QObject and cannot emit signals.
 * and also because it's not possible to refer to PreferencesDialog from
 * QCheckListItem because of C++ mutual reference problem.
 */
class LanguageUpdateNotifier : public QObject {

    Q_OBJECT

public:

    void addLanguage( const QString& language );
    void removeLanguage( const QString& language );

signals:

    void languageAdded( const QString& language );
    void languageRemoved( const QString& language );

private:

};

#endif
