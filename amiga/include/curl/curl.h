/* Critical Mass for AmigaOS: the libcurl calls game/OnlineUpdate.cpp makes,
 * as a stub with no network: curl_easy_init gives no handle, so the game's
 * online check fails at once and it uses its update cache, as it does
 * when the Internet is not there. The AmiCradle Team. GPL 2 or later, like
 * Critical Mass. */
#ifndef AMIGA_CURL_STUB_H
#define AMIGA_CURL_STUB_H
typedef void CURL;
typedef enum { CURLE_OK = 0, CURLE_FAILED_INIT = 2 } CURLcode;
typedef enum {
	CURLOPT_WRITEDATA = 10001,
	CURLOPT_URL = 10002,
	CURLOPT_USERAGENT = 10018,
	CURLOPT_FOLLOWLOCATION = 52,
	CURLOPT_TIMEOUT = 13,
	CURLOPT_WRITEFUNCTION = 20011
} CURLoption;
static inline CURL *curl_easy_init(void) { return 0; }
#define curl_easy_setopt(handle, option, ...) ((void)(handle), CURLE_FAILED_INIT)
static inline CURLcode curl_easy_perform(CURL *handle) { (void)handle; return CURLE_FAILED_INIT; }
static inline void curl_easy_cleanup(CURL *handle) { (void)handle; }
#endif
