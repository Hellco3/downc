#include "ClientTool.h"
extern const char *tmppath; // 保存更新数据的文件路径

char *genRandomString(int length, char *genstring)
{
    int flag, i;
    srand((unsigned)time(NULL));

    for (i = 0; i < length - 1; i++)
    {
        if (0 == i % 200)
        {
            genstring[i] = '\n';
            continue;
        }
        flag = rand() % 3;
        switch (flag)
        {
        case 0:
            genstring[i] = 'A' + rand() % 26;
            break;
        case 1:
            genstring[i] = 'a' + rand() % 26;
            break;
        case 2:
            genstring[i] = '0' + rand() % 10;
            break;
            break;
        default:
            genstring[i] = 'x';
            break;
        }
    }
    genstring[length - 1] = '\0';
    return genstring;
}

void genRandomVec(uint64_t max, int length, vector<uint64_t> &objectvec)
{
    objectvec.resize(length);
    srand(time(nullptr));
    for(int idx = 0; idx < length; idx++)
    {   
        objectvec[idx] = rand() % (max+1);
    }
}

uint64_t get_file_size(const string &filepath)
{
    ifstream inFile(filepath, std::ios::in);
    if (!inFile.is_open())
    {
        std::cout << "Open file " << filepath << " failed!" << std::endl;
        return -1;
    }
    inFile.seekg(0, std::ios_base::end);
    uint64_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios_base::beg);
    inFile.close();
    return fileSize;
}

int create_file(const string &filepath, uint64_t file_size)
{
    ofstream ofile(filepath);
    uint64_t offset = 0;
    while (offset < file_size)
    {
        int nWrite = BUF_SIZE;
        if (file_size - offset < BUF_SIZE)
        {
            nWrite = file_size - offset;
        }
        char genstring[2 * 1024 * 1024] = {0};
        genRandomString(nWrite, genstring);
        ofile.write(genstring, nWrite);
        offset += nWrite;
    }
    ofile.close();
    return 0;
}

int copy_file(const string &oldfile, const string &newfile)
{
    if (oldfile == newfile)
        return 0;
    ifstream ifile(oldfile);
    if (!ifile.is_open())
    {
        printf("open %s failed!", oldfile.c_str());
        return -1;
    }
    ofstream ofile(newfile);
    if (!ofile.is_open())
    {
        printf("open %s failed!", newfile.c_str());
        return -1;
    }
    uint64_t filesize = get_file_size(oldfile);
    uint64_t offset = 0;
    char *databuf = (char *)calloc(BUF_SIZE, 1);
    while (offset < filesize)
    {
        int nWrite = BUF_SIZE;
        if (filesize - offset < BUF_SIZE)
            nWrite = filesize - offset;
        ifile.read(databuf, nWrite);
        ofile.write(databuf, nWrite);
        offset += nWrite;
    }
    free(databuf);
    ifile.close();
    ofile.close();
    return 0;
}
uint64_t update_file(const string &filepath, uint64_t offset, uint64_t size)
{
    uint64_t rear = offset + size;
    std::fstream ofile(filepath, std::ios::binary | std::ios::out | std::ios::in);
    if (!ofile.is_open())
    {
        printf("open %s failed!", filepath.c_str());
        return -1;
    }
    ofile.seekp(offset, std::ios_base::beg);
    ifstream ifile(tmppath);
    if (!ifile.is_open())
    {
        printf("open %s failed!", tmppath);
        return -1;
    }
    char *databuf = (char *)calloc(BUF_SIZE, 1);
    uint64_t tmpoffset = offset;
    while (tmpoffset < rear)
    {

        bzero(databuf, BUF_SIZE);
        if (ifile.tellg() >= 2 * BUF_SIZE)
        {
            ifile.seekg(0, std::ios_base::beg);
        }
        bzero(databuf, BUF_SIZE);
        int nRead = 2 * 1024 * 1024;
        if (rear - tmpoffset < BUF_SIZE)
            nRead = rear - tmpoffset;
        ifile.read(databuf, nRead);
        ofile.write(databuf, nRead);
#ifdef debuf
        cout << databuf;
#endif
        tmpoffset += nRead;
    }
    ifile.close();
    ofile.close();
    return size;
}

int addwrite_file(const string &filepath, uint64_t size)
{
    uint64_t fileSize = get_file_size(filepath);
    return update_file(filepath, fileSize, size);
}

void init_files()
{
    std::vector<std::pair<std::string, uint64_t>> filelist = {
        {"4K", 4 * 1024},
        {"32K", 32 * 1024},
        {"64K", 64 * 1024},
        {"1M", 1 * 1024 * 1024},
        {"2M", 2 * 1024 * 1024},
        {"3M", 3 * 1024 * 1024},
        {"4M", 4 * 1024 * 1024}};
    create_file(tmppath, 2 * BUF_SIZE);
    for (size_t i = 0; i < filelist.size(); i++)
    {
        sleep(1);
        char filename[256] = {0};
        // char tmpbuf[256] = {0};
        sprintf(filename, "../data/file_%s_O", filelist[i].first.c_str());
        create_file(filename, filelist[i].second);
    }
}

int f_addwrite(uint64_t object_id, uint64_t size)
{
    char file_name[256] = {0};
    sprintf(file_name, "../data/file_%ld", object_id);
    return addwrite_file(file_name, size);
}

int f_update(uint64_t object_id, uint64_t offset, uint64_t size)
{
    char file_name[256] = {0};
    sprintf(file_name, "../data/file_%ld", object_id);
    return update_file(file_name, offset, size);
}

int f_upload_file(uint64_t object_id, const char *filename)
{
    char namebuf[256] = {0};
    sprintf(namebuf, "../data/file_%ld", object_id);
    return copy_file(filename, namebuf);
}

int f_upload_list_task(int n, uint64_t start_object_id, uint64_t object_count, const char *filepath)
{
    if (NULL == filepath)
    {
        for (uint64_t tmp_object_id = start_object_id; tmp_object_id < start_object_id + object_count; tmp_object_id++)
        {
            char file_name[256] = {0};
            sprintf(file_name, "../data/file_%ld", tmp_object_id);
            ofstream ofile(file_name);
            char putbuf[100] = {0};
            memcpy(putbuf, "hello world\nthis is an upload test\nto not be covered, this text is long\nexit\n", 100);
            ofile.write(putbuf, strlen(putbuf));
            ofile.close();
        }
    }
    else
    {
        for (uint64_t tmp_object_id = start_object_id; tmp_object_id < start_object_id + object_count; tmp_object_id++)
        {
            f_upload_file(tmp_object_id, filepath);
        }
    }
    return 0;
}

int f_delete_list(uint64_t object_id, uint64_t object_count)
{
    uint64_t start_object_id = object_id;
    for (uint64_t tmp_object_id = start_object_id; tmp_object_id < start_object_id + object_count; tmp_object_id++)
    {
        char file_name[256] = {0};
        sprintf(file_name, "../data/file_%ld", tmp_object_id);
        if (!access(file_name, F_OK))
            unlink(file_name);
    }
    return 0;
}

int f_update_list(int n, uint64_t object_id, uint64_t object_count, uint64_t offset, uint64_t size)
{
    uint64_t start_object_id = object_id + n * object_count;
    for (uint64_t tmp_object_id = start_object_id; tmp_object_id < start_object_id + object_count; tmp_object_id++)
    {
        char file_name[256] = {0};
        sprintf(file_name, "../data/file_%ld", tmp_object_id);
        uint64_t ret = update_file(file_name, offset, size);
        if (ret < 0)
        {
            printf("update object_id:%ld failed!", tmp_object_id);
            return -1;
        }
    }
    return 0;
}

int f_multiupdate(uint64_t object_id, char **args)
{
    int ret = 0;
    for (int i = 0; args[i] != NULL; i += 2)
    {
        uint64_t offset = atol(args[i]);
        if (NULL == args[i + 1])
            break;
        uint64_t size = atol(args[i + 1]);
        ret = f_update(object_id, offset, size);
        if (ret < 0)
        {
            printf("f_update object_id:%ld offset: %ld size: %ld failed!", object_id, offset, size);
            return -1;
        }
    }
    return 0;
}

int f_multiaddwrite(uint64_t object_id, char **args)
{
    int ret = 0;
    for (int i = 0; args[i] != NULL; i++)
    {
        uint64_t size = atol(args[i]);
        ret = f_addwrite(object_id, size);
        if (ret < 0)
        {
            printf("f_addwrite object_id:%ld size: %ld failed!", object_id, size);
            return -1;
        }
    }
    return 0;
}
