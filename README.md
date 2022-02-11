# Update of KROIA/QT-Serial-Communication-example
***
##  Modified to include serial port attributes in drop-downs and a QTextEdit box to show the I/O on the selected port.

<img src="https://github.com/lislej/QT-Serial-Communication-example/blob/develop/images/SerialCommViewer.png" alt="snapshot" style="height: 500px; width:400px;"/>


# Installation
***
## QT-Software
1. clone the git repository.
2. Open the project in Qt Creator.
3. Compile with toolkit of choice.
4. This example uses qDebug() outputs to the console and x, y, and z coordinates to the QEditText box.

***
## Device-Software
* An Arduino Nano v3 has been added to this example which is wired to a BNO055 sensor from Adafruit offering 9DOF.
A demo sketch from Adafruit has been added to the **Device-code** folder called sketch_feb04a.ino.
***

## Building
# Qt Code 
*  Compile in Qt Creator 
*  For more Information click [here](https://wiki.qt.io/Build_Standalone_Qt_Application_for_Windows).
# Arduino Code
* Compile in Arduino IDE
* Load compiled code to Arduino Nano
***

## Arduino Nano with BNO055

   <img src="https://github.com/lislej/QT-Serial-Communication-example/blob/develop/images/arduino-nano-with-BNO055.PNG" alt="snapshot" style="height: 900px; width:400px;"/>
