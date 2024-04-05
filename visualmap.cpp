#include "visualmap.h"
#include <QtMath>
#include <QtWidgets>
#include <QBoxLayout>


void MapView::wheelEvent(QWheelEvent *e)
{
        if (e->angleDelta().y() > 0)
            zoomInBy(10);
        else
            zoomOutBy(10);
        e->accept();
}

QGraphicsView *MapView::tileMap() const
{
    return me;
}

void MapView::zoomToFit()
{
    // resetMatrix();
    resetTransform();
    qreal deltaY = sceneRect().height() / (geometry().height());
    qreal deltaX = sceneRect().width() / (geometry().width());
    qreal delta =0;
    QTransform matrix;

    if (deltaX > 1 && deltaY > 1) {
        delta = deltaX > deltaY ? deltaX : deltaY;
        delta *= 1.05;
    } else if (deltaX < 1 && deltaY < 1) {
        delta = deltaX > deltaY ? deltaX : deltaY;
        delta *= 1.05;
    } else {
        delta = deltaX < deltaY ? deltaY : deltaX;
        delta *= 1.05;
    }
    // delta =
    matrix.scale((1/delta), (1/delta));
    setTransform(matrix);
    qreal power = qLn(1/delta)/qLn(2);
    zoomLevel = static_cast<quint32>((power * qreal(50))+250);


}

void MapView::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomLevel - 250) / qreal(50));
    QTransform matrix;
    matrix.scale(scale, scale);
    setTransform(matrix);
}


MapView::MapView(QWidget *parent)
{
    me = static_cast<QGraphicsView *>(this);

    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    connect(this, &MapView::zoomed, this, &MapView::setupMatrix);
    setupMatrix();
}

void MapView::zoomInBy(int level)
{
    if (level < 0) return;
    if (zoomLevel <= 250 - level) {
        //some max zoom in (about 11x11 tiles at minimum window size)
        zoomLevel += level;
        emit zoomed();
    }
}

void MapView::zoomOutBy(int level)
{
    if (level < 0) return;

    qreal scale = qPow(qreal(2), (zoomLevel - 250) / qreal(50));
    if (sceneRect().width()*scale < geometry().width() &&
            sceneRect().height()*scale < geometry().height()) return;
    //restric zooming out if map is completly seen
        zoomLevel -= level;
        emit zoomed();
}

void MapView::zoomReset()
{
    resetTransform();
    zoomLevel = 150;
    emit zoomed();
    update();
}
