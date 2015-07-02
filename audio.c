#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

int main(void)
{
  al_init();
  al_init_acodec_addon();
  al_install_audio();

  al_reserve_samples(2);

  ALLEGRO_SAMPLE* battle = al_load_sample("gun_battle.wav");
  ALLEGRO_SAMPLE* shotgun = al_load_sample("shotgun_blast.wav");
  ALLEGRO_SAMPLE_INSTANCE* battle_instance = al_create_sample_instance(battle);
  ALLEGRO_SAMPLE_INSTANCE* shotgun_instance = al_create_sample_instance(shotgun);

  al_attach_sample_instance_to_mixer(battle_instance, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(shotgun_instance, al_get_default_mixer());

  al_set_sample_instance_gain(battle_instance, 0.2);
  al_set_sample_instance_gain(shotgun_instance, 0.2);

  al_set_sample_instance_playmode(shotgun_instance, ALLEGRO_PLAYMODE_LOOP);

  al_play_sample_instance(battle_instance);
  al_play_sample_instance(shotgun_instance);

  //al_play_sample(battle, 0.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
  //al_play_sample(shotgun, 0.2, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

  al_rest(10);

  al_destroy_sample(shotgun);
  al_destroy_sample(battle);

  return 0;
}

