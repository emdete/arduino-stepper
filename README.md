Arduino Stepper
===============

Stepper code for arduino.

I was unable to use the original stepper lib from
[Stepper](http://www.arduino.cc/en/Reference/Stepper) which just did not work
for me. The flow seemed to be wrong for my wirering so i wrote something myself
from scratch.

Finally i put it into a class which now is able to drive steppers in a generic
manner. Even the flow is declared at construction.

This code is more generic than the official code as you can have as many pins
as you like and any kind of flow to control these pins.

you define the pins like:

```
	const static unsigned int Pins[] = {8, 9, 10, 11, };
```

which you can given to the constructor with a given macro:

```
	COUNTED_PARAMETER(Pins)
```

(to make sure the object knows how many pins you defined) and the flow can be
defined by something like:

```
	const static unsigned int States[] = {
		0b0001,
		0b0010,
		0b0100,
		0b1000,
		};
```

(nicely readable as gnu c++ supports binary numbers) with the same macro
`COUNTED_PARAMETER` to ensure the right size. Maximum for pins is 5 and states
is 10 (which can easlily be changes - you've got the source).

