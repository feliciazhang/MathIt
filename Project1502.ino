#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

  // Declare variables
  const int buttonPin[] = {0,1,2,3,4,5,6,7,8,9}; // Holds pin locations of buttons
  const int buttonNumber[] = {0,1,2,3,4,5,6,7,8,9}; // Representative of actual digits to be printed
  int count = 0;
  long randNumber1,randNumber2; // Addends of sum function
  int inputAnswer; // Stores user-inputted answer
  int score; // Player score

void setup() {
  Serial.begin(9600);
  // Initialize pushbutton pins as an input
  for (count=0;count<10;count++) {
    pinMode(buttonPin[count], INPUT_PULLUP);
  }
  
  // Set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  
  // Print the problem to the LCD
  randomSeed(analogRead(A0));
  randNumber1 = random(1, 11);
  lcd.print(randNumber1);
  delay(500);
  lcd.print("+");
  delay(500);
  randNumber2 = random(1,11);
  lcd.print(randNumber2); 
  delay(500);
  lcd.print("=");
  delay(500);
}

void loop() {
  Serial.read();
  // Turn on the display
  lcd.display();
  delay(50);

  for (int timer = 0; timer <= 500; timer++) {
  
  // Saves sum of random numbers as the correct answer
  int correctAnswer = randNumber1 + randNumber2;
  
  // Converts correct answer to a string
  String answerString = String(correctAnswer);
  
  // Number of digits in the correct answer
  int arraySize = answerString.length();
  
  // Array for answer that player inputs of variable size
  int playerAnswer[arraySize];

  // Fills array of given size with number corresponding to buttons pushed
  // Also prints to LCD the user-calculated sum
  for (int i = 0; i <= arraySize; i++) {
    if (digitalRead(buttonPin[0])== LOW) {
      lcd.print(buttonNumber[0]);
      playerAnswer[i] = buttonNumber[0];
      }
    else if (digitalRead(buttonPin[1]) == LOW) {
      lcd.print(buttonNumber[1]);
      playerAnswer[i] = buttonNumber[1];
     }
    else if (digitalRead(buttonPin[2]) == LOW) {
      lcd.print(buttonNumber[ 2]);
      playerAnswer[i] = buttonNumber[2];
     }
    else if (digitalRead(buttonPin[3]) == LOW) {
      lcd.print(buttonNumber[3]);
      playerAnswer[i] = buttonNumber[3];
     }
    else if (digitalRead(buttonPin[4]) == LOW) {
      lcd.print(buttonNumber[4]);
      playerAnswer[i] = buttonNumber[4];
     }
    else if (digitalRead(buttonPin[5]) == LOW) {
      lcd.print(buttonNumber[5]);
      playerAnswer[i] = buttonNumber[5];
     }
    else if (digitalRead(buttonPin[6]) == LOW) {
      lcd.print(buttonNumber[6]);
      playerAnswer[i] = buttonNumber[6];
     }
    else if (digitalRead(buttonPin[7]) == LOW) {
      lcd.print(buttonNumber[7]);
      playerAnswer[i] = buttonNumber[7];
     }
    else if (digitalRead(buttonPin[8]) == LOW) {
      lcd.print(buttonNumber[8]);
      playerAnswer[i] = buttonNumber[8];
     }
    else if (digitalRead(buttonPin[9]) == LOW) {
      lcd.print(buttonNumber[9]);
      playerAnswer[i] = buttonNumber[9];
     }
    // delete button
  /*  else if (digitalRead(buttonPin[1]) == LOW) {
      i--;
      playerAnswer[i] = 0; 
    } */
  } // close button push for loop

  // Convert array to integer
  for (int i = 0; i <= arraySize; i++){
     inputAnswer = (playerAnswer[i] * pow(10, arraySize - i)) + inputAnswer;
    }
    
  // Compare inputted and correct answer
  // Also prints current score to user
  if (inputAnswer == correctAnswer){
    lcd.clear();
    lcd.print("Correct!");
    score++;
    timer = 600;
    delay(1500);
  }
      
  delay(10);
 } // close timer for loop

  // When question is not answered correctly in time
  // Outputs wrong answer message and shows final score
  
    lcd.clear();
    lcd.print("Sorry, thats wrong...");
    delay(2000);
    lcd.clear();
    lcd.print("The correct answer was ");
    lcd.print(correctAnswer);
    delay(2000);
    lcd.clear();
    lcd.print("Your score is ");
    lcd.print(score);
  }
  
  delay(50);
} 
