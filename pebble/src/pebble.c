#include <pebble.h>

#define ACCEL_STEP_MS 100

static Window *window;
static TextLayer *text_layer;

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void timer_callback(void *data) {
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
  accel_service_peek(&accel);

  CompassHeadingData compass;
  compass_service_peek(&compass);

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, 1, &(accel.x), sizeof(int), true);
  dict_write_int(iter, 2, &(accel.y), sizeof(int), true);
  dict_write_int(iter, 3, &(accel.z), sizeof(int), true);
  dict_write_int(iter, 4, &(compass.true_heading), sizeof(int), true);
  app_message_outbox_send();

  app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
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
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
