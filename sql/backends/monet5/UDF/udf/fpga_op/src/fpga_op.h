#define MAL_SUCCEED ((str) 0) /* no error */
typedef char *str;

#ifdef __cplusplus
extern "C"{
#endif
str UDFdoubleit_(int *dst, const int *src);
#ifdef __cplusplus
}
#endif