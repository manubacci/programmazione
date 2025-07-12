#include <ncurses.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <locale.h>

enum Direction {
    North, 
    South, 
    East, 
    West,
    NorthEast,
    NorthWest,
    SouthEast,
    SouthWest,
    Point
};

enum Movement {
    Casual,
    Following,
    Teleport,
    Stop
};

enum Attack {
    Directional,
    Plus,
    Cross,
    Body,
    Harmless
};

enum Variant {
    Health,
    Speed,
    Cooldown,
    Shots,
    Range
};

enum Extention {
    Top,
    Bottom,
    Left,
    Right,
    Alone
};

enum Orientation {
    Horizontal,
    Vertical,
    None
};

enum Side {
    Zero,
    First,
    Second
};

enum Destination {
    Next,
    Previous,
    Secondary,
    Special,
    Current
};

enum Breed {
    Fly,
    Mosquito,
    Wasp,
    Hornet,
    Worm,
    Mole,
    Snake,
    Snail,
    Mix
};

enum Subject {
    User,
    Oppenent,
    Shoot
};