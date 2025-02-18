#include "../watchface_ui.h"
#include <lvgl.h>
#include "../zsw_ui_utils.h"

#ifdef __ZEPHYR__
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(watchface_ui, LOG_LEVEL_WRN);
#endif

LV_IMG_DECLARE(hour_minimal);
LV_IMG_DECLARE(minute_minimal);
LV_IMG_DECLARE(second_minimal);

static lv_obj_t *root_page;
static lv_obj_t *ui_minimal_watchface;
static lv_obj_t *ui_hour_img;
static lv_obj_t *ui_min_img;
static lv_obj_t *ui_day_data_label;
static lv_obj_t *ui_second_img;

// Remember last values as if no change then
// no reason to waste resourses and redraw
static int last_hour = -1;
static int last_minute = -1;
static int last_second = -1;
static int last_num_not = -1;

void watchface_init(void)
{
    lv_obj_clean(lv_scr_act());
}

void watchface_show(void)
{
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    root_page = lv_obj_create(lv_scr_act());
    watchface_ui_invalidate_cached();

    lv_obj_clear_flag(root_page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(root_page, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(root_page, LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_set_style_border_width(root_page, 0, LV_PART_MAIN);
    lv_obj_set_size(root_page, 240, 240);
    lv_obj_align(root_page, LV_ALIGN_CENTER, 0, 0);

    ui_minimal_watchface = root_page;
    lv_obj_clear_flag(ui_minimal_watchface, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_img_src(ui_minimal_watchface, global_watchface_bg_img, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_hour_img = lv_img_create(ui_minimal_watchface);
    lv_img_set_src(ui_hour_img, &hour_minimal);
    lv_obj_set_width(ui_hour_img, LV_SIZE_CONTENT);   /// 25
    lv_obj_set_height(ui_hour_img, LV_SIZE_CONTENT);    /// 89
    lv_obj_set_x(ui_hour_img, 0);
    lv_obj_set_y(ui_hour_img, -32);
    lv_obj_set_align(ui_hour_img, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_hour_img, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_pivot(ui_hour_img, 18, 82);
    lv_obj_set_style_img_recolor(ui_hour_img, lv_color_hex(0x0EA7FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_hour_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_min_img = lv_img_create(ui_minimal_watchface);
    lv_img_set_src(ui_min_img, &minute_minimal);
    lv_obj_set_width(ui_min_img, LV_SIZE_CONTENT);   /// 12
    lv_obj_set_height(ui_min_img, LV_SIZE_CONTENT);    /// 104
    lv_obj_set_x(ui_min_img, 0);
    lv_obj_set_y(ui_min_img, -46);
    lv_obj_set_align(ui_min_img, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_min_img, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_pivot(ui_min_img, 11, 104);
    lv_obj_set_style_img_recolor(ui_min_img, lv_color_hex(0xF0FFD5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_min_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_day_data_label = lv_label_create(ui_minimal_watchface);
    lv_obj_set_width(ui_day_data_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_day_data_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_day_data_label, -10);
    lv_obj_set_y(ui_day_data_label, 0);
    lv_obj_set_align(ui_day_data_label, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_day_data_label, "WED 10");
    lv_obj_set_style_text_color(ui_day_data_label, lv_color_hex(0xCF9C60), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_day_data_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_second_img = lv_img_create(ui_minimal_watchface);
    lv_img_set_src(ui_second_img, &second_minimal);
    lv_obj_set_width(ui_second_img, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_second_img, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_second_img, -1);
    lv_obj_set_y(ui_second_img, -47);
    lv_obj_set_align(ui_second_img, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_second_img, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_second_img, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_pivot(ui_second_img, 4, 108);
    lv_obj_set_style_img_recolor(ui_second_img, lv_color_hex(0xFF4242), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_second_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void watchface_remove(void)
{
    lv_obj_del(root_page);
    root_page = NULL;
}

void watchface_set_battery_percent(int32_t percent, int32_t value)
{
}

void watchface_set_hrm(int32_t value)
{
}

void watchface_set_step(int32_t value)
{
}

void watchface_set_time(int32_t hour, int32_t minute, int32_t second)
{
    int hour_minute_offset;

    if (!root_page) {
        return;
    }

    hour = hour % 12;
    // Move hour hand with greater resolution than 12.
    hour_minute_offset = (int)((minute / 60.0) * (3600 / 12.0));
    last_hour = hour_minute_offset + hour * (3600 / 12);
    last_minute = minute * (3600 / 60);
    last_second = second * (3600 / 60);
    lv_img_set_angle(ui_hour_img, last_hour);
    lv_img_set_angle(ui_min_img, last_minute);
    lv_img_set_angle(ui_second_img, last_second);
}

void watchface_set_num_notifcations(int32_t value)
{
}

void watchface_set_ble_connected(bool connected)
{
}

void watchface_set_weather(int8_t temperature, int weather_code)
{
}

void watchface_set_date(int day_of_week, int date)
{
    if (!root_page) {
        return;
    }
    char *days[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

    lv_label_set_text_fmt(ui_day_data_label, "%s %d", days[day_of_week], date);
}

void watchface_set_watch_env_sensors(int temperature, int humidity, int pressure)
{
}

void watchface_ui_invalidate_cached(void)
{
    last_hour = -1;
    last_minute = -1;
    last_num_not = -1;
    last_second = -1;
}
