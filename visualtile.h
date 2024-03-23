#ifndef VISUALTILE_H
#define VISUALTILE_H

#include <QColor>
#include <QGraphicsItem>

enum class States{EMPTY, WALL, START, STOP, PATH};

class VisualTile : public QGraphicsItem
{
    // Q_OBJECT
public:
    VisualTile(const QColor &color, int x, int y, States n_state);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int x;
    int y;
    QColor color;
    States m_state;
};

#endif // VISUALTILE_H
