#ifndef __CURL_CALLBACK__
#define __CURL_CALLBACK__

struct memory
{
    char *response;
    size_t size;
};
size_t redirect_callback(char* buffer, size_t size, size_t nitems, void* userdata);
size_t write_data2(void* contents, size_t size, size_t nmemb, void* stream);

#endif