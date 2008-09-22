#ifndef STUDY_LANGUAGE_ITEM_H
#define STUDY_LANGUAGE_ITEM_H

#include <qlistview.h>
#include <qpe/resource.h>
#include <qstring.h>
#include <qwidget.h>
#include "LanguageUpdateNotifier.h"
#include "Util.h"

class StudyLanguageItem : public QCheckListItem {

public:

    StudyLanguageItem( QListView* parent, LanguageUpdateNotifier* languageUpdateNotifier );
    ~StudyLanguageItem();

protected:

    virtual void stateChange( bool isOn );

private:

    LanguageUpdateNotifier* languageUpdateNotifier;

};

#endif
