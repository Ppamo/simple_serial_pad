#define B0S 4
#define B01 5
#define B02 6
#define B03 7
#define B04 8
#define LEDR 9
#define LEDG 10
#define LEDB 11
#define LEDP 12

#define BRIGHTNESS_RATE 1.85
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 200
#define DELAY 200
#define RESET_INTERVAL 2000

unsigned char led_on = 1;
unsigned char led_blinking = 0;
unsigned char layer = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Init");

  pinMode(B0S, INPUT);
  pinMode(B01, INPUT);
  pinMode(B02, INPUT);
  pinMode(B03, INPUT);
  pinMode(B04, INPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDP, HIGH);
  digitalWrite(B0S, HIGH);
  digitalWrite(B01, HIGH);
  digitalWrite(B02, HIGH);
  digitalWrite(B03, HIGH);
  digitalWrite(B04, HIGH);
}

void checkButton(unsigned char button, unsigned char &state){
  static unsigned long stateHigmMillis = 0;
  if (digitalRead(button) == HIGH){
    if (state != 1) {
      state = 1;
      if (button == B0S) {
        stateHigmMillis = millis();
        led_blinking = 1;
      } else {
        changed(button, 1);
      }
    }
  } else {
    if (state != 0){
      state = 0;
      if (button == B0S) {
        stateHigmMillis = 0;
        led_blinking = 0;
        layer = layer + 1;
      } else {
        changed(button, 0);
      }
    }
  }
  if (button == B0S) {
    if (stateHigmMillis > 0){
      if ((millis() - stateHigmMillis) > RESET_INTERVAL){
        stateHigmMillis = 0;
        led_blinking = 0;
        led_on = 1;
        layer = -1;
      }
    }
  }
}

void writeColor(unsigned char red, unsigned char green, unsigned char blue){
  analogWrite(LEDR, red);
  analogWrite(LEDG, green);
  analogWrite(LEDB, blue);
}

void writeColorToLed(unsigned char layer, unsigned char brightness){
  if (brightness == 0) {
    analogWrite(LEDP, 0);
  } else {
    analogWrite(LEDP, 255);
  }
  switch (layer) {
    case 0: // white
      writeColor(10, 160, 200);
      break;
    case 1: // red
      writeColor(60, 255, 255);
      break;
    case 2: // green
      writeColor(255, 90, 255);
      break;
    case 3: // blue
      writeColor(255, 255, 100);
      break;
    case 4: // yellow
      writeColor(10, 140, 255);
      break;
    case 5: // purple
      writeColor(10, 255, 140);
      break;
    case 6: // cyan
      writeColor(255, 10, 80);
      break;
  }
}

void controlLed(unsigned char &layer, unsigned char &brightness){
  if (led_blinking == 1){
    if (led_on == 1) {
      writeColorToLed(layer, brightness);
      led_on = 0;
    } else {
      writeColorToLed(layer, 0);
      led_on = 1;
    }
  } else {
      writeColorToLed(layer, brightness);
  }
  // Serial.println(brightness);
}

void changed(unsigned char button, unsigned char state){
  Serial.print(layer);
  Serial.print(";");
  Serial.print(button - 4);
  Serial.print(";");
  Serial.println(state);
}

void loop() {
  static unsigned char stateB0S = 0;
  static unsigned char stateB01 = 0;
  static unsigned char stateB02 = 0;
  static unsigned char stateB03 = 0;
  static unsigned char stateB04 = 0;
  static unsigned char brightness = MIN_BRIGHTNESS;
  checkButton(B0S, stateB0S);
  controlLed(layer, brightness);
  checkButton(B01, stateB01);
  checkButton(B02, stateB02);
  checkButton(B03, stateB03);
  checkButton(B04, stateB04);
  delay(DELAY);
}
