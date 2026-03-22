GLshort blender_rotor_vertices[] = {-32,33,16,32,33,16,-32,-33,16,-34,35,99,34,35,99,-34,-35,99,34,-35,99,32,-33,16,-17,-149,60,-17,154,60,-17,-149,94,16,-149,60,16,154,60,16,-149,94,16,154,94,-17,154,94,-153,20,60,151,20,60,-153,20,94,-153,-14,60,151,-14,60,-153,-14,94,151,-14,94,151,20,94};
GLubyte blender_rotor_colors[] = {164,166,170,172,175,178,163,165,168,199,201,206,205,208,212,173,176,179,173,175,179,172,174,178,191,194,198,167,169,172,192,195,199,182,185,189,175,178,181,198,201,205,182,185,188,172,175,178,177,180,183,179,182,186,183,185,189,193,195,199,181,184,187,184,187,191,189,192,196,194,196,200};
const GLubyte blender_rotor_indices[] = {0,4,1,2,3,0,3,6,4,7,5,2,1,6,7,12,8,9,10,9,8,13,8,11,14,11,12,15,13,14,15,12,9,20,16,17,18,17,16,21,16,19,22,19,20,22,18,21,23,20,17,0,3,4,2,5,3,3,5,6,7,6,5,1,4,6,12,11,8,10,15,9,13,10,8,14,13,11,15,10,13,15,14,12,20,19,16,18,23,17,21,18,16,22,21,19,22,23,18,23,22,20};
void register_blender_rotor()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, blender_rotor_vertices, sizeof(blender_rotor_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, blender_rotor_colors, sizeof(blender_rotor_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, blender_rotor_indices, sizeof(blender_rotor_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 102;
    esModelArray_index++;
}
