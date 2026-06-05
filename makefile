ESP32_SKETCH = esp32/motor_control
ESP32_FQBN = esp32:esp32:esp32
ESP32_PORT = /dev/ttyUSB0
ESP32_BUILD_DIR = build/esp32

.PHONY: esp32-compile esp32-upload esp32-monitor pi-build pi-run clean

esp32-compile:
	arduino-cli compile --fqbn $(ESP32_FQBN) \
		--build-path $(ESP32_BUILD_DIR) \
		$(ESP32_SKETCH)

esp32-upload:
	arduino-cli upload -p $(ESP32_PORT) --fqbn $(ESP32_FQBN) \
		--input-dir $(ESP32_BUILD_DIR) \
		$(ESP32_SKETCH)

esp32-monitor:
	arduino-cli monitor -p $(ESP32_PORT) -c baudrate=115200

pi-build:
	$(MAKE) -C pi

pi-run: 
	$(MAKE) -C pi run

clean:
	rm -rf $(ESP32_BUILD_DIR)
	$(MAKE) -C pi clean

