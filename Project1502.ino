// include the library code:
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int b1Pin = 6;
const int b2Pin = 7;
int b1State = 0;
int b1Number = 1;
int b2State = 0;
int b2Number = 8;
long randNumber1;
long randNumber2;
int inputAnswer;

void setup() {
  // initialize the pushbutton pins as an input:
  pinMode(b1Pin, INPUT_PULLUP);
  pinMode(b2Pin, INPUT_PULLUP);
  
  randomSeed(analogRead(A0));
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  randNumber1 = random(1,11);
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
  // Turn on the display:
  lcd.display();
  delay(50);

  // Saves sum of random numbers as the correct answer
  correctAnswer = randNumber1 + randNumber2;
  // Converts correct answer to a string
  String answerString = String(correctAnswer);
  // Number of digits in the correct answer
  arraySize = string.length(answerString);

  // Array for answer that player inputs of variable size
  int playerAnswer[arraySize];

  b1State = digitalRead(b1Pin);
  b2State = digitalRead(b2Pin);

 // For loop fills array of given size with number corresponding to button pushed
  for (i = 0; i <= arraySize; i++) {
    if (b1State == LOW) {
      lcd.print(b1Number);
      playerAnswer[i] = b1Number;
      }
    else if (b2State == LOW) {
      lcd.print(b2Number);
      playerAnswer[i] = b2Number;
     }
    // delete button
    else if (b11State == LOW) {
      i--;
      playerAnswer[i] = 0;
    }
     
  } //close for loop

// Convert array to integer
  for (i = 0; i <= arraySize; i++){
     inputAnswer = (playerAnswer[i] * pow(10, arraySize - i)) + inputAnswer;
    }

// Compare inputted and correct answer
  if (inputAnswer == correctAnswer){
    
  }
  
  
  delay(500);
} //close void loop


