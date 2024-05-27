#ifndef CHARACTERACTION_H
#define CHARACTERACTION_H

#include <3ds.h>

#include "character.h"
#include "hazard.h"

#define CHARACTER_ACTION_MAX_TILES_SELECTED 25

struct board_s;

typedef enum {
    CHARACTER_ACTION_NONE, // Nothing
    CHARACTER_ACTION_MOVE, // Move the character to multiple position
    CHARACTER_ACTION_CREATE_HAZARD, // Do action targeting some tiles from the player, and resolve hazards
} character_action_type_e;

typedef struct {
    bool initialized;
    
    bool first_frame;

    character_action_type_e type;
    character_t* character;
    float duration;

    vec2_t char_pos;
    vec2_t tile_selections[CHARACTER_ACTION_MAX_TILES_SELECTED];
    u32 tile_selections_count; // No .initialized for vec2_t

    hazard_type_e hazard_type;
} character_action_t;

typedef struct {
    bool initialized;
    character_action_t action;
    u32 order;
} ordered_character_action_t;

void CharacterAction_Run(character_action_t* action, struct board_s* board);

#endif // CHARACTERACTION_H