/*
  Pong is a two-dimensional sports game that simulates table tennis.
  by Anderson Costa with ❤ for the Wokwi community
  Visit https://wokwi.com to learn about the Wokwi Simulator
*/
#include <TVout.h>
#include <TVoutfonts/fontALL.h>

#include "logo.h"

#define PLAYER1_INPUT   A0   // Player 1 control input
#define PLAYER2_INPUT   A1   // Player 2 control input

#define SCREEN_WIDTH    128  // Maximum width
#define SCREEN_HEIGHT   96   // Maximum height

#define BALL_TOP_LIMIT  8    // Upper ball collision limit
#define BALL_INF_LIMIT  90   // Lower ball collision limit

#define TOTAL_POINTS    5    // Maximum number of points

TVout TV;

// Player scores
int pointsPlayer1 = 0;
int pointsPlayer2 = 0;

// Set speeds
int speedGame = 25;
int speedBallX = 1;
int speedBallY = -1;

boolean startGame = false;
boolean ownerBall = true;

// Player coordinates
int posPlayer1X = 25;
int posPlayer1Y;
int posPlayer2X = 123;
int posPlayer2Y;
int dirPlayer1;
int dirPlayer2;

// Ball coordinates
int posBallX = 27;
int posBallY;

void setup() {
  TV.begin(_PAL, SCREEN_WIDTH, SCREEN_HEIGHT);

  posPlayers(); // Updates players position

  dirPlayer1 = posPlayer1Y;
  dirPlayer2 = posPlayer2Y;

  posBallY = posPlayer1Y;

  TV.clear_screen();

  // Draw border
  TV.draw_rect(22, 0, TV.hres() - 23, TV.vres() - 1, 1);
  TV.draw_rect(24, 2, TV.hres() - 27, TV.vres() - 5, 1);

  // Draw logo
  TV.bitmap((TV.hres() - pong_logo[0]) / 2 + 11, (TV.vres() - pong_logo[1]) / 2, pong_logo);
  delay(1000);
}

void loop() {
  drawScene();
  checkMotionPlayers();
  movesBall();
  delay(speedGame);

  if (pointsPlayer1 == TOTAL_POINTS) {
    gameOver(1);
  }

  if (pointsPlayer2 == TOTAL_POINTS) {
    gameOver(2);
  }
}

void drawScene()
{
  // Draw the position of the players
  TV.clear_screen();
  TV.select_font(font6x8);
  TV.print(64, 0, pointsPlayer1);
  TV.print(80, 0, pointsPlayer2);
  TV.select_font(font4x6);

  // Draw rectangular square
  TV.draw_rect(22, 8, SCREEN_WIDTH - 23, SCREEN_HEIGHT - 12, WHITE);

  // Draw the center column of the court
  for (int i = 9; i < 90; i = i + 5) {
    TV.draw_column((SCREEN_WIDTH / 2) + 10, i, i + 2, WHITE);
  }

  // Draw player 1
  TV.draw_column(posPlayer1X, (posPlayer1Y - 7), (posPlayer1Y + 7), WHITE);

  // Draw player 2
  TV.draw_column(posPlayer2X, (posPlayer2Y - 7), (posPlayer2Y + 7), WHITE);

  // Draw ball
  TV.set_pixel(posBallX, posBallY, WHITE);
}

void posPlayers()
{
  posPlayer1Y = map(analogRead(PLAYER1_INPUT), 0, 1023, 16, 83);
  posPlayer2Y = map(analogRead(PLAYER2_INPUT), 0, 1023, 16, 83);
}

void checkMotionPlayers()
{
  posPlayers(); // Updates players position

  if (dirPlayer1 < posPlayer1Y) {
    if (!startGame && ownerBall) {
      speedBallY = 1;
      startGame = true;
    }
  } else if (dirPlayer1 > posPlayer1Y) {
    if (!startGame && ownerBall) {
      speedBallY = -1;
      startGame = true;
    }
  }

  if (dirPlayer2 < posPlayer2Y) {
    if (!startGame && !ownerBall) {
      speedBallY = 1;
      startGame = true;
    }
  } else if (dirPlayer2 > posPlayer2Y) {
    if (!startGame && !ownerBall) {
      speedBallY = -1;
      startGame = true;
    }
  }

  dirPlayer1 = posPlayer1Y;
  dirPlayer2 = posPlayer2Y;
}

void movesBall()
{
  int ballX = posBallX;
  int ballY = posBallY;

  if (startGame) {
    ballX = posBallX + speedBallX;
    ballY = posBallY + speedBallY;
  }

  // Check for top and bottom collisions of the ball with the court
  if (ballY == BALL_TOP_LIMIT) {
    speedBallY = 1;
    soundGame(440, 20); // Sound when the ball hits the top
  }
  else if (ballY == BALL_INF_LIMIT) {
    speedBallY = -1;
    soundGame(440, 20); // Sound when the ball hits the bottom
  }

  // Check for ball collisions with players
  // Player 1
  if (ballX == 26) {
    if (posBallY < posPlayer1Y + 8 && posBallY > posPlayer1Y - 8) {
      speedBallX = 1;
      if (startGame) {
        soundGame(220, 20); // Sound when player 1 hits the ball
      }
    }
  } else if (ballX == 21) {
    startGame = false;
    ownerBall = false;
    ballX = 122;
    ballY = posPlayer2Y;
    pointsPlayer2++;
    soundGame(100, 300); // Sound when player 2 scores
  }

  // Player 2
  if (ballX == 122) {
    if (posBallY < posPlayer2Y + 8 && posBallY > posPlayer2Y - 8) {
      speedBallX = -1;
      if (startGame) {
        soundGame(220, 20); // Sound when player 2 hits the ball
      }
    }
  } else if (ballX == SCREEN_WIDTH - 1) {
    startGame = false;
    ownerBall = true;
    ballX = 26;
    ballY = posPlayer1Y;
    pointsPlayer1++;
    soundGame(100, 300); // Sound when player 1 scores
  }

  posBallX = ballX;
  posBallY = ballY;
}

void soundGame(int frequency, int duration)
{
  TV.tone(frequency, duration);
}

void gameOver(byte winner)
{
  startGame = false;

  // Reset points
  pointsPlayer1 = 0;
  pointsPlayer2 = 0;

  TV.clear_screen();
  TV.select_font(font8x8);

  TV.println(54, 32, "WINNER");

  if (winner == 1) {
    ownerBall = true;
    TV.println(47, 50, "PLAYER 1");
  } else {
    ownerBall = false;
    TV.println(47, 50, "PLAYER 2");
  }

  // Sound when player 1 or player 2 wins the game
  soundGame(520, 500);
  delay(5000);
}
