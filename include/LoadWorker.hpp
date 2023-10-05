#ifndef LOADWORKER_HPP
#define LOADWORKER_HPP

#include <QDir>
#include <QObject>

class OdbppModel;

class LoadWorker : public QObject
{
    Q_OBJECT
public:
    explicit LoadWorker(QObject* parent = nullptr);

public slots:
    void loadModel(const QDir& dir);

signals:
    void modelReady(std::shared_ptr<OdbppModel> model);
    void infoMessage(const QString& message);
    void errorMessage(const QString& message);

private:
    std::shared_ptr<OdbppModel> m_model;

};

#endif // LOADWORKER_HPP
