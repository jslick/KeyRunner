/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

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
    void selectDown();
    void selectUp();
};

#endif // RESULTSWIDGET_H
