#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

int main(void)
{
  al_init();
  al_init_image_addon();

  ALLEGRO_DISPLAY *display = al_create_display(640, 400);
  al_clear_to_color(al_map_rgba_f(0.5, 0., 0., 0.5));

  al_set_new_bitmap_flags(ALLEGRO_NO_PREMULTIPLIED_ALPHA);
  //al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);

  ALLEGRO_BITMAP *background = al_load_bitmap("background.png");
  ALLEGRO_BITMAP *mario = al_load_bitmap("mario.png");

  //al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); //with pre alpha
  al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA); //no pre alpha

  al_draw_tinted_scaled_bitmap(background, al_map_rgba_f(1, 1, 1, 1),
                        0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
                        0, 0, 640, 400, 0);
 
  //r = dr * dst + sr * src => dr * (1-sa) + sr * sa
  al_draw_tinted_scaled_bitmap(mario, al_map_rgba_f(1, 1, 1, 1),
                        0, 0, al_get_bitmap_width(mario), al_get_bitmap_height(mario),
                        50, 100, 80, 80, 0);

  al_flip_display();

  al_rest(5);

  al_destroy_bitmap(background);
  al_destroy_bitmap(mario);
  al_destroy_display(display);

  return 0;
}

