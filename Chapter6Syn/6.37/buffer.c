#include "buffer.h"

int insert_item(buffer_item item) {
  if (count == BUFFER_SIZE)
    return 0;
  else {
    buffer[in] = item;
    in = (in+1) % BUFFER_SIZE;
    count++;
    return 1;
  }
  
}

int remove_item(buffer_item *item) {
  if (count == 0)
    return 0;
  else {
    *item = buffer[out];
    out = (out+1) % BUFFER_SIZE;
    count--;
    return 1;
  }

}
