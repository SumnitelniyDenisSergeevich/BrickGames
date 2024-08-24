/*!
 * \file
 * \brief Заголовочный файл с описанием модели змейки
 *
 * Данный файл содержит в себе все методы модели змейки
 */
#pragma once

#include <QList>
#include <QPoint>
#include <QSettings>

#include "Common.h"

namespace s21 {

/// Набор возможных направлений змейки
enum class SnakeLook {
  Left,   ///< Направление на лево
  Up,     ///< Направление вверх
  Right,  ///< Направление на право
  Down    ///< Направление вниз
};

/*!
 * \brief The SnakeModel class
 *
 * Класс, обрабатывающий модель поведения змейки
 */
class SnakeModel : public QObject {
  Q_OBJECT

 public:
  SnakeModel(int fieldWidth, int fieldHeight, QObject* parent = nullptr);
  ~SnakeModel();

 public:
  /*!
   * Обрабатывает ввод пользователя
   * \param action Действие пользователя
   * \param hold Удерживает ли пользователь действие
   */
  void userInput(UserAction action, bool hold = false);
  /*!
   * Выполняет действие, и возвращает обновленное состояние
   */
  GameInfo updateCurrentState();
  /*!
   * Возвращает состояние
   */
  GameInfo currentState();
  /*!
   * Перезапускает модель
   */
  void restart();
  /*!
   * Устанавливает значение конечного автомата
   */
  void setPause(bool pause);

 signals:
  void collision();
  void win();

 private:
  void createApple();
  void snakeMove(QPoint nextPoint);
  QPoint snakeNextPos();
  void restartModel();
  void rotateLeft();
  void rotateRight();
  bool checkColision(const QPoint& nextPoint);
  bool checkApple(const QPoint& nextPoint);

 private:
  GameInfo m_gameData;
  QList<QPoint> m_snakePos;
  QList<QPoint> m_freePos;
  int m_fieldWidth;
  int m_fieldHeight;
  SnakeLook m_look;
  bool m_singleInput;
  QSettings m_appSettings;
};

}  // namespace s21
