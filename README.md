Robot Mower
-----------


This repo contains code for a simple remote controlled robot mower. The remote control code can be found in
aghoward/rc-remote-control repository. Both projects are based on an arduino NANO with a HC05 Bluetooth module.


Hardware
--------

This repository uses a repurposed Husqvarna Automower for its shell and motors. The motors included in that unit are
brushless DC motors, one for each wheel and one for the blade. Each motor is driven by an independent ESC. The wheel
motors use bi-directional ESCs, and the blade motor uses a hefty ESC suitable for a 5S lipo battery. The battery is a
5000mAh LiPo connected through a switch to each ESC, then the ESCs are used to power the bluetooth radio and nano
seperately.


Software
--------

This repo mostly uses the standard arduino library, with the exception of the arduino-timer library. Arduino-timer is
used as a safety kill-switch. Since we are using radios here, if the receiver fails to register any new packets within
500 milliseconds it will automatically shutdown all the motors until it starts receiving messages again. This is a
safety feature to prevent a runaway spinning razor blade.


Building
--------

`NANO=true make && NANO=true make up` to build and upload the code to the mower.
