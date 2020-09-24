const int RAIN_INT = 3;
int my_total = 0;
int pin3_current_value;
int pin3_previous_value = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(RAIN_INT, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pin3_current_value = digitalRead(RAIN_INT); 
  if (pin3_current_value == 1 and pin3_previous_value == 0)
  {
    my_total+=1;
     Serial.println(my_total);
  }
  pin3_previous_value = pin3_current_value;
}
