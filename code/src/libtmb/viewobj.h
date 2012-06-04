#ifndef TUX_VIEW_OBJ_H__
#define TUX_VIEW_OBJ_H__
const char* g_fmtViewObj =
"#include <stdio.h>                                                                       \n"
"#include <atmi.h>                                                                        \n"
"#include \"%s\"                                                                          \n"
"                                                                                         \n"
"int main(int argc, char** argv)                                                          \n"
"{                                                                                        \n"
"	int size_index = 0;                                                                   \n"
"	int offset_index = 0;                                                                 \n"
"	int struct_size[%d] = {0};                                                            \n"
"	int struct_offset[%d] = {0};                                                          \n"
"                                                                                         \n"
"%s                                                                                       \n"
"                                                                                         \n"
"	return genviewbinfile(%s, struct_size, size_index, struct_offset, offset_index);      \n"
"}                                                                                        \n"
;
#endif // END OF TUX_VIEW_OBJ_H__
