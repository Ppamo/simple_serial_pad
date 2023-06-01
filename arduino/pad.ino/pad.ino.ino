#define B0S 4
#define B01 5
#define B02 6
#define B03 7
#define B04 8
const unsigned int RESET_INTERVAL = 2000;
const unsigned int DELAY = 250;

void setup() {
  Serial.begin(9600);
  Serial.println("Init");

  pinMode(B0S, INPUT);
  pinMode(B01, INPUT);
  pinMode(B02, INPUT);
  pinMode(B03, INPUT);
  pinMode(B04, INPUT);
  digitalWrite(B0S, HIGH);
  digitalWrite(B01, HIGH);
  digitalWrite(B02, HIGH);
  digitalWrite(B03, HIGH);
  digitalWrite(B04, HIGH);
}

void checkButton(unsigned int button, unsigned int& state){
  static unsigned long stateHigmMillis = 0;
  if (digitalRead(button) == HIGH){
    if (state != 1) {
      state = 1;
      changed(button, 1);
      if (button == B0S) {
        stateHigmMillis = millis();
      }
    }
  } else {
    if (state != 0){
      state = 0;
      changed(button, 0);
      if (button == B0S) {
        stateHigmMillis = 0;
      }
    }
  }
  if (button == B0S) {
    if (stateHigmMillis > 0){
      if ((millis() - stateHigmMillis) > RESET_INTERVAL){
        Serial.println("Reset state");
        stateHigmMillis = 0;
      }
    }
  }
}

void changed(unsigned int button, unsigned int state){
  Serial.print(button);
  Serial.print(" ");
  Serial.println(state);
}

void loop() {
  static unsigned int stateB0S = 0;
  static unsigned int stateB01 = 0;
  static unsigned int stateB02 = 0;
  static unsigned int stateB03 = 0;
  static unsigned int stateB04 = 0;
  checkButton(B0S, stateB0S);
  checkButton(B01, stateB01);
  checkButton(B02, stateB02);
  checkButton(B03, stateB03);
  checkButton(B04, stateB04);
  delay(DELAY);
}
