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
