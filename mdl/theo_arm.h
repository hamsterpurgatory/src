GLshort theo_arm_vertices[] = {5475,-445,-2495,6625,-4022,-9160,9567,-445,-2495,5475,-4332,-1217,5924,-741,-1960,9118,-741,-1960,5924,-3776,-963,9118,-3776,-963,9567,-4332,-1217,8417,-5724,-8601,6625,-5724,-8601,8417,-4022,-9160,6866,-4772,-10557,6625,-4022,-9160,8176,-4772,-10557,7021,-4974,-10674,6625,-5724,-8601,6866,-6016,-10148,7021,-5923,-10362,8417,-5724,-8601,8176,-6016,-10148,8020,-5923,-10362,8020,-4974,-10674,8417,-4022,-9160};
GLubyte theo_arm_colors[] = {121,100,84,132,111,92,117,97,81,127,107,89,127,107,89,124,104,87,116,96,80,127,107,89,131,111,92,134,113,94,133,112,93,124,103,86,100,82,67,96,79,65,96,79,65,101,83,67,109,89,73,100,82,67,108,88,72,111,90,74,93,76,63,100,82,67,96,79,65,93,77,63};
const GLubyte theo_arm_indices[] = {3,1,10,0,11,1,0,5,2,3,4,0,5,6,7,3,7,6,8,5,7,11,8,9,9,3,10,3,0,1,0,2,11,0,4,5,3,6,4,5,4,6,3,8,7,8,2,5,11,2,8,9,8,3,16,13,12,13,14,12,14,15,12,17,12,15,16,12,17,19,16,17,17,18,21,17,15,18,19,17,20,17,21,20,20,21,22,21,15,22,21,18,15,20,22,14,14,22,15,14,19,20,14,23,19,13,23,14};
void register_theo_arm()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, theo_arm_vertices, sizeof(theo_arm_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, theo_arm_colors, sizeof(theo_arm_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, theo_arm_indices, sizeof(theo_arm_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 108;
    esModelArray_index++;
}
