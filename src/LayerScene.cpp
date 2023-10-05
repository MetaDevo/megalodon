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
    setBackgroundBrush(Qt::black);
    m_defaultPen.setColor(QColor(0, 255, 0, 255));

    setItemIndexMethod(ItemIndexMethod::BspTreeIndex);
}

LayerScene::~LayerScene()
{
}

long long int LayerScene::numFeatures() const
{
    return items().size();
}

void LayerScene::addLayer(std::shared_ptr<Layer> layer)
{
    qDebug() << Q_FUNC_INFO << layer->name;    

    for (const auto& rec : layer->lines) {
        QLineF line(rec.xs * m_mult, -rec.ys * m_mult, rec.xe * m_mult, -rec.ye * m_mult);
        addLine(line, m_defaultPen); ///@todo use specified color
    }

    qDebug() << "bspTreeDepth: " << bspTreeDepth();
}



