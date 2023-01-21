# PROJ_Led_Board_with_REST_API
Projekt - interaktywna tablica led sterowana za pomocą aplikacji z wykorzystaniem interfejsu REST API

### Przydatna dokumentacja
- [Getting started with pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- [Connection to the internet with pico w](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)
- [Raspberry Pi Pico C/C++ SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)

### Budowanie projektu - instrukcja

Robione na Kali Linuxie, powinno działać na każdej dystrybucji opartej na Debianie.

Trzeba utworzyć folder, sklonować repo *pico-sdk* (oraz opcjonalnie *pico-examples* z przykładami) i te repo aby były obok. Zainstalować biblioteki i toolchain.

Przygotowanie

```
mkdir pico
cd pico
git clone https://github.com/raspberrypi/pico-sdk.git --branch master
cd pico-sdk
git submodule update --init
cd ..
git clone https://github.com/raspberrypi/pico-examples.git --branch master
git clone https://github.com/MrMroz/PROJ_Led_Board_with_REST_API.git
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib libusb-1.0-0-dev pkg-config
```

Setup do budowania (działa też dla *pico-examples*). Jeżeli nie trzeba łączności WIFI, to flagi *-DWIFI* są zbędne.
```
cd PROJ_Led_Board_with_REST_API
mkdir build
cd build
export PICO_SDK_PATH=../../pico-sdk
cmake -DPICO_BOARD=pico_w -DWIFI_SSID="nazwa -DWIFI_PASSWORD="haslo" ..
```

Jak przebiegło pomyślnie, to budowanie przebiega tak
```
cd build
make
```
W katalogu build powinny pojawić się podfoldery ze skompilowanym kodem modułów.
Należy pamiętać o dopiswaniu odpowiednich poleceń w CMakeLists przy tworzeniu katalogów.
