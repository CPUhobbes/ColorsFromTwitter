Post a color (only colors in the rainbow) to a Twitter account and the arduino will turn a LED that color. In addition the LCD attached to the arduino that will display the name of the color.

The project requires the use of an external webserver to host the .php files. The .php files that are used are the OAuth framework, a PHP Twitter authorization library (created by abraham@abrah.am, https://github.com/abraham/twitteroauth), and PHP/arduino interface file.

The arduino uses 9 output pins (6 for the LCD and 3 for the LED) and uses the ethernet shield.