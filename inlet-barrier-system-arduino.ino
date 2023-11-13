#include <ezButton.h>

// Definicja enuma Side reprezentującego strony (LEWA, PRAWA)
enum Side {LEFT, RIGHT};

// Tablica zawierająca piny diod panelu lewego
static const int leftDiodeArr[] = {52, 50, 48, 46, 49, 47, 53, 51};

// Tablica zawierająca piny diod panelu prawego
static const int rightDiodeArr[] = {23, 25, 27, 29, 28, 26, 24, 22};

// Piny testowe panelu lewego
static const int leftTestDiodeUp = 39;    // tested on A8
static const int leftTestButton = 21;     // tested on A9
static const int leftTestDiodeDown = 41;  // tested on A10

// Piny testowe panelu prawego
static const int rightTestDiodeUp = 33;   // tested on 7
static const int rightTestButton = 19;    // tested on 6
static const int rightTestDiodeDown = 35; // tested on 5

// Piny sekcji recall
static const int recallDiodeUp = A0;
static const int recallButton = 20;       // tested on A1
static const int recallDiodeDown = 37;    // tested on A2

// Liczba diod włączonych po stronie lewej i prawej
int leftNumOfActiveDiodes = 0;
int rightNumOfActiveDiodes = 0;

// Liczba diod podświetlających
static const int backlightDiodes = 8;

// Zmienna przechowująca dane wejściowe
String inputData;

// Inicjalizacja przełączników bypass przy pomocy biblioteki ezButton
ezButton rightToggleSwitch(18); // tested on 13
ezButton leftToggleSwitch(A15); // tested on 10

// Funkcja inicjalizująca ustawienia
void setup() {
  Serial.begin(9600);
  Serial.println("Inicjalizacja UART");

  // Inicjalizacja paneli diodowych
  initDiodeArr(LEFT, LOW);
  initDiodeArr(RIGHT, LOW);

  // Inicjalizacja panela testowego po lewej stronie
  initLeftTestPanel();

  // Inicjalizacja panela testowego po prawej stronie
  initRightTestPanel();

  // Inicjalizacja sekcji recall
  initRecallPanel();

  // Inicjalizacja diod podświetlających
  // initBacklightDiodes();

  // Ustawienie czasu oczekiwania na stabilizację stanu przycisków
  leftToggleSwitch.setDebounceTime(50);
  rightToggleSwitch.setDebounceTime(50);
}

// Funkcja loop(), główna pętla programu
void loop() {
  // Obsługa przycisku po lewej stronie
  leftToggleSwitch.loop();
  int leftToggleSwitchState = leftToggleSwitch.getState();
  if (leftToggleSwitchState == HIGH)
    digitalWrite(leftTestDiodeDown, LOW);
  else
    digitalWrite(leftTestDiodeDown, HIGH);

  // Obsługa przycisku po prawej stronie
  rightToggleSwitch.loop();
  int rightToggleSwitchState = rightToggleSwitch.getState();
  if (rightToggleSwitchState == HIGH)
    digitalWrite(rightTestDiodeDown, LOW);
  else
    digitalWrite(rightTestDiodeDown, HIGH);

  // Wykonanie testu panelu lewego
  performLeftSideTest();

  // Wykonanie testu panelu prawego
  performRightSideTest();

  // Wykonanie funkcji recall
  performRecall();

  // Odczyt danych z interfejsu UART
  if (Serial.available()) {
    inputData = Serial.readString();
    int index = inputData.charAt(1) - '0';

    /** 
     * Sygnał sterujący LEFT
     * ustawienie stanu wysokiego na N diodach panelu lewego
     *
     * INPUT: L0, L1, L2, L3, L4, L5, L6, L7, L8
     */
    if (inputData.charAt(0) == 'L') {
      for (int i = 0; i < 8; i++)
        digitalWrite(leftDiodeArr[i], LOW);

      for (int i = 0; i < index; i++)
        digitalWrite(leftDiodeArr[i], HIGH);

      leftNumOfActiveDiodes = index;
    }

    /** 
     * Sygnał sterujący RIGHT
     * ustawienie stanu wysokiego na N diodach panelu prawego
     *
     * INPUT: R0, R1, R2, R3, R4, R5, R6, R7, R8
     */
    if (inputData.charAt(0) == 'R') {
      for (int i = 0; i < 8; i++)
        digitalWrite(rightDiodeArr[i], LOW);

      for (int i = 0; i < index; i++)
        digitalWrite(rightDiodeArr[i], HIGH);

      rightNumOfActiveDiodes = index;
    }

    /** 
     * Sygnał sterujący FAULT
     * ustawienie stanu wysokiego na przycisku FAULT
     * 1 = strona lewa, 2 = strona prawa
     *
     * INPUT: F1, F2
     */
    if (inputData.charAt(0) == 'F') {
      if (index == 1) {
        int diodeState = digitalRead(leftTestDiodeUp);
        digitalWrite(leftTestDiodeUp, !diodeState);
      } else {
        int diodeState = digitalRead(rightTestDiodeUp);
        digitalWrite(rightTestDiodeUp, !diodeState);
      }
    }

    /** 
     * Sygnał sterujący CLEAR
     * czyszczenie wszystkich wysokich stanów
     *
     * INPUT: C
     */
    if (inputData.charAt(0) == 'C') {
      leftNumOfActiveDiodes = 0;
      rightNumOfActiveDiodes = 0;

      digitalWrite(leftTestDiodeUp, LOW);
      digitalWrite(leftTestDiodeDown, LOW);
      digitalWrite(rightTestDiodeUp, LOW);
      digitalWrite(rightTestDiodeDown, LOW);
    }
  }
}

// Funkcja inicjalizująca tablicę diod
void initDiodeArr(Side side, int state) {
  const int *diodeArrToOperate;

  if (side == LEFT)
    diodeArrToOperate = leftDiodeArr;
  else
    diodeArrToOperate = rightDiodeArr;

  for (int i = 0; i < 8; ++i) {
    pinMode(diodeArrToOperate[i], OUTPUT);
    digitalWrite(diodeArrToOperate[i], state);
  }
}

// Funkcja wykonująca test panelu lewego
void performLeftSideTest() {
  if (digitalRead(leftTestButton) == LOW) {
    Serial.println("LEFT CLICK");
    int beforeFaultState = digitalRead(leftTestDiodeUp);

    for (int i = 0; i < 3; i++) {
      digitalWrite(leftTestDiodeUp, HIGH);
      digitalWrite(leftTestDiodeDown, HIGH);
      delay(1000);
      digitalWrite(leftTestDiodeUp, LOW);
      digitalWrite(leftTestDiodeDown, LOW);
      delay(1000);
    }

    digitalWrite(leftTestDiodeUp, beforeFaultState);
  }
}

// Funkcja wykonująca test panelu prawego
void performRightSideTest() {
  if (digitalRead(rightTestButton) == LOW) {
    int beforeFaultState = digitalRead(rightTestDiodeUp);

    for (int i = 0; i < 3; i++) {
      digitalWrite(rightTestDiodeUp, HIGH);
      digitalWrite(rightTestDiodeDown, HIGH);
      delay(1000);
      digitalWrite(rightTestDiodeUp, LOW);
      digitalWrite(rightTestDiodeDown, LOW);
      delay(1000);
    }

    digitalWrite(rightTestDiodeUp, beforeFaultState);
  }
}

// Funkcja wykonująca recall
void performRecall() {
  if (digitalRead(recallButton) == LOW) {
    digitalWrite(recallDiodeUp, HIGH);
    digitalWrite(recallDiodeDown, HIGH);

    for (int i = 0; i < 8; i++) {
      digitalWrite(leftDiodeArr[i], HIGH);
      digitalWrite(rightDiodeArr[i], HIGH);
    }

  } else {
    digitalWrite(recallDiodeUp, LOW);
    digitalWrite(recallDiodeDown, LOW);

    for (int i = 0; i < 8; i++) {
      digitalWrite(leftDiodeArr[i], LOW);
      digitalWrite(rightDiodeArr[i], LOW);
    }

    for (int i = 0; i < leftNumOfActiveDiodes; i++) {
      digitalWrite(leftDiodeArr[i], HIGH);
    }

    for (int i = 0; i < rightNumOfActiveDiodes; i++) {
      digitalWrite(rightDiodeArr[i], HIGH);
    }
  }
}

// Inicjalizacja panelu testowego po lewej stronie
void initLeftTestPanel() {
  pinMode(leftTestDiodeUp, OUTPUT);
  digitalWrite(leftTestDiodeUp, LOW);

  pinMode(leftTestButton, INPUT_PULLUP);

  pinMode(leftTestDiodeDown, OUTPUT);
  digitalWrite(leftTestDiodeDown, LOW);
}

// Inicjalizacja panelu testowego po prawej stronie
void initRightTestPanel() {
  pinMode(rightTestDiodeUp, OUTPUT);
  digitalWrite(rightTestDiodeUp, LOW);

  pinMode(rightTestButton, INPUT_PULLUP);

  pinMode(rightTestDiodeDown, OUTPUT);
  digitalWrite(rightTestDiodeDown, LOW);
}

// Inicjalizacja sekcji recall
void initRecallPanel() {
  pinMode(recallDiodeUp, OUTPUT);
  digitalWrite(recallDiodeUp, LOW);

  pinMode(recallButton, INPUT_PULLUP);

  pinMode(recallDiodeDown, OUTPUT);
  digitalWrite(recallDiodeDown, LOW);
}

// Inicjalizacja diod podświetlających
// void initBacklightDiodes() {
//   pinMode(backlightDiodes, OUTPUT);
//   digitalWrite(backlightDiodes, HIGH);
// }
