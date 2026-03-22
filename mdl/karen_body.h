GLshort karen_body_vertices[] = {-9985,9650,-2860,-9174,8722,-6038,-7794,7460,-836,-9985,-10318,-2860,9984,9650,-2860,9984,-10318,-2860,7794,7460,-836,9174,8722,-6038,9174,-9626,-6038,-9174,-9626,-6038,7794,-8128,-836,-7794,-8128,-836,-12995,11674,-28181,12995,11674,-28181,-12995,-14316,-28181,-9910,8589,-31630,9910,8589,-31630,-9910,-11230,-31630,9910,-11230,-31630,12995,-14316,-28181,-9174,8722,-6038,9174,-9626,-6038,-9174,-9626,-6038,9174,8722,-6038};
GLubyte karen_body_colors[] = {254,196,174,254,196,174,254,196,174,254,196,174,254,196,174,254,196,174,254,196,174,248,195,175,254,196,174,254,196,174,254,196,174,254,196,174,221,48,211,216,46,205,222,48,211,244,54,232,218,47,207,205,44,195,217,47,206,228,50,217,224,49,214,232,51,221,223,48,213,218,47,208};
const GLubyte karen_body_indices[] = {3,1,9,0,7,1,0,6,4,3,2,0,4,10,5,7,5,8,8,3,9,6,11,10,5,11,3,13,15,12,12,17,14,18,15,16,14,18,19,19,16,13,21,14,19,13,21,19,22,12,14,12,23,13,3,0,1,0,4,7,0,2,6,3,11,2,4,6,10,7,4,5,8,5,3,6,2,11,5,10,11,13,16,15,12,15,17,18,17,15,14,17,18,19,18,16,21,22,14,13,23,21,22,20,12,12,20,23};
void register_karen_body()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, karen_body_vertices, sizeof(karen_body_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, karen_body_colors, sizeof(karen_body_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, karen_body_indices, sizeof(karen_body_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 108;
    esModelArray_index++;
}
