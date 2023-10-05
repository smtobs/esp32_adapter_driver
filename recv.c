#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

#include "ieee80211_mlme.h"
#include "event.h"
#include "recv.h"
#include "utils.h"

static struct task_struct *recv_thread = NULL;

   static u8 test_recv_buff[] =
   {
        0x50, 0x00, 0x00, 0x00, 0x48, 0x22, 0x54, 0x41, 0x6c, 0x94, 0x42, 0x09, 0xa5, 0x0b, 0x3c, 0x7f,
        0x42, 0x09, 0xa5, 0x0b, 0x3c, 0x7f, 0x00, 0x00, 0xb3, 0xce, 0xbd, 0x95, 0x00, 0x00, 0x00, 0x00,
        0x64, 0x00, 0x11, 0x05, 0x00, 0x12, 0x53, 0x4b, 0x5f, 0x57, 0x69, 0x46, 0x69, 0x47, 0x49, 0x47,
        0x41, 0x33, 0x43, 0x37, 0x43, 0x5f, 0x35, 0x47, 0x01, 0x08, 0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48,
        0x60, 0x6c, 0x07, 0x3c, 0x4b, 0x52, 0x20, 0x24, 0x01, 0x1e, 0x28, 0x01, 0x1e, 0x2c, 0x01, 0x1e,
        0x30, 0x01, 0x1e, 0x34, 0x01, 0x1e, 0x38, 0x01, 0x1e, 0x3c, 0x01, 0x1e, 0x40, 0x01, 0x1e, 0x64,
        0x01, 0x1e, 0x68, 0x01, 0x1e, 0x6c, 0x01, 0x1e, 0x70, 0x01, 0x1e, 0x74, 0x01, 0x1e, 0x78, 0x01,
        0x1e, 0x7c, 0x01, 0x1e, 0x95, 0x01, 0x1e, 0x99, 0x01,
        0x1e, 0x9d, 0x01, 0x1e, 0xa1, 0x01, 0x1e, 0x20,
        0x01, 0x00, 0x23, 0x02, 0x14, 0x00, 0x0b, 0x05,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x2d, 0x1a, 0xef,
        0x11, 0x12, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x18, 0x04, 0x81,
        0x08, 0x00, 0x3d, 0x16, 0x95, 0x05, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xbf, 0x0c, 0xb0, 0x79, 0xc1,
        0x03, 0xfa, 0xff, 0x0c, 0x03, 0xfa, 0xff, 0x0c,
        0x03, 0xc0, 0x05, 0x01, 0x9b, 0x00, 0xf0, 0xff,
        0xdd, 0x1a, 0x00, 0x50, 0xf2, 0x01, 0x01, 0x00,
        0x00, 0x50, 0xf2, 0x02, 0x02, 0x00, 0x00, 0x50,
        0xf2, 0x02, 0x00, 0x50, 0xf2, 0x04, 0x01, 0x00,
        0x00, 0x50, 0xf2, 0x02, 0x30, 0x18, 0x01, 0x00,
        0x00, 0x0f, 0xac, 0x02, 0x02, 0x00, 0x00, 0x0f,
        0xac, 0x02, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00,
        0x00, 0x0f, 0xac, 0x02, 0x00, 0x00, 0xdd, 0x18,
        0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0x00, 0x00,
        0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4, 0x00, 0x00,
        0x42, 0x43, 0x5e, 0x00, 0x62, 0x32, 0x2f, 0x00,
        0xdd, 0x06, 0x00, 0xe0, 0x4c, 0x02, 0x01, 0x60,
        0x7f, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
        0x00, 0x40
    };

static int recv_loop_run(void *data)
{
    while (!kthread_should_stop()) 
    {
        msleep(100);
        recv_frame_handler(test_recv_buff, sizeof(test_recv_buff));
    }
    return 0;
}

int recv_loop_init(void)
{
    int ret;

    TRACE_FUNC_ENTRY();

    recv_thread = kthread_run(recv_loop_run, NULL, "recv_loop_thread");
    if (IS_ERR(recv_thread)) 
    {
        ERROR_PRINT("Failed to create recv_loop_thread\n");
        return PTR_ERR(recv_thread);
    }

    TRACE_FUNC_EXIT();
    return 0;
}

void recv_loop_deinit(void)
{
    TRACE_FUNC_ENTRY();

    kthread_stop(recv_thread);
    
    TRACE_FUNC_EXIT();
}
