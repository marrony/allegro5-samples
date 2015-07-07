#include <stdio.h>
#include <allegro5/allegro.h>

#define DISPLAY_W 600
#define DISPLAY_H 480

int main(int argc, char* argv[]) {
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;

    if(!al_init())
        goto error;

    if(!al_install_keyboard())
        goto error;

    al_set_new_display_flags(ALLEGRO_OPENGL);
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
    al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_REQUIRE);

    display = al_create_display(DISPLAY_W, DISPLAY_H);
    if(!display)
        goto error;

    event_queue = al_create_event_queue();
    if(!event_queue)
        goto error;

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    float color = 0;
    int running = 1;
    while(running) {
        ALLEGRO_EVENT event;

        al_wait_for_event(event_queue, &event);

        switch(event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = 0;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                running = 0;

            if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                color += 0.1;

            if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                color -= 0.1;
            break;
        }

        if(al_is_event_queue_empty(event_queue)) {
            al_clear_to_color(al_map_rgb_f(color, color, color));

            al_flip_display();
        }
    }

error:
    if(event_queue)
        al_destroy_event_queue(event_queue);

    if(display)
        al_destroy_display(display);

    return 0;
}

