#include "visualmap.h"
// #include <QtOpenGL>
#include <QtMath>
#include <QtWidgets>

void GraphicsView::wheelEvent(QWheelEvent *e)
{
        if (e->angleDelta().y() > 0)
            m_map->zoomInBy(6);
        else
            m_map->zoomOutBy(6);
        e->accept();
}

VisualMap::VisualMap(const QString &name, QWidget *parent)
    : QFrame(parent)
{

    graphicsView = new GraphicsView(this);
    // graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    // graphicsView->


    // Label layout


    QGridLayout *topLayout = new QGridLayout;

    topLayout->addWidget(graphicsView, 1, 0);
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
