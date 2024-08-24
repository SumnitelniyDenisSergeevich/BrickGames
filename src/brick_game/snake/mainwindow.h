/*!
 * \file
 * \brief Заголовочный файл графической составляющей Змейки
 *
 * Данный файл содержит в себе методы обработки ввода
 * пользовательских комманд
 */
#pragma once

#include <QKeyEvent>
#include <QMainWindow>
#include <QTimer>

#include "GraphicModel.h"
#include "SnakeModel.h"

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
  /*!
   * Обрабатывает ввод пользователя
   * \param event Действие пользователя
   */
  void keyPressEvent(QKeyEvent *event) override;

 private:
  void updateApp();

 private:
  Ui::MainWindow *ui;

 private:
  s21::SnakeModel m_snakeModel;
  s21::GraphicModel m_graphicModel;
  QTimer m_gameTimer;
  int m_speed;
};
