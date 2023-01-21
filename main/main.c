#include "qpc.h"

#include "qhsm-wifi.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

Q_DEFINE_THIS_FILE

static const char * TAG = "main";

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

static QEvt const *wifiQueueSto[6];

static StackType_t wifiStack[2048];

void app_main(void)
{
    WiFi_ctor(); /* instantiate WiFi active object */

     /* Initialzie QP/C Framework */
    QF_init();

    /* Initialize Event Memory Pool */
    QF_poolInit(smallPoolSto, sizeof(smallPoolSto), sizeof(smallPoolSto[0]));
    QF_poolInit(mediumPoolSto, sizeof(mediumPoolSto), sizeof(mediumPoolSto[0]));
    QF_poolInit(largePoolSto, sizeof(largePoolSto), sizeof(largePoolSto[0]));

    /* Start the active object */
    QActive_setAttr(AO_WiFi, TASK_NAME_ATTR, "WiFi");

    QACTIVE_START(AO_WiFi,   /* AO to start */
                (uint_fast8_t)(1),   /* QP priority of the AO */
                wifiQueueSto,        /* event queue storage */
                Q_DIM(wifiQueueSto), /* queue length [events] */
                wifiStack,           /* stack storage */
                sizeof(wifiStack),   /* stack size [bytes] */
                (QEvt *)0);

    QF_run();

    ESP_LOGI(TAG, "Goodbye app_main()");

}