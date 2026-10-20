# CLAUDE.md

SHT30-Wemos-Shield: a small example that reads temperature and humidity from an SHT30/SHT31 sensor on a Wemos (LOLIN) D1 mini shield using the Adafruit SHT31 library, and serves the readings on a web page. Public repo; the README is a one-line description.

## Layout
- `platformio.ini` - env `d1_mini` (ESP8266, Arduino) with `adafruit/Adafruit SHT31 Library@^2.2.2`.
- `src/SHT31.ino` - the whole sketch (about 100 lines): connects to WiFi, advertises over mDNS, serves a page at `/` (auto-refreshes every 60 s, shows temperature, humidity and a battery voltage read from `A0`), and accepts a POST to `/calibrate` that sets an in-memory temperature offset (lost on reboot). The sensor is started at I2C address `0x45`.
- `include/`, `lib/`, `test/` hold only PlatformIO placeholder READMEs. `LICENSE` is GNU GPL v3.

## Build / flash
- `pio run -t upload`, then `pio device monitor`. I found no tests and no CI.

## Gotchas
- WiFi SSID and password are placeholder constants at the top of the sketch; fill in locally and never commit real values (public repo).
- The battery voltage conversion assumes a 4.2 V full-scale on `A0` (`4.2 / 1023.0`); that depends on the board's divider, so check it before trusting the number.
- The sketch comments say to swap to `<WiFi.h>` / `<WebServer.h>` for ESP32; only the ESP8266 build is configured here.
- The temperature in the web page and calibration message is in Fahrenheit.

## Deploy
- Push to `main`. There are no tags, releases or workflows.
