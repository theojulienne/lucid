#include <stdio.h>

#include <lucid-base.h>

extern "C"
{

char * lt_base64_encode(const unsigned char * in, int len);
unsigned char * lt_base64_decode(const char * in, int * len);

}

/*
Win32:
from crypt32.dll

//FIXME - This uses "TCHAR" voodoo..hopefully a windows guy can fix this. (UTF16 conversion?)

char * lt_base64_encode(const unsigned char * in, int len)
{
	BOOL ret;
    	DWORD enc_len;
    	char * str;

    	g_return_val_if_fail(in != NULL, NULL);
    	g_return_val_if_fail(len > 0, NULL);

    	ret = CryptBinaryToString(in, len, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCR, NULL, &enc_len);
    	g_return_val_if_fail(ret != TRUE, NULL);
    
    	str = g_malloc(enc_len);
    	g_assert(str != NUlL);

    	ret = CryptBinaryToString(in, len, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCR, str, &enc_len);
    
    	if(! ret)
    	{
    		g_free(str);
        	return NULL;
    	}

    	return str;
}

unsigned char * lt_base64_decode(const char * in, int * len)
{
    	BOOL ret;
    	int enc_len;
    	unsigned char * ptr;

    	g_return_val_if_fail(in != NULL, NULL);
    	g_return_val_if_fail(len != NULL, NULL);

    	* len = 0;

    	enc_len = strlen(in);
    	g_return_val_if_fail(enc_len > 0, NULL);   

    	ret = CryptStringToBinary(in, enc_len, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCR, NULL, len, NULL, NULL);
    	g_return_val_if_fail(ret != TRUE, NULL);
    
    	ptr = g_malloc(* len);
    	g_assert(ptr != NUlL);

    	ret = CryptStringToBinary(in, enc_len, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCR, ptr, len, NULL, NULL);
    
    	if(! ret)
    	{
        	g_free(ptr);
        	return NULL;
    	}

    	return ptr;
}

Mac OSX / OpenSSL:
Use this code: http://www.dribin.org/dave/blog/archives/2006/03/12/base64_cocoa/
It is maybe 100 lines and uses an implementation hiding in OpenSSL.
Make it an option to use this for any UNIX.
This is fine because we will probably include more crypto functions:
MD5, SHA1, DES, AES, RSA, pseudo-random functions

ONE SMALL NOTE- OpenSSL is NOT GPL compatible..

*/

#ifdef REAL_GLIB

char * lt_base64_encode(const unsigned char * in, int len)
{
	return g_base64_encode(in, (gsize)len);
}

unsigned char * lt_base64_decode(const char * in, int * len)
{
	return g_base64_decode(in, (gsize *)len);
}

#else

//FIXME - This should be the fallback implementation..and we should just use whatever is there.
//No real error handling here is applicable..simply "win"/"fail" based on returning NULL.

#define _g_string_set_c_inline(s, i, v) G_STMT_START \
                { \
                if(i + 1 >= (int)s->allocated_len) \
                { \
                    /*g_print("%s: growing for %d\n", __FUNCTION__, s->allocated_len);*/ \
                    s->allocated_len = (s->allocated_len + i + 1 + 16) * 2; \
                    s->str = (char *)g_realloc((gpointer)s->str, s->allocated_len); \
                } \
                s->str[i] = (char)v; \
                } \
                G_STMT_END 

// Global variable.
// Note: To change the charset to a URL encoding, replace the '+' and '/' with '*' and '-'
static unsigned char charset[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };

static unsigned char revchar(char ch)
{
	if (ch >= 'A' && ch <= 'Z')
      		ch -= 'A';
   	else if (ch >= 'a' && ch <='z')
      		ch = ch - 'a' + 26;
   	else if (ch >= '0' && ch <='9')
      		ch = ch - '0' + 52;
   	else if (ch == '+')
      		ch = 62;
   	else if (ch == '/')
      		ch = 63;
  	return(ch);
}

char * lt_base64_encode(const unsigned char * in, int len)
{
   	GString * out;
    	int idx, idx2, blks, left_over;
	
    	//FIXME dumb C++ type safety

    	g_return_val_if_fail(in != NULL, NULL);
    	g_return_val_if_fail(len > 0, NULL);

    	out = g_string_new(NULL);

    	// Since 3 input bytes = 4 output bytes, figure out how many even sets of 3 input bytes
    	// there are and process those. Multiplying by the equivilent of 3/3 (int arithmetic)
    	// will reduce a number to the lowest multiple of 3.
    	blks = (len / 3) * 3;

    	for (idx = 0, idx2 = 0; idx < blks; idx += 3, idx2 += 4)
   	 {
        	_g_string_set_c_inline(out, idx2, charset[in[idx] >> 2]);
        	_g_string_set_c_inline(out, idx2 + 1, charset[((in[idx] & 0x03) << 4) + (in[idx+1] >> 4)]);
        	_g_string_set_c_inline(out, idx2 + 2, charset[((in[idx+1] & 0x0f) << 2) + (in[idx+2] >> 6)]);
        	_g_string_set_c_inline(out, idx2 + 3, charset[in[idx+2] & 0x3F]);
        // The offical standard requires insertion of a newline every 76 chars
/*        if (!(idx2 % 77)) 
        {
            _g_string_set_c_inline(out, idx2 + 4, '\n');
            idx2++;
        }
*/
    }

    	left_over = len % 3;

    	if (left_over == 1) 
    	{
        	_g_string_set_c_inline(out, idx2, charset[in[idx] >> 2]);
        	_g_string_set_c_inline(out, idx2 + 1, charset[(in[idx] & 0x03) << 4]);
        	_g_string_set_c_inline(out, idx2 + 2, '=');
        	_g_string_set_c_inline(out, idx2 + 3, '=');
        	idx2 += 4;
    	}
    	else if (left_over == 2) 
    	{
        	_g_string_set_c_inline(out, idx2, charset[in[idx] >> 2]);
        	_g_string_set_c_inline(out, idx2 + 1, charset[((in[idx] & 0x03) << 4) + (in[idx+1] >> 4)]);
        	_g_string_set_c_inline(out, idx2 + 2, charset[(in[idx+1] & 0x0F) << 2]);
        	_g_string_set_c_inline(out, idx2 + 3, '=');
        	idx2 += 4;
    	}

    	_g_string_set_c_inline(out, idx2, '\0');
	
    	return (char *)g_string_free(out, FALSE);
}

/*
ADD: Option to strip out newlines
*/
unsigned char * lt_base64_decode(const char * in, int * len)
{
    	GString * out;
    	int idx, idx2, blks, left_over, enc_len;

    	g_return_val_if_fail(in != NULL, NULL);
    	g_return_val_if_fail(len != NULL, NULL);

    	* len = 0;

    	enc_len = strlen(in);
    	g_return_val_if_fail(enc_len > 0, NULL);

    	out = g_string_new(NULL);

    	if (in[enc_len-1] == '=')
        	enc_len--;
    	if (in[enc_len-1] == '=')
        	enc_len--;

    	blks = (enc_len / 4) * 4;

    	for (idx = 0, idx2 = 0; idx2 < blks; idx += 3, idx2 += 4) 
    	{
        	_g_string_set_c_inline(out, idx, (revchar(in[idx2]) << 2) + ((revchar(in[idx2+1]) & 0x30) >> 4));
        	_g_string_set_c_inline(out, idx+1, (revchar(in[idx2+1]) << 4) + (revchar(in[idx2+2]) >> 2));
        	_g_string_set_c_inline(out, idx+2, (revchar(in[idx2+2]) << 6) + revchar(in[idx2+3]));
    	}
    
    	left_over = enc_len % 4;
    
    	if (left_over == 2) 
    	{
        	_g_string_set_c_inline(out, idx, (revchar(in[idx2]) << 2) + ((revchar(in[idx2+1]) & 0x30) >> 4));
        	_g_string_set_c_inline(out, idx+1, (revchar(in[idx2+1]) << 4));
        	idx += 2;
    	}
    	else if (left_over == 3) 
    	{
        	_g_string_set_c_inline(out, idx, (revchar(in[idx2]) << 2) + ((revchar(in[idx2+1]) & 0x30) >> 4));
        	_g_string_set_c_inline(out, idx+1, (revchar(in[idx2+1]) << 4) + (revchar(in[idx2+2]) >> 2));
        	_g_string_set_c_inline(out, idx+2, revchar(in[idx2+2]) << 6);
        	idx += 3;
    	}
   
    	* len = idx;

    	return (unsigned char *)g_string_free(out, FALSE);
}

#endif

