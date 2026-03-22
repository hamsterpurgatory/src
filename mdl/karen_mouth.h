GLshort karen_mouth_vertices[] = {-6597,583,-4013,6597,583,-4013,-3443,-583,4,3443,-583,4};
GLubyte karen_mouth_colors[] = {215,35,165,215,35,165,215,35,165,215,35,165};
const GLubyte karen_mouth_indices[] = {1,2,0,1,3,2};
void register_karen_mouth()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, karen_mouth_vertices, sizeof(karen_mouth_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, karen_mouth_colors, sizeof(karen_mouth_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, karen_mouth_indices, sizeof(karen_mouth_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 6;
    esModelArray_index++;
}
