#include "LoadWorker.hpp"

#include "OdbppModel.hpp"

LoadWorker::LoadWorker(QObject* parent)
    : QObject{parent}
{
    m_model = std::make_shared<OdbppModel>();
}

void LoadWorker::loadModel(const QDir& dir)
{
    qDebug() << Q_FUNC_INFO;
    m_model->load(dir);
    emit modelReady(m_model);
}
