#ifndef MENU_H
#define MENU_H

#include "Misc/String.h"
#include "Misc/Vector.h"

struct Menu;

typedef struct Screen {
  void (*show)();
  void (*handle_input)(struct Menu* menu, const char* input);
  struct Menu* menu;
} Screen;

DECLARE_VECTORS(Screen);

typedef struct Menu {
  Screen current;
  Screen last;
  Vector_Screen_8 screens;
  String car_start_event;
  String car_stop_event;
} Menu;

typedef enum LogLevel { LOG_INFO, LOG_DEBUG } LogLevel;

void Menu_init(String car_start_event, String car_stop_event);
Menu Menu_instance();
void Menu_show(Screen screen);
void Menu_handle_input(void* usart);
void Menu_log(LogLevel level, const char* str);

Screen Screen_create(void (*show)(),
                     void (*handle_input)(Menu* menu, const char* input),
                     Menu* menu);
bool Screen_equals(Screen a, Screen b);

#endif  // MENU_H