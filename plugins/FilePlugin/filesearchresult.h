#ifndef FILESEARCHRESULT_H
#define FILESEARCHRESULT_H

#include <searchresultinterface.h>

class FileSearchResult : public SearchResultInterface
{
public:
    FileSearchResult(const QString& id, const QString& display, const QString& filename = "");

    virtual bool isDynamicallyAllocated() const;

    virtual const QString& getId() const;

    virtual const QString& getDisplay() const;

    virtual const QString& getSearchTerm() const;

    void setSearchTerm(const QString& searchTerm);

    virtual bool execute();

private:
    QString id;
    QString display;
    QString searchTerm;

    QString filename;
};

#endif // FILESEARCHRESULT_H
