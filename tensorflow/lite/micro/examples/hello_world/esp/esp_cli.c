/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <esp_console.h>
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/uart.h>

#include "esp_main_functions.h"
#include "esp_cli.h"

static TaskHandle_t cli_task;
static int stop;
static const char *TAG = "[esp_cli]";

static int task_dump_cli_handler(int argc, char *argv[])
{
    int num_of_tasks = uxTaskGetNumberOfTasks();
    TaskStatus_t *task_array = calloc(1, num_of_tasks * sizeof(TaskStatus_t));
    /* Just to go to the next line */
    printf("\n");
    if (!task_array) {
        ESP_LOGE(TAG, "Memory not allocated for task list.");
        return 0;
    }
    num_of_tasks = uxTaskGetSystemState(task_array, num_of_tasks, NULL);
    printf("\tName\tNumber\tPriority\tStackWaterMark\n");
    for (int i = 0; i < num_of_tasks; i++) {
        printf("%16s\t%d\t%d\t%d\n",
               task_array[i].pcTaskName,
               task_array[i].xTaskNumber,
               task_array[i].uxCurrentPriority,
               task_array[i].usStackHighWaterMark);
    }
    free(task_array);
    return 0;
}

static int cpu_dump_cli_handler(int argc, char *argv[])
{
    /* Just to go to the next line */
    printf("\n");
#ifndef CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
    printf("%s: To use this utility enable: Component config --> FreeRTOS --> Enable FreeRTOS to collect run time stats\n", TAG);
#else
    char *buf = calloc(1, 2 * 1024);
    vTaskGetRunTimeStats(buf);
    printf("%s: Run Time Stats:\n%s\n", TAG, buf);
    free(buf);
#endif
    return 0;
}

static int mem_dump_cli_handler(int argc, char *argv[])
{
    /* Just to go to the next line */
    printf("\n");
    printf("\tDescription\tInternal\tSPIRAM\n");
    printf("Current Free Memory\t%d\t\t%d\n",
           heap_caps_get_free_size(MALLOC_CAP_8BIT) - heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
           heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    printf("Largest Free Block\t%d\t\t%d\n",
           heap_caps_get_largest_free_block(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL),
           heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));
    printf("Min. Ever Free Size\t%d\t\t%d\n",
           heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL),
           heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM));
    return 0;
}

static int inference_cli_handler(int argc, char *argv[])
{
    /* Just to go to the next line */
    printf("\n");
    if (argc != 2) {
        printf("%s: Incorrect arguments\n", TAG);
        return 0;
    }
    run_inference(atof(argv[1]));
    return 0;
}

static esp_console_cmd_t diag_cmds[] = {
    {
        .command = "mem-dump",
        .help = "",
        .func = mem_dump_cli_handler,
    },
    {
        .command = "task-dump",
        .help = "",
        .func = task_dump_cli_handler,
    },
    {
        .command = "cpu-dump",
        .help = "",
        .func = cpu_dump_cli_handler,
    },
    {
        .command = "sin",
        .help = "sin <float_number>",
        .func = inference_cli_handler,
    },
};

static void esp_cli_task(void *arg)
{
    int uart_num = (int) arg;
    uint8_t linebuf[256];
    int i, cmd_ret;
    esp_err_t ret;
    QueueHandle_t uart_queue;
    uart_event_t event;

    ESP_LOGI(TAG, "Initialising UART on port %d", uart_num);
    uart_driver_install(uart_num, 256, 0, 8, &uart_queue, 0);
    /* Initialize the console */
    esp_console_config_t console_config = {
            .max_cmdline_args = 8,
            .max_cmdline_length = 256,
    };

    esp_console_init(&console_config);
    esp_console_register_help_command();

    while (!stop) {
        uart_write_bytes(uart_num, "\n>> ", 4);
        memset(linebuf, 0, sizeof(linebuf));
        i = 0;
        do {
            ret = xQueueReceive(uart_queue, (void * )&event, (portTickType)portMAX_DELAY);
            if (ret != pdPASS) {
                if(stop == 1) {
                    break;
                } else {
                    continue;
                }
            }
            if (event.type == UART_DATA) {
                while (uart_read_bytes(uart_num, (uint8_t *) &linebuf[i], 1, 0)) {
                    if (linebuf[i] == '\r') {
                        uart_write_bytes(uart_num, "\r\n", 2);
                    } else {
                        uart_write_bytes(uart_num, (char *) &linebuf[i], 1);
                    }
                    i++;
                }
            }
        } while ((i < 255) && linebuf[i-1] != '\r');
        if (stop) {
            break;
        }
        /* Remove the truncating \r\n */
        linebuf[strlen((char *)linebuf) - 1] = '\0';
        ret = esp_console_run((char *) linebuf, &cmd_ret);
        if (ret < 0) {
            printf("%s: Console dispatcher error\n", TAG);
            break;
        }
    }
    ESP_LOGE(TAG, "Stopped CLI");
    vTaskDelete(NULL);
}

int esp_diag_register_cli()
{
    int cmds_num = sizeof(diag_cmds) / sizeof(esp_console_cmd_t);
    int i;
    for (i = 0; i < cmds_num; i++) {
        ESP_LOGI(TAG, "Registering command: %s", diag_cmds[i].command);
        esp_console_cmd_register(&diag_cmds[i]);
    }
    return 0;
}

int esp_cli_init()
{
    static int cli_started;
    if (cli_started) {
        return 0;
    }
#define ESP_CLI_STACK_SIZE 7000
    //StackType_t *task_stack = (StackType_t *) calloc(1, ESP_CLI_STACK_SIZE);
    //static StaticTask_t task_buf;
    if(pdPASS != xTaskCreate(&esp_cli_task, "cli_task", ESP_CLI_STACK_SIZE, NULL, 4,NULL)) {
        ESP_LOGE(TAG, "Couldn't create task");
        return -1;
    }
    cli_started = 1;
    return 0;
}
