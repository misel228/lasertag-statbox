# lasertag-statbox #

A statistics box for Lasertag (we're only at the beginning)

## Components ##

1. RaspberryPi - will have an IR-Reader to receive stats data from the markers, a web interface and database for the statistics 
2. MicroView - to emulate the marker signals (I don't have one at home so I need something I can develop against ) [https://www.sparkfun.com/products/12923](https://www.sparkfun.com/products/12923 "MicroView")

## Content ##

### microview\_led_finder ###
An iterator to find out which pin is which

### microview\_led_test ###
Generates a PWM signal

![picture alt](/images/76kHz.png "Actually we need 56kHz but that should not be such a big deal any longer :)") 