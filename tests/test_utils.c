#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Unity/src/unity.h"

char *url_decode(const char *src);
const char *get_file_extension(const char *file_name);
const char *get_mime_type(const char *file_ext);

void setUp(void) {}

void tearDown(void) {}

void test_url_decode(void)
{
    // Test case: Basic URL decoding
    const char *url_encoded = "Hello%20World";
    char *decoded = url_decode(url_encoded);
    TEST_ASSERT_EQUAL_STRING("Hello World", decoded);
    free(decoded);
}

void test_get_file_extension(void)
{
    // Test case 1: File with extension
    const char *file_name_with_ext = "example.txt";
    const char *extension = get_file_extension(file_name_with_ext);
    TEST_ASSERT_EQUAL_STRING("txt", extension);

    // Test case 2: File without extension
    const char *file_name_without_ext = "example";
    extension = get_file_extension(file_name_without_ext);
    TEST_ASSERT_EQUAL_STRING("", extension);
}

void test_get_mime_type(void)
{
    // Test case 1: Known file extension
    const char *file_ext_html = "html";
    const char *mime_type_html = get_mime_type(file_ext_html);
    TEST_ASSERT_EQUAL_STRING("text/html", mime_type_html);

    // Test case 2: Unknown file extension
    const char *file_ext_unknown = "xyz";
    const char *mime_type_unknown = get_mime_type(file_ext_unknown);
    TEST_ASSERT_EQUAL_STRING("application/octet-stream", mime_type_unknown);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_url_decode);
    RUN_TEST(test_get_file_extension);
    RUN_TEST(test_get_mime_type);

    return UNITY_END();
}
