/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef DAEMON_RED_FONT_H
#define DAEMON_RED_FONT_H

#include "bn_sprite_font.h"
#include "bn_sprite_items_daemon_red.h"
#include "bn_span.h"
namespace common
{

constexpr bn::string_view daemon_red_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr int8_t daemon_red_font_character_widths[] = {
    10,  // 32
    7,  // 33 !
    10,  // 34 "
    15,  // 35 #
    12,  // 36 $
    13,  // 37 %
    11,  // 38 &
    7,  // 39 '
    11,  // 40 (
    10,  // 41 )
    9,  // 42 *
    11,  // 43 +
    11,  // 44 -
    7,  // 45 ,
    7,  // 46 .
    7, //47 /
    12,  // 48 0
    12,  // 49 1
    12,  // 50 2
    12,  // 51 3
    11,  // 52 4
    11,  // 53 5
    11,  // 54 6
    11,  // 55 7
    11,  // 56 8
    7,  // 57 9
    6,  // 58 :
    11,  // 59 ;
    11,  // 60 <
    11,  // 61 =
    11,  // 62 >
    7,  // 63 ?
    8,  // 64 @
    12,  // 65 A
    12,  // 66 B
    12,  // 67 C
    12,  // 68 D
    12,  // 69 E
    12,  // 70 F
    12,  // 71 G
    12,  // 72 H
    12,  // 73 I
    12,  // 74 J
    12,  // 75 K
    12,  // 76 L
    12,  // 77 M
    12,  // 78 N
    12,  // 79 O
    12,  // 80 P
    12,  // 81 Q
    12,  // 82 R
    12,  // 83 S
    12,  // 84 T
    12,  // 85 U
    12,  // 86 V
    12,  // 87 W
    12,  // 88 X
    12,  // 89 Y
    12,  // 90 Z
    7,  // 91 [
    9,  // 92
    7,  // 93 ]
    16,  // 94 ^
    12,  // 95 _
    7,  // 96 `
    12,  // 97 a
    12,  // 98 b
    12,  // 99 c
    12,  // 100 d
    12,  // 101 e
    12,  // 102 f
    12,  // 103 g
    12,  // 104 h
    12,  // 105 i
    12,  // 106 j
    12,  // 107 k
    12,  // 108 l
    12,  // 109 m
    12,  // 110 n
    12,  // 111 o
    12,  // 112 p
    12,  // 113 q
    12,  // 114 r
    12,  // 115 s
    12,  // 116 t
    12,  // 117 u
    12,  // 118 v
    12,  // 119 w
    12,  // 120 x
    12,  // 121 y
    12,  // 122 z
    7,  // 123 {
    5,  // 124 |
    7,  // 125 }
    9, // 126 ~ 
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12
};

constexpr bn::sprite_font daemon_red_font(
        bn::sprite_items::daemon_red, 
        daemon_red_font_utf8_characters,
        daemon_red_font_character_widths);

}

#endif
