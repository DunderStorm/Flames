# Flames
Code for producing flame effects with "neopixels" on an arduino compatible device, Requires the fastLED library.

Each element is assigned a heat value from 0 - 255 and which is then converted to a color analogous to black body radiation (it starts at whites and then gets yellow, orange, red and black as it cools down).

Sparks are created at the edges of the strip and then "rises" towards the center of the strip.

Somewhat based on the Fire2012 example from the fastLED library

Feel free to steal and use for something cool, and please share it further.
