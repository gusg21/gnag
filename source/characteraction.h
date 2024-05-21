#ifndef CHARACTERACTION_H
#define CHARACTERACTION_H

#include <3ds.h>

#include "character.h"

#define CHARACTER_ACTION_MAX_MOVE_DESTINATIONS 16

struct board_s;

typedef enum {
    CHARACTER_ACTION_NONE, // Nothing
    CHARACTER_ACTION_MOVE, // Move the character to multiple position
} character_action_type_e;

typedef struct {
    bool initialized;

    character_action_type_e type;
    character_t* character;
    float duration;

    vec2_t move_source;
    vec2_t move_destinations[CHARACTER_ACTION_MAX_MOVE_DESTINATIONS];
    u32 move_destination_count; // No .initialized for vec2_t
} character_action_t;

typedef struct {
    bool initialized;
    character_action_t action;
    u32 order;
} ordered_character_action_t;

void CharacterAction_Run(character_action_t* action, struct board_s* board);

#endif // CHARACTERACTION_H