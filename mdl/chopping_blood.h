GLshort chopping_blood_vertices[] = {-26473,3168,-2680,-25214,3127,-2680,-26283,4258,-2680,-24975,3936,-2680,-25879,5058,-2680,-25169,4641,-2680,-26213,2632,-2680,-25066,2227,-2680};
GLubyte chopping_blood_colors[] = {229,4,0,218,3,0,218,3,0,209,3,0,222,4,0,197,3,0,247,5,0,207,3,0};
const GLubyte chopping_blood_indices[] = {1,2,0,2,5,4,1,6,7,1,3,2,2,3,5,1,0,6};
void register_chopping_blood()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, chopping_blood_vertices, sizeof(chopping_blood_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, chopping_blood_colors, sizeof(chopping_blood_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, chopping_blood_indices, sizeof(chopping_blood_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 18;
    esModelArray_index++;
}
