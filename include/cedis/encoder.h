#ifndef _CEDIS_ENCODER_H_
#define _CEDIS_ENCODER_H_

char *resp_simple_string_encode(const char *data);
char *resp_bulk_string_encode(const char **data);
char *resp_error_encode(const char *data);

#endif
