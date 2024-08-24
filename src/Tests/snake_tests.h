#pragma once

#include <gtest/gtest.h>

#include "SnakeModel.h"

TEST(SnakeGameTest, MoveSnakeUp) {
  s21::SnakeModel model(10, 20);
  s21::GameInfo gameData = model.currentState();
  model.setPause(false);
  EXPECT_EQ(gameData.field[20 / 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 1][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 3][10 / 2], 1);

  gameData = model.updateCurrentState();

  EXPECT_EQ(gameData.field[20 / 2 - 1][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 1][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 2][10 / 2], 1);
}

TEST(SnakeGameTest, MoveSnakeLeft) {
  s21::SnakeModel model(10, 20);
  s21::GameInfo gameData = model.currentState();
  model.setPause(false);
  EXPECT_EQ(gameData.field[20 / 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 1][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 3][10 / 2], 1);

  model.userInput(s21::UserAction::Left);
  gameData = model.updateCurrentState();
  model.userInput(s21::UserAction::Left);
  gameData = model.updateCurrentState();

  EXPECT_EQ(gameData.field[20 / 2 + 1][10 / 2 - 1], 1);
  EXPECT_EQ(gameData.field[20 / 2][10 / 2 - 1], 1);
  EXPECT_EQ(gameData.field[20 / 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 1][10 / 2], 1);
}

TEST(SnakeGameTest, MoveSnakeRight) {
  s21::SnakeModel model(10, 20);
  s21::GameInfo gameData = model.currentState();
  model.setPause(false);
  EXPECT_EQ(gameData.field[20 / 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 1][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 3][10 / 2], 1);

  model.userInput(s21::UserAction::Right);
  gameData = model.updateCurrentState();
  model.userInput(s21::UserAction::Right);
  gameData = model.updateCurrentState();

  EXPECT_EQ(gameData.field[20 / 2 + 1][10 / 2 + 1], 1);
  EXPECT_EQ(gameData.field[20 / 2][10 / 2 + 1], 1);
  EXPECT_EQ(gameData.field[20 / 2][10 / 2], 1);
  EXPECT_EQ(gameData.field[20 / 2 + 1][10 / 2], 1);
}

TEST(SnakeGameTest, MoveSnakeFast) {
  s21::SnakeModel model(10, 20);
  model.setPause(false);
  for (int i = 0; i < 10; ++i) model.updateCurrentState();
  model.userInput(s21::UserAction::Action);
  s21::GameInfo gameData = model.currentState();

  EXPECT_EQ(gameData.field[0][10 / 2], 1);
  EXPECT_EQ(gameData.field[1][10 / 2], 1);
  EXPECT_EQ(gameData.field[2][10 / 2], 1);
  EXPECT_EQ(gameData.field[3][10 / 2], 1);
}
