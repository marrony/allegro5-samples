#include <stdio.h>
#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

struct mask_t {
    int width;
    int height;
    uint8_t bits[];
};

struct mask_t* new_mask(int width, int height) {
    int nbytes = sizeof(struct mask_t) + width*height;
    struct mask_t* mask = al_malloc(nbytes);
    memset(mask, 0, nbytes);
    mask->width = width;
    mask->height = height;
    return mask;
}

void destroy_mask(struct mask_t* mask) {
    al_free(mask);
}

struct mask_t* create_mask(ALLEGRO_BITMAP* bitmap) {
    int width = al_get_bitmap_width(bitmap);
    int height = al_get_bitmap_height(bitmap);
    struct mask_t* mask = new_mask(width, height);

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            ALLEGRO_COLOR color = al_get_pixel(bitmap, x, y);
            mask->bits[y*width + x] = (color.a != 0);
        }
    }

    return mask;
}

int collides_mask(struct mask_t* mask0, int x0, int y0, struct mask_t* mask1, int x1, int y1) {
    int x = MAX(x0, x1);
    int y = MAX(y0, y1);
    int tx = MIN(x0 + mask0->width, x1 + mask1->width);
    int ty = MIN(y0 + mask0->height, y1 + mask1->height);

    //if tx >= x && ty >= y then exist intersection

    //printf("%d %d %d %d\n", x, y, tx, ty);
    //printf("%d %d %d %d\n", x-x0, y-y0, tx-x0, ty-y0);
    //printf("%d %d %d %d\n", x-x1, y-y1, tx-x1, ty-y1);

    for(int j = y; j < ty; j++) {
        for(int i = x; i < tx; i++) {
            int index0 = (j - y0) * mask0->width + (i - x0);
            int index1 = (j - y1) * mask1->width + (i - x1);

            if(mask0->bits[index0] && mask1->bits[index1])
                return 1;
        }
    }

    return 0;
}

void draw_mask(struct mask_t* mask, int offsetx, int offsety) {
    for(int x = 0; x < mask->width; x++) {
        for(int y = 0; y < mask->height; y++) {
            if(mask->bits[y*mask->width + x])
                al_put_pixel(x+offsetx, y+offsety, al_map_rgb_f(1, 1, 1));
            else
                al_put_pixel(x+offsetx, y+offsety, al_map_rgb_f(0, 0, 0));
        }
    }
}

#define DISPLAY_W 600
#define DISPLAY_H 480

int main(int argc, char* argv[]) {
    uint8_t keys[ALLEGRO_KEY_MAX] = {0};
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_BITMAP* bitmap = NULL;
    struct mask_t* mask = NULL;

    if(!al_init())
        goto error;

    if(!al_install_keyboard())
        goto error;

    if(!al_init_image_addon())
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

    bitmap = al_load_bitmap("orb.png");
    printf("creating mask\n");
    mask = create_mask(bitmap);
    printf("mask created\n");

    int x = 64;
    int y = 62;

    float color = 0.5;
    int running = 1;
    while(running) {
        ALLEGRO_EVENT event;

        al_wait_for_event(event_queue, &event);

        switch(event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = 0;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            keys[event.keyboard.keycode] = 1;
            break;

        case ALLEGRO_EVENT_KEY_UP:
            keys[event.keyboard.keycode] = 0;
            break;
        }

        if(keys[ALLEGRO_KEY_ESCAPE])
            running = 0;

        if(keys[ALLEGRO_KEY_UP])
            y--;

        if(keys[ALLEGRO_KEY_DOWN])
            y++;

        if(keys[ALLEGRO_KEY_LEFT])
            x--;

        if(keys[ALLEGRO_KEY_RIGHT])
            x++;

        if(al_is_event_queue_empty(event_queue)) {
            al_clear_to_color(al_map_rgb_f(color, color, color));

            if(collides_mask(mask, 0, 0, mask, x, y))
                printf("\rcollides      ");
            else
                printf("\rdon't collides");
            fflush(stdout);

            al_draw_bitmap(bitmap, 0, 0, 0);
            al_draw_bitmap(bitmap, x, y, 0);

            al_flip_display();
        }
    }

error:
    if(bitmap)
        al_destroy_bitmap(bitmap);

    if(mask)
        destroy_mask(mask);

    if(event_queue)
        al_destroy_event_queue(event_queue);

    if(display)
        al_destroy_display(display);

    return 0;
}

