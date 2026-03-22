GLshort blender_blood_vertices[] = {-25787,-2567,-2496,-25162,-2567,-2496,-25787,-3224,-2496,-25162,-3224,-2496,-25944,-2402,-1572,-25005,-2402,-1572,-25005,-3389,-1572,-25944,-3389,-1572,-25162,-2895,-2496,-25051,-2895,-1902,-25474,-3224,-2496,-25474,-3340,-1808,-25787,-2896,-2496,-25897,-2897,-1808,-25474,-2567,-2496,-25474,-2451,-1808};
GLubyte blender_blood_colors[] = {234,1,0,217,1,0,209,1,0,205,1,0,226,1,0,213,1,0,219,1,0,211,1,0,247,1,0,221,1,0,202,1,0,234,1,0,229,1,0,233,1,0,241,1,0,196,0,0};
const GLubyte blender_blood_indices[] = {3,11,10,0,15,14,2,13,12,1,9,8,3,9,6,2,11,7,0,13,4,1,15,5,3,6,11,0,4,15,2,7,13,1,5,9,3,8,9,2,10,11,0,12,13,1,14,15};
void register_blender_blood()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, blender_blood_vertices, sizeof(blender_blood_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, blender_blood_colors, sizeof(blender_blood_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, blender_blood_indices, sizeof(blender_blood_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 48;
    esModelArray_index++;
}
