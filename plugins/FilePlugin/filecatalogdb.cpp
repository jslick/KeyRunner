#include "filecatalogdb.h"

FileCatalogDb::FileCatalogDb()
{
}

void FileCatalogDb::add(const QString& searchTerm, const QString& display, const QString& filename)
{
    QHash<QChar,HashValue>* current = &this->index;
    for (int i = 0; i < searchTerm.length(); i++)
    {
        Q_ASSERT(current);

        HashValue& value = (*current)[searchTerm.at(i).toLower()];
        value.files.push_back({ display, filename });
        current = &value.subindex;
    }
}

void FileCatalogDb::find(const QString& searchTerm, std::function<void(const QString& display,const QString& filename)> matchCallback)
{
    QHash<QChar,HashValue>* current = &this->index;
    HashValue* currentValue = 0;
    for (QChar ch : searchTerm.toLower())
    {
        Q_ASSERT(current);

        auto iter = current->find(ch);
        if (iter == current->end())
            return;

        currentValue = &iter.value();
        current = &iter.value().subindex;
    }

    if (currentValue && matchCallback)
    {
        for (const Match& file: currentValue->files)
        {
            matchCallback(file.display, file.filename);
        }
    }
}
