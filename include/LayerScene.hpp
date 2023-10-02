#ifndef LAYERSSCENE_HPP
#define LAYERSSCENE_HPP

#include <memory>
#include <QGraphicsScene>

struct Layer;

class LayerScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit LayerScene(QWidget* parent = nullptr);
    ~LayerScene();

public slots:    
    void addLayer(std::shared_ptr<Layer> layer);

signals:
    void infoMessage(const QString& text);

private:
    QPen m_defaultPen;

    const float m_mult = 1000;
};

#endif // LAYERSSCENE_HPP
