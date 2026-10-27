/*
 * Copyright (c) 2026 LVGL
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <lvgl_mem.h>
#include <lvgl_zephyr.h>
#include <stdio.h>
#include "../ui/ui.h"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

int main(void)
{
	const struct device *display_dev;
	int ret;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Display device not ready, aborting");
		return 0;
	}

	/* LVGL is initialized by the Zephyr LVGL module before main() runs.
	 * The lock protects LVGL state from concurrent access, e.g. by the
	 * shell or a rendering workqueue.
	 */
	lvgl_lock();

    ui_init("A:");
//  Replace the label creation below with a screen load function, like:    
//	lv_screen_load(my_screen_create());
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_obj_center(label);
    lv_obj_set_width(label, lv_pct(80));
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label, "Call lv_screen_load(my_screen_create()) in main.c to load a screen of a UI");


#ifndef CONFIG_LV_Z_RUN_LVGL_ON_WORKQUEUE
	lv_timer_handler();
#endif
	lvgl_unlock();

	ret = display_blanking_off(display_dev);
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("Failed to turn blanking off (error %d)", ret);
		return 0;
	}

#ifdef CONFIG_LV_Z_MEM_POOL_SYS_HEAP
	lvgl_print_heap_info(false);
#else
	printf("lvgl in malloc mode\n");
#endif

	while (1) {
#ifdef CONFIG_LV_Z_RUN_LVGL_ON_WORKQUEUE
		/* LVGL is driven by a dedicated workqueue */
		k_msleep(10);
#else
		uint32_t sleep_ms;

		lvgl_lock();
		sleep_ms = lv_timer_handler();
		lvgl_unlock();

		k_msleep(MIN(sleep_ms, 10));
#endif
	}

	return 0;
}
