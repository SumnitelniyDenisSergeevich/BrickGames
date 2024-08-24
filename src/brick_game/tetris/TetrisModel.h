/*!
 * \file
 * \brief Заголовочный файл с описанием модели тетриса
 *
 * Данный файл содержит в себе все методы модели тетриса
 */
#pragma once

#include <QList>
#include <QMutex>
#include <QPoint>
#include <QSettings>

#include "Common.h"

namespace s21 {

/// Набор возможных типов объектов
enum Item {
  line = 0,       ///< Линия
  leftCorner,     ///< Левый угол
  rightCorner,    ///< Правый угол
  square,         ///< Квадрат
  upRightSquare,  ///< квадрат смещенный сверху вправо
  upLeftSquare,  ///< квадрат смещенный сверху влево
  midleCorner,   ///< центральный угол
};

/// Состояние объекта
struct ItemData {
  Item itemType;          ///< Тип объекта
  QList<QPoint> itemPos;  ///< координаты точек объекта
};

class TetrisModel : public QObject {
  Q_OBJECT

 public:
  TetrisModel(int fieldWidth, int fieldHeight, QObject *parent = nullptr);
  ~TetrisModel();

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

 private:
  void restartModel();

 private:
  bool checkRightCollision();
  bool checkLeftCollision();

 private:
  bool processingLoose();
  bool processingCollision();
  void processLines();

 private:
  void itemDown();
  void itemLeft();
  void itemRight();

 private:
  void itemRotate();

 private:
  void generateItem();
  void generateLine();
  void generateLeftCorner();
  void generateRightCorner();
  void generateSquare();
  void generateUpRightSquare();
  void generateUpLeftSquare();
  void generateMidleCorner();

 private:
  GameInfo m_gameData;
  int m_fieldWidth;
  int m_fieldHeight;
  QSettings m_appSettings;
  bool m_collision;
  ItemData m_itemData;
  QMutex m_actionMutex;
};

}  // namespace s21
