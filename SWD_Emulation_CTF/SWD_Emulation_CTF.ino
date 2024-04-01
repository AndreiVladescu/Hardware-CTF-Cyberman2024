#define SWCLK D3
#define SWDIO D4

#define sw_delay(n) delayMicroseconds(10 * n)
#define clock_up \
  do { \
    digitalWrite(SWCLK, HIGH); \
    sw_delay(1); \
  } while (0)
#define clock_down \
  do { \
    digitalWrite(SWCLK, LOW); \
    sw_delay(1); \
  } while (0)

#define DEBUG_CHARS false

unsigned long previousMillis = 0;    // Variable to store the previous millis value
const unsigned long interval = 300;  // Interval in microseconds
bool clock_state = false;
bool packet_transmitted = false;

void setup() {
  pinMode(SWCLK, OUTPUT);  // SWCLK
  pinMode(SWDIO, OUTPUT);  // SWDIO
  digitalWrite(SWCLK, LOW);
  digitalWrite(SWDIO, LOW);
}

void loop() {
  delayMicroseconds(interval);
  taskData();
  digitalWrite(SWCLK, LOW);
  digitalWrite(SWDIO, LOW);

}

String ctf = "CTF{SWD_R3V3R53}";
int char_index = 0;

void writeCharsToBits() {
  char c = ctf[char_index++];

  char_index %= ctf.length();

  for (int i = 0; i < 8; i++) {
    clock_up;
    bool bit = (c >> i) & 1;
    digitalWrite(SWDIO, bit);
    clock_down;
  }
}

void taskData() {
#if !DEBUG_CHARS
  clock_up;
  digitalWrite(SWDIO, HIGH);  // start bit
  clock_down;

  clock_up;
  digitalWrite(SWDIO, LOW);  // APnDP bit, set to LOW
  clock_down;

  clock_up;
  digitalWrite(SWDIO, LOW);  // RnW bit, set to LOW (write mode  )
  clock_down;

  clock_up;
  digitalWrite(SWDIO, HIGH);  // A[2:3] bits, both set to HIGH
  clock_down;
  clock_up;
  clock_down;

  clock_up;
  digitalWrite(SWDIO, HIGH);  // Parity bit, set to HIGH
  clock_down;

  clock_up;
  digitalWrite(SWDIO, LOW);  // Stop bit, set to LOW
  clock_down;

  clock_up;
  digitalWrite(SWDIO, LOW);  // Park bit, set to LOW
  clock_down;

  clock_up;
  digitalWrite(SWDIO, LOW);  // Stop bit, set to LOW
  clock_down;

  clock_up;
  // Turnaround delay
  clock_down;

  clock_up;
  digitalWrite(SWDIO, HIGH);  // ACK bits, set to 100
  clock_down;
  clock_up;
  digitalWrite(SWDIO, LOW);
  clock_down;
  clock_up;
  clock_down;

  clock_up;
  // Turnaround delay
  clock_down;
#endif
  writeCharsToBits();  // Wdata byte 1
  writeCharsToBits();  // Wdata byte 2
  writeCharsToBits();  // Wdata byte 3
  writeCharsToBits();  // Wdata byte 4

#if !DEBUG_CHARS
  clock_up;
  digitalWrite(SWDIO, HIGH);  // Parity bit, set to HIGH
  clock_down;
#endif
}

void cycleClock() {
  digitalWrite(SWCLK, HIGH);
  delayMicroseconds(10);
  digitalWrite(SWCLK, LOW);
  delayMicroseconds(10);
}

void taskClock() {
  unsigned long currentMillis = micros();

  if (currentMillis - previousMillis >= interval) {

    digitalWrite(SWCLK, !digitalRead(SWCLK));
    clock_state = !clock_state;
    if (packet_transmitted)
      packet_transmitted = false;

    previousMillis = currentMillis;
  }
}