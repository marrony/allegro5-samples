#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

void abort_example(const char* str) {
    printf("%s\n", str);
}

int main(void)
{
   ALLEGRO_DISPLAY *display;
   ALLEGRO_BITMAP *bitmap;
   ALLEGRO_TIMER *timer;
   ALLEGRO_EVENT_QUEUE *queue;

   if (!al_init()) {
      abort_example("Could not init Allegro.\n");
      return 1;
   }

   al_init_primitives_addon();
   al_init_image_addon();
   al_install_keyboard();
   al_install_mouse();

   al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_GENERATE_EXPOSE_EVENTS);
   //al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, true, ALLEGRO_REQUIRE);
   display = al_create_display(640, 480);
   if (!display) {
      abort_example("Error creating display\n");
      return 1;
   }

   bitmap = al_load_bitmap("mario.png");
   if (!bitmap) {
      abort_example("mario.png not found or failed to load\n");
      return 1;
   }

   al_draw_bitmap(bitmap, 0, 0, 0);
   al_flip_display();

   timer = al_create_timer(1.1);

   queue = al_create_event_queue();
   al_register_event_source(queue, al_get_keyboard_event_source());
   al_register_event_source(queue, al_get_mouse_event_source());
   al_register_event_source(queue, al_get_display_event_source(display));
   al_register_event_source(queue, al_get_timer_event_source(timer));
   al_start_timer(timer);

   while (true) {
      ALLEGRO_EVENT event;
      al_wait_for_event(queue, &event);

      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

      if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
         event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
         break;
      }

      if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
         al_acknowledge_resize(event.display.source);
      }

      if (event.type == ALLEGRO_EVENT_DISPLAY_EXPOSE) {
         int x = event.display.x;
         int y = event.display.y;
         int w = event.display.width;
         int h = event.display.height;

         printf("%d %d %d %d\n", x, y, w, h);

         /* Draw a red rectangle over the damaged area. */
         al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
         al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgba_f(1, 0, 0, 1));
         al_flip_display();
      }

      if (event.type == ALLEGRO_EVENT_TIMER) {
         /* Slowly restore the original bitmap. */
         al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

         for (int y = 0; y < al_get_display_height(display); y += 480) {
            for (int x = 0; x < al_get_display_width(display); x += 640) {
               al_draw_tinted_bitmap(bitmap, al_map_rgba_f(1, 1, 1, 0.1), x, y, 0);
            }
         }
         al_flip_display();
      }
   }

   al_destroy_event_queue(queue);
   al_destroy_bitmap(bitmap);

   return 0;
}

