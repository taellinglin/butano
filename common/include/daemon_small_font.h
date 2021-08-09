/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef DAEMON_SMALL_FONT_H
#define DAEMON_SMALLFONT_H

#include "bn_sprite_font.h"
#include "bn_sprite_items_daemon_small.h"
#include "bn_span.h"
namespace common
{

constexpr bn::string_view daemon_small_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr int8_t daemon_small_font_character_widths[] = {
    8,  // 32
    4,  // 33 !
    4,  // 34 "
    7,  // 35 #
    6,  // 36 $
    6,  // 37 %
    8,  // 38 &
    4,  // 39 '
    5,  // 40 (
    5,  // 41 )
    5,  // 42 *
    5,  // 43 +
    5,  // 44 -
    4,  // 45 ,
    4,  // 46 .
    5, //47 /
    6,  // 48 0
    6,  // 49 1
    6,  // 50 2
    6,  // 51 3
    6,  // 52 4
    6,  // 53 5
    6,  // 54 6
    6,  // 55 7
    6,  // 56 8
    6,  // 57 9
    3,  // 58 :
    3,  // 59 ;
    6,  // 60 <
    6,  // 61 =
    6,  // 62 >
    6,  // 63 ?
    8,  // 64 @
    6,  // 65 A
    6,  // 66 B
    6,  // 67 C
    6,  // 68 D
    6,  // 69 E
    6,  // 70 F
    6,  // 71 G
    6,  // 72 H
    6,  // 73 I
    6,  // 74 J
    6,  // 75 K
    6,  // 76 L
    6,  // 77 M
    6,  // 78 N
    6,  // 79 O
    6,  // 80 P
    6,  // 81 Q
    6,  // 82 R
    6,  // 83 S
    6,  // 84 T
    6,  // 85 U
    6,  // 86 V
    6,  // 87 W
    6,  // 88 X
    6,  // 89 Y
    6,  // 90 Z
    4,  // 91 [
    8,  // 92
    4,  // 93 ]
    8,  // 94 ^
    8,  // 95 _
    4,  // 96 `
    6,  // 97 a
    6,  // 98 b
    6,  // 99 c
    6,  // 100 d
    6,  // 101 e
    6,  // 102 f
    6,  // 103 g
    6,  // 104 h
    6,  // 105 i
    6,  // 106 j
    6,  // 107 k
    6,  // 108 l
    6,  // 109 m
    6,  // 110 n
    6,  // 111 o
    6,  // 112 p
    6,  // 113 q
    6,  // 114 r
    6,  // 115 s
    6,  // 116 t
    6,  // 117 u
    6,  // 118 v
    6,  // 119 w
    6,  // 120 x
    6,  // 121 y
    6,  // 122 z
    4,  // 123 {
    3,  // 124 |
    4,  // 125 }
    4, // 126 ~ 
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8,
    8
};

constexpr bn::sprite_font daemon_small_font(
        bn::sprite_items::daemon_small, 
        daemon_small_font_utf8_characters,
        daemon_small_font_character_widths);

}

#endif
