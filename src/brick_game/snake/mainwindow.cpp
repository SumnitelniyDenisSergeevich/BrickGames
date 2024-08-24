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
      m_snakeModel(fieldWidth, fieldHeight),
      m_graphicModel(fieldWidth, fieldHeight, squareWidth),
      m_speed(stepTime) {
  ui->setupUi(this);

  setWindowTitle("Snake");

  ui->graphicsView->setScene(m_graphicModel.scene());
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
  ui->graphicsView->setViewportUpdateMode(
      QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
  ui->graphicsView->setSceneRect(290, 550, 20, 20);

  m_graphicModel.updateModel(m_snakeModel.currentState());
  updateApp();

  grabKeyboard();

  m_gameTimer.start(m_speed);

  connect(ui->startGameButton, &QPushButton::clicked, this,
          [this] { m_snakeModel.setPause(false); });

  connect(ui->pauseGameButton, &QPushButton::clicked, this,
          [this] { m_snakeModel.setPause(true); });

  connect(ui->restartGameButton, &QPushButton::clicked, this, [this] {
    m_snakeModel.restart();
    updateApp();
  });

  connect(ui->terminateGameButton, &QPushButton::clicked, this, [this] {
    m_snakeModel.restart();
    updateApp();
  });

  connect(&m_gameTimer, &QTimer::timeout, this, [this] { updateApp(); });

  connect(&m_snakeModel, &s21::SnakeModel::collision, this, [this] {
    m_snakeModel.restart();
    QMessageBox::information(this, "Collision!!!", "АВАРИЯ!");
    updateApp();
  });

  connect(&m_snakeModel, &s21::SnakeModel::win, this, [this] {
    m_snakeModel.restart();
    QMessageBox::information(this, "Победа!!!", "победа!");
    updateApp();
  });
}

void MainWindow::updateApp() {
  s21::GameInfo data = m_snakeModel.updateCurrentState();
  ui->scoreLabel->setText(QString::number(data.score));
  ui->speedLabel->setText(QString::number(data.speed));
  ui->levelLabel->setText(QString::number(data.level));
  ui->highScoreLabel->setText(QString::number(data.high_score));
  m_speed = stepTime - data.speed * 15;
  m_gameTimer.start(m_speed);
  m_graphicModel.updateModel(data);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key::Key_Left)
    m_snakeModel.userInput(s21::UserAction::Left);
  else if (event->key() == Qt::Key::Key_Right)
    m_snakeModel.userInput(s21::UserAction::Right);
  else if (event->key() == Qt::Key::Key_Up) {
    m_snakeModel.userInput(s21::UserAction::Action);
    m_graphicModel.updateModel(m_snakeModel.currentState());
  }
}

MainWindow::~MainWindow() { delete ui; }
