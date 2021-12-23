#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Define Leds
const int GREEN_LED = 0;
const int RED_LED = 1;
//Define Button
const int BUTTON = 6;
//Define Buzzer
const int SPEAKER = 8;
//Define Start & End
const int END_PIN = 9;
const int START_PIN = 10;
const int WIN_START = 13;
const int WIN_END = 7;

//Global Variables
const char helloMsg[] = "<- Click To Play", touchMsg[] = "TOUCH!", winMsg[] = "Done! In: ";
int btnClicks = 0, strikes = 0;
int start, end_, win_start, win_end;
unsigned long startTime, currentTime, totalTime;

//Define Tones
#define NOTE_C4 262
#define NOTE_G3 196
#define NOTE_A3 220
#define NOTE_B3 247
int winMelody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4};

// State definitions
#define IDLE 0
#define START_GAME 1
#define FINISH 2
#define RESET 3
int state = 0;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(END_PIN, INPUT_PULLUP);
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(WIN_START, INPUT_PULLUP);
  pinMode(WIN_END, INPUT_PULLUP);
}

void loop()
{
  switch (state)
  {

  case IDLE:
    lcd.setCursor(0, 0);
    lcd.print(helloMsg);
    blinkAllLeds();

    // if a click was performed game starts.
    if (digitalRead(BUTTON))
    {
      btnClicks++;
      startTime = millis();
      state = START_GAME;
    }
    break;

  case START_GAME:
    lcd.clear();
    start = digitalRead(START_PIN);
    end_ = digitalRead(END_PIN);
    win_start = digitalRead(WIN_START);
    win_end = digitalRead(WIN_END);
    if (start == LOW && end_ == LOW)
    {
      strikes++;
      lcd.setCursor(5, 0);
      lcd.print(touchMsg);
      beep();
    }
    if (win_start == LOW && win_end == LOW)
    {
      state = FINISH;
    }
    break;

  case FINISH:
    currentTime = millis();
    totalTime = currentTime - startTime;
    lcd.clear();
    playWinSong();
    lcd.setCursor(0, 0);
    lcd.print(winMsg);
    lcd.print(totalTime / 1000);
    lcd.print(" sec");
    lcd.setCursor(3, 1);
    lcd.print(strikes);
    lcd.print(" Strikes");
    delay(5000);
    state = RESET;

    break;

  case RESET:
    btnClicks = 0;
    strikes = 0;
    startTime = 0;
    currentTime = 0;
    totalTime = 0;
    lcd.clear();
    delay(5000);
    state = IDLE;
    break;

  default:
    break;
  }
}

void blinkAllLeds()
{
  digitalWrite(GREEN_LED, HIGH);
  delay(200);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  delay(200);
  digitalWrite(RED_LED, LOW);
}

void playWinSong()
{
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(SPEAKER, winMelody[thisNote], noteDuration);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(SPEAKER);
  }
  delay(1000);
}

void blinkWin()
{

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  delay(300);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void beep()
{
  tone(SPEAKER, HIGH);
  digitalWrite(RED_LED, HIGH);
  delay(800);
  noTone(SPEAKER);
  digitalWrite(RED_LED, LOW);
}
