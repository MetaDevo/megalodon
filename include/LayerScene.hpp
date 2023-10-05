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

    long long int numFeatures() const;

public slots:    
    void addLayer(std::shared_ptr<Layer> layer);

signals:
    void infoMessage(const QString& text);

private:
    QPen m_defaultPen;

    const double m_mult = 1000.0;
};

#endif // LAYERSSCENE_HPP
