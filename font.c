#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int main() {
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* display = al_create_display(640, 480);
    ALLEGRO_FONT* font0 = al_load_font("AlexBrush-Regular.ttf", 48, 0);
    ALLEGRO_FONT* font1 = al_load_font("FFF_Tusj.ttf", 48, 0);
    
    al_clear_to_color(al_map_rgb_f(1, 1, 1));
    al_draw_text(font0, al_map_rgb_f(0, 0, 0), 10, 10, 0, "Marrony Nascimento Neris");
    al_draw_text(font1, al_map_rgb_f(0, 1, 0), 10, 50, 0, "Marrony Nascimento Neris");

    al_flip_display();
    al_rest(5);

    al_destroy_font(font0);
    al_destroy_font(font1);
    al_destroy_display(display);

    return 0;
}

