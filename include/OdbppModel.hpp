#ifndef ODBPPMODEL_HPP
#define ODBPPMODEL_HPP

#include <memory>
#include <vector>

#include <QDir>
#include <QObject>
#include <QThread>

struct LineRecord
{
    double xs = 0.0;
    double ys = 0.0;
    double xe = 0.0;
    double ye = 0.0;
    unsigned int symNum = 0;
    bool polarityPositive = true;
    unsigned int dcode = 0;
    ///@todo atr values
    ///@todo ID
};

struct Layer
{
    unsigned int id;
    unsigned int row;
    QString name;
    std::vector<LineRecord> lines;
    //std::vector<PadRecord> pads;
    //std::vector<ArcRecord> arcs;
    //std::vector<TextRecord> texts;
    //std::vector<BarcodeRecord> barcodes;
    //std::vector<SurfaceRecord> surfaces;
};

struct Step
{
    unsigned int id;
    unsigned int column;
    QString name;
};


class OdbppModel : public QObject
{
    Q_OBJECT
public:

    OdbppModel();
    void load(const QDir& dir);
    std::vector<std::shared_ptr<Step>> steps() { return m_steps; }
    std::vector<std::shared_ptr<Layer>> layers() { return m_layers; }

signals:
    void infoMessage(const QString& message);
    void errorMessage(const QString& message);
    void layerLoaded(std::shared_ptr<Layer> layer);
    void stepLoaded(std::shared_ptr<Step> step);

private:
    bool loadMatrix(const QString& dirPath);
    bool loadSteps(const QString& dirPath);
    void loadStepDir(const QString& dirPath);
    void loadLayerDir(const QString& dirPath);
    void loadLayer(const QString& dirPath, std::shared_ptr<Layer> layer);
    void loadLayerFeatures(const QString& filePath, std::shared_ptr<Layer> layer);
    LineRecord loadLineRecord(const QString& str);
    void readVariables(QTextStream stream);

    QString m_name;
    std::vector<std::shared_ptr<Step>> m_steps;
    std::vector<std::shared_ptr<Layer>> m_layers;
};

#endif // ODBPPMODEL_HPP
