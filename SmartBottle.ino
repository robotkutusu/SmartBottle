#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED
#define bottleLength 15 //Range of the bottle
#define warningTime 10000 // Waiting Time as milliseconds

long duration, distance, newDistance; // Duration used to calculate distance
long idleTime = 0; //Time idle
long lastDrinkTime = 0; //Time last drinked

long threshold = bottleLength / 10; // Error margin as cm

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  calculateNewDistance();
  compareDistance();
  Serial.println(idleTime);

  delay(200);
}

void calculateNewDistance() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  //Distance in cm
  newDistance = duration/58.2;
  Serial.println(newDistance);
}

void compareDistance() {   
  if (newDistance >= bottleLength || newDistance <= 0){
    Serial.println("ERROR"); 
  }
  else {
    long difference = abs(newDistance - distance);
    
    if (difference > threshold) { // Water is drank
      lastDrinkTime = millis();
      idleTime = 0;
    } else {
      idleTime = millis() - lastDrinkTime;
      alert();
    }
    distance = newDistance;
  }
}

void alert() {
  if (idleTime > warningTime) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13,LOW);
  }
}

