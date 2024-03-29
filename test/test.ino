#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

// Declare variables
const int buttonPin[] = {0,1,2,3,4,5,6,7,8,9}; // Holds pin locations of buttons
const int buttonNumber[] = {0,1,2,3,4,5,6,7,8,9}; // Representative of actual digits to be printed
int buttonPressed = 10; // Necessary for multiple arduino communication
int operation; // Representative of four basic arithmetic operations
int difficulty; // Representative of user-inputted difficulty setting
int i = 0;
int randNumber1, randNumber2;
int correctAnswer;
int inputAnswer; // Stores user-inputted answer

void setup() {
  Serial.begin(115200);
  // Initialize pushbutton pins as an input
  for (int count=0;count<10;count++) {
    pinMode(buttonPin[count],INPUT_PULLUP);
  }
  // Set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // Turn on display
  lcd.display();
  // Welcome message
  lcd.print("Welcome to [ma+h i+]!");
  delay(1000);
  lcd.clear();
}

void chooseDifficulty() {
  // Ask for difficulty
  lcd.print("What difficulty level(1-3)?");
  // Check and assign user difficulty input
  if (buttonPressed == buttonNumber[0]) { // Easy
       difficulty = 1;
       lcd.clear();
       lcd.print("Easy");
       delay(300);
  }
  else if (buttonPressed == buttonNumber[1]) { // Medium
       difficulty = 2;
       lcd.clear();
       lcd.print("Medium");
       delay(300);
  }
  else if (buttonPressed == buttonNumber[2]) { // Hard
       difficulty = 3;
       lcd.clear();
       lcd.print("Hard");
       delay(300);
  }
  lcd.clear();
  delay(1000);
}

void chooseOperation() {
  // Ask for operation
  lcd.print("What operation do you ");
  lcd.setCursor(0,1);
  lcd.print("want to study?");
  // Check and assign user operation input
  if (buttonPressed == buttonNumber[0]) { // Addition
    operation = 1;
    lcd.clear();
    lcd.print("Addition");
  }
  else if (buttonPressed == buttonNumber[1]) { // Subtraction
    operation = 2;
    lcd.clear();
    lcd.print ("Subtraction");
  }
  else if (buttonPressed == buttonNumber[2]) { // Multiplication
    operation = 3;
    lcd.clear();
    lcd.print ("Multiplication");
  }
  else if (buttonPressed == buttonNumber[3]) { // Division
    operation = 4;
    lcd.clear();
    lcd.print ("Division");
  }
  delay(300);
  lcd.clear();
  delay(1000);
}

void loop() {
  adjustDifficulty();
  saveCorrect();
  getAnswer();
  evalAnswer();
  clearLCD();
}

void adjustDifficulty() {
  // Adjusts difficulty according to user input
  randomSeed(millis());
  if (difficulty == 1) {
       randNumber1 = random(1,11);
       randNumber2 = random(1,11);
  }
  else if (difficulty == 2) {
    if (operation == 3||4){ // Special cases for multiplication and division
      randNumber1 = random(1,11);
      randNumber2 = random(1,21);
    }
    else {
      randNumber1 = random(1,21);
      randNumber2 = random(1,21);
    }
  }
  else if (difficulty == 3) {
    if (operation = 3||4) { // Special cases for multiplication and division
      randNumber1 = random(1,11);
      randNumber2 = random(1,51);
    }
    else {
      randNumber1 = random(1,51);
      randNumber2 = random(1,51);
    }
  }
}

void saveCorrect() {
  // Saves correct answer from random numbers, according to operation
  if (operation == 1) {
       correctAnswer = randNumber1+randNumber2;
       lcd.print(randNumber1);
       lcd.print("+");
       lcd.print(randNumber2); 
       lcd.print("=");
  }
  else if (operation == 2) {
       correctAnswer = randNumber1;
       lcd.print(randNumber1+randNumber2);
       lcd.print("-");
       lcd.print(randNumber2); 
       lcd.print("=");
  }
  else if (operation == 3) {
       correctAnswer = randNumber1*randNumber2;
       lcd.print(randNumber1);
       lcd.print("×");
       lcd.print(randNumber2); 
       lcd.print("=");
  }
  else if (operation == 4) {
       correctAnswer = randNumber2;
       lcd.print(randNumber1*randNumber2);
       lcd.print("÷");
       lcd.print(randNumber1); 
       lcd.print("=");
  }
}

void getAnswer() {
  int correctAnswer = randNumber1 + randNumber2;
  // Converts correct answer to a string
  String answerString = String(correctAnswer);
  // Number of digits in the correct answer
  int arraySize = answerString.length();
  
  // Array for answer that player inputs of variable size
  int playerAnswer[arraySize];

    // Shorten timer as score gets higher
  int limit = 5000 - 100 * score;
  long currentTime = millis();
  long endTime = currentTime + limit;

// Timer loop: Continues as long as time elapsed is less than endTime
while(millis() < endTime){

  // Fills array of given size with number corresponding to buttons pushed
  // Also prints to LCD the user-calculated sum
  for (i = 0; i < arraySize; i++) {
    // get integer of button pressed from serial
    while(buttonPressed > 9)
    {
      if(Serial.available())
      {
        buttonPressed = Serial.parseInt();
      }
      else{}
     }

    // Add digits to player answer matrix and print on LCD
    if (buttonPressed <= 9) {
      lcd.print(buttonPressed);
      playerAnswer[i] = buttonPressed;
      }
     else {}
     
     buttonPressed = 10;
  }


  // Convert array to integer
  for (int j = 0; j <= arraySize - 1; j++){
     inputAnswer = (playerAnswer[j] * pow(10,arraySize - j - 1)) + inputAnswer;
    }
  delay(200);
}
  
void evalAnswer() {
  // Compare inputted and correct answer
  int score; // User score
  // Also prints current score to user
  if (inputAnswer == correctAnswer){
    lcd.clear();
    lcd.print("Correct!");
    delay(1000);
    score++;
  }    
  else {
    lcd.clear();
    lcd.print("Incorrect.");
    delay(1000);
    lcd.clear();
    lcd.print("The answer ");
    lcd.setCursor(0,1);
    lcd.print("was ");
    lcd.print(correctAnswer);
    delay(1000);
    lcd.clear();
    lcd.print("Your score ");
    lcd.setCursor(0,1);
    lcd.print("was ");
    lcd.print(score);
    delay(1000);
    score = 0;
  } 
}
void clearLCD() {
  // Clear LCD in preparation for next question 
  lcd.clear();
  i = 0;
}
