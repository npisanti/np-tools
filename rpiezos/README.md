`rpiezos`
==============

This is a system composed by a shield to mount on the rPi, a server application running on the rPi to elaborate signals and a client side calibration utility.

You need oF 0.10, and on the rPi you also need [ofxGPIO](https://github.com/kashimAstro/ofxGPIO).

The `rpiezos` app has to be running on the raspberry pi. The configuration is made by editing a JSON file you have to supply by command line.

```
rpiezos path/to/your.json
```
If the file is not found it will be created with default value. Edit the file settings to set destination IP and port, the OSC prefix to the sent messages, the sleep values between sensor reads and "autoreload" which updates the parameters if the settings file is saved.
    
Each piezos has some parameters to change its settings:
- `active` : enables or disables OSC messages for this sensor
- `envelope_low` : low value for the envelope signal 
- `envelope_high` : high value for the envelope signal 
- `trigger_low` : triggers are not sent until the value goes below this value again
- `trigger_high` : when the value is greater than this a trigger is sent
    
   
For more info on how to build the hardware, go into the [hardware](https://github.com/npisanti/ofxClayblocks/tree/master/cb_piezos/hardware) folder.

perfboard prototype:
<p align="center">
  <img src="https://raw.githubusercontent.com/npisanti/ofxClayblocks/master/cb_piezos/prototype.jpg" width="700">
</p>
