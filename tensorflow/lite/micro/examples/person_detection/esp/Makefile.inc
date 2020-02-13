# Adding some esp specific files in the main CMakeLists.txt
ESP_PERSON_DETECTION_SRCS := \
tensorflow/lite/micro/examples/person_detection/esp/app_camera_esp.c \
tensorflow/lite/micro/examples/person_detection/esp/esp_cli.c

ESP_PERSON_DETECTION_HDRS := \
tensorflow/lite/micro/examples/person_detection/esp/app_camera_esp.h \
tensorflow/lite/micro/examples/person_detection/esp/esp_cli.h \
tensorflow/lite/micro/examples/person_detection/esp/esp_main_functions.h

person_detection_SRCS += $(ESP_PERSON_DETECTION_SRCS)
person_detection_HDRS += $(ESP_PERSON_DETECTION_HDRS)
MAIN_SRCS += $(ESP_PERSON_DETECTION_SRCS)

# ESP specific flags and files
CCFLAGS += -Wno-nonnull
CXXFLAGS += -Wno-return-type -Wno-strict-aliasing
ESP_PROJECT_FILES += \
  sdkconfig.defaults \
  main/Kconfig.projbuild \
  partitions.csv \
  components/static_images/CMakeLists.txt \
  components/static_images/sample_images/image0 \
  components/static_images/sample_images/image1 \
  components/static_images/sample_images/image2 \
  components/static_images/sample_images/image3 \
  components/static_images/sample_images/image4 \
  components/static_images/sample_images/image5 \
  components/static_images/sample_images/image6 \
  components/static_images/sample_images/image7 \
  components/static_images/sample_images/image8 \
  components/static_images/sample_images/image9 \
  components/smtp_client/CMakeLists.txt \
  components/smtp_client/Kconfig.projbuild \
  components/smtp_client/esp_logo.png \
  components/smtp_client/server_root_cert.pem \
  components/smtp_client/smtp_client.c \
  components/smtp_client/smtp_client.h 

  # Remap downloaded model files as if they were in tensorflow/lite/micro/examples/..
MODEL_DOWNLOADS_DIR := tensorflow/lite/micro/tools/make/downloads/person_model_grayscale
MODEL_EXAMPLES_DIR := tensorflow/lite/micro/examples/person_detection/person_model_grayscale
person_detection_SRCS := $(patsubst $(MODEL_DOWNLOADS_DIR)/%,$(MODEL_EXAMPLES_DIR)/%,$(person_detection_SRCS))

# Custom rule to transform downloaded model files
$(PRJDIR)person_detection/esp-idf/main/person_model_grayscale/%: $(MODEL_DOWNLOADS_DIR)/%
	@mkdir -p $(dir $@)
	@python tensorflow/lite/micro/tools/make/transform_source.py \
        --platform=esp \
        --is_example_source \
        --source_path="$(patsubst $(MODEL_DOWNLOADS_DIR)/%,$(MODEL_EXAMPLES_DIR)/%,$<)" \
        < $< > $@
