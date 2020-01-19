#include <Wire.h>

int rows[5]={9,7,8,6,5};
int columns[6]={2,3,4,10,11,12};
int buf[2];
int counter = 0;
int x = 1;

void setup() {
  Serial.begin(9600);
  Wire.setClock(400000);
  Wire.begin(x);
  for (int r = 0; r<5; r++) {
    pinMode(rows[r], INPUT_PULLUP);
  }
  for (int c = 0; c<6; c++) {
    pinMode(columns[c], OUTPUT);
    digitalWrite(columns[c], HIGH);
  }
}

void loop() {
  for (int c = 0; c<6; c++) {
    digitalWrite(columns[c], LOW);
    for (int r = 0; r<5; r++) {
      if (!digitalRead(rows[r])) {
        Serial.print("Column: ");
        Serial.print(c);
        Serial.print("Row:");
        Serial.println(r);
        Wire.beginTransmission(0);
        Wire.write(x);
        Wire.write(c);
        Wire.write(r);
        Wire.endTransmission();
        if (counter < 12) {
          buf[counter] = c;
          buf[counter+1] = r;
          counter += 2;
        } else {
          buf[0] = c;
          buf[1] = r;
          counter = 2;
        }
      }
    }
    digitalWrite(columns[c],HIGH);
  }
}
