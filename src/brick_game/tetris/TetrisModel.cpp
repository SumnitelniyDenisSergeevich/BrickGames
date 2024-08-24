#include "TetrisModel.h"

#include <QDateTime>
#include <QSettings>
#include <algorithm>
#include <cstdlib>

s21::TetrisModel::TetrisModel(int fieldWidth, int fieldHeight, QObject* parent)
    : QObject(parent),
      m_fieldWidth(fieldWidth),
      m_fieldHeight(fieldHeight),
      m_appSettings(QSettings::IniFormat, QSettings::UserScope, "MySoft",
                    "Tetris"),
      m_collision(false) {
  srand(QDateTime::currentMSecsSinceEpoch());

  m_gameData.field = new int*[m_fieldHeight];
  for (int i = 0; i < m_fieldHeight; ++i)
    m_gameData.field[i] = new int[m_fieldWidth];

  restartModel();
}

s21::TetrisModel::~TetrisModel() {
  for (int i = 0; i < m_fieldHeight; ++i) delete[] m_gameData.field[i];
  delete[] m_gameData.field;
}

void s21::TetrisModel::userInput(s21::UserAction action, bool hold) {
  Q_UNUSED(hold)
  if (m_gameData.pause == 0) {
    const QMutexLocker lock(&m_actionMutex);
    switch (action) {
      case UserAction::Left:
        itemLeft();
        break;
      case UserAction::Right:
        itemRight();
        break;
      case UserAction::Down:
        if (!m_collision) {
          if (processingCollision()) {
            processLines();
            generateItem();
          } else
            itemDown();
        }
        break;
      case UserAction::Action:
        if (!m_collision) itemRotate();
        break;
      default:
        break;
    }
  }
}

s21::GameInfo s21::TetrisModel::updateCurrentState() {
  if (m_gameData.pause == 0) {
    const QMutexLocker lock(&m_actionMutex);
    if (!m_collision) {
      if (processingCollision()) {
        processLines();
        generateItem();
      } else
        itemDown();
    }
  }
  return m_gameData;
}

s21::GameInfo s21::TetrisModel::currentState() { return m_gameData; }

void s21::TetrisModel::restart() { restartModel(); }

void s21::TetrisModel::setPause(bool pause) {
  m_gameData.pause = pause == true ? 1 : 0;
}

void s21::TetrisModel::itemDown() {
  for (QPoint& p : m_itemData.itemPos) {
    m_gameData.field[p.x()][p.y()] = 0;
    p.setX(p.x() + 1);
  }

  const QList<QPoint> itemPoints = m_itemData.itemPos;

  for (const QPoint& p : itemPoints) m_gameData.field[p.x()][p.y()] = 2;
}

void s21::TetrisModel::itemLeft() {
  int leftColumn = m_fieldWidth;
  const QList<QPoint> itemPoints = m_itemData.itemPos;

  for (const QPoint& p : itemPoints)
    if (p.y() < leftColumn) leftColumn = p.y();

  if (leftColumn != 0 && !checkLeftCollision()) {
    for (const QPoint& p : itemPoints) m_gameData.field[p.x()][p.y()] = 0;

    for (QPoint& p : m_itemData.itemPos) {
      p.setY(p.y() - 1);
      m_gameData.field[p.x()][p.y()] = 2;
    }
  }
}

void s21::TetrisModel::itemRight() {
  int rightColumn = 0;
  const QList<QPoint> itemPoints = m_itemData.itemPos;

  for (const QPoint& p : itemPoints)
    if (p.y() > rightColumn) rightColumn = p.y();

  if (rightColumn != m_fieldWidth - 1 && !checkRightCollision()) {
    for (const QPoint& p : itemPoints) m_gameData.field[p.x()][p.y()] = 0;

    for (QPoint& p : m_itemData.itemPos) {
      p.setY(p.y() + 1);
      m_gameData.field[p.x()][p.y()] = 2;
    }
  }
}

void s21::TetrisModel::itemRotate() {
  if (m_collision || m_itemData.itemType == Item::square) return;

  QPoint r = m_itemData.itemPos[2];
  const QList<QPoint> itemPoints = m_itemData.itemPos;
  QList<QPoint> newPos;
  bool stack = false;

  for (const QPoint& p : itemPoints) {
    int newRow = r.x() + r.y() - p.y();
    int newCol = p.x() + r.y() - r.x();
    newPos.append(QPoint{newRow, newCol});
  }

  while (newPos[0].x() < 0 || newPos[1].x() < 0 || newPos[2].x() < 0 ||
         newPos[3].x() < 0) {
    for (QPoint& p : newPos) p.setX(p.x() + 1);
  }
  while (newPos[0].x() >= m_fieldHeight || newPos[1].x() >= m_fieldHeight ||
         newPos[2].x() >= m_fieldHeight || newPos[3].x() >= m_fieldHeight) {
    for (QPoint& p : newPos) p.setX(p.x() - 1);
  }
  while (newPos[0].y() < 0 || newPos[1].y() < 0 || newPos[2].y() < 0 ||
         newPos[3].y() < 0) {
    for (QPoint& p : newPos) p.setY(p.y() + 1);
  }
  while (newPos[0].y() >= m_fieldWidth || newPos[1].y() >= m_fieldWidth ||
         newPos[2].y() >= m_fieldWidth || newPos[3].y() >= m_fieldWidth) {
    for (QPoint& p : newPos) p.setY(p.y() - 1);
  }

  for (QPoint& p : newPos)
    if (m_gameData.field[p.x()][p.y()] == 1) stack = true;

  if (stack) newPos.clear();

  if (!newPos.empty()) {
    m_itemData.itemPos.clear();
    for (const QPoint& p : itemPoints) m_gameData.field[p.x()][p.y()] = 0;
    for (QPoint& p : newPos) {
      m_itemData.itemPos.append(QPoint{p.x(), p.y()});
      m_gameData.field[p.x()][p.y()] = 2;
    }
  }
}

void s21::TetrisModel::restartModel() {
  m_gameData.pause = 1;

  for (int i = 0; i < m_fieldHeight; ++i)
    for (int j = 0; j < m_fieldWidth; ++j) m_gameData.field[i][j] = 0;

  m_itemData.itemPos.clear();

  m_gameData.high_score = m_appSettings.value("hightScore", 0).toInt();
  m_gameData.score = 0;
  m_gameData.level = 1;
  m_gameData.speed = 1;

  generateItem();

  m_collision = false;
}

bool s21::TetrisModel::checkRightCollision() {
  bool result = false;

  QList<QPoint> itemRightPoints;

  const QList<QPoint> itemPoints = m_itemData.itemPos;
  for (const QPoint& p : itemPoints)
    if (!itemPoints.contains({p.x(), p.y() + 1})) itemRightPoints.append(p);

  const QList<QPoint> LinePoints = itemRightPoints;

  for (const QPoint& p : LinePoints)
    if (m_gameData.field[p.x()][p.y() + 1] > 0) {
      result = true;
      break;
    }

  return result;
}

bool s21::TetrisModel::checkLeftCollision() {
  bool result = false;

  QList<QPoint> itemLeftPoints;

  const QList<QPoint> itemPoints = m_itemData.itemPos;
  for (const QPoint& p : itemPoints)
    if (!itemPoints.contains({p.x(), p.y() - 1})) itemLeftPoints.append(p);

  const QList<QPoint> LinePoints = itemLeftPoints;

  for (const QPoint& p : LinePoints)
    if (m_gameData.field[p.x()][p.y() - 1] > 0) {
      result = true;
      break;
    }

  return result;
}

bool s21::TetrisModel::processingLoose() {
  bool result = false;
  const QList<QPoint> itemPoints = m_itemData.itemPos;

  for (const QPoint& p : itemPoints)
    if (m_gameData.field[p.x()][p.y()] > 0) {
      result = true;
      break;
    }
  return result;
}

bool s21::TetrisModel::processingCollision() {
  bool result = false;
  int bottomItemLine = 0;

  const QList<QPoint> itemPoints = m_itemData.itemPos;

  for (const QPoint& p : itemPoints) {
    if (p.x() > bottomItemLine && p.x() < m_fieldHeight) bottomItemLine = p.x();
  }

  if (bottomItemLine == m_fieldHeight - 1)
    result = true;
  else {
    QList<QPoint> itemBottomPoints;

    for (const QPoint& p : itemPoints)
      if (!itemPoints.contains({p.x() + 1, p.y()})) itemBottomPoints.append(p);

    const QList<QPoint> LinePoints = itemBottomPoints;

    for (const QPoint& p : LinePoints)
      if (m_gameData.field[p.x() + 1][p.y()] > 0) {
        result = true;
        break;
      }
  }

  if (result) {
    for (const QPoint& p : itemPoints) m_gameData.field[p.x()][p.y()] = 1;
  }

  return result;
}

void s21::TetrisModel::processLines() {
  QList<int> precessingLines;
  const QList<QPoint> itemPoints = m_itemData.itemPos;

  for (const QPoint& p : itemPoints)
    if (!precessingLines.contains(p.x())) precessingLines.append(p.x());

  std::sort(precessingLines.begin(), precessingLines.end());

  for (int processLine : precessingLines) {
    bool fullLine = true;
    for (int i = 0; i < m_fieldWidth; ++i)
      if (m_gameData.field[processLine][i] == 0) {
        fullLine = false;
        break;
      }

    if (fullLine) {
      for (int i = processLine; i > 0; --i)
        for (int j = 0; j < m_fieldWidth; ++j)
          m_gameData.field[i][j] = m_gameData.field[i - 1][j];

      for (int j = 0; j < m_fieldWidth; ++j) m_gameData.field[0][j] = 0;

      ++m_gameData.score;
      if (m_gameData.score > m_gameData.high_score) {
        m_gameData.high_score = m_gameData.score;
        m_appSettings.setValue("hightScore", m_gameData.high_score);
      }

      m_gameData.level = m_gameData.speed = 1 + m_gameData.score / 5;
    }
  }
}

void s21::TetrisModel::generateItem() {
  int itemIndex = rand() % 7;

  m_itemData.itemPos.clear();

  switch (itemIndex) {
    case Item::line:
      generateLine();
      m_itemData.itemType = Item::line;
      break;
    case Item::leftCorner:
      generateLeftCorner();
      m_itemData.itemType = Item::leftCorner;
      break;
    case Item::rightCorner:
      generateRightCorner();
      m_itemData.itemType = Item::rightCorner;
      break;
    case Item::square:
      generateSquare();
      m_itemData.itemType = Item::square;
      break;
    case Item::upRightSquare:
      generateUpRightSquare();
      m_itemData.itemType = Item::upRightSquare;
      break;
    case Item::upLeftSquare:
      generateUpLeftSquare();
      m_itemData.itemType = Item::upLeftSquare;
      break;
    case Item::midleCorner:
      generateMidleCorner();
      m_itemData.itemType = Item::midleCorner;
      break;
  }

  if (processingLoose())
    emit collision();
  else {
    const QList<QPoint> itemPoints = m_itemData.itemPos;

    for (const QPoint& p : itemPoints) m_gameData.field[p.x()][p.y()] = 2;
  }
}

void s21::TetrisModel::generateLine() {
  m_itemData.itemPos.append({0, m_fieldWidth / 2 - 2});
  m_itemData.itemPos.append({0, m_fieldWidth / 2});
  m_itemData.itemPos.append({0, m_fieldWidth / 2 - 1});
  m_itemData.itemPos.append({0, m_fieldWidth / 2 + 1});
}

void s21::TetrisModel::generateLeftCorner() {
  m_itemData.itemPos.append({0, m_fieldWidth / 2 - 1});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 - 1});
  m_itemData.itemPos.append({1, m_fieldWidth / 2});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 + 1});
}

void s21::TetrisModel::generateRightCorner() {
  m_itemData.itemPos.append({0, m_fieldWidth / 2 + 1});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 - 1});
  m_itemData.itemPos.append({1, m_fieldWidth / 2});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 + 1});
}

void s21::TetrisModel::generateSquare() {
  m_itemData.itemPos.append({0, m_fieldWidth / 2});
  m_itemData.itemPos.append({0, m_fieldWidth / 2 + 1});
  m_itemData.itemPos.append({1, m_fieldWidth / 2});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 + 1});
}

void s21::TetrisModel::generateUpRightSquare() {
  m_itemData.itemPos.append({0, m_fieldWidth / 2});
  m_itemData.itemPos.append({0, m_fieldWidth / 2 + 1});
  m_itemData.itemPos.append({1, m_fieldWidth / 2});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 - 1});
}

void s21::TetrisModel::generateUpLeftSquare() {
  m_itemData.itemPos.append({0, m_fieldWidth / 2 - 1});
  m_itemData.itemPos.append({0, m_fieldWidth / 2});
  m_itemData.itemPos.append({1, m_fieldWidth / 2});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 + 1});
}

void s21::TetrisModel::generateMidleCorner() {
  m_itemData.itemPos.append({0, m_fieldWidth / 2});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 - 1});
  m_itemData.itemPos.append({1, m_fieldWidth / 2});
  m_itemData.itemPos.append({1, m_fieldWidth / 2 + 1});
}
