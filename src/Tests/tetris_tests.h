#pragma once

#include <gtest/gtest.h>

#include "TetrisModel.h"

TEST(TetrisGameTest, MoveLeft) {
  s21::TetrisModel model(10, 20);
  s21::GameInfo gameData = model.currentState();
  model.setPause(false);

  QList<QPoint> itemPos;

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 10; ++j)
      if (gameData.field[i][j] > 0) itemPos.append(QPoint{i, j});

  model.userInput(s21::UserAction::Left);
  gameData = model.currentState();

  for (QPoint& p : itemPos) {
    p.setY(p.y() - 1);
    EXPECT_EQ(gameData.field[p.x()][p.y()], 2);
  }
}

TEST(TetrisGameTest, MoveRight) {
  s21::TetrisModel model(10, 20);
  s21::GameInfo gameData = model.currentState();
  model.setPause(false);

  QList<QPoint> itemPos;

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 10; ++j)
      if (gameData.field[i][j] > 0) itemPos.append(QPoint{i, j});

  model.userInput(s21::UserAction::Right);
  gameData = model.currentState();

  for (QPoint& p : itemPos) {
    p.setY(p.y() + 1);
    EXPECT_EQ(gameData.field[p.x()][p.y()], 2);
  }
}

TEST(TetrisGameTest, MoveDownAction) {
  s21::TetrisModel model(10, 20);
  s21::GameInfo gameData = model.currentState();
  model.setPause(false);

  QList<QPoint> itemPos;

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 10; ++j)
      if (gameData.field[i][j] > 0) itemPos.append(QPoint{i, j});

  model.userInput(s21::UserAction::Down);
  gameData = model.currentState();

  for (QPoint& p : itemPos) {
    p.setX(p.x() + 1);
    EXPECT_EQ(gameData.field[p.x()][p.y()], 2);
  }
}

TEST(TetrisGameTest, MoveDown) {
  s21::TetrisModel model(10, 20);
  s21::GameInfo gameData = model.currentState();
  model.setPause(false);

  QList<QPoint> itemPos;

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 10; ++j)
      if (gameData.field[i][j] > 0) itemPos.append(QPoint{i, j});

  gameData = model.updateCurrentState();

  for (QPoint& p : itemPos) {
    p.setX(p.x() + 1);
    EXPECT_EQ(gameData.field[p.x()][p.y()], 2);
  }
}

TEST(TetrisGameTest, Rotate) {
  s21::TetrisModel model(10, 20);
  model.setPause(false);
  for (int i = 0; i < 40; ++i) model.userInput(s21::UserAction::Down);

  s21::GameInfo gameData = model.updateCurrentState();
  model.userInput(s21::UserAction::Action);
  gameData = model.currentState();

  QList<QPoint> itemsPos;

  for (int i = 0; i < 20; ++i)
    for (int j = 0; j < 10; ++j)
      if (gameData.field[i][j] > 0) itemsPos.append(QPoint{i, j});

  EXPECT_EQ(itemsPos.size(), 12);
}
