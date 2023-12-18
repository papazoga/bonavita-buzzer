int counter = 0;
int adc;

bool onbase_signal;
bool heat_signal;

enum {
  IDLE,
  HEAT,
  BUZZ,
  OFFB,
} state;
//state can be assigned any of the 4 values of the enum

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //baud rate for the uart
  state = IDLE;
}

void display_status() { //void means function doesn't return anything
  char out[50]; //string of 50 characters
  const char *state_string = (state == IDLE ? "IDLE" :
                              state == HEAT ? "HEAT" :
                              state == BUZZ ? "BUZZ" :
                              state == OFFB ? "OFFB" :
                                              "????");

  sprintf(out, "state=%s, adc=%d, heat=%d, onbase=%d\n",
          state_string , adc, heat_signal, onbase_signal);

  Serial.print(out);
}

void loop() {
  delay(1);

  adc = analogRead(A0);
  if (adc > 5) { //scaled value from raw adc reading
    onbase_signal = true;
  } else {
    onbase_signal = false;
  }

  if (digitalRead(3)) { //if the signal on pin3 is positive/true
    heat_signal = true;
  } else {
    heat_signal = false;
  }

  if (counter++ == 1000) {
    display_status();
    counter = 0;
  }

  // The main state machine
  if (state == HEAT) {
    if (heat_signal == 0)
      state = BUZZ;
  } else if (state == OFFB) {
    if (onbase_signal == true)
      state = IDLE;
  }

  if (heat_signal == true)
    state = HEAT;

  if (onbase_signal == false)
    state = OFFB;

  // Set the buzzer to the correct state
  digitalWrite(2, (state == BUZZ ? 1 : 0));
}
