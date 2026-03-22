GLshort sci_mouth_vertices[] = {-2046,900,-6197,2046,900,-6197,-7708,-900,3,7708,-900,3};
GLubyte sci_mouth_colors[] = {192,0,57,192,0,57,192,0,57,192,0,57};
const GLubyte sci_mouth_indices[] = {0,3,2,0,1,3};
void register_sci_mouth()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, sci_mouth_vertices, sizeof(sci_mouth_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, sci_mouth_colors, sizeof(sci_mouth_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, sci_mouth_indices, sizeof(sci_mouth_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 6;
    esModelArray_index++;
}
