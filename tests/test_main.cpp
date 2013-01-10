#include <stdio.h>

#include "../rcp_pch.h"
#include "../rcp_utility.h"

int test_json(void);
int test_tree(void);
int test_array(void);
int test_struct(void);
int test_user(void);
int test_at(void);

int main (int argc, const char** argv){

	test_json();
	test_tree();
	test_array();
	test_user();
	test_at();
}
