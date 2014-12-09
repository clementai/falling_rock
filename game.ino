#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define A 7
#define B 2

Adafruit_PCD8544 display = Adafruit_PCD8544(8, 9, 10, 12, 13);

const int width = display.width();
const int height = display.height();
const int ground = display.height() * 9 / 10;

const int nbrRock = 8;

int pos = width / 2;
int rock[nbrRock][3];
int currentRock = 0;
int displayARock = 0;
int life = 0;
int score = 0;
int highScore = 0;

void setup() {
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  
  Serial.begin(9600);
  
  display.begin();
  
  display.setContrast(50);
  
  displaySplatScreen();
  
  delay(1000);
  
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  
  int a = digitalRead(A);
  int b = digitalRead(B);
  /*
  Serial.print("A = ");
  Serial.print(a);
  Serial.print("\n");
  Serial.print("B = ");
  Serial.print(b);
  Serial.print("\n");
  Serial.print("Pos = ");
  Serial.print(pos);
  Serial.print("\n");
  Serial.print("displayARock = ");
  Serial.print(displayARock);
  Serial.print("\n");
  */
  
  Serial.print("Score = ");
  Serial.print(score);
  Serial.print("\n");
  
  if (!life) {
      
    // BUTTON PART
    
    if (a) {
      if (!(pos < 5)) pos--;
    } else if (b) {
      if (!(pos > width - 5)) pos++;
    }
    
    // POP A ROCK
    
    if (displayARock == 10) {
      int posInitRock = random(4, width - 4);
      rock[currentRock][0] = posInitRock;
      rock[currentRock][1] = 4;
      rock[currentRock][2] = 1;
      currentRock++;
      if (currentRock > nbrRock - 1) currentRock = 0;
      displayARock = 0;
    } else {
      displayARock++;
    }
    
    // ROCK FALL
    
    for(int i = 0; i < nbrRock; i++) {
      if (rock[i][2])
        rock[i][1] += 1;
      if (rock[i][1] > height - 4) {
        rock[i][0] = 0;
        rock[i][1] = 0;
        rock[i][2] = 0;
        score++;
      }
    }
    
    // COLISION
    
    for(int i = 0; i < nbrRock; i++) {
      if(rock[i][0] + 4 > pos - 3 && rock[i][0] - 4 < pos + 3 && rock[i][1] + 4> ground - 8)
        life = 1;
    }
    
    // DISPLAY PART
    
    displayGround();
    displayHuman(pos);
    for(int i = 0; i < nbrRock; i++) {
      if (rock[i][2])
        displayRock(rock[i][0], rock[i][1]);
    }
  } else {
    if (score > highScore) highScore = score;
    displayHuman(width / 2);
    displayScore();
    if(a && b) {
      life = 0;
      pos = width / 2;
      displayARock = 0;
      score = 0;
      for(int i = 0; i < nbrRock; i++) {
        rock[i][0] = 0;
        rock[i][1] = 0;
        rock[i][2] = 0;
      }
    }
  }
  
  display.display();
}

void displayGround() {
  display.fillRect(0, ground, width, height, BLACK);
}

void displaySplatScreen() {
  display.clearDisplay();
  displayGround();
  displayRock(10, 10);
  displayRock(25, 30);
  displayRock(50, 20);
  displayRock(70, 5);
  displayHuman(width / 2);
  display.display();
}

void displayHuman(int x) {
  display.drawLine(x - 3, ground, x, ground - 3, BLACK);
  display.drawLine(x + 3, ground, x, ground - 3, BLACK);
  display.drawLine(x, ground - 3, x, ground - 6, BLACK);
  display.drawLine(x - 3, ground - 5, x + 3, ground - 5, BLACK);
  display.fillCircle(x, ground - 8, 2, BLACK);
}

void displayRock(int x, int y) {
  display.fillCircle(x, y, 4, BLACK);
}

void displayScore() {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(width * 10 / 40, height * 1 / 8);
  display.print("Score ");
  display.print(score);
  display.print("\n");
  display.setCursor(width * 2 / 40, height * 3 / 8);
  display.print("HighScore ");
  display.print(highScore);
  display.print("\n");
}
  
