# Smart Robot Car with Arduino 

Basically, this project is based on the construction of an intelligent car through the combination of the various components that enable basic operations that make it possible: going forward or backward with the car and turning left or right. This can all be managed through communication with the Bluetooth module to a device connected with an app for Android. Two modes have been devised:
 - the first mode allows a user to connect to the Bluetooth module with a dedicated app to be able to control the movements of the car 
 - the second mode is such that the car is able to follow a black line and stop the moment it encounters an obstacle. This is all through the use of specific sensors. 
The common aspect is the presence of a gyroscope/accelerometer that can provide data to the wifi module, which in turn not only displays it on the screen graphically but also sends it to Blynk cloud.

## Getting Started

Basically it's possible to download files with the *.ino extension and try them within the IDE. 

### First mode

![diagram](https://github.com/LamFra/Smart-Robot-Car-Arduino/blob/main/img/firstmode.png?raw=true)

### Second mode

![diagram](https://github.com/LamFra/Smart-Robot-Car-Arduino/blob/main/img/secondmode.png?raw=true)

### Common aspect

![diagram](https://github.com/LamFra/Smart-Robot-Car-Arduino/blob/main/img/commonaspect.png?raw=true)

## Built With

  - [Arduino IDE](https://www.arduino.cc/en/software)
  - Robot Car kit
  - SMRAZA compatible with Arduino Uno 
  - Bluetooth Module HC05
  - Motor Driver Controller L298N
  - [Bluetooth Controller App](https://play.google.com/store/apps/details?id=com.giumig.apps.bluetoothserialmonitor)
  - IR Proximity Sensor FC51
  - Two Infrared Sensors TCRT5000
  - Gyro Accelerometer GY-521
  - WiFi Kit 8 ESP8266
  - Blynk cloud

## Authors

  - **Francesca La Manna** - f.lamanna5@studenti.unisa.it
  - **Gioacchino Caliendo** - g.caliendo16@studenti.unisa.it
