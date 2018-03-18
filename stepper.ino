#define STEPS_360 4096l
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

int Steps = 0;
unsigned long CurrentMicros = 0;
unsigned long c;

void setup()
{
	Serial.begin(38400);
	Serial.println("Starting");
	randomSeed(analogRead(0));
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, LOW);
	Serial.println("Setup");
	c = millis();
}

void loop()
{
	Serial.println("Loop");
	//int d = random(2)? 1: -1;
	//int a = (random(5) + 1) * 60;
	stepper(STEPS_360 / 60); //(STEPS_360 * d * a) / 360);
	//delay(200);
	//Serial.print(d * a);
	//Serial.print(' ');
	//Serial.print(CurrentMicros / 1000.);
	//Serial.println("Wait");
	// exit
	c += 1000;
	while (millis() < c) {
		delay(20);
		yield();
	}
	Serial.println("Loop done");
}

void stepper(int count) {
	int Direction = 1;
	int Delta = 1000;
	if (count < 0) {
		Direction = -1;
		count = -count;
	}
	for (int i=0;i<count;i++) {
		if (CurrentMicros > 0) {
			while (micros() < CurrentMicros + Delta) {
				yield();
			}
		}
		CurrentMicros = micros();
		switch(Steps) {
		case 0:
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
		break;
		case 1:
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, HIGH);
			digitalWrite(IN4, HIGH);
		break;
		case 2:
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, HIGH);
			digitalWrite(IN4, LOW);
		break;
		case 3:
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, HIGH);
			digitalWrite(IN4, LOW);
		break;
		case 4:
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
		break;
		case 5:
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, HIGH);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
		break;
		case 6:
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
		break;
		case 7:
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, HIGH);
		break;
		default:
			Serial.print(Steps);
			Serial.println("Error!");
		break;
		}
		Steps = (Steps + 8 + Direction) % 8;
	}
}
