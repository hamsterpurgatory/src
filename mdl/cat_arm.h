GLshort cat_arm_vertices[] = {9573,-5562,-15213,11400,-5562,-15213,7632,474,-3966,9573,-7298,-14642,11400,-7298,-14642,13340,-4948,-2183,13340,474,-3966,7632,-4948,-2183,12714,-4173,-1829,12714,60,-3220,8259,-4173,-1829,8259,60,-3220,9573,-5562,-15213,11400,-5562,-15213,9573,-7298,-14642,10486,-7169,-17175,11400,-7298,-14642};
GLubyte cat_arm_colors[] = {231,187,149,231,187,149,231,187,149,231,187,149,231,187,149,231,187,149,231,187,149,231,187,149,231,187,149,231,187,149,231,187,149,231,187,149,254,0,21,254,0,21,254,0,21,254,0,21,254,0,21};
const GLubyte cat_arm_indices[] = {1,5,4,7,4,5,5,9,8,11,8,9,7,0,3,7,11,2,2,9,6,6,0,2,7,8,10,14,15,16,16,15,13,12,15,14,13,15,12,1,6,5,7,3,4,5,6,9,11,10,8,7,2,0,7,10,11,2,11,9,6,1,0,7,5,8};
void register_cat_arm()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, cat_arm_vertices, sizeof(cat_arm_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, cat_arm_colors, sizeof(cat_arm_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, cat_arm_indices, sizeof(cat_arm_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 66;
    esModelArray_index++;
}
