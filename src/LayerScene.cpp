#include <QCoreApplication>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QLineF>

#include "LayerScene.hpp"
#include "OdbppModel.hpp"

LayerScene::LayerScene(QWidget* parent) : QGraphicsScene(parent)
{
    QRectF sceneRect(-5000, -5000, 8000, 8000);
    setSceneRect(sceneRect);

    setBackgroundBrush(Qt::black);
    m_defaultPen.setColor(QColor(0, 255, 0, 255));
}

LayerScene::~LayerScene()
{
}

void LayerScene::addLayer(std::shared_ptr<Layer> layer)
{
    qDebug() << Q_FUNC_INFO << layer->name;    

    for (const auto& rec : layer->lines) {
        QLineF line(rec.xs * m_mult, rec.ys * m_mult, rec.xe * m_mult, rec.ye * m_mult);
        addLine(line, m_defaultPen); ///@todo use specified color
    }

    //qDebug() << "bspTreeDepth: " << bspTreeDepth();
    //qDebug() << Q_FUNC_INFO << "total items: " << items().size();
}


