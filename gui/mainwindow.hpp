#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>

#include <QGraphicsScene>
#include <QMainWindow>
#include <QSettings>

#include "LayerScene.hpp"
#include "OdbppModel.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void appendLog(const QString& text);
    void appendErrorLog(const QString& text);
    void updateStep(std::shared_ptr<Step> step);
    void newModelReady(std::shared_ptr<OdbppModel> model);

private slots:
    void on_openButton_clicked();

signals:
    void startLoad(const QDir& dir);

private:
    Ui::MainWindow *ui;

    QSettings m_settings;
    std::shared_ptr<OdbppModel> m_model;
    LayerScene* m_scene = nullptr;
    QThread m_workerThread;
};
#endif // MAINWINDOW_HPP
