#include "usermap.h"

#include "visualtile.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


QRectF VisualTile::boundingRect() const
{
    return QRectF(0, 0, 50, 50);
}

QPainterPath VisualTile::shape() const
{
    QPainterPath path;
    path.addRect(1, 1, 48, 48);
    return path;
}

void VisualTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = color;
//    QColor fillColor = (option->state & QStyle::State_Selected) ? color.darker(150) : color;
//    if (option->state & QStyle::State_MouseOver)
//        fillColor = fillColor.lighter(125);

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
//    if (lod < 0.2) {
        if (lod < 0.2) {
            painter->fillRect(QRectF(0, 0, 50, 50), fillColor);
            return;
        }

        QBrush b = painter->brush();
        painter->setBrush(fillColor);
        painter->drawRect(1, 1, 48, 48);
        painter->setBrush(b);
        return;
//    }

//    QPen oldPen = painter->pen();
//    QPen pen = oldPen;
//    int width = 0;
//    if (option->state & QStyle::State_Selected)
//        width += 2;

//    pen.setWidth(width);
//    QBrush b = painter->brush();
//    painter->setBrush(QBrush(fillColor.darker(option->state & QStyle::State_Sunken ? 120 : 100)));

//    painter->drawRect(QRect(3, 3, 44, 44));
//    painter->setBrush(b);
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
