#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_span.h"
#include "bn_vector.h"
#include "bn_random.h"
#include "daemon_small_font.h"
#include "daemon_font.h"
#include "daemon_red_font.h"
#include "daemon_blue_font.h"
#include "bn_music_items.h"
#include "bn_music_actions.h"
#include "bn_log.h"
#include "bn_format.h"

int main()
{
    constexpr bn::fixed text_y_inc = 14;
    constexpr bn::fixed text_y_limit = (bn::display::height() / 2) - text_y_inc;
    bn::random rng;
    int random_width;
    int random_height;
    int random_char;
    int random_num;
    constexpr bn::string_view random_nums[] = {

        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9"
    };
  constexpr bn::string_view random_chars[] = {
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "g",
        "h",
        "i",
        "j",
        "k",
        "l",
        "m",
        "n",
        "o",
        "p",
        "q",
        "r",
        "s",
        "t",
        "u",
        "v",
        "w",
        "x",
        "y",
        "z"
    };
    bn::core::init();
    bn::vector<bn::sprite_ptr, 96> text_sprites;
    bn::vector<bn::sprite_ptr, 32> text_sprites_big;
    bn::sprite_text_generator text_generator_small(common::daemon_small_font);
    bn::sprite_text_generator text_generator_big(common::daemon_font);

    bn::optional<bn::sprite_palette_item> original_palette_item = text_generator_big.palette_item();
    text_generator_big.set_center_alignment();
    text_generator_small.set_center_alignment();

    bn::music_items::winter_stage_1.play();
   

    text_generator_big.set_palette_item(original_palette_item.value());
     text_generator_big.generate(0, -text_y_limit, "[Artist]", text_sprites_big);
    text_generator_big.generate(0, text_y_limit, "[Track Name]", text_sprites_big);
    while(true)
    {
        BN_LOG(text_sprites.size());
        if(text_sprites.size() >= 92){
            text_sprites.pop_back();
            text_sprites.pop_back();
            text_sprites.pop_back();
            text_sprites.pop_back();
            text_sprites.clear();

        }
        random_width = rng.get() % 240;
        random_height = rng.get() % 160;
        random_char = rng.get() % 26;
        random_num = rng.get() % 9;
        text_generator_small.set_palette_item(bn::sprite_items::daemon_red.palette_item());
        text_generator_small.generate(random_width-120,random_height-80, random_chars[random_char], text_sprites);
         random_width = rng.get() % 240;
        random_height = rng.get() % 160;
        random_char = rng.get() % 26;
        random_num = rng.get() % 9;
        text_generator_small.set_palette_item(bn::sprite_items::daemon_blue.palette_item());
        text_generator_small.generate(random_width-120, random_height-80, random_chars[random_char], text_sprites);
        text_generator_big.set_palette_item(original_palette_item.value());
        text_sprites_big.clear();
        text_generator_big.generate(0, -text_y_limit, "Tael Ling Lin", text_sprites_big);
        text_generator_big.generate(0, text_y_limit, "Winter Stage", text_sprites_big);
        text_generator_big.generate(0, 0, random_nums[random_num], text_sprites_big);
        bn::core::update();
    }
}


