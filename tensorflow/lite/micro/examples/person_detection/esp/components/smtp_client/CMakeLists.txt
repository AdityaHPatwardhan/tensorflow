idf_component_register(SRCS "smtp_client.c"
                        INCLUDE_DIRS "."
                        EMBED_TXTFILES server_root_cert.pem esp_logo.png
                        REQUIRES nvs_flash protocol_examples_common mbedtls)
