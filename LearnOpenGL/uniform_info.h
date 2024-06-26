#ifndef _UNIFORM_INFO_H
#define _UNIFORM_INFO_H


// https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions?tab=readme-ov-file#ideal-way-of-retrieving-all-uniform-names
struct uniform_info {
    GLint location;

    GLsizei count;

};
#endif
