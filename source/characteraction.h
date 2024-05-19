#ifndef CHARACTERACTION_H
#define CHARACTERACTION_H

#include <3ds.h>

#include "character.h"

struct board_s;

typedef enum {
    ACTION_NONE, // Nothing
    ACTION_MOVE // Move the character to a position
} character_action_type_e;

typedef struct {
    bool initialized;

    character_action_type_e type;
    character_t* character;
    float duration;

    vec2_t move_source;
    vec2_t move_destination;
} character_action_t;

typedef struct {
    bool initialized;
    character_action_t action;
    u32 order;
} ordered_character_action_t;

void CharacterAction_Run(character_action_t* action, struct board_s* board);

#endif // CHARACTERACTION_H