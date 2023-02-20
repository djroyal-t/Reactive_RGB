// Define the pin numbers for the red, green, and blue LEDs
const uint8_t RED_PIN = 9;
const uint8_t GREEN_PIN = 10;
const uint8_t BLUE_PIN = 11;
const uint8_t MIC_PIN = A2;

// Define the maximum and minimum brightness levels for each LED
const uint8_t MAX_BRIGHTNESS = 255;
const uint8_t MIN_BRIGHTNESS = 0;

// Define the calibration values for each LED (these need to be determined experimentally)
const float RED_CALIBRATION = 1.0;
const float GREEN_CALIBRATION = 0.9;
const float BLUE_CALIBRATION = 1.1;

// Define the lerp function to interpolate between two values
inline float lerp(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // Set the pin mode for the red, green, and blue pins to output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  // Read the input from the microphone
  uint8_t mic_value = analogRead(MIC_PIN);

  // Map the mic input to a hue value
  uint8_t hue = map(mic_value, 0, 1023, 0, 255);

  // Calculate the saturation and brightness based on the mic input
  uint8_t saturation = 255;
  uint8_t brightness = 255;

  // Declare variables for the current and next colors
  uint8_t r1, g1, b1, r2, g2, b2;

  // Calculate the current and next hue values
  const uint8_t hue1 = hue % 256;
  const uint8_t hue2 = (hue + 1) % 256;

  // Convert the current and next hue values to corresponding red, green, and blue values
  hsvToRgb(hue1, saturation, brightness, r1, g1, b1);
  hsvToRgb(hue2, saturation, brightness, r2, g2, b2);

  // Declare variables for the blended red, green, and blue values
  const uint8_t r = lerp((float)r1, (float)r2, (float)hue / 256.0);
  const uint8_t g = lerp((float)g1, (float)g2, (float)hue / 256.0);
  const uint8_t b = lerp((float)b1, (float)b2, (float)hue / 256.0);

  // Adjust the brightness of each LED using the calibration values and the desired brightness levels
  analogWrite(RED_PIN, (uint8_t)(RED_CALIBRATION * lerp((float)r, 0.0, 255.0, 0.0, (float)MAX_BRIGHTNESS)));
  analogWrite(GREEN_PIN, (uint8_t)(GREEN_CALIBRATION * lerp((float)g, 0.0, 255.0, 0.0, (float)MAX_BRIGHTNESS)));
  analogWrite(BLUE_PIN, (uint8_t)(BLUE_CALIBRATION * lerp((float)b, 0.0, 255.0, 0.0, (float)MAX_BRIGHTNESS)));

  // Increment the hue value
  hue++;

  // Wait for a short time to slow down the color transition
  delay(10);
}
