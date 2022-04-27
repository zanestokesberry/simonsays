#define led8 8
#define led7 7
#define led6 6
#define led5 5

#define button8 led8 + 4
#define button7 led7 + 4
#define button6 led6 + 4
#define button5 led5 + 4

#define levels 50
#define buzzer 3

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int simonSaid[levels];

void setup() {
  Serial.begin(9600);

  pinMode(led8, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led5, OUTPUT);

  digitalWrite(led8, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led5, LOW);

  pinMode(button8, INPUT_PULLUP);         //when button is pressed the value at the pin goes to 0
  pinMode(button7, INPUT_PULLUP);
  pinMode(button6, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  

  randomSeed(analogRead(0));


  for (int i = 0; i < levels; i++) {
    simonSaid[i] = random(5,9);
  }

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(1,0);
  display.println("Simon Says");
  display.display();
  delay(2000);
}

void loop() {

  bool gameOver = 0;

  while (!gameOver) {
    for (int i = 1; i <= levels; i++) {
      
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setCursor(1,0);
      if (i < 10) {
        display.setTextSize(3);
        display.print("Level ");
      } else {
        display.setTextSize(2);
        display.print(" Level ");
      }
      display.println(i);
      display.display();
      delay(400);


      for (int g = 0; g < i; g++) {
        digitalWrite(simonSaid[g], HIGH);
        playBuzzer(simonSaid[g]);
        delay(200);
        digitalWrite(simonSaid[g], LOW);
        delay(50);
      }

      for(int g = 0; g < i; g++) {
        int sensorButton8 = 1;
        int sensorButton7 = 1;
        int sensorButton6 = 1;
        int sensorButton5 = 1;

        int buttonPressed = 0;

        while (sensorButton5 && sensorButton6 && sensorButton7 && sensorButton8) {
          sensorButton8 = digitalRead(button8);
          sensorButton7 = digitalRead(button7);
          sensorButton6 = digitalRead(button6);
          sensorButton5 = digitalRead(button5);
        }

        delay(200);

        if(!sensorButton8) {
          buttonPressed = 8;
        }
        else if(!sensorButton7) {
          buttonPressed = 7;
        }
        else if(!sensorButton6) {
          buttonPressed = 6;
        }
        else if(!sensorButton5) {
          buttonPressed = 5;
        }

        Serial.print(simonSaid[g]);
        Serial.print("\t");
        Serial.println(buttonPressed);
        digitalWrite(buttonPressed, HIGH);
        playBuzzer(buttonPressed);
        digitalWrite(buttonPressed, LOW);

        if (buttonPressed != simonSaid[g]) {
          gameOver = 1;
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(1,0);
          display.println("GAME OVER");
          display.print("at lvl ");
          display.print(i);
          display.display();
          digitalWrite(led8, HIGH);
          digitalWrite(led7, HIGH);
          digitalWrite(led6, HIGH);
          digitalWrite(led5, HIGH);

          delay(300);
          tone(buzzer, 294);
          delay(400);
          noTone(buzzer);
          delay(180);
          
          tone(buzzer, 277);
          delay(400);
          noTone(buzzer);
          delay(180);
          
          tone(buzzer, 262);
          delay(400);
          noTone(buzzer);
          delay(180);
          
          tone(buzzer, 247);
          delay(1600);
          noTone(buzzer);
  
          digitalWrite(led8, LOW);
          digitalWrite(led7, LOW);
          digitalWrite(led6, LOW);
          digitalWrite(led5, LOW);
          delay(2000);
          softReset();
        }

        
      }
    }
  }
  

}

void softReset() {
  asm volatile("  jmp 0");
}

void playBuzzer(int button) {
  if (button == 8) {
    tone(buzzer, 523);
    delay(300);
    noTone(buzzer);
  }
  else if (button == 7) {
    tone(buzzer, 659);
    delay(300);
    noTone(buzzer);
  }
  else if (button == 6) {
    tone(buzzer, 784);
    delay(300);
    noTone(buzzer);
  }
  else if(button == 5) {
    tone(buzzer, 1047);
    delay(300);
    noTone(buzzer);
  }
}
