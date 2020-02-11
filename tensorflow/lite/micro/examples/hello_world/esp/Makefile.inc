# Adding some esp specific files in the main CMakeLists.txt
ESP_HELLO_WORLD_SRCS := \
tensorflow/lite/micro/examples/hello_world/esp/esp_cli.c

ESP_HELLO_WORLD_HDRS := \
tensorflow/lite/micro/examples/hello_world/esp/esp_cli.h \
tensorflow/lite/micro/examples/hello_world/esp/esp_main_functions.h

hello_world_SRCS += $(ESP_HELLO_WORLD_SRCS)
hello_world_HDRS += $(ESP_HELLO_WORLD_HDRS)
MAIN_SRCS += $(ESP_HELLO_WORLD_SRCS)

# ESP specific flags and files
CCFLAGS += -Wno-nonnull
CXXFLAGS += -Wno-return-type -Wno-strict-aliasing
ESP_PROJECT_FILES += \
  sdkconfig.defaults \
  main/Kconfig.projbuild