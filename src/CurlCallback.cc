#include <string.h>
#include <stdlib.h>
#include "CurlCallback.h"


size_t redirect_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    // 将目标URL存储在userdata中
    memcpy(userdata, buffer, nitems * size);
    return nitems * size;
}

size_t write_data2(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;              // 本次响应数据的实际大小
    struct memory *mem = (struct memory *)userp; // 获取用户传进的变量

    // realloc作用:调整已有的内存。1）若已有内存后面有足够的空闲位置,则直接在后面扩展缺少的内存(此时ptr=mem->response)；
    // 2）如若不足，则开辟新的空间并将原来内存的数据拷贝到新内存的前面部分，并且原内存被释放掉。
    // 注意：如果调用成功，不管当前内存段后面的空闲空间是否满足要求，都会释放掉原来的指针，重新返回一个指针，这就导致1）的两个指针都无法再使用，只能根据返回指针使用，这里为ptr
    // 并且返回的指针ptr的内存已经由另一个进程管理，不需要我们在free(ptr)，类似智能指针，所以这里不再需要free任何内存，只需要realloc

    // 只能使用返回值ptr
    char *ptr = (char *)realloc(mem->response, mem->size + realsize + 1); // 扩展realsize + 1个字节空间(平时绝大部分都是足够的),ptr指向内存大小为mem->size + realsize + 1
    if (ptr == NULL)
    {
        return 0; /* out of memory! */
    }

    // 交给mem->response使用
    mem->response = ptr;

    // 从下标为size的地址开始存储本次的响应数据,数据大小为realsize
    memcpy(&(mem->response[mem->size]), data, realsize);
    // memcpy_s(&(mem->response[mem->size]), realsize, data, realsize);只有windows下有
    mem->size += realsize; // 将用户已有的数据大小也累积进去

    mem->response[mem->size] = 0; // 这里是多次请求时将上一次的数据清掉，当对于项目只需要单次请求的可以注释掉，已经测试过。(但最好留着反正没碍着)

    return realsize;
}