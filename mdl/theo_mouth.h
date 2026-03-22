GLshort theo_mouth_vertices[] = {-936,109,-747,936,109,-747,-3528,-109,3,3528,-109,3};
GLubyte theo_mouth_colors[] = {191,0,57,191,0,57,191,0,57,191,0,57};
const GLubyte theo_mouth_indices[] = {0,3,2,0,1,3};
void register_theo_mouth()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, theo_mouth_vertices, sizeof(theo_mouth_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, theo_mouth_colors, sizeof(theo_mouth_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, theo_mouth_indices, sizeof(theo_mouth_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 6;
    esModelArray_index++;
}
