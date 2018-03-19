//
class Stepper {
private:
	unsigned int steps360; // how many steps do one 360° round
	unsigned int delta; // how many microsec to wait before the next step is taken
	unsigned int pincount; // how many pins make up this steppermotor
	unsigned int pins[5]; // the actual pins for this steppermotor
	unsigned int statecount; // number of states to go through
	unsigned int states[10]; // the flow of states
	unsigned int currentstep; // current stop [0..statecount[)
	unsigned long currentmicro; // current microsec to calc when next step can take place
public:
	/**
	* Construct a stepper object.
		steps360: number of steps to do a full round
		delta: number of microseconds between steps
		pincount: number of pins
		pins[]: list of pins
		statecount: number of states
		states[]: list of states
	*/
	Stepper(const unsigned int steps360, const unsigned int delta, const unsigned int pincount, const unsigned int* pins, const unsigned int statecount, const unsigned int* states) {
		this->steps360 = steps360;
		this->delta = delta;
		this->pincount = pincount;
		for (unsigned int i=0;i<pincount;i++)
			this->pins[i] = pins[i];
		this->statecount = statecount;
		for (unsigned int i=0;i<statecount;i++)
			this->states[i] = states[i];
		this->currentstep = 0;
		this->currentmicro = micros();
	}
	/**
	* take count steps
	*/
	void step(int count) {
		int Direction = 1;
		if (count < 0) {
			Direction = -1;
			count = -count;
		}
		for (int i=0;i<count;i++) {
			this->currentmicro += this->delta;
			while (micros() < this->currentmicro) {
				yield();
			}
			for (unsigned int i=0;i<this->pincount;i++) {
				digitalWrite(this->pins[i], this->states[this->currentstep]&(1<<i)? HIGH: LOW);
			}
			this->currentstep = (this->currentstep + this->statecount + Direction) % this->statecount;
			this->currentmicro = micros();
		}
	}
};
//
#define countof(s) (sizeof(s)/sizeof(*s))
// config of the stepper motor
const static unsigned long Steps360 = 4096l;
const static unsigned int Pins[] = {8, 9, 10, 11, };
const static unsigned long Delta = 1000;
const static unsigned int States[] = {1, 3, 2, 6, 4, 12, 8, 9, };
//const static int States[] = {1, 2, 4, 8, };
//const static int States[] = {3, 6, 12, 9, };
static unsigned long c;

Stepper stepper = Stepper(Steps360, Delta, countof(Pins), Pins, countof(States), States);

void setup()
{
	// init debug print
	Serial.begin(38400);
	Serial.println("Serial");
	// init random
	randomSeed(analogRead(0));
	// init stepper
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
	stepper.step(Steps360/24); //(Steps360 * d * a) / 360);
	delay(200);
	stepper.step(-(int)Steps360/12); //(Steps360 * d * a) / 360);
	delay(200);
	stepper.step(Steps360/24); //(Steps360 * d * a) / 360);
	//Serial.print(d * a);
	//Serial.print(' ');
	//Serial.println("Wait");
	// exit
	c += 3000;
	while (millis() < c) {
		delay(20);
		yield();
	}
	Serial.println("Loop done");
}
