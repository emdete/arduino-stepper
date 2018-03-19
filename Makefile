#!/usr/bin/env make -f
all:
	mkdir -p cache build
	/usr/lib/arduino/arduino-builder \
		-compile \
		-hardware /usr/lib/arduino/hardware \
		-tools /usr/lib/arduino/tools-builder \
		-built-in-libraries /usr/lib/arduino/libraries \
		-libraries `pwd`/../libraries \
		-fqbn=arduino:avr:nano:cpu=atmega328 \
		-ide-version=10805 \
		-build-path `pwd`/build \
		-warnings=all \
		-build-cache `pwd`/cache \
		-prefs=build.warn_data_percentage=75 \
		`pwd`/stepper.ino

run:

dbg: all
	/usr/bin/avrdude \
		-C/etc/avrdude.conf \
		-v \
		-patmega328p \
		-carduino \
		-P/dev/ttyUSB0 \
		-b57600 \
		-D \
		-Uflash:w:`pwd`/build/stepper.ino.hex:i

