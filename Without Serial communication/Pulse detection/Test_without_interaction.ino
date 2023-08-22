int sensorPin = 4;
int tilt= 5;
int LED=13;
int LED2=18;
int val;

void setup() {
   Serial.begin(9600);
   pinMode(tilt, INPUT);
   pinMode(sensorPin, INPUT);
   pinMode(LED, OUTPUT);
}


void loop ()
{ 
   val = digitalRead(tilt);
if(val == HIGH)
{ 
  digitalWrite(LED, HIGH);
  digitalWrite(LED, HIGH);
  int pulse;
  int sum = 0;
  for (int i = 0; i < 20; i++)
    sum += analogRead(sensorPin);
  pulse = sum / 20.00;
  int mapPulse= map(pulse, 0, 4095, 0,220);
  Serial.println(mapPulse);
  delay(100);
}
else
{
digitalWrite(LED,LOW);
}
}
