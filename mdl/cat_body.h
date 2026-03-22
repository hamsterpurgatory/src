GLshort cat_body_vertices[] = {-6840,5393,-1436,-6284,4757,-3617,-5337,3890,-47,-6840,-8311,-1436,6864,5393,-1436,6864,-8311,-1436,5361,3890,-47,6308,4757,-3617,6308,-7836,-3617,-6284,-7836,-3617,5361,-6808,-47,-5337,-6808,-47,-2181,57,-19707,2206,57,-19707,-2181,-4330,-19707,-1660,-464,-20290,1685,-464,-20290,-1660,-3809,-20290,1685,-3809,-20290,2206,-4330,-19707,-6284,4757,-3617,6309,-7836,-3617,-6284,-7836,-3617,6309,4757,-3617};
GLubyte cat_body_colors[] = {254,196,174,254,196,174,254,196,174,254,196,174,254,196,174,254,196,174,254,196,174,248,195,175,254,196,174,254,196,174,254,196,174,254,196,174,226,49,215,216,46,205,222,48,211,244,54,232,227,49,216,205,44,195,220,48,209,225,49,214,217,47,207,232,51,221,223,48,213,216,47,206};
const GLubyte cat_body_indices[] = {3,1,9,0,7,1,0,6,4,3,2,0,4,10,5,7,5,8,5,9,8,6,11,10,5,11,3,13,15,12,12,17,14,18,15,16,14,18,19,13,18,16,21,14,19,13,21,19,22,12,14,20,13,12,3,0,1,0,4,7,0,2,6,3,11,2,4,6,10,7,4,5,5,3,9,6,2,11,5,10,11,13,16,15,12,15,17,18,17,15,14,17,18,13,19,18,21,22,14,13,23,21,22,20,12,20,23,13};
void register_cat_body()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, cat_body_vertices, sizeof(cat_body_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, cat_body_colors, sizeof(cat_body_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, cat_body_indices, sizeof(cat_body_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 108;
    esModelArray_index++;
}
