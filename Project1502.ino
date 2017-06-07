#include <SoftwareSerial.h>

#define rxPin 9    // Serial input (connects to Emic 2 SOUT)
#define txPin 10    // Serial output (connects to Emic 2 SIN)
#define ledPin 12  // Most Arduino boards have an on-board LED on this pin
#define statPin = 8; // status LED

// Sets up a new serial port for text to speech
SoftwareSerial emicSerial = SoftwareSerial(rxPin, txPin);

// Declare universal variables
const int buttonNumber[] = {0,1,2,3,4,5,6,7,8,9}; // corresponds to digits of buttons

int buttonPressed = 10; // When no button is being pressed
int op; // One of four basic arithmetic operations
int diff; // User-inputted difficulty setting
int randNumber1, randNumber2; // Numbers used to create new math fact
int rightAnswer; // Correct answer for given math fact
int inputAnswer = 0; // Stores user-inputted answer
int score = 0;
int sum;
int product;
int refresh = 1; // Game starts over when refresh is true (0)
bool timeOut = 0; // Means that timer ran out when timeOut == 1
String question; // Math fact to be asked by device

void setup() {  
  // Define pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(statPin, OUTPUT);

  digitalWrite(statPin, HIGH);  // turn on status LED
  digitalWrite(ledPin, LOW);  // turn TTS LED off

  // Initialize connection to TTS
  emicSerial.begin(9600);

  // TTS voice settings and initialization
  emicInit();
  emicSerial.print("L0\n"); // english
  while (emicSerial.read() != ':');
  emicSerial.print("V15\n"); // volume up
  while (emicSerial.read() != ':');
  emicSerial.print("N2\n");   // voices that sound ok: 0,2,4,7
  while (emicSerial.read() != ':');

  // Welcome message -- game begins
  emicSay("Welcome to math it!");

  // User selects game settings
  op = chooseOperation();
  buttonPressed = 10;
  delay(200);
  diff = chooseDifficulty();
}

void loop() {
  // Continue asking questions with given game settings
    if(refresh == 1){
    rightAnswer = getProblem(diff, op);
    evalAnswer(rightAnswer);
    }
    else { // Timer ran out, give too slow feedback and start game over
    buttonPressed = 10;
    emicSay("Too slow. Correct answer was");
    emicSay(String(rightAnswer));
    emicSay("Final score");
    delay(100);
    emicSay(String(score));
    refresh = 1;
    score = 0;
    emicSay("Let's play again");
    timeOut = 0;
    buttonPressed = 10;
    delay(2000);
    } 
}


// Initialize text to speech
void emicInit() {
  emicSerial.print('\n');             // Send a CR in case the system is already up
  while (emicSerial.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                          // Short delay
  emicSerial.flush();                 // Flush the receive buffer
}


// Speak the passed in value; wrapper function for the function in Print.
void emicSay(String words) {
  emicSerial.print('S');
  emicSerial.print(words);
  emicSerial.print('\n');
  while (emicSerial.read() != ':');
}


// Prompt user to pick one of four operations
int chooseOperation() {
  int operation;
  // Ask for operation
  emicSay("Choose an operation");
  
  // Check and assign user operation input
  while (buttonPressed > 4 || buttonPressed < 1) {
    buttonPressed = getButton();
  }
  if (buttonPressed == buttonNumber[1]) { // Addition
    operation = 1;
    emicSay("Addition");
  }
  else if (buttonPressed == buttonNumber[2]) { // Subtraction
    operation = 2;
    emicSay("Subtraction");
  }
  else if (buttonPressed == buttonNumber[3]) { // Multiplication
    operation = 3;
    emicSay("Multiplication");
  }
  else if (buttonPressed == buttonNumber[4]) { // Division
    operation = 4;
    emicSay("Division");
  }
  delay(500);
  return operation;
}


// Prompt user to pick one of 3 difficulty levels
int chooseDifficulty() {
  int difficulty;
  // Ask for difficulty
  emicSay("Choose a difficulty level");
  // Check and assign user difficulty input
  while(buttonPressed > 3 || buttonPressed < 1) {
    buttonPressed = getButton();
  }
  if (buttonPressed == buttonNumber[1]) { // Easy
       difficulty = 1;
       emicSay("Easy");
  }
  else if (buttonPressed == buttonNumber[2]) { // Medium
       difficulty = 2;
       emicSay("Medium");
  }
  else if (buttonPressed == buttonNumber[3]) { // Hard
       difficulty = 3;
       emicSay("Hard");
  }
  delay(500);
  return difficulty;
}


// Generates appropriate math fact with random numbers based on chosen game settings
int getProblem(int difficulty, int operation) {
  // Adjusts difficulty according to user input
  randomSeed(millis());
  switch (difficulty) {
    case 1:
      randNumber1 = random(1,11);
      randNumber2 = random(1,11);
      break;
    case 2:
    if (operation == 3||4){ // Special cases for multiplication and division
      randNumber1 = random(1,11);
      randNumber2 = random(1,21);
    }
    else {
      randNumber1 = random(1,21);
      randNumber2 = random(1,21);
    }
      break;
    case 3:
    if (operation == 3||4) { // Special cases for multiplication and division
      randNumber1 = random(1,11);
      randNumber2 = random(1,51);
    }
    else {
      randNumber1 = random(1,51);
      randNumber2 = random(1,51);
    }
      break;
  }
  
  int correctAnswer;
  // Saves correct answer from random numbers, according to operation
  switch (operation) {
  case 1: 
       correctAnswer = randNumber1+randNumber2;
       question = String(randNumber1) + " plus " + String(randNumber2);
       emicSay(question);
       break;
       
  case 2: // Gets subtraction by forming addition first to avoid negative answers
       correctAnswer = randNumber1;
       sum = randNumber1 + randNumber2;
       question = String(sum) + " minus " + String(randNumber2);
       emicSay(question);
       break;    
         
  case 3:
       correctAnswer = randNumber1 * randNumber2;
       question = String(randNumber1) + " times " + String(randNumber2);
       emicSay(question);
       break;   
         
  case 4: // Gets division by forming multiplication problem first to avoid fractions
       correctAnswer = randNumber2;
       product = randNumber1 * randNumber2;
       question = String(product) + " divided by " + String(randNumber1);
       emicSay(question);
       break;
  }
  return correctAnswer;
}


// Takes in and evaluates player inputted answer for correctness
int evalAnswer(int correctAnswer) {
  buttonPressed = 10; 
  
  // Shorten timer as score gets higher until at a 2 second limit
  // Greater difficulty has longer time limit
  int limit = 5000;
  if (score >= 30){
    limit = 2000;
  }
  else {
    limit = 6000 - 100 * score;
  }

  if (diff == 3){
    limit = limit + 4000;
  }
  else if (diff == 2){
    limit = limit + 2000;
  }
  
  long currentTime = millis();
  long endTime = currentTime + limit;
  
  // Converts correct answer to a string
  String answerString = String(correctAnswer);
  // Number of digits in the correct answer
  int arraySize = answerString.length();
  int playerAnswer[arraySize];

  // Fills array of given size with number corresponding to buttons pushed
  for(int i = 0; i < arraySize; i++) {
    // get integer of button pressed
    while (buttonPressed > 9)
    {
      buttonPressed = getButton();
      if(millis() > endTime && endTime > 1) // Is the time past the limit?
      {
        inputAnswer = 0;
        timeOut = 1;
        break;
      }
    }
    
    // Add digits to player answer matrix
    if (buttonPressed <= 9) {
      playerAnswer[i] = buttonPressed;
      buttonPressed = 10;
      }
      else if(timeOut)
      {
        break;
      }
     else {}
     delay(250);
  }

  // Return to loop and start game over if time runs past the limit
  if(timeOut)
  {
    return refresh = 0;
  }

  // Convert array to integer answer for comparison
  for (int j = 0; j < arraySize; j++){
     inputAnswer = (playerAnswer[j] * pow(10, arraySize - j - 1)) + inputAnswer;
    }
  delay(200);
  
  // Compare inputted and correct answer  
  // Give pos/negative feedback (correct or incorrect)
  if (inputAnswer == correctAnswer){
    score++;
    // Random praise response
    String positive;
    int response = random(1,6);
    switch(response){
      case 1:
      positive = "Swell";
      break;

      case 2:
      positive = "Top notch";
      break;

      case 3:
      positive = "Yippeeeeeeeee!";
      break;

      case 4:
      positive = "Gnarly bruh";
      break;

      case 5:
      positive = "Rock on";
      break;
    }
    emicSay(positive);
    delay(600);
  }
  else { // If answer is incorrect, states correct answer and final score. Game starts over
    emicSay("Incorrect. Correct answer was");
    emicSay(String(correctAnswer));
    emicSay("Final score");
    delay(100);
    emicSay(String(score));
    score = 0;
    emicSay("Let's play again");
    delay(2000);
  }
    inputAnswer = 0;  // reset player input
    buttonPressed = 10;
    return refresh = 1; // Ask another question
}


// Get Number of button pressed, 5 from each analog pin
// Input values based off various resistors used
int getButton() {
  int button;
  int input = analogRead(A0);
  delay(10);
  input = analogRead(A0);
  delay(10);
  int input2 = analogRead(A1);
  delay(10);
  input2 = analogRead(A1); 
  if(input < 10 && input2 < 10) {
    button = 10;
  }
  else if(input < 973 && input > 963) {
    button = 0;
  }
  else if(input < 841 && input > 831) {
    button = 1;
  }
  else if(input < 655 && input > 645) {
    button = 2;
  }
  else if(input < 508 && input > 498) {
    button = 3;
  }
  else if(input < 1000 && input > 980) {
    button = 4;
  }
  else if(input2 < 973 && input2 > 963) {
    button = 5;
  }
  else if(input2 < 841 && input2 > 831) {
    button = 6;
  }
  else if(input2 < 655 && input2 > 645) {
    button = 7;
  }
  else if(input2 < 508 && input2 > 498) {
    button = 8;
  }
  else if(input2 < 1000 && input2 > 980) {
    button = 9;
  }
  return button;
}
