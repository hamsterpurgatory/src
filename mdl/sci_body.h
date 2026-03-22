GLshort sci_body_vertices[] = {-3387,2375,-31855,3485,2375,-31855,-3387,-4497,-31855,-2571,1559,-32767,2669,1559,-32767,-2571,-3681,-32767,2669,-3681,-32767,3485,-4497,-31855,10782,10733,-3238,-10683,-10733,-3238,-10683,10733,-3238,10782,-10733,-3238,8427,-8378,-1062,8427,8378,-1062,-8329,-8378,-1062,-8329,8378,-1062};
GLubyte sci_body_colors[] = {254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254};
const GLubyte sci_body_indices[] = {1,3,0,10,1,0,9,0,2,2,3,5,6,3,4,2,6,7,1,6,4,1,11,7,11,2,7,8,12,11,11,14,9,13,14,12,9,15,10,10,13,8,1,4,3,10,8,1,9,10,0,2,0,3,6,5,3,2,5,6,1,7,6,1,8,11,11,9,2,8,13,12,11,12,14,13,15,14,9,14,15,10,15,13};
void register_sci_body()
{
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].vid, sci_body_vertices, sizeof(sci_body_vertices), GL_STATIC_DRAW);
    esBind(GL_ARRAY_BUFFER, &esModelArray[esModelArray_index].cid, sci_body_colors, sizeof(sci_body_colors), GL_STATIC_DRAW);
    esBind(GL_ELEMENT_ARRAY_BUFFER, &esModelArray[esModelArray_index].iid, sci_body_indices, sizeof(sci_body_indices), GL_STATIC_DRAW);
    esModelArray[esModelArray_index].itp = GL_UNSIGNED_BYTE;
    esModelArray[esModelArray_index].ni = 84;
    esModelArray_index++;
}
