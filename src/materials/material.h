#ifndef POWDERGAMEO_MATERIAL_H
#define POWDERGAMEO_MATERIAL_H

//todo phase out
typedef struct {
    const GLubyte ODD_BIT = 0b10000000;
    const GLubyte CLEAR_ODD_BIT = 0b01111111;

    const GLubyte LIQUID_TRAV_LEFT_BIT = 0b01000000;
    const GLubyte CLEAR_TRAV_LEFT_BIT = 0b10111111;

    const GLubyte COLOR_BITS = 0b00001111;
    const GLubyte CLEAR_COLOR_BITS = 0b11110000;

    const GLubyte FLAG_BITS = 0b11110000;
    const GLubyte CLEAR_FLAG_BITS = 0b00001111;

    bool oddFrame = false;

    GLubyte targetOddBit;
    GLubyte nonTargetOddBit;
} PGInfo ;

class material {
public:
    GLubyte color;
    GLubyte velocity;
    GLubyte density;
    void (*updateFunc)();
    PGInfo pgInfo;
};


#endif //POWDERGAMEO_MATERIAL_H
