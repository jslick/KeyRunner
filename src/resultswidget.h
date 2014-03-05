#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QListWidget>

class SearchResultInterface;

class ResultsWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ResultsWidget(QWidget *parent = 0);

    void addResult(SearchResultInterface* result);

    SearchResultInterface* getCurrentResult();

signals:

public slots:

};

#endif // RESULTSWIDGET_H
