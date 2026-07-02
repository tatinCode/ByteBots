# Colon
an ai robot car to have as a pet.

## Dependencies

### Raspberry Pi

Tested on Raspberry Pi OS / Debian 12 Bookworm.

Install build tools and camera/AI dependencies:

```sh
sudo apt update
sudo apt install build-essential pkg-config
sudo apt install libcamera-dev libopencv-dev
sudo apt install v4l-utils
Optional camera test tools, usually already present on Raspberry Pi OS:
sudo apt install rpicam-apps
ESP32 / Arduino
Install arduino-cli, then add the ESP32 board index:
arduino-cli config init
arduino-cli config add board_manager.additional_urls https://espressif.github.io/arduino-esp32/package_esp32_index.json
arduino-cli core update-index
arduino-cli core install esp32:esp32
If arduino-cli was installed into the project directory:
export PATH="$PWD/bin:$PATH"
Future Memory / Fleet Sync
Planned dependencies:
sudo apt install sqlite3 libsqlite3-dev
sudo apt install protobuf-compiler libprotobuf-dev
SQLite will be used for local robot memory/upload queues. Protobuf will be used for compact robot-to-backend messages.

I’d also add a quick verification section:

```md
## Dependency Checks

```sh
pkg-config --modversion opencv4
pkg-config --modversion libcamera
rpicam-hello --list-cameras
arduino-cli version

And a note:

```md
## Runtime Notes

The Pi controller currently uses `/dev/mem` for GPIO access, so it must be run with root permissions:

```sh
sudo ./pi/bin/pi-controller
