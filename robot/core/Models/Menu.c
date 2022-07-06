#include "Menu.h"

#include "EventSystem/EventSystem.h"
#include "EventSystem/USARTEvent.h"
#include "Misc/Utils.h"
#include "Models/USART.h"
DEFINE_VECTORS(Screen);

#define SCREEN_MAIN 0
#define SCREEN_LOGS 1
#define SCREEN_DEBUG_LOGS 2

static Menu instance;
static bool initialized = false;
static LogLevel log_level = LOG_INFO;

Vector_Screen_8 createScreens(Menu* menu);
void write(const char* str);

void Menu_init(String car_start_event, String car_stop_event) {
  if (initialized) return;
  initialized = true;
  instance.screens = createScreens(&instance);
  instance.current = instance.screens.data[SCREEN_MAIN];
  instance.car_start_event = car_start_event;
  instance.car_stop_event = car_stop_event;
}

Menu Menu_instance() { return instance; }

void Menu_show(Screen screen) {
  instance.last = instance.current;
  instance.current = screen;
  screen.show();
}

void Menu_handle_input(void* usart) {
  USARTEvent* event = (USARTEvent*)usart;
  instance.current.handle_input(&instance, event->data);
}

void Menu_log(LogLevel level, const char* str) {
  if ((Screen_equals(instance.current, instance.screens.data[SCREEN_LOGS]) ||
       Screen_equals(instance.current,
                     instance.screens.data[SCREEN_DEBUG_LOGS])) &&
      log_level >= level) {
    write(level == LOG_INFO ? "i " : "d ");
    write(str);
  }
}

Screen Screen_create(void (*show)(),
                     void (*handle_input)(Menu* menu, const char* input),
                     Menu* menu) {
  Screen screen;
  screen.show = show;
  screen.handle_input = handle_input;
  screen.menu = menu;
  return screen;
}

bool Screen_equals(Screen a, Screen b) {
  return a.show == b.show && a.handle_input == b.handle_input;
}

void write(const char* str) { USART_send_str(USART_instance(), str); }

void screen_main_show() {
  write("### Main menu ###\n");
  write(" 1 - Start car\n");
  write(" 2 - Stop car\n");
  write(" 3 - Show logs\n");
  write(" 4 - Show debug logs (!)\n\n");
}

void screen_main_handle_input(Menu* menu, const char* input) {
  if (String_equals_trimmed(input, "1")) {
    EventSystem_send_event(EventSystem_instance(),
                           Event_create(menu->car_start_event));
    write("!! Car started, brumm brumm !!\n\n");
    Menu_show(menu->screens.data[SCREEN_MAIN]);
  } else if (String_equals_trimmed(input, "2")) {
    EventSystem_send_event(EventSystem_instance(),
                           Event_create(menu->car_stop_event));
    write("!! Car stopped !!\n\n");
    Menu_show(menu->screens.data[SCREEN_MAIN]);
  } else if (String_equals_trimmed(input, "3")) {
    Menu_show(menu->screens.data[SCREEN_LOGS]);
  } else if (String_equals_trimmed(input, "4")) {
    Menu_show(menu->screens.data[SCREEN_DEBUG_LOGS]);
  } else {
    write("!! Unknown command !!\n\n");
    Menu_show(menu->current);
  }
}

void screen_logs_show() {
  log_level = LOG_INFO;
  write("### Logs ###\n");
  write("!! Send anything to go back !!\n\n");
}

void screen_logs_handle_input(Menu* menu, const char* input MAYBE_UNUSED) {
  Menu_show(menu->screens.data[SCREEN_MAIN]);
}

void screen_debug_logs_show() {
  log_level = LOG_DEBUG;
  write("### Debug logs ###\n");
  write("!! Send anything to go back !!\n\n");
}

void screen_debug_logs_handle_input(Menu* menu,
                                    const char* input MAYBE_UNUSED) {
  Menu_show(menu->screens.data[SCREEN_MAIN]);
}

Vector_Screen_8 createScreens(Menu* menu) {
  Vector_Screen_8 screens;

  Vector_Screen_8_push_back(
      &screens,
      Screen_create(screen_main_show, screen_main_handle_input, menu));
  Vector_Screen_8_push_back(
      &screens,
      Screen_create(screen_logs_show, screen_logs_handle_input, menu));
  Vector_Screen_8_push_back(
      &screens, Screen_create(screen_debug_logs_show,
                              screen_debug_logs_handle_input, menu));

  return screens;
}