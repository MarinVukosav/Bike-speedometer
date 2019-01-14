# Bike-speedometer
A speedometer for your bike using arduino

test2, and test4 directories have the code for the radio transmission between two Arduinos, inludicng the calculation of the speed using
a magnet that obtains the Hall effect.

vjezba1.py is a script for a linux device (in my case a raspberry pi), that reads the data (speed) and sends it using a mqtt protocol to 
a cloud server where you can read the exact speed.
