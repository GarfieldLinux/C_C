#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#include <iconv.h>
 
static unsigned char hexchars[] = "0123456789ABCDEF";
/**
 * 16进制数转换成10进制数
 * 如：0xE4=14*16+4=228
 */
static int vdn_htoi(char *s)
{
    int value;
    int c;

    c = ((unsigned char *)s)[0];
    if (isupper(c))
        c = tolower(c);
    value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

    c = ((unsigned char *)s)[1];
    if (isupper(c))
        c = tolower(c);
    value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

    return (value);
}


char *vdn_url_encode(char const *s, int len, int *new_length)
{
    register unsigned char c;
    unsigned char *to, *start;
    unsigned char const *from, *end;
    
    from = (unsigned char *)s;
    end  = (unsigned char *)s + len;
    start = to = (unsigned char *) calloc(1, 3*len+1);

    while (from < end) 
    {
        c = *from++;
//空格转为+  非%20?
        if (c == ' ') 
        {
            *to++ = '+';
        } 
        else if ((c < '0' && c != '-' && c != '.' && c != '/') ||
                 (c < 'A' && c > '9') ||
                 (c > 'Z' && c < 'a' && c != '_') ||
                 (c > 'z')) 
        {
            to[0] = '%';
            to[1] = hexchars[c >> 4];//将2进制转换成16进制表示
            to[2] = hexchars[c & 15];//将2进制转换成16进制表示
            to += 3;
        }
        else 
        {
            *to++ = c;
        }
    }
    *to = 0;
    if (new_length) 
    {
        *new_length = to - start;
    }
    return (char *) start;
}


int vdn_url_decode(char *str, int len)
{
    char *dest = str;
    char *data = str;

    while (len--) 
    {
        if (*data == '+') 
        {
            *dest = ' ';
        }
        else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) && isxdigit((int) *(data + 2))) 
        {
            *dest = (char) vdn_htoi(data + 1);
            data += 2;
            len -= 2;
        } 
        else 
        {
            *dest = *data;
        }
        data++;
        dest++;
    }
    *dest = '\0';
    return dest - str;
}

int code_convert(const char *from_charset, const char *to_charset, char *inbuf,
		size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;
 
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) != 0)
		return -1;
	iconv_close(cd);
	return 0;
}
 
int u2g(char *inbuf, int inlen, char *outbuf, int outlen)
{
	return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
}
 
int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
}
 

int main(int argc, char ** argv)
{
	const char* utf8 = "www.garfieldvdn.com/测试.pn g";
	char buff[1024] = { 0 };
	u2g((char*) utf8, strlen(utf8), buff, sizeof(buff));
	printf("UTF8   字符集:%s\n", utf8);
	printf("gb2312 字符集:%s\n\n", buff);

//char *str = "www.baidu.com/爱宝的妍",*new_str;
const char *str = utf8,*new_str;

    int len = 0,new_len = 0,old_len = 0;
    len = strlen(str);
    new_str = vdn_url_encode(str,len,&new_len);
    printf("utf8 urlencode : %s,new length : %d\n",new_str,new_len);
    old_len = vdn_url_decode(new_str,new_len);
    printf("utf8 old string : %s,old length : %d\n\n",new_str,old_len);

    str = buff;
    len = 0,new_len = 0,old_len = 0;
    len = strlen(str);
    new_str = vdn_url_encode(str,len,&new_len);
    printf("gb2312 urlencode : %s,new length : %d\n",new_str,new_len);
    old_len = vdn_url_decode(new_str,new_len);
    printf("gb2312 old string : %s,old length : %d\n",new_str,old_len);




    return 0;
}
