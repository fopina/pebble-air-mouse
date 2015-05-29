#include <pebble.h>

#define ACCEL_STEP_MS 50


static Window *window;
static TextLayer *text_layer;

static void accel_callback(AccelData *data, uint32_t num_samples) {
  AccelData accel = data[num_samples - 1];
  static char s_text_buf[32];
  snprintf(s_text_buf, sizeof(s_text_buf),
      "X: %d\nY: %d\nZ: %d",
      accel.x,
      accel.y,
      accel.z
    );

  text_layer_set_text(text_layer, s_text_buf);
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

  accel_data_service_subscribe(1, accel_callback);
}

static void deinit(void) {
  window_destroy(window);
  accel_data_service_unsubscribe();
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
