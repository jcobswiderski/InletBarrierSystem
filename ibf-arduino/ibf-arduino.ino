#include <ezButton.h>

// Definicja enuma Side reprezentującego strony (LEWA, PRAWA)
enum Side {LEFT, RIGHT};

// Piny i stany lewej sekcji diod
static const int leftDiodeArr[] = {52, 50, 48, 46, 49, 47, 53, 51};
int leftNumOfActiveDiodes = 0;

// Piny i stany prawej sekcji diod
static const int rightDiodeArr[] = {23, 25, 27, 29, 28, 26, 24, 22};
int rightNumOfActiveDiodes = 0;

// Piny i stany lewego panelu testowego
static const int leftTestDiodeUp = 39;    
static const int leftTestButton = 21;     
static const int leftTestDiodeDown = 41;  
int leftFaultState;

// Piny i stany prawego panelu testowego
static const int rightTestDiodeUp = 33;   
static const int rightTestButton = 19;    
static const int rightTestDiodeDown = 35; 
int rightFaultState;

// Piny sekcji recall
static const int recallDiodeUp = A0;
static const int recallButton = 20;      
static const int recallDiodeDown = 37;    

// Zmienna przechowująca aktualny sygnał sterujący
String inputData;

// Inicjalizacja przełączników bypass przy pomocy biblioteki ezButton
ezButton rightToggleSwitch(18); 
ezButton leftToggleSwitch(A15); 

// Funkcja inicjalizująca ustawienia
void setup() {
  Serial.begin(9600);
  Serial.println("[SYSTEM] UART initialized successfully.");

  leftFaultState = digitalRead(leftTestDiodeUp);
  rightFaultState = digitalRead(rightTestDiodeUp);

  // Inicjalizacja paneli diodowych
  initDiodeArr(LEFT, LOW);
  initDiodeArr(RIGHT, LOW);

  // Inicjalizacja panela testowego po lewej stronie
  initLeftTestPanel();

  // Inicjalizacja panela testowego po prawej stronie
  initRightTestPanel();

  // Inicjalizacja sekcji recall
  initRecallPanel();

  // Ustawienie czasu oczekiwania na stabilizację stanu przycisków
  leftToggleSwitch.setDebounceTime(50);
  rightToggleSwitch.setDebounceTime(50);
}

// Funkcja loop(), główna pętla programu
void loop() {

  // Obsługa przełącznika po lewej stronie
  leftToggleSwitch.loop();
  int leftToggleSwitchState = leftToggleSwitch.getState();
  if (leftToggleSwitchState == HIGH)
    digitalWrite(leftTestDiodeDown, LOW);
  else
    digitalWrite(leftTestDiodeDown, HIGH);

  // Obsługa przełącznika po prawej stronie
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
    Serial.println("[SYSTEM] input->" + inputData);
    int index = inputData.charAt(1) - '0';

    /** 
     * Sygnał sterujący LEFT
     * ustawienie stanu wysokiego na N diodach panelu lewego
     *
     * INPUT: L0, L1, L2, L3, L4, L5, L6, L7, L8
     */
    if (inputData.charAt(0) == 'L' || inputData.charAt(0) == 'l') {
      if(index == 9)
        --index;

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
    if (inputData.charAt(0) == 'R' || inputData.charAt(0) == 'r') {
      if(index == 9)
        --index;

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
    if (inputData.charAt(0) == 'F' || inputData.charAt(0) == 'f') {
      if (index == 1) {
        int diodeState = digitalRead(leftTestDiodeUp);
        leftFaultState = 1;
        digitalWrite(leftTestDiodeUp, HIGH);
      } else {
        int diodeState = digitalRead(rightTestDiodeUp);
        rightFaultState = 1;
        digitalWrite(rightTestDiodeUp, HIGH);
      }
    }

    /** 
     * Sygnał sterujący CLEAR
     * czyszczenie wszystkich wysokich stanów
     *
     * INPUT: C
     */
    if (inputData.charAt(0) == 'C' || inputData.charAt(0) == 'c') {
      Serial.println("[SYSTEM] clear states");

      leftNumOfActiveDiodes = 0;
      rightNumOfActiveDiodes = 0;
      leftFaultState = 0;
      rightFaultState = 0;

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
    for (int i = 0; i < 3; i++) {
      digitalWrite(leftTestDiodeUp, HIGH);
      digitalWrite(leftTestDiodeDown, HIGH);
      delay(1000);
      digitalWrite(leftTestDiodeUp, LOW);
      digitalWrite(leftTestDiodeDown, LOW);
      delay(1000);
    }
  }
  digitalWrite(leftTestDiodeUp, leftFaultState);
}

// Funkcja wykonująca test panelu prawego
void performRightSideTest() {
  if (digitalRead(rightTestButton) == LOW) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(rightTestDiodeUp, HIGH);
      digitalWrite(rightTestDiodeDown, HIGH);
      delay(1000);
      digitalWrite(rightTestDiodeUp, LOW);
      digitalWrite(rightTestDiodeDown, LOW);
      delay(1000);
    }
  }
  digitalWrite(rightTestDiodeUp, rightFaultState);
}

// Funkcja wykonująca recall
void performRecall() {
  if (digitalRead(recallButton) == LOW) {
    
      Serial.println("[SYSTEM] input->recall");

    // if(leftNumOfActiveDiodes >= 4 || rightNumOfActiveDiodes >= 4) {
    //   digitalWrite(recallDiodeUp, HIGH);
    //   digitalWrite(recallDiodeDown, HIGH);
    // }
    
    for (int i = 0; i < 9; i++) {
      digitalWrite(leftDiodeArr[i], HIGH);
    }

    for (int i = 0; i < 9; i++) {
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
