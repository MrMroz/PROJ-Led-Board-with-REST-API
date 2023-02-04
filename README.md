# PROJ-Led-Board-with-REST-API
Projekt - interaktywna tablica led sterowana za pomocą aplikacji z wykorzystaniem interfejsu REST API

## Spis treści

- [PROJ-Led-Board-with-REST-API](#proj-led-board-with-rest-api)
  - [Spis treści](#spis-treści)
  - [Przydatna dokumentacja](#przydatna-dokumentacja)
  - [Budowanie projektu - instrukcja](#budowanie-projektu---instrukcja)
  - [Pokazowe programy](#pokazowe-programy)


## Przydatna dokumentacja
- [Getting started with pico](https://datasheets.raspberrypi.com/pico/-getting-started-with-pico.pdf)
- [Connection to the internet with pico w](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)
- [Raspberry Pi Pico C/C++ SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)

## Budowanie projektu - instrukcja

Robione na **Kali Linux**, powinno działać na każdej dystrybucji opartej na **Debian**.
Utworzyć folder, sklonować **pico-sdk** (oraz opcjonalnie **pico-examples** z przykładami) i te repo aby były obok. Zainstalować biblioteki i toolchain.

Przygotowanie
```
mkdir pico
cd pico
git clone https://github.com/raspberrypi/pico-sdk.git --branch master
cd pico-sdk
git submodule update --init
cd ..
git clone https://github.com/raspberrypi/pico-examples.git --branch master
git clone https://github.com/MrMroz/PROJ-Led-Board-with-REST-API.git
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib libusb-1.0-0-dev pkg-config
```

Setup do budowania (działa też dla **pico-examples**). Jeżeli nie trzeba łączności WIFI, to flagi -DWIFI są zbędne.
```
cd PROJ\_Led\_Board\_with\_REST\_API
git submodule update --init
mkdir build
cd build
export PICO_SDK_PATH=../../pico-sdk
cmake -DPICO_BOARD=pico_w -DWIFI_SSID="nazwa" -DWIFI_PASSWORD="haslo" ..
```

Jak przebiegło pomyślnie, to budowanie przebiega w poniższy sposób
```
cd build
make -j
```
W katalogu build powinny pojawić się podfoldery ze skompilowanym kodem modułów.
Należy pamiętać o dopiswaniu odpowiednich poleceń w CMakeLists przy tworzeniu katalogów.


## Pokazowe programy

Aktualnie napisanych jest pare pokazowych programów w celach naukowych:
- **blink** - mruga wbudowanym LED'em i nic więcej
- **print-blink** - mruga LED'em i wypisuje wiadomości poprzez *printf*, do testowania czy działa odbieranie output'u z pico
- **3led-blink** - należy podpiąć 3 zewn. LED'y pod **GPIO 0,10,20** (druga nóżka do **GND**), program mruga po kolei nimi
- **9led-anim** - podpiąć 9 LED'ów pod **GPIO 0-8** (najlepiej ustawić je w szeregu jeden za drugim) oraz dwa kable do **GPIO 9,10** z możliwością podania stanu wysokiego na nie. Program robi animację fali, podawanie stanu wysokiego na **GPIO 9/10** odwraca/przyspiesza animację
- **http-led-test** - proste REST API w oparciu o bibliotekę **picow-http** z dwoma endpoint'ami **GET /led/** i **PUT /led/**, zwracającymi stan LED'a na **GPIO 0** i sterującymi nimi.