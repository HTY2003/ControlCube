#include <Wire.h>

int previous, keycounter, mod, key, side, column, row;
int settings[5][6][5][3][10];

//Common Windows Shortcuts
int copee[3][10] = {{MODIFIERKEY_CTRL,KEY_C,0,0,0,0,0,0,0,0},{},{}};
int paste[3][10] = {{MODIFIERKEY_CTRL,KEY_V,0,0,0,0,0,0,0,0},{},{}};

//Media Controls
int syssleep[3][10] = {{KEY_SYSTEM_SLEEP,0,0,0,0,0,0,0,0,0},{},{}};
int syswake[3][10] = {{KEY_SYSTEM_WAKE_UP,0,0,0,0,0,0,0,0,0},{},{}};


//AutoCAD
int line[3][10] = {{KEY_L,KEY_I,KEY_N,KEY_ENTER,0,0,0,0,0,0},{},{}};
int circle[3][10] = {{KEY_C,KEY_I,KEY_R,KEY_ENTER,0,0,0,0,0,0},{},{}};

void setMacro(int side, int column, int row, int combinations[3][10]) {
  for (int a = 0; a < 3; a++) {
    for (int b = 0; b < 10; b++) {
      settings[side][column][row][a][b] = combinations[a][b];
    }
  }
}

void activateMacro(int macro[3][10]) {
  for (int i = 0; i<3;i++) {
    //reset keys
    mod = 0;
    keycounter = 0;
    Keyboard.set_modifier(0);
    Keyboard.set_key1(0);
    Keyboard.set_key2(0);
    Keyboard.set_key3(0);
    Keyboard.set_key4(0);
    Keyboard.set_key5(0);
    Keyboard.set_key6(0);
    //set new keys
    for (int j = 0; j<10;j++) {
      key = macro[i][j];
      if (key == MODIFIERKEY_CTRL || key == MODIFIERKEY_SHIFT || key == MODIFIERKEY_ALT || key == MODIFIERKEY_GUI){
        if (mod > 0) mod = mod | key;
        if (mod == 0) mod = key;
      } else if (key > 0) {
          switch (keycounter) {
              case 5: Keyboard.set_key6(key); break;
              case 4: Keyboard.set_key5(key); break;
              case 3: Keyboard.set_key4(key); break;
              case 2: Keyboard.set_key3(key); break;
              case 1: Keyboard.set_key2(key); break;
              case 0: Keyboard.set_key1(key); break;
            }
          if (keycounter < 6) keycounter++;
      }
      //send report
      Keyboard.set_modifier(mod);
      Keyboard.send_now();
    }
  }
}
void setup() {
  Wire.setClock(400000);
  Wire.begin(0);
  Serial.begin(9600);
  previous = millis();
  Wire.onReceive(receiveEvent);
  mod = keycounter = 0;
  setMacro(1,0,0, copee);
  setMacro(1,1,0, paste);

  setMacro(1,0,1, syssleep);
  setMacro(1,1,1, syswake);

  setMacro(1,0,2, line);
  setMacro(1,1,2, circle);
  
  Keyboard.end();
  Keyboard.begin();
}

void loop() {
}

void receiveEvent(int numBytes) {
  side = Wire.read();
  column = Wire.read();
  row = Wire.read();
  if ((millis() - previous) > 200) {
    activateMacro(settings[side][column][row]);
    previous = millis();
  }
}
