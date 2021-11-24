#define PIN_SCLCK 2
#define PIN_SIN 3
#define PIN_SOUT 4
#define PIN_HOLD 5
#define PIN_SELECT 6
#define PIN_WP 7

#define inst_write 0x02
#define inst_read 0x03
#define inst_we 0x06
#define inst_wd 0x04
#define inst_regread 0x05

#define del 5

void binOut(unsigned char data, unsigned int len = 8){
  for(int i = len-1; i >= 0; i--){
    digitalWrite(PIN_SIN, (data >> i) & 0x01);
    digitalWrite(PIN_SCLCK, HIGH);
    digitalWrite(PIN_SCLCK, LOW);
    digitalWrite(PIN_SIN, LOW);
  }
}

unsigned char readEEPROM(const uint16_t address = 0){
  unsigned char data_out = 0;
  digitalWrite(PIN_SELECT, LOW);

  binOut(inst_read);
  binOut(address, 16);

  for(int i = 7; i >= 0; i--){
    unsigned char temp_val = 0;
    digitalWrite(PIN_SCLCK, HIGH);
    temp_val = (temp_val | digitalRead(PIN_SOUT)) << i;
    data_out = data_out | temp_val;
    digitalWrite(PIN_SCLCK, LOW);
  }

  digitalWrite(PIN_SELECT, HIGH);  
  return(data_out);
}

unsigned char* readEEPROMBlock(const uint16_t address = 0, const uint16_t len = 1){
  unsigned char* data_block = new unsigned char[len];
  for(int i = 0; i < len; i++){
    data_block[i] = readEEPROM(address+i);
  }
  return data_block;
}

void writeEEPROM(const unsigned char data, const uint16_t address = 0){
  digitalWrite(PIN_SELECT, LOW);
  binOut(inst_we);
  digitalWrite(PIN_SELECT, HIGH);
  
  digitalWrite(PIN_SELECT, LOW);
  binOut(inst_write);
  binOut(address, 16);
  binOut(data);
  
  digitalWrite(PIN_SELECT, HIGH);
  delay(5);
}

void writeEEPROMBlock(const unsigned char* data_block, uint16_t address = 0, const uint16_t len = 1){
  for(int i = 0; i < len; i++){
    writeEEPROM(data_block[i], address+i); 
  }
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
  delay(1000);
  
  //unsigned char data_block[4] = {16, 4, 8, 9};
  //writeEEPROMBlock(data_block, 0, 4);
  unsigned char* data_read = readEEPROMBlock(0, 4);
  for(int i = 0; i < 4; i++){
    Serial.println(data_read[i]);
    
  }
}

void loop() {

}

void statRead(){
  digitalWrite(PIN_SELECT, LOW);
  
  binOut(inst_regread);

  for(int i = 0; i < 8; i++){
    digitalWrite(PIN_SCLCK, HIGH);
    Serial.print(digitalRead(PIN_SOUT));
    digitalWrite(PIN_SCLCK, LOW);
  }

  digitalWrite(PIN_SELECT, HIGH);
  Serial.println("");
  
}
