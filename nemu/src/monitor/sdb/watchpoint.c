#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  char *EXPR;
  int Old_value,New_value,val;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }
  wp_pool[NR_WP - 1].next = NULL;
  head = NULL;
  free_ = wp_pool;
}

WP* find_wp(int N) {

	WP *wp = head;
	
	if (wp == NULL) {
		printf("No watchpoints. \n");
		return false;	
	}

	
	while(wp) {	
		if(wp->NO == N) break;
		else wp = wp->next;
	}

	return wp;
}

WP* find_pre_wp(WP *wp) {
	WP* pre_wp = head;
	
	while(pre_wp) {
		if (pre_wp->next == wp) break;
		else pre_wp = pre_wp->next;	
	}
	return pre_wp;

}

WP* new_wp(char *EXPR) {

	if (free_ == NULL) { 
		printf("there is no free wp!\n");
		return NULL;
	}

	bool success = true;
	expr(EXPR, &success);
	if (success == false) {
		printf("No symbol \"%s\" in current context. \n", EXPR);
		return NULL;	
	}

	WP* node = free_;
	free_ = free_->next;
	strcpy(node->EXPR, EXPR);

	node->next = head;
	head = node;
	return node;

}

bool free_wp(int N) {
	WP *wp = find_wp(N);
	
	WP *pre_wp = find_pre_wp(wp);
		
	if (wp == NULL) {
		printf("There is no %d watchpoint. \n", N);
		return false;
	}
	if (wp == head) {
		head = NULL;
	} else {
		pre_wp->next = wp->next;
	}
		wp->next =  free_;
		free_ = wp;
	
	printf("NO %d watchpoint has been deleted\n", N);
	return true; 
}

void watchpoint_display(int N) {

	WP *wp = find_wp(N);
	if (wp == NULL) {
		printf("No breakpoint number %d\n", N);
		return;
	} 

	printf("Num \t EXPR\n"); // TODO: complete this function 
	printf("%d\t%s\n", wp->NO, wp->EXPR);

}

void watchpoint_all_display() {
	
	WP *wp = head;
	if (wp == NULL) {	
		printf("No watchpoints\n");	
		return;
	} 

	printf("Num \t EXPR\n"); // TODO: complete this function
	
	while (wp) {		
		printf("%d\t%s\n", wp->NO, wp->EXPR);
		wp = wp->next;
	}
}

bool watchpoint_monitor(){
	
	WP *wp = head;
	if (wp == NULL) { 
		return false;
	} else {
		bool success = true;

		int New_value = expr(wp->EXPR, &success);
		if (success && New_value != wp->New_value) {			
			printf("watchpoint No.%d\n", wp->NO);	
			printf("watchpoint EXPR:\t%s\n", wp->EXPR);
			printf("watchpoint Old_value:\t%d\n", wp->Old_value);
			printf("watchpoint New_value:\t%d\n", New_value);
			wp->Old_value = wp->New_value;
			wp->New_value = New_value;
			return true;			
		}
		
		return false;

	}
		
}

/* TODO: Implement the functionality of watchpoint */

