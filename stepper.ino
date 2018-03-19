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
	Construct a stepper object.
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
	}

	void init() {
		this->currentstep = 0;
		this->currentmicro = micros();
		for (unsigned int i=0;i<pincount;i++) {
			pinMode(this->pins[i], OUTPUT);
			digitalWrite(this->pins[i], LOW);
		}
	}
	/**
	take count steps.
	*/
	void step(int count) {
		int direction = 1;
		if (count < 0) {
			direction = -1;
			count = -count;
		}
		for (int i=0;i<count;i++) {
			if (micros() < this->currentmicro) {
				// after around 70min ulong overruns
				this->currentmicro = this->delta;
			}
			else {
				this->currentmicro += this->delta;
			}
			while (micros() < this->currentmicro) {
				yield();
			}
			for (unsigned int i=0;i<this->pincount;i++) {
				digitalWrite(this->pins[i], this->states[this->currentstep]&(1<<i)? HIGH: LOW);
			}
			this->currentstep = (this->currentstep + this->statecount + direction) % this->statecount;
			this->currentmicro = micros();
		}
	}
	/**
	set a new delta
	*/
	void setDelta(const unsigned int delta) {
		this->delta = delta;
	}
};
//
#define countof(s) (sizeof(s)/sizeof(*s))
#define COUNTED_PARAMETER(x) countof(x), x
// config of the stepper motor
const static unsigned long Steps360 = 4096l;
const static unsigned int Pins[] = {8, 9, 10, 11, };
const static unsigned long Delta = 1000;
const static unsigned int States[] = {
	0b0001,
	0b0011,
	0b0010,
	0b0110,
	0b0100,
	0b1100,
	0b1000,
	0b1001,
	};
const static unsigned int StatesSingle[] = {
	0b0001,
	0b0010,
	0b0100,
	0b1000,
	};
const static unsigned int StatesDouble[] = {
	0b0011,
	0b0110,
	0b1100,
	0b1001,
	};
static unsigned long c;

Stepper stepper = Stepper(Steps360, Delta, COUNTED_PARAMETER(Pins), COUNTED_PARAMETER(States));

void setup()
{
	// init debug print
	Serial.begin(38400);
	Serial.println("Serial");
	// init random
	randomSeed(analogRead(0));
	// init stepper
	stepper.init();
	// init loop
	c = millis();
	Serial.println("Setup done");
}

void loop()
{
	Serial.println("Loop");
	/*
	switch (random(2)) {
	case 0: stepper.setDelta(Delta+200); break;
	default: stepper.setDelta(Delta);
	}
	*/
	stepper.step(Steps360/10);
	delay(100);
	stepper.step(-(int)Steps360/5);
	delay(100);
	stepper.step(Steps360/10);
	c += 30000;
	while (millis() < c) {
		delay(20);
		yield();
	}
	Serial.println("Loop done");
}
