$(eval $(call add_third_party_download,$(PERSON_MODEL_URL),$(PERSON_MODEL_MD5),person_model_grayscale,))

doorbell_camera_MODEL_SRCS := \
tensorflow/lite/micro/examples/doorbell_camera/model_settings.cc \
tensorflow/lite/micro/examples/doorbell_camera/person_detect_model_data.cc

doorbell_camera_MODEL_HDRS := \
tensorflow/lite/micro/examples/doorbell_camera/model_settings.h \
tensorflow/lite/micro/examples/doorbell_camera/person_detect_model_data.h

doorbell_camera_TEST_SRCS := \
tensorflow/lite/micro/examples/doorbell_camera/doorbell_camera_test.cc \
$(MAKEFILE_DIR)/downloads/person_model_grayscale/no_person_image_data.cc \
$(MAKEFILE_DIR)/downloads/person_model_grayscale/person_image_data.cc \
$(doorbell_camera_MODEL_SRCS)

doorbell_camera_TEST_HDRS := \
tensorflow/lite/micro/examples/doorbell_camera/no_person_image_data.h \
tensorflow/lite/micro/examples/doorbell_camera/person_image_data.h \
$(doorbell_camera_MODEL_HDRS)

IMAGE_PROVIDER_TEST_SRCS := \
tensorflow/lite/micro/examples/doorbell_camera/image_provider.cc \
tensorflow/lite/micro/examples/doorbell_camera/image_provider_test.cc \
tensorflow/lite/micro/examples/doorbell_camera/model_settings.cc

IMAGE_PROVIDER_TEST_HDRS := \
tensorflow/lite/micro/examples/doorbell_camera/image_provider.h \
tensorflow/lite/micro/examples/doorbell_camera/model_settings.h

DETECTION_RESPONDER_TEST_SRCS := \
tensorflow/lite/micro/examples/doorbell_camera/detection_responder.cc \
tensorflow/lite/micro/examples/doorbell_camera/detection_responder_test.cc

DETECTION_RESPONDER_TEST_HDRS := \
tensorflow/lite/micro/examples/doorbell_camera/detection_responder.h

doorbell_camera_SRCS := \
tensorflow/lite/micro/examples/doorbell_camera/detection_responder.cc \
tensorflow/lite/micro/examples/doorbell_camera/image_provider.cc \
tensorflow/lite/micro/examples/doorbell_camera/main.cc \
tensorflow/lite/micro/examples/doorbell_camera/main_functions.cc \
$(doorbell_camera_MODEL_SRCS)

doorbell_camera_HDRS := \
tensorflow/lite/micro/examples/doorbell_camera/detection_responder.h \
tensorflow/lite/micro/examples/doorbell_camera/image_provider.h \
tensorflow/lite/micro/examples/doorbell_camera/main_functions.h \
$(doorbell_camera_MODEL_HDRS)

#Find any platform - specific rules for this example.
include $(wildcard tensorflow/lite/micro/examples/doorbell_camera/*/Makefile.inc)

# Builds a standalone object recognition binary.
$(eval $(call microlite_test,doorbell_camera,\
$(doorbell_camera_SRCS),$(doorbell_camera_HDRS)))
