#include <QDir>
#include <QFileDialog>
#include <QGraphicsView>

#include "LoadWorker.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene = new LayerScene(this);
    ui->layerGraphicsView->setScene(m_scene);

    LoadWorker* worker = new LoadWorker;
    worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::startLoad, worker, &LoadWorker::loadModel);
    connect(worker, &LoadWorker::modelReady, this, &MainWindow::newModelReady);
    m_workerThread.start();

    connect(m_scene, &LayerScene::infoMessage, this, &MainWindow::appendLog);
}

MainWindow::~MainWindow()
{
    delete ui;

    m_workerThread.quit();
    m_workerThread.wait();
}

void MainWindow::appendLog(const QString& text)
{
    ui->logText->appendPlainText(text);
}

void MainWindow::appendErrorLog(const QString& text)
{
    ui->errorLogText->appendPlainText(text);
}

void MainWindow::updateStep(std::shared_ptr<Step> step)
{
    qDebug() << Q_FUNC_INFO;

    int currentIndex = ui->viewTabWidget->currentIndex();
    ui->viewTabWidget->setTabText(currentIndex, step->name);
}

void MainWindow::newModelReady(std::shared_ptr<OdbppModel> model)
{
    ///@todo what if there's multiple steps
    int currentIndex = ui->viewTabWidget->currentIndex();
    ui->viewTabWidget->setTabText(currentIndex, model->steps().at(0)->name);

    appendLog("Loading graphics...");

    ui->layerGraphicsView->setUpdatesEnabled(false);

    std::vector<std::shared_ptr<Layer>> layers = model->layers();
    for (auto& layer : layers) {
        m_scene->addLayer(layer);
    }

    //QRectF bounding = m_scene->itemsBoundingRect();
    //qDebug() << Q_FUNC_INFO << "bounding: " << bounding;
    //m_scene->setSceneRect(bounding);

    ui->layerGraphicsView->setUpdatesEnabled(true);
    ui->layerGraphicsView->centerOn(QPointF(0.0, 0.0));
}


///@todo if we already have something open, load into a new tab
void MainWindow::on_openButton_clicked()
{
    QString lastDir;
    if (m_settings.contains("lastdir")) {
        lastDir = m_settings.value("lastdir").toString();
    } else {
        lastDir = QDir::home().path();
    }

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open ODB++ Folder"),
        lastDir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        appendLog("Loading " + dir);

        emit startLoad(dir);

        m_settings.setValue("lastdir", dir);
    }
}

