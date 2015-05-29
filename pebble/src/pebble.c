#include <pebble.h>

#define ACCEL_STEP_MS 50


static Window *window;
static TextLayer *text_layer;

static char s_text_buf[64];
static bool compass_calibrated;

static void compass_heading_handler(CompassHeadingData heading_data) {
  switch (heading_data.compass_status) {
    case CompassStatusDataInvalid:
      snprintf(s_text_buf, sizeof(s_text_buf), "%s", "Compass is calibrating!\n\nMove your arm to aid calibration.");
      break;
    case CompassStatusCalibrating:
      snprintf(s_text_buf, sizeof(s_text_buf), "%s", "Fine tuning...");
      break;
    case CompassStatusCalibrated:
      snprintf(s_text_buf, sizeof(s_text_buf), "%d", ((int)heading_data.true_heading));

      break;
  }
  text_layer_set_text(text_layer, s_text_buf);
}

static void accel_callback(AccelData *data, uint32_t num_samples) {
  if (compass_calibrated) {
    AccelData accel = data[num_samples - 1];

    snprintf(s_text_buf, sizeof(s_text_buf),
        "X: %d\nY: %d\nZ: %d",
        accel.x,
        accel.y,
        accel.z
      );

    text_layer_set_text(text_layer, s_text_buf);
  };
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create(GRect(5, 0, bounds.size.w - 10, bounds.size.h));
  text_layer_set_text(text_layer, "No data yet");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  compass_calibrated = false;
  accel_data_service_subscribe(1, &accel_callback);
  compass_service_subscribe(&compass_heading_handler);
}

static void deinit(void) {
  window_destroy(window);
  accel_data_service_unsubscribe();
  compass_service_unsubscribe();
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
