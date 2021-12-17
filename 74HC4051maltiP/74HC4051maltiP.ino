//=============================================
//Interfacing 74HC4051 multiplexer with Arduino
//=============================================
#define S0 8
#define S1 9
#define S2 10
#define MUX_IN 5
int MUX_channel[6] = {0};
//==========================================================
void setup()
{
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(MUX_IN, INPUT);
}
//==========================================================
void loop()
{
  for (int i=0; i<6; i++)
  {
    digitalWrite(S0, HIGH && (i & 0b00000001));
    digitalWrite(S1, HIGH && (i & 0b00000010));
    digitalWrite(S2, HIGH && (i & 0b00000100));
    MUX_channel[i] = digitalRead(MUX_IN);
  }
}
