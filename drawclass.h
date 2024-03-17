#ifndef DRAWCLASS_H
#define DRAWCLASS_H

#include <QObject>
#include <QWidget>

class DrawClass : public QObject
{
    Q_OBJECT
public:
    explicit DrawClass(QObject *parent = nullptr);

signals:

};

#endif // DRAWCLASS_H
