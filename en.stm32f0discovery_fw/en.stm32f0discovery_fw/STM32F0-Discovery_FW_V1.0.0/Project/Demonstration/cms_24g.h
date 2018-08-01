#ifndef CMS_24G_H
#define CMS_24G_H

#include "cms_spi.h"
#include "userConfig.h"

void rf_init(void);
void rf_send(u8 *data, u8 num);
void rf_receive(void);
#endif

