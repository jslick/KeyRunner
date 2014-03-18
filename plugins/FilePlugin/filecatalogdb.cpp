/*
 * Copyright (c) 2014 KeyRunner developers
 *
 * See the file license.txt for copying permission.
 */

#include "filecatalogdb.h"

#include <QStringList>
#include <QRegularExpression>

FileCatalogDb::FileCatalogDb()
{
}

void FileCatalogDb::add(const QString& searchTerm, const QString& display, const QString& filename)
{
    QStringList words = searchTerm.split(QRegularExpression("\\s+"));
    for (const QString& word : words)
    {
        QHash<QChar,HashValue>* current = &this->index;
        for (int i = 0; i < word.length(); i++)
        {
            Q_ASSERT(current);

            HashValue& value = (*current)[word.at(i).toLower()];
            value.files.push_back({ display, filename });
            current = &value.subindex;
        }
    }
}

void FileCatalogDb::find(const QString& searchTerm, std::function<void(const QString& display,const QString& filename)> matchCallback)
{
    QStringList words = searchTerm.toLower().split(QRegularExpression("\\s+"));
    const HashValue* matchValue = 0;
    for (const QString& word : words)
    {
        const HashValue* wordMatchValue = this->matchWord(word);
        if (matchValue == 0)
            matchValue = wordMatchValue;
    }

    if (matchValue && matchCallback)
    {
        for (const Match& file: matchValue->files)
        {
            matchCallback(file.display, file.filename);
        }
    }
}

const FileCatalogDb::HashValue* FileCatalogDb::matchWord(const QString& word) const
{
    const QHash<QChar,HashValue>* current = &this->index;
    const HashValue* currentValue = 0;
    for (QChar ch : word.toLower())
    {
        Q_ASSERT(current);

        auto iter = current->find(ch);
        if (iter == current->end())
            return 0;

        currentValue = &iter.value();
        current = &iter.value().subindex;
    }

    return currentValue;
}
