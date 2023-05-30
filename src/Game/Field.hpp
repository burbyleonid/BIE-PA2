#ifndef Field_hpp
#define Field_hpp

#include <stdio.h>

struct Field {
    Field(int type, int workerCount = 0): m_type(type) {}
public:
    // by default - 0, -1 - empty field, -2 - road, from 0 to 3 buildings, 5 - town hall
    int m_type = 0;  
};

#endif /* Field_hpp */
