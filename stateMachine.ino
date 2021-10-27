#define IDLE_STATE 	 0
#define BLINK_STATE  1
#define TIMING_STATE 2
#define BUZZER		 3
#define BUZZING		 4

#define BLINK_TIME 500

#define BUZZER_PIN 8
#define BUZZER_NOTE 440
#define NOTE_DURATION 10000

int gsm = IDLE_STATE;
int old_gsm = IDLE_STATE;
long time = 0;
String cmd;
int note_changer;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop()
{
  if(Serial.available()){
    cmd = Serial.readString();
    
    if (cmd.equalsIgnoreCase("blink"))
      gsm = BLINK_STATE;
    else if (cmd.equalsIgnoreCase("timing"))
      gsm = TIMING_STATE;
    else if (cmd.equalsIgnoreCase("off"))
      gsm = IDLE_STATE;
    else if (cmd.equalsIgnoreCase("buzzer"))
      gsm = BUZZER;
  }
  
  //STATE MACHINE
  if (gsm!=old_gsm)
  {
    digitalWrite(LED_BUILTIN, LOW);
    old_gsm = gsm;
    Serial.println("STATE = "+ String(gsm));
  }
  switch(gsm){
  	case IDLE_STATE:
    	digitalWrite(LED_BUILTIN, LOW);
    	break;
    case BLINK_STATE:
    	if (millis()-time>BLINK_TIME){
          digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
          time = millis();
        }
    	break;
    case TIMING_STATE:
	    if (!digitalRead(LED_BUILTIN)){
        	digitalWrite(LED_BUILTIN,HIGH);
          	time = millis();
        }
    	else{
          if (millis()-time > 10000)
          {
            digitalWrite(LED_BUILTIN,LOW);
            gsm = IDLE_STATE;
          }
    	}
    	break;
    case BUZZER:
    	tone(BUZZER_PIN, BUZZER_NOTE+note_changer);
	    time = millis();
    	gsm = BUZZING;
	    break;
    case BUZZING:
	    if (millis()-time> NOTE_DURATION) {
        	noTone(BUZZER_PIN);
            gsm = IDLE_STATE;
        }
    	else
          tone(BUZZER_PIN, BUZZER_NOTE+note_changer);
    	break;
  }
  
  
  note_changer = analogRead(0);
  note_changer = map(note_changer,0,1023,0,100);
}
