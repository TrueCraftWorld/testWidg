#ifndef VISUALMAP_H
#define VISUALMAP_H

#include <QFrame>
#include <QPushButton>
#include <QGraphicsView>
#include <QObject>



QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
QT_END_NAMESPACE

class VisualMap;



class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    MapView(VisualMap *v) : QGraphicsView(), m_map(v) { }

private:
    void wheelEvent(QWheelEvent *) override;

private:
    VisualMap *m_map;
};




class VisualMap : public QFrame
{
    Q_OBJECT
public:
    explicit VisualMap(QWidget *parent = nullptr);

    QGraphicsView *tileMap() const;

public slots:
    void zoomInBy(int level);
    void zoomOutBy(int level);
    void zoomReset();

private slots:
    void setupMatrix();

signals:
    void zoomed();

private:
    MapView *graphicsView;


    qint32 zoomLevel = 120;
};

#endif // VISUALMAP_H
