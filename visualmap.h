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


class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    MapView(QWidget *parent = nullptr);
    void zoomInBy(int level);
    void zoomOutBy(int level);
    void zoomReset();
    QGraphicsView *tileMap() const;
    void zoomToFit(void);

signals:
    void zoomed();

private:
    void setupMatrix();
    void wheelEvent(QWheelEvent *) override;
    QGraphicsView *me;
    qint32 zoomLevel = 120;
    qint32 zoomMax = 500;
};

#endif // VISUALMAP_H
