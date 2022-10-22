
#include "list.h"

int main() {

    list_t* list = list_init();
    list_append(list, 1);
    list_append(list, 2);
    list_append(list, 3);
    list_append(list, 4);
    list_dump(list);
    list_remove(list, 2);
    list_dump(list);
    return 0;
}