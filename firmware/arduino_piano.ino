#define clockPin 10
#define dataPin  9
#define latchPin 8
#define buzzerPin 11

int numOfRegisters = 3;
byte* registerState;

void setup() {
  registerState = new byte[numOfRegisters];
  for (size_t i = 0; i < numOfRegisters; i++) {
    registerState[i] = 0;
  }

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600); // Initialize serial communication at 9600 baud
}

void loop() {
  if (Serial.available() > 0) {
    String chordProgression = Serial.readStringUntil('\n');
    chordProgression.trim();
    playChordProgression(chordProgression); // New function to handle progression
  }
}
void playChordProgression(String progression) {
  char* strtokIndx;
  char* chordToken = strtok_r((char*)progression.c_str(), " ", &strtokIndx);
  while (chordToken != NULL) {
    String chord = String(chordToken);
    playChord(chord); // Use your existing function to play each chord
    chordToken = strtok_r(NULL, " ", &strtokIndx);
  }
}

void playChord(String chord) {
 String noteArray[] = {
    "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
    "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5", "C6"
  };

  int notePins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ,14, 15, 16, 17,18,19,20,21,22,23};

   int noteFrequencies[] = {
    261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
    523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047
  };

  
  // Turn off all LEDs before playing a chord
  for (int i = 0; i < numOfRegisters * 8; i++) {
    regWrite(i, LOW);
  }

  // Parse the chord string and activate LEDs
  char* strtokIndx;
  char* noteToken = strtok_r((char*)chord.c_str(), " ", &strtokIndx);
  while (noteToken != NULL) {
    String note = String(noteToken);
    for (int i = 0; i < 24; i++) {
      if (note == noteArray[i]) {
        regWrite(notePins[i], HIGH); // Light up the corresponding LED
      }
    }
    noteToken = strtok_r(NULL, " ", &strtokIndx);
  }

  // Play each note in the chord in succession
  noteToken = strtok_r((char*)chord.c_str(), " ", &strtokIndx);
  while (noteToken != NULL) {
    String note = String(noteToken);
    for (int i = 0; i < 24; i++) {
      if (note == noteArray[i]) {
        tone(buzzerPin, noteFrequencies[i], 100); // Play the note sound
        delay(120); // Short delay between notes to simulate chord
      }
    }
    noteToken = strtok_r(NULL, " ", &strtokIndx);
  }
}

void notesToPlay() {
  // Starting with a simple C major chord progression
  playChord("C E G");
  delay(500); // Shorter delay for a quicker progression

  // Moving to G major
  playChord("G B D");
  delay(500);

  // Transition through A minor
  playChord("A C E");
  delay(500);

  // F major to add a different feel
  playChord("F A C");
  delay(500);

  // Quick D minor chord
  playChord("D F A");
  delay(500);

  // E minor for a slight melancholy touch
  playChord("E G B");
  delay(500);

  // Repeat C major to G major transition for resolution
  playChord("C E G");
  delay(500);
  playChord("G B D");
  delay(500);

  // End on C major for a classic finish
  playChord("C E G");
  delay(1000); // Longer delay for final chord to ring out
}

void regWrite(int pin, bool state) {
  int reg = pin / 8;
  int actualPin = pin % 8;
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < numOfRegisters; i++) {
    byte* states = &registerState[i];
    if (i == reg) {
      bitWrite(*states, actualPin, state);
    }
    shiftOut(dataPin, clockPin, MSBFIRST, *states);
  }
  digitalWrite(latchPin, HIGH);
}

