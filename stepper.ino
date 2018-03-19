//
#define countof(s) (sizeof(s)/sizeof(*s))
// config of the stepper motor
const static long Steps360 = 4096l;
const static unsigned int Pins[] = {8, 9, 10, 11, };
const static unsigned long Delta = 1000;
const static int States[] = {1, 3, 2, 6, 4, 12, 8, 9, };
//const static int States[] = {1, 2, 4, 8, };
//const static int States[] = {3, 6, 12, 9, };
// state of the stepper
static int Step = 0;
static unsigned long CurrentMicros = 0;
// state of the loop
static unsigned long c;

static void step(int count) {
	int Direction = 1;
	if (count < 0) {
		Direction = -1;
		count = -count;
	}
	for (int i=0;i<count;i++) {
		CurrentMicros += Delta;
		while (micros() < CurrentMicros) {
			yield();
		}
		for (unsigned int i=0;i<countof(Pins);i++) {
			digitalWrite(Pins[i], States[Step]&(1<<i)? HIGH: LOW);
		}
		Step = (Step + countof(States) + Direction) % countof(States);
		CurrentMicros = micros();
	}
}

void setup()
{
	// init debug print
	Serial.begin(38400);
	Serial.println("Serial");
	// init random
	randomSeed(analogRead(0));
	// init stepper
	CurrentMicros = micros();
	for (unsigned int i=0;i<countof(Pins);i++) {
		pinMode(Pins[i], OUTPUT);
		digitalWrite(Pins[i], LOW);
	}
	// init loop
	c = millis();
	Serial.println("Setup done");
}

void loop()
{
	Serial.println("Loop");
	//int d = random(2)? 1: -1;
	//int a = (random(5) + 1) * 60;
	step(Steps360/24); //(Steps360 * d * a) / 360);
	delay(200);
	step(-Steps360/12); //(Steps360 * d * a) / 360);
	delay(200);
	step(Steps360/24); //(Steps360 * d * a) / 360);
	//Serial.print(d * a);
	//Serial.print(' ');
	//Serial.print(CurrentMicros / 1000.);
	//Serial.println("Wait");
	// exit
	c += 30000;
	while (millis() < c) {
		delay(20);
		yield();
	}
	Serial.println("Loop done");
}
