#ifndef GRADE_BUTTON_H
#define GRADE_BUTTON_H

#include <qpushbutton.h>
#include <qwidget.h>

class GradeButton : public QPushButton {

    Q_OBJECT

public:

    GradeButton( int grade, QWidget* parent = 0 );
    ~GradeButton();

    int getGrade() const;

signals:

    void gradeSubmitted( int grade );

public slots:

    void gradeAnswer();

private:

    int grade;

};

#endif


