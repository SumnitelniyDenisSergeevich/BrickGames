#include "GraphicModel.h"

#include <QGraphicsRectItem>
#include <QGraphicsScene>

s21::GraphicModel::GraphicModel(int width, int height, int sideWidth,
                                QObject* parent)
    : QObject(parent),
      m_scene(new QGraphicsScene),
      m_width(width),
      m_height(height),
      m_sideWidth(sideWidth) {
  m_colors = QList<QColor>{QColor{255, 117, 24}, QColor{34, 139, 34},
                           QColor{27, 85, 131},  QColor{0, 80, 100},
                           QColor{0, 100, 50},   QColor{0, 100, 0},
                           QColor{200, 120, 0}};

  m_scene->setSceneRect(0, 0, sideWidth * width, sideWidth * height);
  m_scene->setBackgroundBrush(QBrush{QColor{250, 231, 181}});

  for (int i = 0; i < height; ++i) {
    QList<QGraphicsRectItem*> row;
    for (int j = 0; j < width; ++j) {
      row.append(new QGraphicsRectItem(sideWidth + j * sideWidth,
                                       sideWidth + i * sideWidth, sideWidth,
                                       sideWidth));
      m_scene->addItem(row.back());
    }
    m_field.append(row);
  }
}

s21::GraphicModel::~GraphicModel()
{
  delete m_scene;
}

void s21::GraphicModel::updateModel(const GameInfo& gameData) {
  for (int i = 0; i < m_height; ++i)
    for (int j = 0; j < m_width; ++j) {
      if (gameData.field[i][j] == 0)
        m_field[i][j]->setBrush(m_colors[0]);
      else if (gameData.field[i][j] < 0 ||
               gameData.field[i][j] > m_colors.size())
        m_field[i][j]->setBrush(m_colors[m_colors.size() - 1]);
      else
        m_field[i][j]->setBrush(m_colors[gameData.field[i][j]]);
    }
}

QGraphicsScene* s21::GraphicModel::scene() { return m_scene; }
