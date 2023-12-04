#ifndef POWDERGAMEO_MATERIAL_H
#define POWDERGAMEO_MATERIAL_H

class material {
public:
    GLubyte color;
    GLubyte velocity;
    GLubyte density;
    void (*updateFunc)();
};

#endif //POWDERGAMEO_MATERIAL_H
