//Professor,
//For the second part of my project
//I upgraded project part 1 to be
//able to handle scrolling right
//as well as left, and also octal
//and hexadecimal addition, there
//are a lot of things commented out on here,
//but because I'm reaching the due date,
//I plan to resubmit tomorrow after
//cleaning everything up.
//-------------
#include <LiquidCrystal.h>
#include <stdint.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int buttonOne = 6;
const int buttonTwo = 7;
const int buttonThree = 8;
const int buttonFour = 9;
int switchStateOne = 0;
int switchStateTwo = 0;
int switchStateThree = 0;
int switchStateFour = 0;
int prevSwitchStateOne = 0;
int prevSwitchStateTwo = 0;
int prevSwitchStateThree = 0;
int prevSwitchStateFour = 0;
//firstCol and secondCol will be used for
//arithmetic with all bases available
//--
int firstCol = 0;
int secondCol = 0;
int currentNum = 0;
int currentNumTwo = 0;
String currentHex = "A";
unsigned long lastEqualButtonTime = 0;
unsigned long debounceTimeMs = 100;
int screen = 0;
const int contrast = 10;
//connected to Vo 
float contrastMeasure = 55.0;
boolean hex = false;
boolean binary = false;
boolean octal = false;
boolean halt = false;

//BYTE 1:
//(Also to be used by base 8 and
// base 16 arithmetic, which will
// not utilize more than 4 digits)
long oneZero = 0;
String oneZeroHex = "n";
long oneOne = 0;
String oneOneHex = "n";
long oneTwo = 0;
String oneTwoHex = "n";
long oneThree = 0;
String oneThreeHex = "n";
long oneFour = 0;
long oneFive = 0;
long oneSix = 0;
long oneSeven = 0;
//BYTE 2:
//(Also to be used by base 8 and
// base 16 arithmetic, which will
// not utilize more than 4 digits)
long twoZero = 0;
String twoZeroHex = "n";
long twoOne = 0;
String twoOneHex = "n";
long twoTwo = 0;
String twoTwoHex = "n";
long twoThree = 0;
String twoThreeHex = "n";
long twoFour = 0;
long twoFive = 0;
long twoSix = 0;
long twoSeven = 0;
//SOLUTION FOR BINARY:
long solZero = 0;
long solOne = 0;
long solTwo = 0;
long solThree = 0;
long solFour = 0;
long solFive = 0;
long solSix = 0;
long solSeven = 0;
long solOverOne = 0;
long solOverTwo = 0;
int solHexOne = 0;
int solHexTwo = 0;
int hexSolution = 0;
//SOLUTION VALUES BY BIT FOR HEX CONV.:
long zero = 0;
long one = 0;
long two = 0;
long three = 0;
long four = 0;
long five = 0;
long six = 0;
long seven = 0;
long solutionCarryOne = 0;
long solutionCarryTwo = 0;
//SOLUTION IN HEX:
long hexDigitZero = 0;
long hexDigitOne = 0;
long hexDigitTwo = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  analogWrite(contrast,contrastMeasure);
  binary = false;
  octal = false;
  hex = false;
  lcd.begin(16, 2);
  lcd.print("Okay, ALU Ready!");
  lcd.setCursor(0, 1);
  lcd.print("Press 4 to begin");
  screen = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  switchStateOne = digitalRead(buttonOne);
  switchStateTwo = digitalRead(buttonTwo);
  switchStateThree = digitalRead(buttonThree);
  switchStateFour = digitalRead(buttonFour);
  if (switchStateFour != prevSwitchStateFour && switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree || firstCol > 7 || secondCol > 7) {
    //^Pressing the 4th button will jump from one screen to the next
    if(firstCol == 8){
      firstCol = 0;
    }
    if(screen == 4){
      screen = 1;
      binary = false;
    }
    if(secondCol == 8){
      secondCol = 0;
    }
    /*if(screen == 12){
      hex = false;
    }*/
    if(screen == 4 && binary == false){
      screen = 5;
    }
    //Brings us to octal result screen:
    if(screen == 9 && hex == true){
      screen = 11;
    }
    if(screen == 11 && octal == true){
      screen = 14;
    }
    /*if(screen > 15){
      octal = false;
    }*/
    else{
      screen++;
    }
    //The boolean values used here
    //are just measures to prevent
    //a user from leaving the screen
    //without selecting one
    //--
    if(binary == false && octal == false && hex == false){
      screen = 1;
      newSet();
    }
    delay(250);
    switch (screen) {
      /*case 1: {
        instruction();
        break;
      }*/
      case 1: {
        selection();
        break;
      }
      case 2: {
        firstByte();
        break;
      }
      case 3: {
        secondByte();
        break;
      }
      case 4: {
        solution();
        break;
      }
      case 5: {
        contrastScreen();
        break;
      }
      case 6: {
        hexScreen();
        break;
      }
      case 7: {
        clearALU();
        break;
      }
      case 8: {
        firstHex();
        break;
      }
      case 9: {
        secondHex();
        break;
      }
      case 10: {
        octalOne();
        break;
      }
      case 11: {
        octalTwo();
        break;
      }
      case 12: {
        hexCalc();
        break;
      }
      case 13: {
        clearALU();
        break;
      }
      case 14: {
        octalCalc();
        break;
      }
    }
  }
  if (screen == 1){
    if (millis() - lastEqualButtonTime > debounceTimeMs){
      if(switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        //The boolean values used here
        //are just measures to prevent
        //a user from leaving the screen
        //without selecting one
        //--
        binary = true;
        screen = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Press 4 to");
        lcd.setCursor(0, 1);
        lcd.print("continue!");
      }
      if(switchStateOne == prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        octal = true;
        screen = 9;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Press 4 to");
        lcd.setCursor(0, 1);
        lcd.print("continue!");
      }
      if(switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree != prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        hex = true;
        screen = 7;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Press 4 to");
        lcd.setCursor(0, 1);
        lcd.print("continue!");
      }
    }
  }
  if (screen == 2) {
    //BYTE 1:
    if (millis() - lastEqualButtonTime > debounceTimeMs) {
      //Left button: moves cursor to the left
      if (switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree != prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
        if(firstCol < 8){
          byteOneMoveCursorRight();
          lastEqualButtonTime = millis();        
        }
      }
      if (switchStateOne == prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
        if(firstCol > 0){
          byteOneMoveCursorLeft();
          lastEqualButtonTime = millis();        
        }
      }
    }
    lcd.setCursor(firstCol, 1);
    //Right button: changes the value of the bit between 1 and 0
    if (switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
      //if(firstCol <= 7){
        byteOneChangeVal();
        lastEqualButtonTime = millis();
      //}
    }
  }
  if (screen == 3) {
    //BYTE 2:
    if (millis() - lastEqualButtonTime > debounceTimeMs) {
      //Left button: moves cursor to the left
      if (switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree != prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
        if(secondCol <= 7){
          byteTwoMoveCursorRight();
          lastEqualButtonTime = millis();
        }
      }
      if (switchStateOne == prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
        if(secondCol > 0){
          byteTwoMoveCursorLeft();
          lastEqualButtonTime = millis();
        }
      }
    }      
    lcd.setCursor(secondCol, 1);
    //Right button: changes the value of the bit between 1 and 0
    if (switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
      if(secondCol <= 7){
        byteTwoChangeVal();
        lastEqualButtonTime = millis();
      }
    }
  }
  if(screen == 4){
    //Solution will automically present, 
    //this only resets the values held by the bytes.
    //For hex conversion purposes, this is moved
    //to screen 6 and taken out here.
    //--
    //if(switchStateOne != prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo){
    //  newSet();
    //}
  }
  if(screen == 5){
    if(millis() - lastEqualButtonTime > debounceTimeMs){
      //Left button: increase the contrast
      if(switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        increaseContrast();
        delay(150);
      }
      analogWrite(contrast, contrastMeasure);
    }
    //Right button: decrease the contrast
    if(switchStateOne == prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
      decreaseContrast();
      delay(150);
    }
    analogWrite(contrast, contrastMeasure);
  }
  if(screen == 6){
    if(millis() - lastEqualButtonTime > debounceTimeMs){
      if(switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        solutionHexConversion();
        delay(150);
      }
    }
  }
  if(screen == 7){
    if(millis() - lastEqualButtonTime > debounceTimeMs){
      newSet();
      delay(100);
      //Is this code necessary?
      //DOUBLE CHECK
      if(switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        setup();
      }
      if(switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour != prevSwitchStateFour){
        setup();
      }
    }
  }
  //For the next two hexadecimal parts,
  //I chose to start out the lcd cursor at (3, 4)
  //so that the values of each digit were gathered
  //correctly from the users point of view.
  if(screen == 8){
    if (millis() - lastEqualButtonTime > debounceTimeMs) {
      //Left button: moves cursor to the left
      if (switchStateOne == prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
        if(firstCol > 0){
          hexOneMoveCursorLeft();
          delay(150);
          lastEqualButtonTime = millis();        
        }
      }
      if (switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree != prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
        if(firstCol < 2){
          hexOneMoveCursorRight();
          delay(150);
          lastEqualButtonTime = millis();
        }
      }
    }
    lcd.setCursor(firstCol, 1);
    //Right button: changes the value of the bit between 1 and 0
    if (switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
    && switchStateFour == prevSwitchStateFour) {
      if(firstCol < 5){
        hexOneChangeVal();
        delay(150);
        lastEqualButtonTime = millis();
      }
    }
  }
  if(screen == 9){
    if (millis() - lastEqualButtonTime > debounceTimeMs) {
      //Left button: moves cursor to the left
      if (switchStateOne == prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
        if(secondCol > 0){
          hexTwoMoveCursorLeft();
          delay(150);
          lastEqualButtonTime = millis();        
        }
      }
      if (switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree != prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour) {
        if(secondCol < 2){
          hexTwoMoveCursorRight();
          delay(150);
          lastEqualButtonTime = millis();
        }
      }
    }
    lcd.setCursor(secondCol, 1);
    //Right button: changes the value of the bit between 1 and 0
    if (switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
    && switchStateFour == prevSwitchStateFour) {
      if(secondCol < 5){
        hexTwoChangeVal();
        delay(150);
        lastEqualButtonTime = millis();
      }
    }
  }
  if(screen == 10){
    if(millis() - lastEqualButtonTime > debounceTimeMs){
      if (switchStateOne == prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        if(firstCol > 0){
          octalOneMoveCursorLeft();
          delay(150); 
        }
      }
      if (switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree != prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        if(firstCol < 3){
          octalOneMoveCursorRight();
          delay(150);
        }
      }
    }
    if (switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
    && switchStateFour == prevSwitchStateFour){
      octalOneChangeVal();
      delay(150);
    }
  }
  if(screen == 11){
    if(millis() - lastEqualButtonTime > debounceTimeMs){
      if (switchStateOne == prevSwitchStateOne && switchStateTwo != prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        if(secondCol > 0){
          octalTwoMoveCursorLeft();
          delay(150); 
        }
      }
      if (switchStateOne == prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree != prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        if(secondCol < 3){
          octalTwoMoveCursorRight();
          delay(150);
        }
      }
      if (switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        octalTwoChangeVal();
        delay(150);
      }
    }
  }
  if(screen == 12){
    if(millis() - lastEqualButtonTime > debounceTimeMs){
      if (switchStateOne != prevSwitchStateOne && switchStateTwo == prevSwitchStateTwo && switchStateThree == prevSwitchStateThree
      && switchStateFour == prevSwitchStateFour){
        hexCalc();
        delay(150);
      }
    }
  }
}

/*void instruction(){
  screen = 13;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("To increase value, press 1");
  for(int n = 0; n < 16; n++){
    lcd.scrollDisplayLeft();
    delay(150);
  }
}*/

void selection(){
  screen = 1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1=Base2, 2=Base8,");
  lcd.setCursor(4, 5);
  lcd.print("3=Base16 !");
}
void firstByte() {
  //Output to lcd for first byte
  screen = 2;
  firstCol = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BYTE 1:");
  lcd.setCursor(0, 1);
  lcd.print("00000000");
  lcd.setCursor(0, 1);
}

void secondByte() {
  //Output to screen for second byte
  screen = 3;
  secondCol = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BYTE 2:");
  lcd.setCursor(0, 1);
  lcd.print("00000000");
  lcd.setCursor(0, 1);
}
void firstHex(){
  screen = 8;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1st BASE 16 #:");
  lcd.setCursor(11, 12);
  lcd.print("(4->)");
  //Allow for Hex numbers up to 3 digits
  lcd.setCursor(2, 3);
  lcd.print(0);
  firstCol = 2;
}

void secondHex(){
  currentNum = 0;
  screen = 9;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("2nd BASE 16 #:");
  lcd.setCursor(11, 12);
  lcd.print("(4->)");
  //Allow for Hex numbers up to 3 digits
  lcd.setCursor(2, 3);
  lcd.print(0);
  secondCol = 2;
}

void finalHexSolution(){
  screen = 12;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press 1 for");
  lcd.setCursor(0, 1);
  lcd.print("Base16 Solution!");
  delay(150);
}

void hexScreen() {
  screen = 6;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press 1 for Hex:");
  lcd.setCursor(0,1);
  lcd.print("Solution: ");
}

void clearALU(){
  screen = 7;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press 1");
  lcd.setCursor(0, 1);
  lcd.print("to clear ALU!");
  delay(150);
}

void octalOne(){
  screen = 10;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1st BASE 8 #:");
  //Allow for octal numbers up to 4 digits
  lcd.setCursor(0, 1);
  lcd.print("0000");
  lcd.setCursor(3, 4);
  firstCol = 3;
}

void octalTwo(){
  screen = 11;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("2nd BASE 8 #:");
  //Allow for octal numbers up to 4 digits
  lcd.setCursor(0, 1);
  lcd.print("0000");
  lcd.setCursor(3, 4);
  secondCol = 3;
}

void byteOneMoveCursorRight() {
  //allows input to move cursor to the right on first byte screen
  currentNum = 0;
  delay(175);
  firstCol = firstCol + 1;
  lcd.setCursor(firstCol, 1);
  if(firstCol == 1){
    currentNum = oneOne;
  }
  if(firstCol == 2){
    currentNum = oneTwo;
  }
  if(firstCol == 3){
    currentNum = oneThree;
  }
  if(firstCol == 4){
    currentNum = oneFour;
  }
  if(firstCol == 5){
    currentNum = oneFive;
  }
  if(firstCol == 6){
    currentNum = oneSix;
  }
  if(firstCol == 7){
    currentNum = oneSeven;
  }
}

void byteOneMoveCursorLeft() {
  //allows input to move cursor to the left on first byte screen
  delay(175);
  if(firstCol >= 1){
    firstCol = firstCol - 1;
    lcd.setCursor(firstCol, 1);
  }
  if(firstCol == 0){
    lcd.setCursor(0, 1);
    currentNum = oneZero;
  }
  if(firstCol == 1){
    currentNum = oneOne;
  }
  if(firstCol == 2){
    currentNum = oneTwo;
  }
  if(firstCol == 3){
    currentNum = oneThree;
  }
  if(firstCol == 4){
    currentNum = oneFour;
  }
  if(firstCol == 5){
    currentNum = oneFive;
  }
  if(firstCol == 6){
    currentNum = oneSix;
  }
}

void byteTwoMoveCursorLeft(){
  //allows input to move cursor to the left on second byte screen
  delay(175);
  if(secondCol >= 1){
    secondCol = secondCol - 1;
    lcd.setCursor(secondCol, 1);
  }
  if(secondCol == 0){
    lcd.setCursor(0, 1);
    currentNum = twoZero;
  }
  if(secondCol == 1){
    currentNum = twoOne;
  }
  if(secondCol == 2){
    currentNum = twoTwo;
  }
  if(secondCol == 3){
    currentNum = twoThree;
  }
  if(secondCol == 4){
    currentNum = twoFour;
  }
  if(secondCol == 5){
    currentNum = twoFive;
  }
  if(secondCol == 6){
    currentNum = twoSix;
  }
}

void byteTwoMoveCursorRight(){
  //allows input to move cursor to the right on second byte screen
  currentNum = 0;
  delay(175);
  secondCol = secondCol + 1;
  lcd.setCursor(secondCol, 1);
  if(secondCol == 1){
    currentNum = twoOne;
  }
  if(secondCol == 2){
    currentNum = twoTwo;
  }
  if(secondCol == 3){
    currentNum = twoThree;
  }
  if(secondCol == 4){
    currentNum = twoFour;
  }
  if(secondCol == 5){
    currentNum = twoFive;
  }
  if(secondCol == 6){
    currentNum = twoSix;
  }
  if(secondCol == 7){
    currentNum = twoSeven;
  }
}

void byteOneChangeVal() {
  //changes the byte in the respective column for first byte
  if (currentNum == 1) {
    currentNum = 0;
  }
  else{
    currentNum = 1;
  }
  //change the number based on column
  if (firstCol == 0) {
    oneZero = currentNum;
  }
  else if (firstCol == 1) {
    oneOne = currentNum;
  }
  else if (firstCol == 2) {
    oneTwo = currentNum;
  }
  else if (firstCol == 3) {
    oneThree = currentNum;
  }
  else if (firstCol == 4) {
    oneFour = currentNum;
  }
  else if (firstCol == 5) {
    oneFive = currentNum;
  }
  else if (firstCol == 6) {
    oneSix = currentNum;
  }
  else if (firstCol == 7) {
    oneSeven = currentNum;
  }
  lcd.print(currentNum);
  delay(175);
}

void byteTwoChangeVal(){
  //changes the byte in the respective column for the secnd byte
  if (currentNum == 1) {
    currentNum = 0;
  }
  else{
    currentNum = 1;
  }
  //change the number based on column
  if (secondCol == 0) {
    twoZero = currentNum;
  }
  else if (secondCol == 1) {
    twoOne = currentNum;
  }
  else if (secondCol == 2) {
    twoTwo = currentNum;
  }
  else if (secondCol == 3) {
    twoThree = currentNum;
  }
  else if (secondCol == 4) {
    twoFour = currentNum;
  }
  else if (secondCol == 5) {
    twoFive = currentNum;
  }
  else if (secondCol == 6) {
    twoSix = currentNum;
  }
  else if (secondCol == 7) {
    twoSeven = currentNum;
  }
  lcd.print(currentNum);
  delay(175);
}

void octalOneMoveCursorRight(){
  delay(175);
  firstCol = firstCol + 1;
  lcd.setCursor(firstCol, 1);
  if(firstCol == 0){
    currentNum = oneZero;
  }
  if(firstCol == 1){
    currentNum = oneOne;
  }
  if(firstCol == 2){
    currentNum = oneTwo;
  }
  if(firstCol == 3){
    currentNum = oneThree;
  }
}

void octalTwoMoveCursorRight(){
  delay(175);
  secondCol = secondCol + 1;
  lcd.setCursor(secondCol, 1);
  if(secondCol == 0){
    currentNumTwo = twoZero;
  }
  if(secondCol == 1){
    currentNumTwo = twoOne;
  }
  if(secondCol == 2){
    currentNumTwo = twoTwo;
  }
  if(secondCol == 3){
    currentNumTwo = twoThree;
  }
}

void octalOneMoveCursorLeft(){
  delay(175);
  firstCol = firstCol - 1;
  lcd.setCursor(firstCol, 1);
  if(firstCol == 0){
    currentNum = oneZero;
  }
  if(firstCol == 1){
    currentNum = oneOne;
  }
  if(firstCol == 2){
    currentNum = oneTwo;
  }
  if(firstCol == 3){
    currentNum = oneThree;
  }
}

void octalTwoMoveCursorLeft(){
  delay(175);
  secondCol = secondCol - 1;
  lcd.setCursor(secondCol, 1);
  if(secondCol == 0){
    currentNumTwo = twoZero;
  }
  if(secondCol == 1){
    currentNumTwo = twoOne;
  }
  if(secondCol == 2){
    currentNumTwo = twoTwo;
  }
  if(secondCol == 3){
    currentNumTwo = twoThree;
  }
}

void hexOneMoveCursorRight(){
  //currentNum = 0;
  delay(175);
  firstCol = firstCol + 1;
  lcd.setCursor(firstCol, 1);
  if(firstCol == 0){
    currentNum = oneZero;
  }
  if(firstCol == 1){
    currentNum = oneOne;
  }
  if(firstCol == 2){
    currentNum = oneTwo;
  }
}

void hexTwoMoveCursorRight(){
  //currentNum = 0;
  delay(175);
  secondCol = secondCol + 1;
  lcd.setCursor(secondCol, 1);
  if(secondCol == 0){
    currentNumTwo = twoZero;
  }
  if(secondCol == 1){
    currentNumTwo = twoOne;
  }
  if(secondCol == 2){
    currentNumTwo = twoTwo;
  }
}

void hexOneMoveCursorLeft(){
  //currentNum = 0;
  delay(175);
  firstCol = firstCol - 1;
  lcd.setCursor(firstCol, 1);
  if(firstCol == 0){
    lcd.setCursor(0, 1);
    currentNum = oneZero;
  }
  if(firstCol == 1){
    currentNum = oneOne;
  }
  if(firstCol == 2){
    currentNum = oneTwo;
  }
}

void hexTwoMoveCursorLeft(){
  currentNum = 0;
  delay(175);
  secondCol = secondCol - 1;
  lcd.setCursor(secondCol, 1);
  if(secondCol == 0){
    lcd.setCursor(0, 1);
    currentNumTwo = twoZero;
  }
  if(secondCol == 1){
    currentNumTwo = twoOne;
  }
  if(secondCol == 2){
    currentNumTwo = twoTwo;
  }
}

void octalOneChangeVal(){
  lcd.setCursor(firstCol, 1);
  if(currentNum >= 7){
    currentNum = 0;
  }
  else{
    currentNum++;
  }
  if(firstCol == 0){
    oneZero = currentNum;
  }
  if(firstCol == 1){
    oneOne = currentNum;
  }
  if(firstCol == 2){
    oneTwo = currentNum;
  }
  if(firstCol == 3){
    oneThree = currentNum;
  }
  lcd.print(currentNum);
}

void octalTwoChangeVal(){
  lcd.setCursor(secondCol, 1);
  if(currentNumTwo >= 7){
    currentNumTwo = 0;
  }
  else{
    currentNumTwo++;
  }
  if(secondCol == 0){
    twoZero = currentNumTwo;
  }
  if(secondCol == 1){
    twoOne = currentNumTwo;
  }
  if(secondCol == 2){
    twoTwo = currentNumTwo;
  }
  if(secondCol == 3){
    twoThree = currentNumTwo;
  }
  lcd.print(currentNumTwo);
}

void hexOneChangeVal(){
  //Here is where the user input will determine hexadecimal
  //value. The maximum digits must be 4 (only for input of course)
  //in order to prevent my code from becoming unnecessarily long
  //--
  lcd.setCursor(firstCol, 1);
  if(currentNum >= 16){
    currentNum = 0;  
  }
  else{
    currentNum++;
  }
  //FIRST COLUMN(DIGIT):
  if (firstCol == 0){
    if(currentNum == 10){
      oneZeroHex = "A";
      lcd.print(oneZeroHex);
    }
    if(currentNum == 11){
      oneZeroHex = "B";
      lcd.print(oneZeroHex);
    }
    if(currentNum == 12){
      oneZeroHex = "C";
      lcd.print(oneZeroHex);
    }
    if(currentNum == 13){
      oneZeroHex = "D";
      lcd.print(oneZeroHex);
    }
    if(currentNum == 14){
      oneZeroHex = "E";
      lcd.print(oneZeroHex);
    }
    if(currentNum == 15){
      oneZeroHex = "F";
      lcd.print(oneZeroHex);
    }
    if(currentNum < 10){
      lcd.print(currentNum);
    }
    oneZero = currentNum;
  }
  //SECOND COLUMN(DIGIT):
  if (firstCol == 1){
    if(currentNum == 10){
      oneOneHex = "A";
      lcd.print(oneOneHex);
    }
    if(currentNum == 11){
      oneOneHex = "B";
      lcd.print(oneOneHex);
    }
    if(currentNum == 12){
      oneOneHex = "C";
      lcd.print(oneOneHex);
    }
    if(currentNum == 13){
      oneOneHex = "D";
      lcd.print(oneOneHex);
    }
    if(currentNum == 14){
      oneOneHex = "E";
      lcd.print(oneOneHex);
    }
    if(currentNum == 15){
      oneOneHex = "F";
      lcd.print(oneOneHex);
    }
    if(currentNum < 10){
      lcd.print(currentNum);
    }
    oneOne = currentNum;
  }
  //THIRD COLUMN (DIGIT):
  if (firstCol == 2){
    if(currentNum == 10){
      oneTwoHex = "A";
      lcd.print(oneTwoHex);
    }
    if(currentNum == 11){
      oneTwoHex = "B";
      lcd.print(oneTwoHex);
    }
    if(currentNum == 12){
      oneTwoHex = "C";
      lcd.print(oneTwoHex);
    }
    if(currentNum == 13){
      oneTwoHex = "D";
      lcd.print(oneTwoHex);
    }
    if(currentNum == 14){
      oneTwoHex = "E";
      lcd.print(oneTwoHex);
    }
    if(currentNum == 15){
      oneTwoHex = "F";
      lcd.print(oneTwoHex);
    }
    if(currentNum < 10){
      lcd.print(currentNum);
    }
    oneTwo = currentNum;
  }
}

void hexTwoChangeVal(){
  //Here is where the user input will determine hexadecimal
  //value. The maximum digits must be 4 (only for input of course)
  //in order to prevent my code from becoming unnecessarily long
  //--
  lcd.setCursor(secondCol, 1);
  if(currentNumTwo >= 16){
    currentNumTwo = 0;  
  }
  else{
    currentNumTwo++;
  }
  //FIRST COLUMN(DIGIT):
  if (secondCol == 0){
    if(currentNumTwo == 10){
      twoZeroHex = "A";
      lcd.print(twoZeroHex);
    }
    if(currentNumTwo == 11){
      twoZeroHex = "B";
      lcd.print(twoZeroHex);
    }
    if(currentNumTwo == 12){
      twoZeroHex = "C";
      lcd.print(twoZeroHex);
    }
    if(currentNumTwo == 13){
      twoZeroHex = "D";
      lcd.print(twoZeroHex);
    }
    if(currentNumTwo == 14){
      twoZeroHex = "E";
      lcd.print(twoZeroHex);
    }
    if(currentNumTwo == 15){
      twoZeroHex = "F";
      lcd.print(twoZeroHex);
    }
    if(currentNumTwo < 10){
      lcd.print(currentNumTwo);
    }
    twoZero = currentNumTwo;
  }
  //SECOND COLUMN(DIGIT):
  if (secondCol == 1){
    if(currentNumTwo == 10){
      twoOneHex = "A";
      lcd.print(twoOneHex);
    }
    if(currentNumTwo == 11){
      twoOneHex = "B";
      lcd.print(twoOneHex);
    }
    if(currentNumTwo == 12){
      twoOneHex = "C";
      lcd.print(twoOneHex);
    }
    if(currentNumTwo == 13){
      twoOneHex = "D";
      lcd.print(twoOneHex);
    }
    if(currentNumTwo == 14){
      twoOneHex = "E";
      lcd.print(twoOneHex);
    }
    if(currentNumTwo == 15){
      twoOneHex = "F";
      lcd.print(twoOneHex);
    }
    if(currentNumTwo < 10){
      lcd.print(currentNumTwo);
    }
    twoOne = currentNumTwo;
  }
  //THIRD COLUMN (DIGIT):
  if (secondCol == 2){
    if(currentNumTwo == 10){
      twoTwoHex = "A";
      lcd.print(twoTwoHex);
    }
    if(currentNumTwo == 11){
      twoTwoHex = "B";
      lcd.print(twoTwoHex);
    }
    if(currentNumTwo == 12){
      twoTwoHex = "C";
      lcd.print(twoTwoHex);
    }
    if(currentNumTwo == 13){
      twoTwoHex = "D";
      lcd.print(twoTwoHex);
    }
    if(currentNumTwo == 14){
      twoTwoHex = "E";
      lcd.print(twoTwoHex);
    }
    if(currentNumTwo == 15){
      twoTwoHex = "F";
      lcd.print(twoTwoHex);
    }
    if(currentNumTwo < 10){
      lcd.print(currentNumTwo);
    }
    twoTwo = currentNumTwo;
  }
}

void octalCalc(){
  long octSolZero = 0;
  long octSolOne = 0;
  long octSolTwo = 0;
  long octSolThree = 0;
  long octSolFour = 0;
  long octSolFive = 0;
  long octSolSix = 0;
  long octSolSeven = 0;
  long octSolSumOne = 0;
  long octSolSumTwo = 0;
  long octSum = 0;
  
  //first octal #:
  if(oneZero != 0){
    octSolZero = oneZero*512;
  }
  if(oneOne != 0){
    octSolOne = oneOne*64;
  }
  if(oneTwo != 0){
    octSolTwo = oneTwo*8;
  }
  if(oneThree != 0){
    octSolThree = oneThree*1;
  }
  //second octal #:
  if(twoZero != 0){
    octSolFour = twoZero*512;
  }
  if(twoOne != 0){
    octSolFive = twoOne*64;
  }
  if(twoTwo != 0){
    octSolSix = twoTwo*8;
  }
  if(twoThree != 0){
    octSolSeven = twoThree*1;
  }
  //decimal value for first octal #:
  octSolSumOne = octSolZero + octSolOne + octSolTwo + octSolThree;
  //decimal value for second octal #:
  octSolSumTwo = octSolFour + octSolFive + octSolSix + octSolSeven;
  //final value:
  octSum = octSolSumOne + octSolSumTwo;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Base8 Solution:");
  lcd.setCursor(0, 1);
  lcd.print(octSum, OCT);
  octal = false;
}

void hexCalc(){
  //This function converts each hex value
  //into a decimal value, performs the math necessary,
  //then leaves us with our hexadecimal solution
  //--
  long hexSolZero = 0;
  long hexSolOne = 0;
  long hexSolTwo = 0;
  uint32_t hexSolThree = 0;
  long hexSolFour = 0;
  long hexSolFive = 0;
  long hexSolSix = 0;
  uint32_t hexSolSeven = 0;
  //First #:
  if(oneZeroHex == "A"){
    hexSolZero = oneZero*256; 
  }
  if(oneZeroHex == "B"){
    hexSolZero = 11*256;
  }
  if(oneZeroHex == "C"){
    hexSolZero = 3072;
  }
  if(oneZeroHex == "D"){
    hexSolZero = 3328;
  }
  if(oneZeroHex == "E"){
    hexSolZero = 3584;
  }
  if(oneZeroHex == "F"){
    hexSolZero = 3840;
  }
  if(oneZero < 10){
    hexSolZero = oneZero*256;
  }
  if(oneOneHex == "A"){
    hexSolOne = 160; 
  }
  if(oneOneHex == "B"){
    hexSolOne = 176;
  }
  if(oneOneHex == "C"){
    hexSolOne = 192;
  }
  if(oneOneHex == "D"){
    hexSolOne = 208;
  }
  if(oneOneHex == "E"){
    hexSolOne = 224;
  }
  if(oneOneHex == "F"){
    hexSolOne = 240;
  }
  if(oneOne < 10){
    hexSolOne = oneOne*16;
  }
  if(oneTwoHex == "A"){
    hexSolTwo = 10; 
  }
  if(oneTwoHex == "B"){
    hexSolTwo = 11;
  }
  if(oneTwoHex == "C"){
    hexSolTwo = 12;
  }
  if(oneTwoHex == "D"){
    hexSolTwo = 13;
  }
  if(oneTwoHex == "E"){
    hexSolTwo = 14;
  }
  if(oneTwoHex == "F"){
    hexSolTwo = 15;
  }
  if(oneTwo < 10){
    hexSolTwo = oneTwo*1;
  }
  //First # Solution in decimal:
  solHexOne = hexSolZero + hexSolOne + hexSolTwo;
  //This time use the other 4 solution options,
  //Second #:
  if(twoZeroHex == "A"){
    hexSolFour = 2560; 
  }
  if(twoZeroHex == "B"){
    hexSolFour = 2816;
  }
  if(twoZeroHex == "C"){
    hexSolFour = 3072;
  }
  if(twoZeroHex == "D"){
    hexSolFour = 3328;
  }
  if(twoZeroHex == "E"){
    hexSolFour = 3584;
  }
  if(twoZeroHex == "F"){
    hexSolFour = 3840;
  }
  if(twoZero < 10){
    hexSolFour = twoZero*256;
  }
  if(twoOneHex == "A"){
    hexSolFive = 160; 
  }
  if(twoOneHex == "B"){
    hexSolFive = 176;
  }
  if(twoOneHex == "C"){
    hexSolFive = 192;
  }
  if(twoOneHex == "D"){
    hexSolFive = 208;
  }
  if(twoOneHex == "E"){
    hexSolFive = 224;
  }
  if(twoOneHex == "F"){
    hexSolFive = 240;
  }
  if(twoOne < 10){
    hexSolFive = twoOne*16;
  }
  if(twoTwoHex == "A"){
    hexSolSix = 10; 
  }
  if(twoTwoHex == "B"){
    hexSolSix = 11;
  }
  if(twoTwoHex == "C"){
    hexSolSix = 12;
  }
  if(twoTwoHex == "D"){
    hexSolSix = 13;
  }
  if(twoTwoHex == "E"){
    hexSolSix = 14;
  }
  if(twoTwoHex == "F"){
    hexSolSix = 15;
  }
  if(twoTwo < 10){
    hexSolSix = twoTwo*1;
  }
  //Second # solution in decimal:
  solHexTwo = hexSolFour + hexSolFive + hexSolSix; 
  hexSolution = solHexOne + solHexTwo;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Base16 Solution:");
  lcd.setCursor(0, 1);
  lcd.print(hexSolution, HEX);
  delay(150);
  hex = false;
}

void binaryCalc(){
  //Because the addition is in binary,
  //the first and second byte's last bits (the seventh bit)
  //must be added together to get the first
  //bit of the solution, however the solution will print
  //in reverse order: 76543210, however the first (0,1+) positions
  //may be taken over by bits from another byte if overflow occurs
  //due to carrying
  //--
  solZero = twoSeven + oneSeven;
  if(solZero >= 2){
    solZero = solZero % 2;
    oneSix = oneSix + 1; 
  }
  solOne = twoSix + oneSix;
  if(solOne >= 2){
    solOne = solOne % 2;
    oneFive = oneFive + 1;
  }
  solTwo = twoFive + oneFive;
  if(solTwo >= 2){
    solTwo = solTwo % 2;
    oneFour = oneFour + 1;
  }
  solThree = twoFour + oneFour;
  if(solThree >= 2){
    solThree = solThree % 2;
    oneThree = oneThree + 1;
  }
  solFour = twoThree + oneThree;
  if(solFour >= 2){
    solFour = solFour % 2;
    oneTwo = oneTwo + 1;
  }
  solFive = twoTwo + oneTwo;
  if(solFive >= 2){
    solFive = solFive % 2;
    oneOne = oneOne + 1;
  }
  solSix = twoOne + oneOne;
  if(solSix >= 2){
    solSix = solSix % 2;
    oneZero = oneZero + 1;
  }
  solSeven = twoZero + oneZero;
  if(solSeven >= 2){
    solSeven = solSeven % 2;
    solOverOne = solOverOne + 1;
  }
  //if even more carry is necessary,
  //this piece handles that:
  if(solOverOne >= 2){
    solOverOne = solOverOne % 2;
    solOverTwo = solOverTwo + 1;
  }
}
  
void solution(){
  //print result of binaryCalc()
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Solution:");
  lcd.setCursor(0,1);
  binaryCalc();
  //Check if we need to show overflow bits(if any exist)
  if(solOverTwo != 0){
    lcd.print(solOverTwo);
  }
  if(solOverOne != 0){
    lcd.print(solOverOne);    
  }
  lcd.print(solSeven);
  lcd.print(solSix);
  lcd.print(solFive);
  lcd.print(solFour);
  lcd.print(solThree);
  lcd.print(solTwo);
  lcd.print(solOne);
  lcd.print(solZero);
}

void solutionHexConversion(){
  //This function converts the solution byte
  //to hexadecimal value, and in order to do so
  //we must take the values from the top of our code
  //and change/keep them depending on
  //the value of each bit
  //--
  if(solOverTwo != 0){
    solutionCarryTwo = 2;
  }
  if(solOverOne != 0){
    solutionCarryOne = 1;
  }
  hexDigitZero = solutionCarryTwo + solutionCarryOne;
  //First Four bits to calculate:
  if(solSeven != 0){
    seven = 2*2*2;
  }
  if(solSix != 0){
    six = 2*2;
  }
  if(solFive != 0){
    five = 2;
  }
  if(solFour != 0){
    four = 1;
  }
  //First hex digit:
  hexDigitOne = seven + six + five + four;
  //Second four bits to calculate:
  if(solThree != 0){
    three = 2*2*2;
  }
  if(solTwo != 0){
    two = 2*2;
  }
  if(solOne != 0){
    one = 2;
  }
  if(solZero != 0){
    zero = 1;
  }
  //Second hex digit:
  hexDigitTwo = three + two + one + zero;
  lcd.setCursor(9, 10);
  //If the values of hexDigitOne or hexDigitTwo are
  //above 9, then their values become A(10), B(11),
  //C(12), D(13), E(14), and F(15) due to hexadecimal
  //rules for values 10 and above
  //--
  //Note: Because we can just take the bits in
  //groups of four and convert them, the maximum
  //value we can reach in a single digit is F, so
  //accounting for values of 16 and above is not
  //necessary
  //-- 
  //Print hex value for carried bits only if necessary:
  if(hexDigitZero != 0){
    lcd.print(hexDigitZero);
  }
  //print hex value for next 4 bits:
  if(hexDigitOne == 10){
    lcd.print("A"); 
  }
  if(hexDigitOne == 11){
    lcd.print("B");
  }
  if(hexDigitOne == 12){
    lcd.print("C");
  }
  if(hexDigitOne == 13){
    lcd.print("D");
  }
  if(hexDigitOne == 14){
    lcd.print("E");
  }
  if(hexDigitOne == 15){
    lcd.print("F");
  }
  if(hexDigitOne <= 9 && hexDigitOne > 0){
    lcd.print(hexDigitOne);
  }
  if(hexDigitZero != 0 && hexDigitOne == 0){
    lcd.print(hexDigitOne);
  }
  //Print hex value for next last bits:
  if(hexDigitTwo == 10){
    lcd.print("A"); 
  }
  if(hexDigitTwo == 11){
    lcd.print("B");
  }
  if(hexDigitTwo == 12){
    lcd.print("C");
  }
  if(hexDigitTwo == 13){
    lcd.print("D");
  }
  if(hexDigitTwo == 14){
    lcd.print("E");
  }
  if(hexDigitTwo == 15){
    lcd.print("F");
  }
  if(hexDigitTwo <= 9){
    lcd.print(hexDigitTwo); 
  }
}

void newSet(){
  //Clear input for all bytes and solution
  //BYTE 1:
  oneZero = 0;
  oneOne = 0;
  oneTwo = 0;
  oneThree = 0;
  oneFour = 0;
  oneFive = 0;
  oneSix = 0;
  oneSeven = 0;
  //BYTE 2:
  twoZero = 0;
  twoOne = 0;
  twoTwo = 0;
  twoThree = 0;
  twoFour = 0;
  twoFive = 0;
  twoSix = 0;
  twoSeven = 0;
  //SOLUTION:
  solZero = 0;
  solOne = 0;
  solTwo = 0;
  solThree = 0;
  solFour = 0;
  solFive = 0;
  solSix = 0;
  solSeven = 0;
  solOverOne = 0;
  solOverTwo = 0;
  firstCol = 0;
  secondCol = 0;
  currentNum = 0;
  currentHex = "A";
  //HEX:
  oneZeroHex = "n";
  oneOneHex = "n";
  oneTwoHex = "n";
  oneThreeHex = "n";
  twoZeroHex = "n";
  twoOneHex = "n";
  twoTwoHex = "n";
  twoThreeHex = "n";
}

void contrastScreen(){
  //Default contrast is 20
  //Left button(switch one) will increase
  //Right button(switch two) will decrease
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Contrast=215.0");
  lcd.setCursor(0,1);
  lcd.print("B1+:");
  lcd.setCursor(9,10);
  lcd.print("B2-:");
}

void increaseContrast(){
  contrastMeasure = contrastMeasure + 1;
  analogWrite(contrast, contrastMeasure / 12.7);
  lcd.setCursor(12,0);
  lcd.print(contrastMeasure);
}

void decreaseContrast(){
  contrastMeasure = contrastMeasure - 1;
  analogWrite(contrast, contrastMeasure / 12.7);
  lcd.setCursor(12,0);
  lcd.print(contrastMeasure);
}