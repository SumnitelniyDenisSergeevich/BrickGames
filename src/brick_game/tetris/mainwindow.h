#pragma once

#include <QKeyEvent>
#include <QMainWindow>
#include <QTimer>

#include "GraphicModel.h"
#include "TetrisModel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void keyPressEvent(QKeyEvent *event) override;

 private:
  void updateApp(const s21::GameInfo &data);

 private:
  Ui::MainWindow *ui;

 private:
  s21::TetrisModel m_tetrisModel;
  s21::GraphicModel m_graphicModel;
  QTimer m_gameTimer;
  int m_speed;
};
