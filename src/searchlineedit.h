/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#ifndef SEARCHLINEEDITOR_H
#define SEARCHLINEEDITOR_H

#include <QLineEdit>

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchLineEdit(QWidget* parent = 0);

protected:
    void keyPressEvent(QKeyEvent* event);

signals:
    void keyUp();
    void keyDown();

public slots:

};

#endif // SEARCHLINEEDITOR_H
