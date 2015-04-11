#include "zft_memory.h"



void* memcpy(void* destination, const void* source, size_t length){
	int8_t* byte_dest_ptr = (int8_t*)destination;
	int8_t* byte_source_ptr = (int8_t*)source;
	for(size_t i = 0; i < length; ++i){
		*byte_dest_ptr = *byte_source_ptr;
		byte_source_ptr++;
		byte_dest_ptr++; 
	}
	return destination;
}



void* memduplicate(void* destination, const void* source, size_t source_length, size_t times){
	int8_t* byte_dest_ptr = (int8_t*)destination;
	int8_t* byte_source_ptr = (int8_t*)source;
	int8_t* temp_byte_source_ptr = byte_source_ptr;
	for(size_t i = 0; i < times; ++i){
		for(size_t j = 0; j < source_length; ++j){
			*byte_dest_ptr = *temp_byte_source_ptr;
			temp_byte_source_ptr++;
			byte_dest_ptr++; 
		}
		temp_byte_source_ptr = byte_source_ptr;
	}
	return destination;
}



void* memzero(void* destination, size_t length){
	int8_t* byte_dest_ptr = (int8_t*)destination;
	for (size_t i = 0; i < length; ++i){
		byte_dest_ptr[i] = 0;
	}
	return destination;
}


extern "C"
void *memset(void *s, int c, size_t n)
{
	unsigned char* p = (unsigned char*)s;
	while (n--) {
		*p = (unsigned char) c;
		p++;
	}
	return s;
}
