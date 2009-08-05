#include "GradeButton.h"

GradeButton::GradeButton( int grade, QWidget* parent /* = 0 */ ) : QPushButton( parent ), grade( grade ) {
}

GradeButton::~GradeButton() {
}

int GradeButton::getGrade() const {
    return( grade );
}

void GradeButton::gradeAnswer() {
    emit( gradeSubmitted( grade ) );
}

