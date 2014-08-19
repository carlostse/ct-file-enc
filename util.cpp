#include "util.h"
#include <sys/stat.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#ifdef WIN32
#include "windows.h"
#endif

std::string Util::hexString(byte *b, uint32_t len)
{
    if (!b)
        return "";

    char *hex = (char *)malloc(len * 2 + 1);
    for (uint i = 0; i < len; i++){
        sprintf(&hex[i * 2], "%02x", b[i]);
    }
    hex[len * 2] = 0;
    std::string result = std::string(hex);
    free(hex);
    return result;
}

std::string Util::hexString(ByteArray *array)
{
    return hexString(array->data(), array->size());
}

byte *Util::key(std::string str)
{
    byte *digest = new byte[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)str.c_str(), str.length(), digest);
    return digest;
}

byte *Util::iv(std::string str)
{
    byte *digest = new byte[MD5_DIGEST_LENGTH];
    MD5((const unsigned char *)str.c_str(), str.length(), digest);
    return digest;
}

bool Util::isFileExists(const char *fileName)
{
    struct stat buf;
    return stat(fileName, &buf) != -1;
}

ByteArray *Util::readFile(const char* fileName)
{
    if (!isFileExists(fileName)){
        std::cout << fileName << " does not exists" << std::endl;
        return NULL;
    }

    std::ifstream file(fileName);
    file.seekg(0, std::ios::end);
    size_t len = file.tellg();
    file.seekg(0, std::ios::beg);

    if (len < 1){
        std::cout << fileName << " empty" << std::endl;
        return NULL;
    }

    ByteArray *array = new ByteArray(len);
    file.read((char *)array->data(), len);
    file.close();

    return array;
}

void Util::writeFile(const char* fileName, ByteArray *data)
{
    std::fstream file(fileName, std::ios::out | std::ios::binary);
    file.seekp(0);
    file.write ((char*)data->data(), data->size());
    file.close();
}

int Util::lastIndexOf(const char *str, char c)
{
    int len = strlen(str);
    while (len >= 0){
        if (str[len] == c){
            return len;
        }
        len--;
    }
    return -1;
}

std::string Util::getExeFileName()
{
#ifdef WIN32
    wchar_t wc[MAX_PATH];
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, wc, MAX_PATH);
    WideCharToMultiByte(CP_UTF8, 0, wc, MAX_PATH, buffer, MAX_PATH, 0, 0);
    return std::string(buffer);
#endif
}

std::string Util::getExePath()
{
#ifdef WIN32
    std::string f = getExeFileName();
    return f.substr(0, f.find_last_of("\\/"));
#endif
}

