#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <vector>


#include <filesystem>

using namespace std;

ALLEGRO_COLOR random_color() {
    int r, g, b;
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
    return al_map_rgb(r, g, b);
}

int main(int argc, char** argv) {
    ALLEGRO_DISPLAY* Screen = NULL;

    if (!al_init())
    {
        al_show_native_message_box(NULL, "Error!", "Allegro has failed to initialize.", 0, 0,ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    cout << "Working directory: " << std::filesystem::current_path() << endl;

    if (!al_install_mouse())
    {
        al_show_native_message_box(NULL, "Error!", "Failed to install mouse.", 0, 0,ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if (!al_install_keyboard())
    {
        al_show_native_message_box(NULL, "Error!", "Failed to install keyboard.", 0, 0,ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if (!al_init_primitives_addon())
    {
        al_show_native_message_box(NULL, "Error!", "Failed to initialize primitives addon.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    int width = 1920, height = 1080;
    Screen = al_create_display(width, height);

    if (Screen == NULL)
    {
        al_show_native_message_box(NULL, "Error!", "Failed to create the display.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT* mocha_font = al_load_font("Fonts/MochaChoco.otf", 64, 0);

    if (!mocha_font) {
        al_show_native_message_box(Screen, "Warning", "Failed to load font. Using built-in font.", 0, 0,ALLEGRO_MESSAGEBOX_WARN);
        mocha_font = al_create_builtin_font();
    }

    ALLEGRO_EVENT_QUEUE* EventQueue = NULL;
    ALLEGRO_EVENT Event;
    EventQueue = al_create_event_queue();
    if (EventQueue == NULL)
    {
        al_show_native_message_box(Screen, "Error!", "Failed to create the event queue.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_register_event_source(EventQueue, al_get_display_event_source(Screen));
    al_register_event_source(EventQueue, al_get_mouse_event_source());
    al_register_event_source(EventQueue, al_get_keyboard_event_source());

    bool running = true;
    int pos_x = width / 2, pos_y = height / 2;
    int mouse_x = 0;
    int mouse_y = 0;

    ALLEGRO_COLOR bg_color = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR text_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR blue = al_map_rgb(0, 55, 255);
    ALLEGRO_COLOR yellow = al_map_rgb(155, 155, 0);


    //game loop
    while (running) {
        while (al_get_next_event(EventQueue, &Event))
        {
            if (Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                running = false;
            }
            else if (Event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                mouse_x = Event.mouse.x;
                mouse_y = Event.mouse.y;
            }
            else if (Event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                pos_x = mouse_x;
                pos_y = mouse_y;

                if (pos_x <= 955 && pos_y <= 535) {
                    bg_color = white;
                    text_color = black;
                }
                else if (pos_x <= 955 && pos_y >= 536) {
                    bg_color = blue;
                    text_color = yellow;
                }
                else if (pos_x >= 956 && pos_y <= 535) {
                    bg_color = black;
                    text_color = white;
                }
                else if (pos_x >= 956 && pos_y >= 536) {
                    bg_color = yellow;
                    text_color = blue;
                }
            }
            else if (Event.type == ALLEGRO_EVENT_KEY_DOWN && Event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                running = false;
            }
        }

        al_clear_to_color(bg_color);
        
        //grid lines
        al_draw_filled_rectangle(955, 0, 965, height, al_map_rgba(99, 99, 99, 50));
        al_draw_filled_rectangle(0, 535, width, 545, al_map_rgba(99, 99, 99, 50));

        //circle object
        al_draw_filled_circle(pos_x, pos_y, 25, al_map_rgb(99, 99, 199));

        //text
        al_draw_textf(mocha_font, text_color, 50, 50, 0, "mouse x: %i", pos_x);
        al_draw_textf(mocha_font, text_color, 50, 105, 0, "mouse y: %i", pos_y);

        al_flip_display();
    }

    al_destroy_font(mocha_font);
    al_destroy_event_queue(EventQueue);
    al_destroy_display(Screen);
    return 0;
}