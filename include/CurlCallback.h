#ifndef __CURL_CALLBACK__
#define __CURL_CALLBACK__

size_t redirectCallback(char* buffer, size_t size, size_t nitems, void* userdata);
size_t writeData(void* contents, size_t size, size_t nmemb, void* stream);

#endif