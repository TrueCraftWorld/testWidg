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



class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(VisualMap *v) : QGraphicsView(), m_map(v) { }

private:
    void wheelEvent(QWheelEvent *) override;

private:
    VisualMap *m_map;
};




class VisualMap : public QFrame
{
    Q_OBJECT
public:
    explicit VisualMap(const QString &name, QWidget *parent = nullptr);

    QGraphicsView *tileMap() const;

public slots:
    void zoomInBy(int level);
    void zoomOutBy(int level);

private slots:
    void setupMatrix();

signals:
    zoomed();

private:
    GraphicsView *graphicsView;
    QPushButton *findPathButton;

    qint32 zoomLevel = 0;
};

#endif // VISUALMAP_H
