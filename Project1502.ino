#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

  // Declare variables
  const int buttonPin[] = {0,1,2,3,4,5,6,7,8,9}; // Holds pin locations of buttons
  const int buttonNumber[] = {0,1,2,3,4,5,6,7,8,9}; // Representative of actual digits to be printed
  int count = 0;
  long randNumber1,randNumber2; // Addends of sum function
  int inputAnswer; // Stores user-inputted answer
  int score; // User score
  int buttonPressed = 10; // Necessary for multiple arduino communication
  int i = 0;
  
void setup() {
  Serial.begin(115200);
  // Initialize pushbutton pins as an input
  for (count=0; count<10; count++) {
    pinMode(buttonPin[count],INPUT_PULLUP);
  
  // Set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  }
}

void loop() {

  lcd.display();
  delay(50);
 // Print the problem to the LCD
  randomSeed(millis());
  randNumber1 = random(1,11);
  randNumber2 = random(1,11);
  lcd.print(randNumber1);
  lcd.print("+");
  lcd.print(randNumber2); 
  lcd.print("=");
  inputAnswer = 0;
  
  // Saves sum of random numbers as the correct answer
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
  
  // Compare inputted and correct answer a exit timer loop
  if (inputAnswer == correctAnswer){
    endTime = 0;
  }    
  else {
    endTime = 1;
  }
  
}

  // Clear LCD in preparation for next question 
  lcd.clear();
  
  // Print correct, incorrect, or too slow
  if (endTime == 0){
    score++;
    lcd.print("Correct!");
    delay(400);
    lcd.clear();
    lcd.print("Your score ");
    lcd.setCursor(0,1);
    lcd.print("is ");
    lcd.print(score);
    delay(400);
  }
  else if (endTime == 1){
    lcd.print("Incorrect.");
    delay(400);
    lcd.clear();
    lcd.print("Answer: ");
    lcd.setCursor(0,1);
    lcd.print(correctAnswer);
    delay(500);
    lcd.clear();
    score = 0;
  }
  else (endTime > 1){
    lcd.print("Too slow!");
    lcd.clear();
    lcd.print("Answer: ");
    lcd.setCursor(0,1);
    lcd.print(correctAnswer);
    delay(500);
    lcd.clear();
    score = 0;
  }
}
