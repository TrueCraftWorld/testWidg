#include "visualmap.h"
#include <QtMath>
#include <QtWidgets>
#include <QBoxLayout>


void MapView::wheelEvent(QWheelEvent *e)
{
        if (e->angleDelta().y() > 0)
            m_map->zoomInBy(6);
        else
            m_map->zoomOutBy(6);
        e->accept();
}

VisualMap::VisualMap(QWidget *parent)
    : QFrame(parent)
{

    graphicsView = new MapView(this);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    // graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    QBoxLayout *topLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    topLayout->addWidget(graphicsView);
    setLayout(topLayout);


    connect(this, &VisualMap::zoomed, this, &VisualMap::setupMatrix);

    setupMatrix();
}

QGraphicsView *VisualMap::tileMap() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}

void VisualMap::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomLevel - 250) / qreal(50));
    QTransform matrix;
    matrix.scale(scale, scale);
    graphicsView->setTransform(matrix);
}


void VisualMap::zoomInBy(int level)
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

void VisualMap::zoomOutBy(int level)
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

void VisualMap::zoomReset()
{
    graphicsView->resetTransform();
    // if (zoomLevel != 100) {
        zoomLevel = 100;
        emit zoomed();
    // }
    update();
}
