#ifndef VISUALTILE_H
#define VISUALTILE_H

#include <QColor>
#include <QGraphicsObject>



class VisualTile : public QGraphicsObject
{
    Q_OBJECT
public:
    // VisualTile(const QColor &color, int x, int y, States n_state);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void setColor(const QColor& );
    void setCoords(int n_x, int n_y);

signals:
    void mouseEntered(QPoint);
    void mouseLeaved(QPoint);

protected:
    bool event(QEvent *ev) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int x;
    int y;
    QColor color;
    // States m_state;
};

#endif // VISUALTILE_H
