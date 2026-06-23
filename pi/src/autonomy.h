#pragma once

#ifdef __cplusplus
extern "C" {
#endif


int autonomy_init(void);
const char *autonomy_next_command(void);
void autonomy_cleanup(void);


#ifdef __cplusplus
}
#endif
