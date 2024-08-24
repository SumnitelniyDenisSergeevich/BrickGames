#include "mainwindow.h"

#include <QMessageBox>

#include "./ui_mainwindow.h"

namespace {
const int squareWidth = 50;
const int fieldWidth = 10;
const int fieldHeight = 20;
const int stepTime = 600;
}  // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_tetrisModel(fieldWidth, fieldHeight),
      m_graphicModel(fieldWidth, fieldHeight, squareWidth),
      m_speed(stepTime) {
  ui->setupUi(this);

  setWindowTitle("Tetris");

  ui->graphicsView->setScene(m_graphicModel.scene());
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
  ui->graphicsView->setViewportUpdateMode(
      QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
  ui->graphicsView->setSceneRect(290, 550, 20, 20);

  updateApp(m_tetrisModel.currentState());

  grabKeyboard();
  m_gameTimer.start(m_speed);

  connect(ui->startGameButton, &QPushButton::clicked, this,
          [this] { m_tetrisModel.setPause(false); });

  connect(ui->pauseGameButton, &QPushButton::clicked, this,
          [this] { m_tetrisModel.setPause(true); });

  connect(ui->restartGameButton, &QPushButton::clicked, this, [this] {
    m_tetrisModel.restart();
    updateApp(m_tetrisModel.currentState());
  });

  connect(ui->terminateGameButton, &QPushButton::clicked, this, [this] {
    m_tetrisModel.restart();
    updateApp(m_tetrisModel.currentState());
  });

  connect(&m_gameTimer, &QTimer::timeout, this,
          [this] { updateApp(m_tetrisModel.updateCurrentState()); });

  connect(&m_tetrisModel, &s21::TetrisModel::collision, this, [this] {
    m_tetrisModel.restart();
    QMessageBox::information(this, "Collision!!!", "АВАРИЯ!");
    updateApp(m_tetrisModel.currentState());
  });
}

void MainWindow::updateApp(const s21::GameInfo &data) {
  ui->scoreLabel->setText(QString::number(data.score));
  ui->speedLabel->setText(QString::number(data.speed));
  ui->levelLabel->setText(QString::number(data.level));
  ui->highScoreLabel->setText(QString::number(data.high_score));
  m_speed = stepTime - data.speed * 15;
  m_gameTimer.start(m_speed);
  m_graphicModel.updateModel(data);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key::Key_Left) {
    m_tetrisModel.userInput(s21::UserAction::Left);
    m_graphicModel.updateModel(m_tetrisModel.currentState());
  } else if (event->key() == Qt::Key::Key_Right) {
    m_tetrisModel.userInput(s21::UserAction::Right);
    m_graphicModel.updateModel(m_tetrisModel.currentState());
  } else if (event->key() == Qt::Key::Key_Down) {
    m_tetrisModel.userInput(s21::UserAction::Down);
    m_graphicModel.updateModel(m_tetrisModel.currentState());
  } else if (event->key() == Qt::Key::Key_Up) {
    m_tetrisModel.userInput(s21::UserAction::Action);
    m_graphicModel.updateModel(m_tetrisModel.currentState());
  }
}

MainWindow::~MainWindow() { delete ui; }
