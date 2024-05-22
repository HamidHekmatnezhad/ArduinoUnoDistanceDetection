#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
const unsigned int TRIG_PIN=10, ECHO_PIN=9, RELAY_PIN=8, THRESHOLD_DISTANCE=100;
const unsigned int max_distance=250, DELAY_TIME=50; //100cm
const unsigned long duration;
int distance, i, bar_graph;
long timer_start=0, timer_end=0, timer_calculated=0, THRESHOLD_TIME=30; //30sec
bool timer_check=false;
bool bar_graph_switch=1;// bar graph for distance =1 ; bar graph for time =0

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  lcd.begin(16,2);
}

void loop()  {
  // Calculate distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN,  HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH); 
  distance = (duration * 0.0343) / 2;

  timer_calculated = (timer_start - timer_end) * 1000;

  // check if the distance is less than the threshold distance and the timer is less than the threshold time
  // if yes, turn on the relay
  if((distance <= THRESHOLD_DISTANCE) && (timer_calculated <= THRESHOLD_TIME)){
    digitalWrite(RELAY_PIN, HIGH);
    timer_start = millis();
    timer_check = true;
  
  }
  // if not, turn off the relay
  else{
    digitalWrite(RELAY_PIN, LOW);
    timer_end = millis();
    timer_check = false;
  }

  // print on LCD 
  if(timer_check == true){
    timer_calculated = abs((timer_start - timer_end) * 1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(distance);
    lcd.print("cm ");

    lcd.print(timer_calculated);
    lcd.print("s");

    // bar graph
    if(bar_graph_switch){bar_graph = map(distance, 2, max_distance, 0, 15);} // for distance
    else{bar_graph = map(timer_calculated, 0, THRESHOLD_TIME, 0, 15);} // for timer 
    lcd.setCursor(0,1);
    for(i=0; i<bar_graph; i++){
      lcd.write(255);}
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(distance);
    lcd.print("cm ");
    lcd.print("0s");
  }
  delay(DELAY_TIME);
}