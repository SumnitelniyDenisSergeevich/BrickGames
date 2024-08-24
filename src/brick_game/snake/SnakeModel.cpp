#include "SnakeModel.h"

#include <QDateTime>
#include <QSettings>
#include <cstdlib>

s21::SnakeModel::SnakeModel(int fieldWidth, int fieldHeight, QObject* parent)
    : QObject(parent),
      m_fieldWidth(fieldWidth),
      m_fieldHeight(fieldHeight),
      m_appSettings(QSettings::IniFormat, QSettings::UserScope, "MySoft",
                    "Snake") {
  srand(QDateTime::currentMSecsSinceEpoch());

  m_gameData.field = new int*[m_fieldHeight];
  for (int i = 0; i < m_fieldHeight; ++i)
    m_gameData.field[i] = new int[m_fieldWidth];

  restartModel();
}

s21::SnakeModel::~SnakeModel() {
  for (int i = 0; i < m_fieldHeight; ++i) delete[] m_gameData.field[i];
  delete[] m_gameData.field;
}

void s21::SnakeModel::userInput(s21::UserAction action, bool hold) {
  Q_UNUSED(hold)
  if (m_gameData.pause == 0) {
    if (!m_singleInput) {
      m_singleInput = true;
      switch (action) {
        case UserAction::Left:
          rotateLeft();
          break;
        case UserAction::Right:
          rotateRight();
          break;
        case UserAction::Action:
          updateCurrentState();
          break;
        default:
          break;
      }
    }
  }
}

s21::GameInfo s21::SnakeModel::updateCurrentState() {
  if (m_gameData.pause == 0) {
    QPoint nextPoint = snakeNextPos();
    if (checkColision(nextPoint))
      emit collision();
    else if (checkApple(nextPoint)) {
      m_gameData.score += 1;
      if (m_gameData.score > m_gameData.high_score) {
        m_gameData.high_score = m_gameData.score;
        m_appSettings.setValue("hightScore", m_gameData.high_score);
      }

      m_gameData.speed = (m_gameData.score / 5) + 1;
      m_gameData.level = (m_gameData.score / 5) + 1;

      if (m_gameData.score == 200)
        emit win();
      else {
        QPoint snakeTail = m_snakePos.back();
        snakeMove(nextPoint);
        m_snakePos.push_back(snakeTail);
        m_gameData.field[snakeTail.x()][snakeTail.y()] = 1;
        m_freePos.erase(m_freePos.begin() + m_freePos.indexOf(snakeTail));
        createApple();
      }
    } else
      snakeMove(nextPoint);

    m_singleInput = false;
  }
  return m_gameData;
}

s21::GameInfo s21::SnakeModel::currentState() { return m_gameData; }

void s21::SnakeModel::restart() { restartModel(); }

void s21::SnakeModel::setPause(bool pause) {
  m_gameData.pause = pause == true ? 1 : 0;
}

void s21::SnakeModel::createApple() {
  int freeIndex = rand() % m_freePos.size();
  m_gameData.field[m_freePos[freeIndex].x()][m_freePos[freeIndex].y()] = 2;
}

void s21::SnakeModel::snakeMove(QPoint nextPoint) {
  m_gameData.field[m_snakePos.back().x()][m_snakePos.back().y()] = 0;
  m_gameData.field[nextPoint.x()][nextPoint.y()] = 1;
  m_freePos.append(m_snakePos.back());
  m_snakePos.push_front(nextPoint);
  m_freePos.erase(m_freePos.begin() + m_freePos.indexOf(m_snakePos.front()));
  m_snakePos.pop_back();
}

QPoint s21::SnakeModel::snakeNextPos() {
  QPoint result;

  switch (m_look) {
    case SnakeLook::Left:
      result = QPoint{m_snakePos.front().x(), m_snakePos.front().y() - 1};
      break;
    case SnakeLook::Up:
      result = QPoint{m_snakePos.front().x() - 1, m_snakePos.front().y()};
      break;
    case SnakeLook::Right:
      result = QPoint{m_snakePos.front().x(), m_snakePos.front().y() + 1};
      break;
    case SnakeLook::Down:
      result = QPoint{m_snakePos.front().x() + 1, m_snakePos.front().y()};
      break;
  }

  return result;
}

void s21::SnakeModel::restartModel() {
  m_gameData.pause = 1;
  m_singleInput = false;
  for (int i = 0; i < m_fieldHeight; ++i)
    for (int j = 0; j < m_fieldWidth; ++j) m_gameData.field[i][j] = 0;

  m_gameData.high_score = m_appSettings.value("hightScore", 0).toInt();
  m_gameData.score = 0;
  m_gameData.level = 1;
  m_gameData.speed = 1;

  m_snakePos.clear();
  m_snakePos.push_back(QPoint{m_fieldHeight / 2, m_fieldWidth / 2});
  m_snakePos.push_back(QPoint{m_fieldHeight / 2 + 1, m_fieldWidth / 2});
  m_snakePos.push_back(QPoint{m_fieldHeight / 2 + 2, m_fieldWidth / 2});
  m_snakePos.push_back(QPoint{m_fieldHeight / 2 + 3, m_fieldWidth / 2});

  m_look = SnakeLook::Up;

  m_gameData.field[m_fieldHeight / 2][m_fieldWidth / 2] = 1;
  m_gameData.field[m_fieldHeight / 2 + 1][m_fieldWidth / 2] = 1;
  m_gameData.field[m_fieldHeight / 2 + 2][m_fieldWidth / 2] = 1;
  m_gameData.field[m_fieldHeight / 2 + 3][m_fieldWidth / 2] = 1;

  m_freePos.clear();
  for (int i = 0; i < m_fieldHeight; ++i)
    for (int j = 0; j < m_fieldWidth; ++j) m_freePos.push_back(QPoint{i, j});

  m_freePos.erase(
      m_freePos.begin() +
      m_freePos.indexOf(QPoint{m_fieldHeight / 2, m_fieldWidth / 2}));
  m_freePos.erase(
      m_freePos.begin() +
      m_freePos.indexOf(QPoint{m_fieldHeight / 2 + 1, m_fieldWidth / 2}));
  m_freePos.erase(
      m_freePos.begin() +
      m_freePos.indexOf(QPoint{m_fieldHeight / 2 + 2, m_fieldWidth / 2}));
  m_freePos.erase(
      m_freePos.begin() +
      m_freePos.indexOf(QPoint{m_fieldHeight / 2 + 3, m_fieldWidth / 2}));

  createApple();
}

void s21::SnakeModel::rotateLeft() {
  switch (m_look) {
    case SnakeLook::Left:
      m_look = SnakeLook::Down;
      break;
    case SnakeLook::Up:
      m_look = SnakeLook::Left;
      break;
    case SnakeLook::Right:
      m_look = SnakeLook::Up;
      break;
    case SnakeLook::Down:
      m_look = SnakeLook::Right;
      break;
  }
}

void s21::SnakeModel::rotateRight() {
  switch (m_look) {
    case SnakeLook::Left:
      m_look = SnakeLook::Up;
      break;
    case SnakeLook::Up:
      m_look = SnakeLook::Right;
      break;
    case SnakeLook::Right:
      m_look = SnakeLook::Down;
      break;
    case SnakeLook::Down:
      m_look = SnakeLook::Left;
      break;
  }
}

bool s21::SnakeModel::checkColision(const QPoint& nextPoint) {
  return nextPoint.x() < 0 || nextPoint.x() >= m_fieldHeight ||
         nextPoint.y() < 0 || nextPoint.y() >= m_fieldWidth ||
         (m_snakePos.contains(nextPoint) && m_snakePos.back() != nextPoint);
}

bool s21::SnakeModel::checkApple(const QPoint& nextPoint) {
  return (m_gameData.field[nextPoint.x()][nextPoint.y()] == 2);
}
