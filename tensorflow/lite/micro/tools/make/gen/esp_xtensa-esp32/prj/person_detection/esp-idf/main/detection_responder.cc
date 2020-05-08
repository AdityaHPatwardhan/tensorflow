/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "detection_responder.h"
#include "app_camera.h"
#include "app_wifi.h"
#include "app_httpd.h"
#include "iostream"
#include "smtp_client.h"
#define CAMERA_WEB_SERVER

using namespace std;
extern bool camera_web_server_turned_on;
// This dummy implementation writes person and no person scores to the error
// console. Real applications will want to take some custom action instead, and
// should implement their own versions of this function.
static void esp_app_main()
{
    app_wifi_main();
    app_camera_main();
    app_httpd_main();
}

void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        uint8_t person_score, uint8_t no_person_score) {
  error_reporter->Report("person score:%d no person score %d", person_score,
                         no_person_score);
        error_reporter->Report("Result : %s \n", (person_score > 230) ? "Person Detected" : "No Person Detected");
#ifdef CAMERA_WEB_SERVER 
    if(person_score > 230) {
        cout << "Person detected starting a server at http://192.168.0.4/" << endl;
        esp_app_main();
        //send_email();
        camera_web_server_turned_on = true;
    }
#endif

}


