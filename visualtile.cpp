#include "usermap.h"

#include "visualtile.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


QRectF VisualTile::boundingRect() const
{
    return QRectF(0, 0, tileSize, tileSize);
}

QPainterPath VisualTile::shape() const
{
    QPainterPath path;
    path.addRect(1, 1, tileSize-2, tileSize-2);
    return path;
}

void VisualTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = color;

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 0.2) {
        painter->fillRect(QRectF(0, 0, tileSize, tileSize), fillColor);
        return;
    }

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    painter->drawRect(1, 1, tileSize-2, tileSize-2);
    painter->setBrush(b);
    return;
}

void VisualTile::setColor(const QColor &n_color)
{
    color = n_color;
}

void VisualTile::setCoords(int n_x, int n_y)
{
    x = n_x;
    y = n_y;
    setZValue((x + y) % 2);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

bool VisualTile::event(QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneHoverEnter)
    {
        emit mouseEntered(QPoint(x,y));
        event->accept();
    }
    if (event->type() == QEvent::GraphicsSceneHoverLeave)
    {
        emit mouseLeaved(QPoint(x,y));
        event->accept();
    }
    return QObject::event(event);
}

void VisualTile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit mousePressed(QPoint(x,y));
    QGraphicsItem::mousePressEvent(event);
    update();
}
