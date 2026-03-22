GLshort microwave_blood_vertices[] = {-24852,23500,-2701,-24852,23500,-1270,-24852,20970,-1270,-24852,20970,-2701,-24852,22235,-2701,-24852,22235,-1270,-24852,21603,-2701,-24852,21603,-1270,-24852,22868,-1270,-24852,22868,-2701,-24852,21286,-2701,-24852,21286,-2266,-24852,21919,-1735,-24852,21919,-2701,-24852,22560,-2701,-24852,22560,-1949,-24852,23184,-2034,-24852,23184,-2701};
GLubyte microwave_blood_colors[] = {252,1,0,231,1,0,248,1,0,252,1,0,232,1,0,221,1,0,222,1,0,230,1,0,231,1,0,237,1,0,215,1,0,208,1,0,226,1,0,194,0,0,236,1,0,247,1,0,222,1,0,210,1,0};
const GLubyte microwave_blood_indices[] = {0,16,17,4,12,13,6,11,10,9,15,14,3,11,2,6,12,7,4,15,5,9,16,8,0,1,16,4,5,12,6,7,11,9,8,15,3,10,11,6,13,12,4,14,15,9,17,16};
void register_microwave_blood()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, microwave_blood_vertices, sizeof(microwave_blood_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, microwave_blood_colors, sizeof(microwave_blood_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, microwave_blood_indices, sizeof(microwave_blood_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 48;
    esModelArray_index++;
}
