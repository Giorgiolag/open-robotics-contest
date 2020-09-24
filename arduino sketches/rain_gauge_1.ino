const int RAIN_INT = 3;
int my_total = 0;
int flag;
unsigned long current_time;
unsigned long previous_time;
int pin3_value;
void setup() {
  // put your setup code here, to run once:
  pinMode(RAIN_INT, INPUT);
  digitalWrite(RAIN_INT,LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pin3_value = digitalRead(RAIN_INT); 
  current_time = millis();
  if (digitalRead(RAIN_INT) == 0)
  {
    previous_time = current_time;
    flag=1;
     
  }
  else
  {
    if (current_time-previous_time >=350 and flag==1)
    {
      my_total+=1; 
      Serial.println(my_total);
      flag = 0;
      
    }
  }
}
