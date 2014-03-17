#include "searchlineedit.h"

#include <QKeyEvent>

SearchLineEdit::SearchLineEdit(QWidget* parent) :
    QLineEdit(parent)
{
}

void SearchLineEdit::keyPressEvent(QKeyEvent* event)
{
    if (event && event->key() == Qt::Key_Up)
        emit keyUp();
    else if (event && event->key() == Qt::Key_Down)
        emit keyDown();
    else
        QLineEdit::keyPressEvent(event);
}
