#include "qpc.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "esp_heap_caps.h"

#include "wifi_button.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

static const char * TAG = "main";

void heap_caps_alloc_failed_hook(size_t requested_size, uint32_t caps, const char *function_name) {
    printf("%s was called but failed to allocate %d bytes with 0x%X capabilities. \n",function_name, requested_size, caps);
}

/*
 * small event memory pool
 */
static QF_MPOOL_EL(QEvt) smallPoolSto[CONFIG_QPC_SMALL_POOL_SIZE];

/*
 * medium size event memory pool
 * size: QEvt + CONFIG_QPC_MEDIUM_POOL_ENTRY_SIZE words
 */
typedef struct {
    QEvt super;
    uint32_t data[CONFIG_QPC_MEDIUM_POOL_ENTRY_SIZE];
} mediumPool;
static QF_MPOOL_EL(mediumPool) mediumPoolSto[CONFIG_QPC_MEDIUM_POOL_SIZE];

/*
 * large size event memory pool
 * size: QEvt + CONFIG_QPC_LARGE_POOL_ENTRY_SIZE words
 */
typedef struct {
    QEvt super;
    uint32_t data[CONFIG_QPC_LARGE_POOL_ENTRY_SIZE];
} largePool;
static QF_MPOOL_EL(largePool) largePoolSto[CONFIG_QPC_LARGE_POOL_SIZE];

static QEvt const *wifiQueueSto[10];
static QEvt const *buttonQueueSto[10];
static QEvt const *cloudQueueSto[10];
static QEvt const *sensorQueueSto[10];

static QSubscrList subscrSto[MAX_PUB_SIG];

static StackType_t wifiStack[4096];
static StackType_t buttonStack[2048];
static StackType_t cloudStack[4096];
static StackType_t sensorStack[4096];

void app_main(void)
{
    /* Initialize GPIO */
     bsp_init();

    /* ESP SDK initialization stuff*/
    heap_caps_register_failed_alloc_callback(heap_caps_alloc_failed_hook);    

    esp_err_t ret = ESP_OK;
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    /* end of ESP SDK initialization*/

    WiFi_ctor(); /* instantiate WiFi active object */
    Button_ctor(); /* instantiate Button active object */
    Cloud_ctor(); /* instantiate Cloud active object */
    Sensor_ctor(); /* instantiate Sensor active object */

     /* Initialzie QP/C Framework */
    QF_init();

    /* initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* Initialize Event Memory Pool */
    QF_poolInit(smallPoolSto, sizeof(smallPoolSto), sizeof(smallPoolSto[0]));
    QF_poolInit(mediumPoolSto, sizeof(mediumPoolSto), sizeof(mediumPoolSto[0]));
    QF_poolInit(largePoolSto, sizeof(largePoolSto), sizeof(largePoolSto[0]));

    /* Start the active object */
    QActive_setAttr(AO_WiFi, TASK_NAME_ATTR, "WiFi");
    QActive_setAttr(AO_Button, TASK_NAME_ATTR, "Button");
    QActive_setAttr(AO_Cloud, TASK_NAME_ATTR, "Cloud");
    QActive_setAttr(AO_Sensor, TASK_NAME_ATTR, "Sensor");

    QACTIVE_START(AO_WiFi,   /* AO to start */
                (uint_fast8_t)(3),   /* QP priority of the AO */
                wifiQueueSto,        /* event queue storage */
                Q_DIM(wifiQueueSto), /* queue length [events] */
                wifiStack,           /* stack storage */
                sizeof(wifiStack),   /* stack size [bytes] */
                (QEvt *)0);

    QACTIVE_START(AO_Button,   /* AO to start */
                (uint_fast8_t)(4),   /* QP priority of the AO */
                buttonQueueSto,        /* event queue storage */
                Q_DIM(buttonQueueSto), /* queue length [events] */
                buttonStack,           /* stack storage */
                sizeof(buttonStack),   /* stack size [bytes] */
                (QEvt *)0);

    QACTIVE_START(AO_Cloud,   /* AO to start */
                (uint_fast8_t)(5),   /* QP priority of the AO */
                cloudQueueSto,        /* event queue storage */
                Q_DIM(cloudQueueSto), /* queue length [events] */
                cloudStack,           /* stack storage */
                sizeof(cloudStack),   /* stack size [bytes] */
                (QEvt *)0);

    QACTIVE_START(AO_Sensor,   /* AO to start */
                (uint_fast8_t)(6),   /* QP priority of the AO */
                sensorQueueSto,        /* event queue storage */
                Q_DIM(sensorQueueSto), /* queue length [events] */
                sensorStack,           /* stack storage */
                sizeof(sensorStack),   /* stack size [bytes] */
                (QEvt *)0);

    QF_run();

    ESP_LOGI(TAG, "Goodbye app_main()");

}