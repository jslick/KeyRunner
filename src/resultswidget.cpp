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
