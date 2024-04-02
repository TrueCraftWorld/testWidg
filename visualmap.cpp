#include "visualmap.h"
#include <QtMath>
#include <QtWidgets>
#include <QBoxLayout>


void MapView::wheelEvent(QWheelEvent *e)
{
        if (e->angleDelta().y() > 0)
            zoomInBy(6);
        else
            zoomOutBy(6);
        e->accept();
}

QGraphicsView *MapView::tileMap() const
{
    return me;
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
    if (zoomLevel <= 500 - level) {
        zoomLevel += level;
        emit zoomed();
    } else if (zoomLevel < 500) {
        zoomLevel = 500;
        emit zoomed();
    }
}

void MapView::zoomOutBy(int level)
{
    if (level < 0) return;
    if(zoomLevel >= level) {
        zoomLevel -= level;
        emit zoomed();
    } else if (zoomLevel > 0) {
        zoomLevel = 0;
        emit zoomed();
    }
}

void MapView::zoomReset()
{
    resetTransform();
    zoomLevel = 150;

    emit zoomed();
    update();
}
