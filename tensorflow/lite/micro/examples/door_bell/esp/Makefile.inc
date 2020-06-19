# Copyright 2020 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ifeq ($(TARGET), esp)

# Adding some esp specific files in the main CMakeLists.txt
ESP_DOOR_BELL_SRCS := \
tensorflow/lite/micro/examples/door_bell/esp/app_camera_esp.c

ESP_DOOR_BELL_HDRS := \
tensorflow/lite/micro/examples/door_bell/esp/app_camera_esp.h

door_bell_SRCS += $(ESP_DOOR_BELL_SRCS)
door_bell_HDRS += $(ESP_DOOR_BELL_HDRS)
MAIN_SRCS += $(ESP_DOOR_BELL_SRCS)

# ESP specific flags and files
CCFLAGS += -Wno-nonnull
CXXFLAGS += -Wno-return-type -Wno-strict-aliasing
door_bell_ESP_PROJECT_FILES := \
  sdkconfig.defaults \
  main/Kconfig.projbuild \
  partitions.csv \
  components/smtp_client/CMakeLists.txt \
  components/smtp_client/Kconfig.projbuild \
  components/smtp_client/esp_logo.png \
  components/smtp_client/server_root_cert.pem \
  components/smtp_client/smtp_client.c \
  components/smtp_client/smtp_client.h

# Remap downloaded model files as if they were in tensorflow/lite/micro/examples/..
MODEL_DOWNLOADS_DIR := tensorflow/lite/micro/tools/make/downloads/person_model_grayscale
MODEL_EXAMPLES_DIR := tensorflow/lite/micro/examples/door_bell/person_model_grayscale
door_bell_SRCS := $(patsubst $(MODEL_DOWNLOADS_DIR)/%,$(MODEL_EXAMPLES_DIR)/%,$(door_bell_SRCS))

# Custom rule to transform downloaded model files
$(PRJDIR)door_bell/esp-idf/main/person_model_grayscale/%: $(MODEL_DOWNLOADS_DIR)/%
	@mkdir -p $(dir $@)
	@python tensorflow/lite/micro/tools/make/transform_source.py \
        --platform=esp \
        --is_example_source \
        --source_path="$(patsubst $(MODEL_DOWNLOADS_DIR)/%,$(MODEL_EXAMPLES_DIR)/%,$<)" \
        < $< > $@

endif
