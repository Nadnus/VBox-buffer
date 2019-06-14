#include <linux/kernel.h>
#include <linux/page-flags.h>
#include <linux/mmzone.h>
#include <linux/mm_types.h>

unsigned long swapped, evicted;

void init_swapps(void) {
    swapped = evicted = 0;
}

asmlinkage char *stats(void) {
	enum lru_list lru_list;
	struct zone *current_zone;
	struct page *current_page;
	struct list_head list_ptr;
	unsigned int active_pages = 0;
	unsigned int inactive_pages = 0;
	unsigned int active_ref = 0;
	unsigned int inactive_ref = 0;
	char *myArr = vmalloc(1000 * sizeof(char));
	char *myPtr = myArr;


	for_each_zone(current_zone) {
		for_each_lru(lru_list) {
			list_ptr = current_zone->zone_pgdat->lruvec.lists[lru_list]; 

			list_for_each_entry(current_page, &list_ptr, lru) {
    			if (is_active_lru(lru_list)) {
				active_pages ++;
    				if (PageReferenced(current_page)) active_ref ++;
    			}
    			
    			else {
				inactive_pages ++;
    				if (PageReferenced(current_page)) inactive_ref ++;
    			}
			}
		}

	}

	myPtr += sprintf(myPtr, "active pages:%u\n", active_pages);
	myPtr += sprintf(myPtr, "inactive pages:%u\n", inactive_pages);
	myPtr += sprintf(myPtr, "active references:%u\n", active_ref);
	myPtr += sprintf(myPtr, "inactive references:%u\n", inactive_ref);
//	myPtr += sprintf(myPtr, "swapped:%u\n", swapped);
//	myPtr += sprintf(myPtr, "evicted:%u\n", evicted);
	
	return myArr;
}

