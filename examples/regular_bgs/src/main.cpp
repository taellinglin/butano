/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bgs_mosaic.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_regular_bg_attributes_hbe_ptr.h"

#include "bn_regular_bg_items_cutscene_background.h"
#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void regular_bgs_position_scene(bn::sprite_text_generator& text_generator)
    {


        bn::regular_bg_ptr blue_bg = bn::regular_bg_items::cutscene_background.create_bg(512, 512);

        while(true)
        {
            if(bn::keypad::left_held())
            {
                blue_bg.set_x(blue_bg.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                blue_bg.set_x(blue_bg.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                blue_bg.set_y(blue_bg.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                blue_bg.set_y(blue_bg.y() + 1);
            }

            bn::core::update();
        }
    }

}

    int main()
    {
        bn::core::init();

        bn::sprite_text_generator text_generator(variable_8x16_sprite_font);
        //bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

        while(true)
        {

            regular_bgs_position_scene(text_generator);
            bn::core::update();

        }
    }

