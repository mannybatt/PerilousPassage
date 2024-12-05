# PerilousPassage
All the code required to bring my annual Haunt to life! Located in my house's entry corridor. This year's theme is based off the Horror classic, Scream!

Enjoy this Tour of the Scream corridor!

[URL]


This consists of several ESP8266 Microcontrollers designed to manage various inputs and outputs with various communications handled through MQTT!
Each piece of code controls the following components

Porch: Control Various effects in the "Porch" portion of the haunt
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Detection of people entering the haunt, sending MQTT triggers for Fog, House background audio
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Dectect doorbell press, sending MQTT trigger for activation of GhostFace Mech
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Change the color of the Haunt's pulsed entry signal (Colored Eyeball)
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of Porch background audio (Creepy woods sounds)
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of GhostFace Mech Sound trigger (Ghost Face Lines)
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activate audio input relay (to change Porch BG audio speaker to proper input

House: Control various effects in the "House" portion of the haunt
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Send MQTT signal to pulsed entry signal when people have vacated the House portion of haunt
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of House background audio (Music from Scream)
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Detect people in front of full body Ghost Face scare
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of full body Ghost Face scare audio (Ghost Face Lines)
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of full body Ghost Face RGB spotlight
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Detection of House phone being lifted off the hook
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Activation of House phone Ghost Face lines

GhostFaceMech: Control Mechanisms of homemade ghostface jumping animatronic
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of jumping motor relay
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Activation of stopSensor for jumping motor to cease
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Test button for instant operation
  
FogControl: Fog Machine Control
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Fog Control Remote Relay On
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Fog Control Remote Relay Off

In addition to these four microcontrollers there were also 5 WLED controllers and 1 PixelBlaze controller
for additional lighting control!
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;WLED:
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-Spotlight for doorbell sign
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB Porch light 
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB Porch door window creepy lighting
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB Porch Cloud 
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB Porch Forest lighting
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB House Chandelier
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB House Sconces (x2)
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-RGB House window forest lighting
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PixelBlaze:
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-House curtain lightning effect (x2)
