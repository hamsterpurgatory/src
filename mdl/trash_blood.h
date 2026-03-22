GLshort trash_blood_vertices[] = {-397,-490,-23,-634,-780,739,-397,468,-23,-634,758,739,390,-490,-23,627,-780,739,390,468,-23,627,758,739,390,-11,-23,627,-11,596,-4,-490,-23,-4,-780,596,3,468,-23,6,758,596,-397,-11,-23,-634,-11,596};
GLubyte trash_blood_colors[] = {221,0,16,213,0,14,238,0,19,243,0,19,218,0,16,230,0,18,198,0,13,193,0,13,213,0,14,220,0,17,230,0,18,223,0,16,238,0,18,223,0,17,221,0,17,240,0,19};
const GLubyte trash_blood_indices[] = {15,2,14,13,6,12,9,4,8,11,0,10,6,9,8,4,11,10,2,13,12,0,15,14,15,3,2,13,7,6,9,5,4,11,1,0,6,7,9,4,5,11,2,3,13,0,1,15};
void register_trash_blood()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, trash_blood_vertices, sizeof(trash_blood_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, trash_blood_colors, sizeof(trash_blood_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, trash_blood_indices, sizeof(trash_blood_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 48;
    esModelArray_index++;
}
