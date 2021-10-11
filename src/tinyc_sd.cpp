#include "tinyc.h"
#include <SD.h>

/* Globals.
 */
 
static SDLib::SDClass *sd=0;

/* Initialize if necessary.
 */
 
static int8_t tinyc_sd_require() {
  if (sd) return 0;
  if (!(sd=new SDLib::SDClass())) return -1;
  if (!sd->begin()) {
    delete sd;
    sd=0;
    return -1;
  }
  return 0;
}

/* Read file.
 */
 
int32_t tinyc_file_read(void *dst,int32_t dsta,const char *path) {
  if (dsta>0xffff) dsta=0xffff;
  if (tinyc_sd_require()<0) return -1;
  File file=sd->open(path);
  if (!file) return -1;
  int32_t dstc=file.read(dst,dsta);
  file.close();
  return dstc;
}

/* Write file.
 */
 
int8_t tinyc_file_write(const char *path,const void *src,int32_t srcc) {
  if (srcc<0) return -1;
  if (tinyc_sd_require()<0) return -1;
  File file=sd->open(path,O_RDWR|O_CREAT|O_TRUNC);
  if (!file) return -1;
  size_t err=file.write((const uint8_t*)src,(size_t)srcc);
  file.close();
  if (err<0) return -1;
  return 0;
}
