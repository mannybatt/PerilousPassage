# PerilousPassage
All the code required to bring my annual Haunt to life! Located in my house's entry corridor. This year's theme is based off the Horror classic, Scream!<br>

Enjoy this Tour of the Scream corridor!<br>

[URL]<br>


This consists of several ESP8266 Microcontrollers designed to manage various inputs and outputs with various communications handled through MQTT!
Each piece of code controls the following components<br>

<h3>Porch</h3>: Control Various effects in the "Porch" portion of the haunt<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Detection of people entering the haunt, sending MQTT triggers for Fog, House background audio<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Dectect doorbell press, sending MQTT trigger for activation of GhostFace Mech<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Change the color of the Haunt's pulsed entry signal (Colored Eyeball)<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of Porch background audio (Creepy woods sounds)<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of GhostFace Mech Sound trigger (Ghost Face Lines)<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activate audio input relay (to change Porch BG audio speaker to proper input<br>

##House: Control various effects in the "House" portion of the haunt<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Send MQTT signal to pulsed entry signal when people have vacated the House portion of haunt<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of House background audio (Music from Scream)<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Detect people in front of full body Ghost Face scare<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of full body Ghost Face scare audio (Ghost Face Lines)<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of full body Ghost Face RGB spotlight<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Detection of House phone being lifted off the hook<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Activation of House phone Ghost Face lines<br>

##GhostFaceMech: Control Mechanisms of homemade ghostface jumping animatronic<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of jumping motor relay<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of stopSensor for jumping motor to cease<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Test button for instant operation<br>
  
##FogControl: Fog Machine Control<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Fog Control Remote Relay On<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Fog Control Remote Relay Off<br>

In addition to these four microcontrollers there were also 5 WLED controllers and 1 PixelBlaze controller
for additional lighting control!<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;##WLED:<br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Spotlight for doorbell sign<br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB Porch light <br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB Porch door window creepy lighting<br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB Porch Cloud <br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB Porch Forest lighting<br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB House Chandelier<br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB House Sconces (x2)<br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB House window forest lighting<br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;##PixelBlaze:<br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-House curtain lightning effect (x2)<br>
