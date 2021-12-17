int AIN1 = 2;
int AIN2 = 3;
int APWM = 9;
int ASTBY = 5;
const int INPUT_PIN = A0;  // 入力ピンをA0に固定

int raw;
int spd = 185;   
int target;
String line;    // 受信文字列
int line_len;   // 受信文字列の長さ
long led_duty;
void setup() {
  Serial.begin(9600);  // シリアル通信の開始
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(APWM, OUTPUT);
  pinMode(ASTBY, OUTPUT);
  target = random(0,100);
}                                        
void loop() {
 setFaderPos(0);
 shortbrake();
 delay(1000);
 setFaderPos(50);
 shortbrake();
 delay(1000);
 Serial.println(map(analogRead(INPUT_PIN),0,1023,0,100));
 delay(1000);
 //setFaderPos(50);
 //shortbrake();
 //delay(100);
 //Serial.println(map(analogRead(INPUT_PIN),0,1023,0,100));
 setFaderPos(100);
 shortbrake();
 delay(1000);
  
 }
void setFaderPos(int pos){
  int curpos;
  curpos = map(analogRead(INPUT_PIN),0,1023,0,100);
  while(abs(curpos - pos ) > 1){
    if(curpos > pos && abs(curpos - pos)>1 ){
      down();
    }else if (curpos < pos && abs(curpos - pos)>1 ){
      up();
    }
    delay(1);
    brake();
    curpos = map(analogRead(INPUT_PIN),0,1023,0,100);
  }
  brake();
  //setFaderPos(pos);
}

void up(){
  digitalWrite(ASTBY, HIGH);
  analogWrite(APWM,spd);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
}
void down(){
  digitalWrite(ASTBY, HIGH);
  analogWrite(APWM,spd);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
}
void standby(){
  digitalWrite(ASTBY, LOW);
}
void shortbrake(){
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,HIGH);
  digitalWrite(ASTBY, HIGH);
}
void brake(){
  digitalWrite(ASTBY, HIGH);
  digitalWrite(APWM,HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
}
