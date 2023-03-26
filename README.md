# PROJ_Led_Board_with_REST_API
Projekt - interaktywna tablica led sterowana za pomocą aplikacji z wykorzystaniem interfejsu REST API

HUB75E Pinout:

```
    /-----\
R0  | o o | G0
B0  | o o | GND
R1  | o o | G1
B1  \ o o | GND
A   / o o | B
C   | o o | D
CLK | o o | LATCH
OEn | o o | GND
    \-----/
```

Wiring:

```
R1 - GPIO0
G1 - GPIO1
B1 - GPIO2
R2 - GPIO3
G2 - GPIO4
B2 - GPIO5
A - GPIO6
B - GPIO7
C - GPIO8
D - GPIO9
CLK - GPIO10
Latch - GPIO11
OEn - GPIO12
```

This is a 1/32nd scan panel. The inputs A, B, C, D select one of 32 rows, starting at the top and working down (assuming the first pixel to be shifted is the one on the left of the screen, even though this is the "far end" of the shift register). R0, B0, G0 contain pixel data for the upper half of the screen. R1, G1, B1 contain pixel data for the lower half of the screen, which is scanned simultaneously with the upper half.
