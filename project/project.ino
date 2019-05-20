

//defines value and pin number
//photosensor
int redled = D6; 
int photosensor = A0; 
int analogvalue; 
bool empty;


//motionsensor
int blueled = D4;                 // choose the pin for the LED
int PIR = D0;               // choose the PIR sensor pin
bool detected;                  // status of motion
int motionCounter = 0;           // variable to count motion events

Timer timer(30000, determineMotion); // software timer to check every 30s

void setup() {

    //photosensor
	Serial.begin(9600);
	pinMode(photosensor, INPUT);
	pinMode(redled, OUTPUT); 
	digitalWrite(redled, LOW);
	Particle.variable("analogvalue", &analogvalue, INT);
	
	//motion
	pinMode(PIR, INPUT);
	pinMode(blueled, OUTPUT);
	digitalWrite(blueled, LOW);

}

//motion
void determineMotion() {    // this function determines if there's motion
    if(motionCounter < 2) { // if very little motion was detected
        if(detected == false) { // only publish if the status changed
            Particle.publish("motion", "Motion-detected",PRIVATE); 
            }
        detected = true; // set the status to available
    } else if (motionCounter >= 2) {
        if(detected == true) { // only publish if the status changed
            Particle.publish("motion", "No-motion",PRIVATE); 
            }
        detected = false; // set the status to in use
    }
    motionCounter = 0; // reset motion counter
}



void loop() {
    
    if(digitalRead(PIR) == HIGH) {  // check if the input is HIGH
	    Particle.publish("motion",String(motionCounter),60,PRIVATE);
        digitalWrite(blueled, HIGH);         // turn LED ON if high
        motionCounter++;                    // increment motion counter
  } else{
      digitalWrite(blueled, LOW);          // turn LED OFF if no input
  }
    
   	// check to see what the value of the photoresistor or phototransistor is and store it in the int variable analogvalue
	analogvalue = (analogRead(photosensor));

	// This prints the value to the USB debugging serial port (for optional debugging purposes)
	Serial.printlnf("%d", analogvalue);
	
	if(analogvalue < 55){//set the reading start
	    //for(int i=0; i<800; i++){//set the maximum reading; as the max light at room around 150-250, then I set it higher for the sunlight.
	        analogWrite(redled,analogvalue);
	        digitalWrite(redled,LOW);
	    //}
	}
	else{
	    analogWrite(redled,analogvalue);
	    digitalWrite(redled,HIGH);
	}

	Particle.publish("analogvalue", String(analogvalue),PRIVATE);//send data to the thinkspeak to analysis the data.
	
	// This delay is just to prevent overflowing the serial buffer, plus we really don't need to read the sensor more than
	// 10 times per second (100 millisecond delay)
	delay(500);
	

}

