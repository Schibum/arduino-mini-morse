#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>



static const char* CHAR_TO_MORSE[128] = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, "-.-.--", ".-..-.", NULL, NULL, NULL, NULL, ".----.",
  "-.--.", "-.--.-", NULL, NULL, "--..--", "-....-", ".-.-.-", "-..-.",
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...",
  "---..", "----.", "---...", NULL, NULL, "-...-", NULL, "..--..",
  ".--.-.", ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
  "....", "..", ".---", "-.-", ".-..", "--", "-.", "---",
  ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--",
  "-..-", "-.--", "--..", NULL, NULL, NULL, NULL, "..--.-",
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

// Text to output in morse code.
const char* text = "Hello World";
int duration_dot = 400;
int duration_dash = duration_dot * 3;
int inter_word_space = duration_dot * 7;
int inter_char_space = duration_dot * 4;
int loop_space = duration_dot * 50;

void enter_sleep() {
  power_all_disable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  noInterrupts();
  sleep_enable();

  // turn off brown-out enable
  MCUCR = bit(BODS) | bit (BODSE);
  MCUCR = bit(BODS);
  interrupts();
  sleep_cpu();
}

const char* char_to_morse(char c) {
  if (islower(c))
    c += ('A' - 'a');

  return CHAR_TO_MORSE[(int) c];
}

void blink_morse_char(const char* morse) {
  Serial.write("morse: ");
  Serial.write(morse);
  Serial.write("\n");
  for (int i = 0; i < strlen(morse); ++i) {
    char c = morse[i];
    digitalWrite(13, HIGH);
    if (c == '.') {
      delay(duration_dot);
    } else {
      delay(duration_dash);
    }
    digitalWrite(13, LOW);
    delay(duration_dot);
  }
}

void setup() {

  // disable ADC
  ADCSRA = 0;
  wdt_disable();

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(9600);

  Serial.write("Initialized\n");
}

void loop() {
  Serial.write("Starting...\n");
  for (int i = 0; i < strlen(text); ++i) {
    char c = text[i];
    Serial.write("Char: ");
    Serial.write(c);
    Serial.write("\n");
    if (c == ' ') {
      delay(inter_word_space);
    } else {
      blink_morse_char(char_to_morse(c));
      delay(inter_char_space);
    }
  }
  enter_sleep();
}
