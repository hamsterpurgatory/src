GLshort cat_mouth_vertices[] = {-1563,400,-2750,1563,400,-2750,-2363,-400,8,2363,-400,8};
GLubyte cat_mouth_colors[] = {215,35,165,215,35,165,215,35,165,215,35,165};
const GLubyte cat_mouth_indices[] = {0,3,2,0,1,3};
void register_cat_mouth()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, cat_mouth_vertices, sizeof(cat_mouth_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, cat_mouth_colors, sizeof(cat_mouth_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, cat_mouth_indices, sizeof(cat_mouth_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 6;
    esModelArray_index++;
}
