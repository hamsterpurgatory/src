GLshort karen_arm_vertices[] = {13930,-6312,-22952,16592,-6312,-22952,11103,2483,-6565,13930,-8841,-22121,14246,-6722,-23189,16276,-6722,-23189,14246,-8651,-22555,16276,-8651,-22555,16592,-8841,-22121,19419,-5417,-3968,19419,2483,-6565,11103,-5417,-3968,18507,-4287,-3452,18507,1880,-5479,12015,-4287,-3452,12015,1880,-5479};
GLubyte karen_arm_colors[] = {232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150,232,188,150};
const GLubyte karen_arm_indices[] = {0,5,4,2,1,0,11,0,3,0,6,3,7,4,5,3,7,8,8,5,1,1,9,8,9,3,8,10,12,9,11,12,14,13,14,12,11,15,2,2,13,10,0,1,5,2,10,1,11,2,0,0,4,6,7,6,4,3,6,7,8,7,5,1,10,9,9,11,3,10,13,12,11,9,12,13,15,14,11,14,15,2,15,13};
void register_karen_arm()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, karen_arm_vertices, sizeof(karen_arm_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, karen_arm_colors, sizeof(karen_arm_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, karen_arm_indices, sizeof(karen_arm_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 84;
    esModelArray_index++;
}
