# PROJ_Led_Board_with_REST_API
Projekt - interaktywna tablica led sterowana za pomocą aplikacji z wykorzystaniem interfejsu REST API

HUB75E Pinout:

```
    /-----\
R1  | o o | G1
B1  | o o | GND
R2  | o o | G2
B2  \ o o | GND
A   / o o | B
C   | o o | D
CLK | o o | LATCH
OEn | o o | GND
    \-----/
```

Wiring:

```
R1 - GPIO8
G1 - GPIO0
B1 - GPIO1
R2 - GPIO9
G2 - GPIO2
B2 - GPIO4
A - GPIO10
B - GPIO3
C - GPIO11
D - GPIO5
CLK - GPIO12
Latch - GPIO6
OEn - GPIO7
```

This is a 1/32nd scan panel. The inputs A, B, C, D select one of 32 rows, starting at the top and working down (assuming the first pixel to be shifted is the one on the left of the screen, even though this is the "far end" of the shift register). R0, B0, G0 contain pixel data for the upper half of the screen. R1, G1, B1 contain pixel data for the lower half of the screen, which is scanned simultaneously with the upper half.
