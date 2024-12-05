# PerilousPassage
All the code required to bring my annual Haunt to life! Located in my house's entry corridor. This year's theme is based off the Horror classic, Scream!

Enjoy this Tour of the Scream corridor!

[URL]


This consists of several ESP8266 Microcontrollers designed to manage various inputs and outputs with various communications handled through MQTT!
Each piece of code controls the following components

Porch: Control Various effects in the "Porch" portion of the haunt
    -Detection of people entering the haunt, sending MQTT triggers for Fog, House background audio
    -Dectect doorbell press, sending MQTT trigger for activation of GhostFace Mech
    -Change the color of the Haunt's pulsed entry signal (Colored Eyeball)
    -Activation of Porch background audio (Creepy woods sounds)
    -Activation of GhostFace Mech Sound trigger (Ghost Face Lines)
    -Activate audio input relay (to change Porch BG audio speaker to proper input

House: Control various effects in the "House" portion of the haunt
    -Send MQTT signal to pulsed entry signal when people have vacated the House portion of haunt
    -Activation of House background audio (Music from Scream)
    -Detect people in front of full body Ghost Face scare
    -Activation of full body Ghost Face scare audio (Ghost Face Lines)
    -Activation of full body Ghost Face RGB spotlight
    -Detection of House phone being lifted off the hook
    -Activation of House phone Ghost Face lines

GhostFaceMech: Control Mechanisms of homemade ghostface jumping animatronic
    -Activation of jumping motor relay
    -Activation of stopSensor for jumping motor to cease
    -Test button for instant operation
  
FogControl: Fog Machine Control
    -Fog Control Remote Relay On
    -Fog Control Remote Relay Off

In addition to these four microcontrollers there were also 5 WLED controllers and 1 PixelBlaze controller
for additional lighting control!
    WLED:
        -Spotlight for doorbell sign
        -RGB Porch light 
        -RGB Porch door window creepy lighting
        -RGB Porch Cloud 
        -RGB Porch Forest lighting
        -RGB House Chandelier
        -RGB House Sconces (x2)
        -RGB House window forest lighting
    PixelBlaze:
        -House curtain lightning effect (x2)
