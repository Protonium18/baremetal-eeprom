#define PIN_SCLCK 2
#define PIN_SIN 3
#define PIN_SOUT 4
#define PIN_HOLD 5
#define PIN_SELECT 6
#define PIN_WP 7

#define del 5

bool inst_write[8] = {0, 0, 0, 0, 0, 0, 1, 0};
bool inst_read[8] = {0, 0, 0, 0, 0, 0, 1, 1};
bool inst_we[8] = {0, 0, 0, 0, 0, 1, 1, 0};
bool inst_wd[8] = {0, 0, 0, 0, 0, 1, 0, 0};
bool inst_regread[8] = {0, 0, 0, 0, 0, 1, 0, 1};
bool data_sample[8] = {0, 0, 0, 0, 1, 1, 0, 1};

bool* byteAddress(const uint16_t address){
  bool* add_array = new bool[16];
  uint16_t kb_address = address;
  
  for(int i = 0; i < 16 ; i++){
    add_array[15-i] = ((kb_address >> i) & 0x01);  
  }

  Serial.println("");
  return(add_array);
}

bool* readEEPROM(const uint16_t address = 0, const uint16_t bytesRead = 1){

  bool* add_array = byteAddress(address);
  bool* out_array = new bool[bytesRead*8];
  
  digitalWrite(PIN_SELECT, LOW);
  for(int i = 0; i < 8; i++){
    digitalWrite(PIN_SIN, inst_read[i]);
    digitalWrite(PIN_SCLCK, HIGH);
    digitalWrite(PIN_SCLCK, LOW);
    digitalWrite(PIN_SIN, LOW);
  }
  for(int i = 0; i < 16; i++){
    digitalWrite(PIN_SIN, add_array[i]);
    digitalWrite(PIN_SCLCK, HIGH);
    digitalWrite(PIN_SCLCK, LOW);
    digitalWrite(PIN_SIN, LOW);
  }
  for(int i = 0; i < bytesRead*8; i++){
    digitalWrite(PIN_SCLCK, HIGH);
    out_array[i] = digitalRead(PIN_SOUT);
    digitalWrite(PIN_SCLCK, LOW);
  }

  digitalWrite(PIN_SELECT, HIGH);  
  delete[](add_array);
  return(out_array);
}

void writeEEPROM(const bool* data, const uint16_t address = 0, const int bytes_write = 1){
  bool* add_array = byteAddress(address);
  statInst(inst_we);
  
  digitalWrite(PIN_SELECT, LOW);
  for(int i = 0; i < 8; i++){
    digitalWrite(PIN_SIN, inst_write[i]);
    digitalWrite(PIN_SCLCK, HIGH);
    digitalWrite(PIN_SCLCK, LOW);
    digitalWrite(PIN_SIN, LOW);
  }
  for(int i = 0; i < 16; i++){
    digitalWrite(PIN_SIN, add_array[i]);
    digitalWrite(PIN_SCLCK, HIGH);
    digitalWrite(PIN_SCLCK, LOW);
    digitalWrite(PIN_SIN, LOW);
  }
  for(int i = 0; i < bytes_write*8; i++){
    digitalWrite(PIN_SIN, data[i]);
    digitalWrite(PIN_SCLCK, HIGH);
    digitalWrite(PIN_SCLCK, LOW);
    digitalWrite(PIN_SIN, LOW);
  }

  digitalWrite(PIN_SELECT, HIGH);
  delay(5);

  delete[](add_array);
}

void setup() {
  pinMode(PIN_SCLCK, OUTPUT);
  pinMode(PIN_SIN, OUTPUT);
  pinMode(PIN_SOUT, INPUT);
  pinMode(PIN_SELECT, OUTPUT);
  pinMode(PIN_WP, OUTPUT);

  digitalWrite(PIN_SELECT, HIGH);
  digitalWrite(PIN_HOLD, HIGH);
  digitalWrite(PIN_SIN, LOW);
  digitalWrite(PIN_SCLCK, LOW);
  digitalWrite(PIN_WP, LOW);
 
  
  Serial.begin(57600);
  delay(100);
  writeEEPROM(data_sample, 2, 1);
  delay(1000);
  bool* arrayt = readEEPROM(0, 3);
  Serial.println("");
  for(int i = 0; i < 24; i++){
    if(i%8 == 0){
      Serial.println();
    }
    Serial.print(arrayt[i]);
  }

  
  
}

void loop() {
  

}

void statInst(const bool* data){
  digitalWrite(PIN_SELECT, LOW);
  for(int i = 0; i < 8; i++){
    digitalWrite(PIN_SIN, data[i]);
    digitalWrite(PIN_SCLCK, HIGH);
    digitalWrite(PIN_SCLCK, LOW);
    digitalWrite(PIN_SIN, LOW);
  }
  digitalWrite(PIN_SELECT, HIGH);
  Serial.println("");
}

void statRead(){

  digitalWrite(PIN_SELECT, LOW);
  
  for(int i = 0; i < 8; i++){
    digitalWrite(PIN_SIN, inst_regread[i]);
    digitalWrite(PIN_SCLCK, HIGH);
    digitalWrite(PIN_SCLCK, LOW);
    digitalWrite(PIN_SIN, LOW);
  }

  for(int i = 0; i < 8; i++){
    digitalWrite(PIN_SCLCK, HIGH);
    Serial.print(digitalRead(PIN_SOUT));
    digitalWrite(PIN_SCLCK, LOW);
  }

  digitalWrite(PIN_SELECT, HIGH);
  Serial.println("");
  
}
