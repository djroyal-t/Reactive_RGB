#define GREEN 0
#define BLUE 1
#define RED 2
#define Mic A2
#define sampleTime 50

int micOut;
int brightness = 0;
int brightness_prev;

//store color values
int R;
int G;
int B;

void setup() {
  //Serial.begin(9600);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  //digitalWrite(GREEN, LOW);
  //digitalWrite(BLUE, LOW);
  //digitalWrite(RED, LOW);
}

void loop() {
  brightness_prev = brightness;

  // Time variables to find the peak-to-peak amplitude
  unsigned long startTime = millis(); // Start of sample window
  unsigned int PTPAmp = 0;

  // Signal variables to find the peak-to-peak amplitude
  unsigned int maxAmp = 0;
  unsigned int minAmp = 1023;

  // Find the max and min of the mic output within the 50 ms timeframe
  while (millis() - startTime < sampleTime)
  {
    micOut = analogRead(Mic);
    if ( micOut < 1023) //prevent erroneous readings
    {
      if (micOut > maxAmp)
      {
        maxAmp = micOut; //save only the max reading
      }
      else if (micOut < minAmp)
      {
        minAmp = micOut; //save only the min reading
      }
    }
  }

  PTPAmp = maxAmp - minAmp; // (max amp) - (min amp) = peak-to-peak amplitude
  brightness = map(PTPAmp, 0, 1023, 0, 255);

  if (brightness - brightness_prev > 20) { //if the new volume sample is substantially louder than the last, reduce delta to avoid jarring color transition
    brightness = brightness_prev + 20;     //add 20 to the last value instead of using new value
  }

  else if (brightness_prev - brightness > 20) { //if the new volume sample is substantially quieter than the last, reduce delta to avoid jarring color transition
    brightness = brightness_prev - 20;   //reduce last value by 20 instead of using new value
  }

//Map RGB values from cold to hot according to linear interpolation fit

//Map Red values
  if (brightness < 102) {
    R = 255;
  }
  else if (brightness >= 102 && brightness < 136) {
    R = map(brightness, 102, 136, 255, 0);
  }
  else if (brightness >= 136) {
    R = 0;
  }

//Map Green values
  if (brightness < 34) {
    G = 255;
  }
  else if (brightness >= 34 && brightness < 68) {
    G = map(brightness, 34, 68, 255, 0);
  }
  else if (brightness >= 68 && brightness < 136) {
    G = 0;
  }
  else if (brightness >= 136) {
    G = map(brightness, 136, 169, 0, 255);
  }

  //Map Blue values
  if (brightness < 34) {
    B = map(brightness, 0, 34, 255, 0);
  }
  else if (brightness >= 34 && brightness < 68) {
    B = 0;
  }
  else if (brightness >= 68 && brightness < 102) {
    B = map(brightness, 68, 102, 0, 255);
  }
  else if (brightness >= 102) {
    B = 255;
  }

  //Light'er up
  analogWrite(RED, R);
  analogWrite(GREEN, G);
  analogWrite(BLUE, B);

  //Uncomment this line for help debugging
  /*  Serial.print(brightness);
    Serial.print(", R: ");
    Serial.print(R);
    Serial.print(", G: ");
    Serial.print(G);
    Serial.print(", B: ");
    Serial.println(B);
  */



  delay(10);  //remember there is a sampleTime delay baked into the PTPAmp function, so keep this low for smooth transitions
}
