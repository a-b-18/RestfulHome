const int ledPin =  11;      // the number of the LED pin

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    delay(10);
    digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(10);
    digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
}