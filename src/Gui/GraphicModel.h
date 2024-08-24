/*!
 * \file
 * \brief Заголовочный графический файл
 *
 * Данный файл содержит в себе все для отрисовки моделей
 */

#pragma once

#include <QList>
#include <QObject>

#include "Common.h"

class QGraphicsScene;
class QGraphicsRectItem;

namespace s21 {

class GraphicModel : public QObject {
  Q_OBJECT

 public:
  explicit GraphicModel(int width, int height, int sideWidth,
                        QObject* parent = nullptr);
  ~GraphicModel();

 public:
  /*!
   * Обновляет графическу составляющую
   * по входным данным состояния модели
   */
  void updateModel(const GameInfo& gameData);
  /*!
   * Возвращает графическую сцену
   */
  QGraphicsScene* scene();

 private:
  QGraphicsScene* m_scene;
  int m_width;
  int m_height;
  int m_sideWidth;
  QList<QList<QGraphicsRectItem*>> m_field;
  QList<QColor> m_colors;
};

}  // namespace s21
