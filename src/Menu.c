#include "Menu.h"
#include "Misc/utils.h"
#include "Models/USART.h"
#include "EventSystem/EventSystem.h"
#include "EventSystem/USARTEvent.h"
DEFINE_VECTORS(Screen);

#define SCREEN_INVALID_INPUT 0
#define SCREEN_MAIN 1

static Menu instance;
static bool initialized = false;
static USART usart;

Vector_Screen_8 createScreens(Menu* menu);

void Menu_init(String car_start_event, String car_stop_event)
{
  if (initialized) return;
  initialized = true;
  usart = USART_create();
  instance.screens = createScreens(&instance);
  instance.current = instance.screens.data[SCREEN_MAIN];
  instance.car_start_event = car_start_event;
  instance.car_stop_event = car_stop_event;
}

Menu Menu_instance() 
{
  return instance;
}

void Menu_show(Screen screen)
{
  instance.last = instance.current;
  instance.current = screen;
  screen.show();
}

void Menu_handle_input(void* usart)
{
  USARTEvent* event = (USARTEvent*) usart;
  instance.current.handle_input(&instance, event->data);
}

Screen Screen_create(void(*show)(), void(*handle_input)(Menu* menu, const char* input), Menu* menu)
{
  Screen screen;
  screen.show = show;
  screen.handle_input = handle_input;
  screen.menu = menu;
  return screen;
}

void write(const char* str)
{
  USART_send_str(&usart, str);
}

void screen_invalid_input_show() {
  write("### Invalid input ###\n");
  write(" Send anything to go back\n\n");
}

void screen_invalid_input_handle_input(Menu* menu, const char* input MAYBE_UNUSED) {
  Menu_show(menu->last);
}

void screen_main_show() {
  write("### Main menu ###\n");
  write(" 1 - Start car\n");
  write(" 2 - Stop car\n\n");
}

void screen_main_handle_input(Menu* menu, const char* input) {
  if (String_contains(input, "1")) {
    EventSystem_send_event(EventSystem_instance(), Event_create(menu->car_start_event));
    write("!! car started !!\n\n");
    Menu_show(menu->screens.data[SCREEN_MAIN]);
  }
  else if (String_contains(input, "2")) {
    EventSystem_send_event(EventSystem_instance(), Event_create(menu->car_stop_event));
    write("!! car stopped !!\n\n");
    Menu_show(menu->screens.data[SCREEN_MAIN]);
  }
  else
    Menu_show(menu->screens.data[SCREEN_INVALID_INPUT]);
}

Vector_Screen_8 createScreens(Menu* menu)
{
  Vector_Screen_8 screens;

  Vector_Screen_8_push_back(&screens, Screen_create(screen_invalid_input_show, screen_invalid_input_handle_input, menu));
  Vector_Screen_8_push_back(&screens, Screen_create(screen_main_show, screen_main_handle_input, menu));

  return screens;
}