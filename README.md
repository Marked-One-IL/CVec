# CVec
std::vector but for C<br>
This library is fully templated via macros<br>
No void* and casting hacks<br>

![pic](assets/pic.jpg)<br>

Vec_init - creates an empty vector with no memory allocated<br>
Vec_initArr - creates a vector by copying elements from a raw array<br>
Vec_initVec - creates a vector by copying another vector<br>
Vec_subVec - creates a sub vector by copying a slice of another vector<br>
Vec_free - frees the internal buffer and resets the vector<br>
Vec_clear - remove all the elements<br>
Vec_reserve - ensures capacity is at least a given size<br>
Vec_resize - resize the vector. If the new size is bigger the elements are set to 0<br>
Vec_resizeV - resize the vector. If the new size is bigger the elements are set to a given value<br>
Vec_push - appends a single element to the end<br>
Vec_pushArr - appends multiple elements from a raw array<br>
Vec_pushVec - appends all elements from another vector<br>
Vec_pop - removes the last element<br>
Vec_popN - removes N elements from the end<br>
Vec_insert - inserts a single element at an index, shifting right<br>
Vec_insertArr - inserts multiple elements from an array at an index<br>
Vec_insertVec - inserts another vector at an index<br>
Vec_erase - removes one element at an index, shifting left<br>
Vec_eraseN - removes N elements starting at an index, shifting left
