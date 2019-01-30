#ifndef SELECTABLE_H
#define SELECTABLE_H

#include <QObject>
#include <string>

class Selectable : public QObject{
    Q_OBJECT
public:
    virtual ~Selectable();
    virtual std::string path() const = 0;
    virtual void setSelected(bool selected) = 0;
};

#endif // SELECTABLE_H
