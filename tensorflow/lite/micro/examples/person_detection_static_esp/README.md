# Person detection example

This example shows how you can use Tensorflow Lite to run a 250 kilobyte neural
network to recognize people in images captured by a camera.  It is designed to
run on systems with small amounts of memory such as microcontrollers and DSPs.

## Running on ESP32

 The following instructions will help you build and deploy this sample
 to [ESP32](https://www.espressif.com/en/products/hardware/esp32/overview)
 devices using the [ESP IDF](https://github.com/espressif/esp-idf).

 The sample has been tested on ESP-IDF version 4.0 with the following devices:
 - [ESP32-DevKitC](http://esp-idf.readthedocs.io/en/latest/get-started/get-started-devkitc.html)
 - [ESP-EYE](https://github.com/espressif/esp-who/blob/master/docs/en/get-started/ESP-EYE_Getting_Started_Guide.md)

 ESP-EYE is a board which has a built-in camera which can be used to run this example , if you want to use other esp boards you will have to connect camera externally and write your own +image_provider.cc+ and +esp_app_camera.cc+.
 ### Install the ESP IDF

 Follow the instructions of the
 [ESP-IDF get started guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html)
 to setup the toolchain and the ESP-IDF itself.

 The next steps assume that the
 [IDF environment variables are set](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#step-4-set-up-the-environment-variables) :

  * The `IDF_PATH` environment variable is set
  * `idf.py` and Xtensa-esp32 tools (e.g. `xtensa-esp32-elf-gcc`) are in `$PATH`
  * `esp32-camera` should be downloaded in `comopnents/` dir of example as explained in `Building the example`(below)

 ### Generate the examples
 The example project can be generated with the following command:
 ```
 make -f tensorflow/lite/micro/tools/make/Makefile TARGET=esp generate_person_detection_esp_project
 ```

 ### Building the example

 Go the the example project directory
 ```
 cd tensorflow/lite/micro/tools/make/gen/esp_xtensa-esp32/prj/person_detection/esp-idf
 ```

 As the `person_detection` example requires an external component `esp32-camera` for functioning hence we will have to manually clone it in `components/` directory of the example with following command.
 ```
 git clone https://github.com/espressif/esp32-camera.git components/esp32-camera
 ```

 Then build with `idf.py`
 ```
 idf.py build
 ```

 ### Load and run the example



 To flash (replace `/dev/ttyUSB0` with the device serial port):
 ```
 idf.py --port /dev/ttyUSB0 flash
 ```

 Monitor the serial output:
 ```
 idf.py --port /dev/ttyUSB0 monitor
 ```

 Use `Ctrl+]` to exit.

 The previous two commands can be combined:
 ```
 idf.py --port /dev/ttyUSB0 flash monitor
 ```
## Training your own model

You can train your own model with some easy-to-use scripts. See
[training_a_model.md](training_a_model.md) for instructions.
