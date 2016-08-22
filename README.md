# Alarm-Activated Automated Curtains
A system to automatically open the curtains when my cell phone alarm goes off

![] (https://github.com/jerwil/Automated_Curtains/raw/master/Media/Opening_GIF.gif)

##Features

* Uses inexpensive off-the-shelf parts to open curtain
* Seamlessly works with alarm app I already use
* Automatically retracts cord to allow curtains to be closed

##Architecture

![] (https://github.com/jerwil/Automated_Curtains/raw/master/Media/Comms%20Flowchart.png)

My alarm clock app, Sleep as Android, has the ability to interface with IFTTT. This is an awesome system that takes care of API calls for you and talks to hundreds of different services.

Sleep as Android >>> Adafruit IO

When the alarm goes off, Sleep as Android sends "alarm_alert_start" to a Maker channel feed. Using the [Sleep as Android documentation](http://sleep.urbandroid.org/documentation/integration/ifttt/), I created an [IFTTT recipe](https://ifttt.com/recipes/456351-when-my-sleep-as-android-alarm-goes-off-send-data-to-adafruit-io) that looks for this message, and writes a "1" to an Adafruit IO feed. When the alarm is disabled, "alarm_alert_dismiss" is sent, and a "0" is written to the data feed.

Adafruit IO >>> ESP8266

On the hardware side, I have an ESP8266 checking the Adafruit IO feed. I followed the basics of the [Adafruit guide](https://learn.adafruit.com/esp8266-temperature-slash-humidity-webserver/wiring) to get set up. When the feed returns a "1" the alarm is sounding, and the ESP8266 then lets the Arduino know it's time to open the curtain.

ESP8266 >>> Arduino

The connection between the ESP8266 and Arduino is very basic, just an on/off digital signal. When it's time to open the curtain, the ESP8266 writes a HIGH value on the digital line connecting the two microcontrollers.

Arduino Code

When the digital input is HIGH, the Arduino operates the servo to open the curtain, pauses, and then retracts the cord so that the curtain can be closed in the evening. The Arduino also has two buttons attached to it that allow the servo to be manually moved in either direction. These buttons also interrupt the automated open/close operation in case anything unexpected occurs.

The Arduino code also has a timeout (2 hours for now) during which no other alarm events will trigger the curtain opening. This avoids snoozing triggering the curtain multiple times in one morning.

## Wiring

![] (https://github.com/jerwil/Automated_Curtains/raw/master/Media/Breadboard%20Labeled.jpg)

See the included Fritzing diagram (.fzz file) for details. Thanks to [ydonnelly](https://github.com/ydonnelly/ESP8266_fritzing) for the ESP8266 Fritzing part!

![] (https://github.com/jerwil/Automated_Curtains/raw/master/Media/Breadboad.jpg)

## Links

[Youtube video](https://www.youtube.com/watch?v=KcBI1g3s2vk)

[Hackaday.io Project Page](https://hackaday.io/project/13099-alarm-activated-curtains)

### License

<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="http://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Automated Curtains</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="http://www.JeremyAdamWilson.com">Jeremy Wilson</a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
