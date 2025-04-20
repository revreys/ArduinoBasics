#include <LiquidCrystal.h>

// LCD pin setup: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 3, 4, 5, A0, A1);

// LED Pins
int whiteLED2 = 12;
int whiteLED1 = 8;
int greenLED = 11;
int yellowLED = 10;
int redLED = 9;

// Buzzer and button pins
int buzzer = 7;
int button2 = 13;
int button1 = 6;

// Game state
bool buttonPressed = 0;
int score1 = 0;
int score2 = 0;

// Settings (customizable)
int debounceDelay = 200;          // ms for button debounce
int winnerDelay = 3000;           // ms before game resets after win
int minDelay = 500;               // min random delay before green LED
int maxDelay = 3000;              // max random delay before green LED

// Setup function
void setup() {
  // Set pin modes
  pinMode(whiteLED1, OUTPUT);
  pinMode(whiteLED2, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  // Seed randomness
  randomSeed(analogRead(0));

  // Initialize LCD
  lcd.begin(16, 2);

  startupSequence();        // Show startup lights + sound
  updateScoreDisplay();     // Show initial scores
}

// Startup animation with lights + jingle
void startupSequence() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(redLED, HIGH); tone(buzzer, 330); delay(100);
    digitalWrite(redLED, LOW); tone(buzzer, 392); delay(100);
    digitalWrite(yellowLED, HIGH); tone(buzzer, 523); delay(100);
    digitalWrite(yellowLED, LOW); noTone(buzzer);
    delay(100);
  }

  // Final "GOATED" welcome
  lcd.clear();
  lcd.setCursor(2, 0); lcd.print("WHOS MORE");
  lcd.setCursor(3, 1); lcd.print("GOATED??");
  delay(3000);
  lcd.clear();
}

// Show player scores on LCD
void updateScoreDisplay() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Score");
  lcd.setCursor(4, 1);
  lcd.print(score1); lcd.print(" - "); lcd.print(score2);
}

// Flash LEDs in sync with melody
void victoryFlash(int noteDuration) {
  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, HIGH);
  digitalWrite(greenLED, HIGH);
  delay(noteDuration / 2);
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  delay(noteDuration / 2);
}

// Victory sound + flashing lights
void victorySound() {
  int melody[] = {
    330, 392, 440, 494, 523, 0,
    523, 494, 440, 392, 349, 0,
    392, 440, 494, 523, 587, 659,
    698, 784, 0, 784, 784, 784
  };

  int durations[] = {
    100, 100, 100, 100, 200, 100,
    100, 100, 100, 100, 200, 100,
    100, 100, 100, 100, 100, 100,
    100, 300, 100, 100, 100, 400
  };

  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    if (melody[i] == 0) {
      noTone(buzzer);
    } else {
      tone(buzzer, melody[i], durations[i]);
    }
    victoryFlash(durations[i]); // Flash LEDs with each note
  }

  noTone(buzzer);
}

// Display who won and reset game
void showWinner(int player) {
  // Mini buzz effect
  for (int i = 0; i < 5; i++) {
    digitalWrite(buzzer, HIGH); delay(50);
    digitalWrite(buzzer, LOW); delay(50);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Player ");
  lcd.print(player);
  lcd.print(" is");
  lcd.setCursor(0, 1);
  lcd.print("MORE GOATED!");
  victorySound();
  delay(winnerDelay);

  // Prompt to replay
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Play again?");
  lcd.setCursor(2, 1);
  lcd.print("Press Any Btn");

  // Wait for any button press
  while (true) {
    if (digitalRead(button1) == LOW || digitalRead(button2) == LOW) {
      delay(debounceDelay);
      score1 = 0;
      score2 = 0;
      updateScoreDisplay();
      break;
    }
  }
}

// Begin a round
void startGame() {
  int delayTime = random(minDelay, maxDelay);

  digitalWrite(redLED, HIGH); tone(buzzer, 330); delay(200); digitalWrite(redLED, LOW); noTone(buzzer); delay(200);
  digitalWrite(yellowLED, HIGH); tone(buzzer, 440); delay(200); digitalWrite(yellowLED, LOW); noTone(buzzer);
  
  delay(delayTime); // Wait

  digitalWrite(greenLED, HIGH); // Go!
}

// Turn off all LEDs + buzzer
void endGame() {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(whiteLED1, LOW);
  digitalWrite(whiteLED2, LOW);
}

// Main loop
void loop() {
  startGame(); // Begin one round

  while (buttonPressed == 0) {
    digitalWrite(whiteLED1, HIGH);
    digitalWrite(whiteLED2, HIGH);

    if (digitalRead(button1) == LOW) {
      buttonPressed = 1;
      digitalWrite(whiteLED2, LOW);
      score1++;
      updateScoreDisplay();
      delay(debounceDelay);

      if (score1 == 5) {
        lcd.clear(); lcd.setCursor(3, 0); lcd.print("We have");
        lcd.setCursor(4, 1); lcd.print("WINNER!!");
        delay(1000);
        showWinner(1);
      }
    }

    if (digitalRead(button2) == LOW) {
      buttonPressed = 1;
      digitalWrite(whiteLED1, LOW);
      score2++;
      updateScoreDisplay();
      delay(debounceDelay);

      if (score2 == 5) {
        lcd.clear(); lcd.setCursor(3, 0); lcd.print("We have");
        lcd.setCursor(4, 1); lcd.print("WINNER!!");
        delay(1000);
        showWinner(2);
      }
    }
  }

  tone(buzzer, 250); delay(300); noTone(buzzer); delay(1000);
  endGame(); buttonPressed = 0;
}
