#include <QDebug>
#include <QFileInfoList>
#include <QTextStream>

#include "odbppmodel.hpp"

OdbppModel::OdbppModel()
{

}

void OdbppModel::load(const QDir& dir)
{
    qDebug() << Q_FUNC_INFO;
    bool matrixFound = false;
    bool stepsFound = false;

    const QFileInfoList fileList = dir.entryInfoList();
    for (const auto& file : fileList)
    {
        if(file.fileName() == "matrix") {
            if (file.isDir()) {
                matrixFound = loadMatrix(file.absoluteFilePath());
            }
        }
        if(file.fileName() == "steps") {
            if (file.isDir()) {
                stepsFound = loadSteps(file.absoluteFilePath());
            }
        }
    }

    if (!matrixFound) {
        emit errorMessage(dir.dirName() + ": Matrix not found.");
    }

    if (!stepsFound) {
        emit errorMessage(dir.dirName() + ": Steps not found.");
    }

    qInfo() << Q_FUNC_INFO << "Loading complete.";
}

bool OdbppModel::loadMatrix(const QString& dirPath)
{
    QDir dir(dirPath);
    const QFileInfoList fileList = dir.entryInfoList();
    for (const auto& file : fileList)
    {
        if (file.fileName() == "matrix" && file.isFile()) {
            emit infoMessage("Loading matrix...");
            QFile data(file.absoluteFilePath());
            if (data.open(QFile::ReadOnly)) {
                QTextStream stream(&data);
                QString line;
                while (stream.readLineInto(&line)) {
                    ///@todo load Matrix
                }
            } else {
                emit errorMessage("Couldn't open matrix file");
            }
            return true;
        }
    }

    return false;
}

bool OdbppModel::loadSteps(const QString& dirPath)
{
    QDir dir(dirPath);

    const QFileInfoList fileList = dir.entryInfoList();
    for (const auto& file : fileList)
    {
        if (file.isDir() && file.fileName() != "." && file.fileName() != "..") {
            std::shared_ptr<Step> step = std::make_shared<Step>();
            step->name = file.fileName();
            emit infoMessage("Loading step " + step->name + "...");

            loadStepDir(file.absoluteFilePath());
            m_steps.push_back(step);
            emit infoMessage("Step loaded.");
            //emit stepLoaded(step);

            ///@todo load all steps
            return true;
        }
    }

    return false;
}

void OdbppModel::loadStepDir(const QString& dirPath)
{
    QDir dir(dirPath);

    const QFileInfoList fileList = dir.entryInfoList();
    qDebug() << Q_FUNC_INFO << fileList.size();
    for (const auto& file : fileList)
    {
        if (file.isDir() && file.fileName() == "layers") {
            qDebug() << Q_FUNC_INFO << file.fileName() << "is a layer folder.";
            loadLayerDir(file.absoluteFilePath());
        }
    }
}

void OdbppModel::loadLayerDir(const QString& dirPath)
{
    QDir dir(dirPath);

    const QFileInfoList fileList = dir.entryInfoList();
    qDebug() << Q_FUNC_INFO << fileList.size();
    for (const auto& file : fileList)
    {
         if (file.isDir() && file.fileName() != "." && file.fileName() != "..") {
            std::shared_ptr<Layer> layer = std::make_shared<Layer>();
            layer->name = file.fileName();
            emit infoMessage("Loading layer " + layer->name + "...");
            loadLayer(file.absoluteFilePath(), layer);
            m_layers.push_back(layer);
            emit infoMessage("Layer loaded.");
            emit layerLoaded(layer);
         }
    }
}

void OdbppModel::loadLayer(const QString& dirPath, std::shared_ptr<Layer> layer)
{
    if (!layer) {
         emit errorMessage("Layer null.");
         return;
    }
    QDir dir(dirPath);
    qDebug() << Q_FUNC_INFO << dir.dirName();

    const QFileInfoList fileList = dir.entryInfoList();
    qDebug() << Q_FUNC_INFO << fileList.size();
    for (const auto& file : fileList)
    {
         if (file.fileName() == "features") {
            loadLayerFeatures(file.absoluteFilePath(), layer);
         }
    }
}

void OdbppModel::loadLayerFeatures(const QString& filePath, std::shared_ptr<Layer> layer)
{
    QChar prefixLine('L');
    QFile data(filePath);
    if (data.open(QFile::ReadOnly)) {
        QTextStream stream(&data);

        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith(prefixLine)) {
                layer->lines.push_back(loadLineRecord(line));
            }
        }
    } else {
        emit errorMessage("Couldn't open file: " + filePath);
    }
}

LineRecord OdbppModel::loadLineRecord(const QString& str)
{
    LineRecord rec;
    QStringList values = str.split(' ');
    if (values.size() < 4) {
        emit errorMessage("Not enough values.");
    } else {
        rec.xs = values[1].toDouble();
        rec.ys = values[2].toDouble();
        rec.xe = values[3].toDouble();
        rec.ye = values[4].toDouble();
    }
    return rec;
}

void OdbppModel::readVariables(QTextStream stream)
{
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList fields = line.split('=');

        if (fields.size() == 2)
        {
            QString name = fields[0].trimmed();
            QString value = fields[1].trimmed();

            qDebug() << "Name:" << name << ", Value:" << value;
        }
    }
}
