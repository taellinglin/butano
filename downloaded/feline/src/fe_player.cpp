#include "bn_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_size.h"
#include "bn_optional.h"
#include "bn_span.h"
#include "bn_log.h"

#include "bn_sprite_items_cat_sprite.h"
#include "bn_sprite_items_text_bg.h"
#include "bn_sprite_items_spin_glow.h"

#include "fe_hitbox.h"
#include "fe_player.h"
#include "fe_elevator.h"
#include "fe_level.h"
#include "fe_extras.h"
#include "fe_enemy.h"
#include "fe_enemy_type.h"

#include "bn_affine_bg_items_house.h"
#include "bn_sound_items.h"


namespace fe
{
    enum directions{up, down, left, right};

    [[nodiscard]] int get_map_cell(bn::fixed x, bn::fixed y, bn::affine_bg_ptr& map, bn::span<const bn::affine_bg_map_cell> cells)
    {
        int map_size = map.dimensions().width();
        int cell =  modulo((y.safe_division(8).right_shift_integer() * map_size/8 + x/8), map_size*8).integer();
        return cells.at(cell);
    }

    [[nodiscard]] bool contains_cell(int tile, bn::vector<int, 32> tiles)
    {
        for(int index = 0; index < tiles.size(); ++index)
        {
            if(tiles.at(index) == tile)
            {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool check_collisions_map(bn::fixed_point pos, directions direction, Hitbox hitbox,bn::affine_bg_ptr& map, fe::Level level, bn::span<const bn::affine_bg_map_cell> cells)
    {
        bn::fixed l = pos.x() - hitbox.width() / 2 + hitbox.x();
        bn::fixed r = pos.x() + hitbox.width() / 2 + hitbox.x();
        bn::fixed u = pos.y() - hitbox.height() / 2 + hitbox.y();
        bn::fixed d = pos.y() + hitbox.height() / 2 + hitbox.y();
        
        bn::vector<int, 32> tiles;
        if(direction == down){
            tiles = level.floor_tiles();
        } else if(direction == left || direction == right){
            tiles = level.wall_tiles();
        } else if(direction == up){
            tiles = level.ceil_tiles();
        }

        if(contains_cell(get_map_cell(l, u, map, cells), tiles) ||
        contains_cell(get_map_cell(l, d, map, cells), tiles) ||
        contains_cell(get_map_cell(r, u, map, cells), tiles) ||
        contains_cell(get_map_cell(l, d, map, cells), tiles)){
            return true;
        } else {
            return false;
        }
    }

    //constants
    constexpr const bn::fixed gravity = 0.2;
    constexpr const bn::fixed jump_power = 4;
    constexpr const bn::fixed acc = 0.4;
    constexpr const bn::fixed max_dy = 6;
    constexpr const bn::fixed friction = 0.85;

    Player::Player(bn::sprite_ptr sprite) :
        _sprite(sprite),
        _camera(bn::camera_ptr::create(0,0)),
        _map(bn::affine_bg_items::house.create_bg(0, 0)),
        _text_bg1(bn::sprite_items::text_bg.create_sprite(0, 0)),
        _text_bg2(bn::sprite_items::text_bg.create_sprite(0, 0)),
        _healthbar(fe::Healthbar()),
        _data(fe::Data()),
        _tele_sprite(bn::sprite_items::cat_sprite.create_sprite(0, 0))
    {
        _map.value().set_visible(false); // why can't I leave something uninitialised
        _sprite.put_above();
        _sprite.set_visible(false);
        _tele_sprite.set_bg_priority(1);
        _tele_sprite.put_above();
        _tele_sprite.set_item(bn::sprite_items::cat_sprite, 16);
        _tele_sprite.set_visible(false);
        

        _healthbar.set_visible(false);

        _text_bg1.set_scale(2);
        _text_bg1.set_bg_priority(0);
        _text_bg1.put_above();
        _text_bg1.set_visible(false);
        _text_bg2.set_scale(2);
        _text_bg2.set_bg_priority(0);
        _text_bg2.put_above();
        _text_bg2.set_visible(false);
    }

    void Player::set_can_teleport(bool can_teleport){
        _can_teleport = can_teleport;
    }

    void Player::set_hp(int hp){
        _healthbar.set_hp(hp);
    }

    int Player::hp(){
        return _healthbar.hp();
    }

    void Player::set_healthbar_visibility(bool is_visible){
        _healthbar.set_visible(is_visible);
    }

    void Player::spawn(bn::fixed_point pos, bn::camera_ptr camera, bn::affine_bg_ptr map, bn::vector<Enemy,16>& enemies){
        _pos = pos;
        _camera = camera;
        _map = map;
        _map_cells = map.map().cells_ref().value();
        _enemies = &enemies;
        _map.value().set_visible(true);
        _sprite.set_visible(true);
        _healthbar.set_visible(false);
        reset();
    }

    void Player::delete_data(){
        _camera.reset();
        _map.reset();
        _map_cells.reset();
        // _camera.reset();
    }

    void Player::reset(){
        _sprite.set_camera(_camera);
        _sprite.set_bg_priority(1);
        _sprite.put_above();
        _text_bg1.set_camera(_camera);
        _text_bg2.set_camera(_camera);
        _tele_sprite.set_visible(true);
        _tele_sprite.set_camera(_camera);
        _update_camera(1);
        _dy = 0;
        _dy = 0;
        _jumping = false;
        _falling = false;
        _running = false;
        _listening = false;
        _grounded = false;
        _sliding = false;
        _already_running = false;
        _attacking = false;
    }

    bn::fixed_point Player::pos()
    {
        return _pos;
    }

    void Player::set_listening(bool is_listening)
    {
        _listening = is_listening;
        _text_bg1.set_visible(_listening);
        _text_bg2.set_visible(_listening);
    }

    bool Player::is_listening()
    {
        return _listening;
    }

    void Player::jump()
    {
        if(_grounded && !_listening){
            _dy-= jump_power;
            _grounded = false;
        }
    }

    void Player::attack()
    {
        _attacking = true;
        bn::sound_items::swipe.play();
    }

    bool Player::is_right()
    {
        return !_sprite.horizontal_flip();
    }

    void Player::check_attack(){
        if(_attacking){
            Hitbox attack_hitbox = Hitbox(_pos.x(),_pos.y(), 20, 20);
            if(_sprite.horizontal_flip()){
                attack_hitbox.set_x(_pos.x() - 8);
            } else {
                attack_hitbox.set_x(_pos.x() + 8);
            }
            
            for(int i = 0; i < _enemies->size(); i++)
            {
                if(_enemies->at(i).is_hit(attack_hitbox))
                {
                    if(_sprite.horizontal_flip()){
                        _enemies->at(i).damage_from_left(1);
                    } else {
                        _enemies->at(i).damage_from_right(1);
                    }
                    
                }
            }
        }
        
    }

    void Player::collide_with_enemies(){
        Hitbox collide_hitbox = Hitbox(_pos.x(),_pos.y()+2, 8, 12);
        for(int i = 0; i < _enemies->size(); i++)
        {
            if(_enemies->at(i).is_hit(collide_hitbox))
            {
                if(!_invulnerable){
                    _invulnerable = true;
                    _healthbar.set_hp(_healthbar.hp() - 1);
                    _dy -= 0.3;
                    if(_dx < 0){
                        _dx += 6;
                    } else if(_dx > 0){
                        _dx -= 6;
                    }
                }
            }
        }
    }

    void Player::collide_with_objects(bn::affine_bg_ptr map, fe::Level level){
        // if falling
        if(_dy > 0){
            _falling = true;
            _grounded = false;
            _jumping = false;
            
            // clamp max fall speed
            if(_dy > max_dy){
                _dy = max_dy;
            }

            if(check_collisions_map(_pos, down, _hitbox_fall, map, level, _map_cells.value()))
            {
                _grounded = true;
                _falling = false;
                _dy = 0;
                _pos.set_y(_pos.y() - modulo(_pos.y(),8));
                //todo if they pressed jump a few milliseconds before hitting the ground then jump now
            }
        } 
        else if(_dy < 0) // jumping
        {
            _jumping = true;
            _falling = false;
            if(check_collisions_map(_pos, up, _hitbox_jump, map, level, _map_cells.value()))
            {
                _dy = 0;
            }
        }

        if(_dx > 0) // moving right
        {
            if(check_collisions_map(_pos, right,_hitbox_right, map, level, _map_cells.value())){
                _dx = 0;
            }
        } 
        else if (_dx < 0) // moving left
        {
            if(check_collisions_map(_pos, left, _hitbox_left, map, level, _map_cells.value())){
                _dx = 0;
            }
        }
    }

    void Player::hide(){
        _tele_sprite.set_visible(false);
        _healthbar.set_visible(false);
        _sprite.set_visible(false);
    }

    void Player::move_right(){
        _sprite.set_horizontal_flip(false);
        _dx+= acc;
        _running = true;
        _sliding = false;
    }

    void Player::move_left(){
        _sprite.set_horizontal_flip(true);
        _dx-= acc;
        _running = true;
        _sliding = false;
    }

    void Player::apply_animation_state(){

        if(_attacking && _action.done()){
            _attacking = false;
        }
        _sprite.set_vertical_scale(1);
        if(_attacking){
            if(_action.graphics_indexes().front() != 14){
                _action = bn::create_sprite_animate_action_once(
                            _sprite, 1, bn::sprite_items::cat_sprite.tiles_item(), 14,14,14,14,14,14,14,14,15,15);
            }
        } else if(_jumping){
            _action = bn::create_sprite_animate_action_forever(
                            _sprite, 6, bn::sprite_items::cat_sprite.tiles_item(), 12,12,12,12,12,12,12,12,12,12);
        } else if(_falling){
            _action = bn::create_sprite_animate_action_forever(
                            _sprite, 6, bn::sprite_items::cat_sprite.tiles_item(), 13,13,13,13,13,13,13,13,13,13);
        } else if(_sliding){
            _action = bn::create_sprite_animate_action_forever(
                            _sprite, 6, bn::sprite_items::cat_sprite.tiles_item(), 6,6,6,6,6,6,6,6,6,6);
        } else if(_running){
            if(_action.graphics_indexes().front() != 8){
                _action = bn::create_sprite_animate_action_forever(
                        _sprite, 2.5, bn::sprite_items::cat_sprite.tiles_item(), 8, 9,10,11, 2, 3, 4, 5, 6,7);
            }
        } else {
            //idle
            if(_action.graphics_indexes().front() != 0){
                _action = bn::create_sprite_animate_action_forever(
                        _sprite, 30, bn::sprite_items::cat_sprite.tiles_item(), 0,1,0,1,0,1,0,1,0,1);
            }
        }

        _action.update();
    }

    void Player::_update_camera(int lerp){
        // update camera
        if(_pos.x() < 122+30)
        {
            _camera.value().set_x(_camera.value().x()+ (122-_camera.value().x()) /lerp);
        } else if (_pos.x() > 922-30){
            _camera.value().set_x(_camera.value().x()+ (922-20-_camera.value().x()) /lerp);
        }
        else
        {
            if(_sprite.horizontal_flip()){
                _camera.value().set_x(_camera.value().x()+ (_pos.x() - 30-_camera.value().x() + _dx*8) /lerp);
            } else {
                _camera.value().set_x(_camera.value().x()+ (_pos.x() +30 -_camera.value().x() + _dx*8) /lerp);
            }            
        }

        if(_pos.y() < 942){
            _camera.value().set_y(_camera.value().y()+ (_pos.y()-10-_camera.value().y()) /lerp);
        } else {
            _camera.value().set_y(_camera.value().y()+(942-_camera.value().y()) /lerp);
        }
    }

    void Player::update_position(bn::affine_bg_ptr map, fe::Level level){
        _update_camera(30 - bn::abs(_dx.integer())*5);

        // apply friction
        _dx = _dx * friction;

        //apply gravity
        _dy+= gravity;

        // take input
        if(bn::keypad::left_held() && !_listening)
        {
            move_left();
        } 
        else if(bn::keypad::right_held() && !_listening)
        {
            move_right();
        }
        else if(_running) //slide to a stop
        {
            if(!_falling & !_jumping){
                _sliding = true;
                _running = false;
            }
        } 
        else if (_sliding) //stop sliding
        {
            if (bn::abs(_dx) < 0.1 || _running){
                _sliding = false;
            }
        } 
        
        if(_listening){
            _text_bg1.set_position(_camera.value().x()+64+8, _camera.value().y() + 40+24);
            _text_bg2.set_position(_camera.value().x()-64+8, _camera.value().y() + 40+24);
        }

        // jump
        if(bn::keypad::a_pressed() && !_listening)
        {
            jump();
        } 

        // attack
        if(bn::keypad::b_pressed() && !_listening)
        {
            attack();
        } 

        // attack
        if(bn::keypad::l_pressed() && !_listening)
        {
            if(_can_teleport && _healthbar.is_glow_ready()){
                BN_LOG(_tele_sprite.position().x());
                _tele_sprite.set_position(_pos);
                _tele_sprite.set_horizontal_flip(!is_right());
                _healthbar.activate_glow();
                bn::sound_items::teleport.play();
                int dist_to_wall = 80;
                if(is_right()){
                    for(int index = 80; index > 0; index-=4){
                        if(check_collisions_map(bn::fixed_point(_pos.x() + index, _pos.y()), right,_hitbox_right, map, level, _map_cells.value())){
                            dist_to_wall = index-4;
                        }
                    }
                    _pos.set_x(_pos.x() + dist_to_wall);
                } else {
                    for(int index = 80; index > 0; index-=4){
                        if(check_collisions_map(bn::fixed_point(_pos.x() - index, _pos.y()), right,_hitbox_right, map, level, _map_cells.value())){
                            dist_to_wall = index-4;
                        }
                    }
                    _pos.set_x(_pos.x() - dist_to_wall);
                }
            }
        } 
        _healthbar.update();

        check_attack();

        // collide
        collide_with_objects(map, level);

        // collide with enemies
        collide_with_enemies();
        
        // ouch
        if(_invulnerable){
            ++_inv_timer;
            if(modulo(_inv_timer/5, 2) == 0){
                _sprite.set_visible(true);
            } else {
                _sprite.set_visible(false);
            }
            if(_inv_timer > 120){
                _invulnerable = false;
                _inv_timer = 0;
                _sprite.set_visible(true);
            }
        }

        // update position
        _pos.set_x(_pos.x() + _dx);
        _pos.set_y(_pos.y() + _dy);

        // lock player position to map limits x
        if(_pos.x() > 1016){
            _pos.set_x(1016);
        } else if(_pos.x() < 4){
            _pos.set_x(4);
        }

        // update sprite position
        _sprite.set_x(_pos.x());
        _sprite.set_y(_pos.y());

    }
}
