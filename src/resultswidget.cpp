/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "resultswidget.h"

#include <searchresultinterface.h>
#include <QListWidgetItem>

class ResultsListItem : public QListWidgetItem
{
public:
    SearchResultInterface* result;

    ResultsListItem(SearchResultInterface* result)
        : QListWidgetItem(),
          result(result)
    {
        if (result)
        {
            this->setText(result->getDisplay());
        }
    }

    ~ResultsListItem()
    {
        if (result && result->isDynamicallyAllocated())
            delete result;
    }
};

ResultsWidget::ResultsWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void ResultsWidget::addResult(SearchResultInterface* result)
{
    if (!result)
        return;

    for (int i = 0; i < this->count(); i++)
    {
        ResultsListItem* item = static_cast<ResultsListItem*>(this->item(i));
        if (!item)
            continue;

        Q_ASSERT(item->result);

        if (item->result == result || item->result->getId() == result->getId())
            return;
    }

    ResultsListItem* item = new ResultsListItem(result);
    this->addItem(item);
    if (this->count() == 1)
        this->setCurrentRow(0);
}

SearchResultInterface* ResultsWidget::getCurrentResult()
{
    auto item = dynamic_cast<ResultsListItem*>(this->currentItem());
    return item ? item->result : 0;
}

void ResultsWidget::selectDown()
{
    if (this->currentRow() < this->count() - 1)
        this->setCurrentRow(this->currentRow() + 1);
}

void ResultsWidget::selectUp()
{
    if (this->currentRow() < 0)
        this->setCurrentRow(this->count() - 1);
    else if (this->currentRow() > 0)
        this->setCurrentRow(this->currentRow() - 1);
}
