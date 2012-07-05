#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../types/rcp_number.h"
#include "../rcp_tree.h"

#include <random>

typedef int(*cmp)(const void*, void*, void*);

int test_tree(void){

	rcp_tree_ref tree = rcp_tree_new((cmp)rcp_uint32_compare, NULL);
	std::mt19937 eng(time(NULL));
	std::uniform_int_distribution<int> dist(0,500);

	//add
	for (int i = 0; i<200; i++){
		uint32_t dat = dist(eng);
		rcp_tree_node_ref node = rcp_tree_find(tree, &dat);
		uint32_t *p = (uint32_t*)rcp_tree_node_data(node);
		if (node != NULL){
			if (*p != dat)
				rcp_error("it's not mine...");
			continue;
		}
		node = rcp_tree_node_new(sizeof (uint32_t));
		p = (uint32_t*)rcp_tree_node_data(node);
		*p = dat;
		rcp_tree_add(tree, node);
		node = rcp_tree_find(tree, &dat);
		if (node == NULL){
			rcp_error("good bye, my data...");
			rcp_tree_verify(tree);
			continue;
		}
		p = (uint32_t*)rcp_tree_node_data(node);
		if (*p != dat)
			rcp_error("it's not mine...");
		rcp_tree_verify(tree);
		//rcp_info("ok");
	}
	rcp_tree_delete(tree);

	//iterate
	tree = rcp_tree_new((cmp)rcp_uint32_compare, NULL);
	for (int i = 0; i<200; i++){
		uint32_t dat = i;
		rcp_tree_node_ref node = rcp_tree_node_new(sizeof (uint32_t));
		uint32_t *p = (uint32_t*)rcp_tree_node_data(node);
		*p = dat;
		rcp_tree_add(tree, node);
	}
	rcp_tree_node_ref itr = rcp_tree_begin(tree);
	bool frag[200];
	for (int i = 0; i<200; i++){
		frag[i] = false;
	}
	while (itr){
		uint32_t *p = (uint32_t*)rcp_tree_node_data(itr);
		frag[*p] = true;
		itr = rcp_tree_node_next(itr);
	}
	for (int i = 0; i<200; i++){
		if (not frag[i])
			rcp_error("map:itr");
	}

	//remove
	for (int i = 0; i<200; i++){
		uint32_t dat = dist(eng);
		rcp_tree_node_ref node = rcp_tree_find(tree, &dat);
		uint32_t *p = (uint32_t*)rcp_tree_node_data(node);
		if (node != NULL){
			if (*p != dat)
				rcp_error("it's not mine...");
			continue;
		}
		node = rcp_tree_node_new(sizeof (uint32_t));
		p = (uint32_t*)rcp_tree_node_data(node);
		*p = dat;
		rcp_tree_add(tree, node);
		node = rcp_tree_find(tree, &dat);
		if (node == NULL){
			rcp_error("good bye, my data...");
			rcp_tree_verify(tree);
			continue;
		}
		p = (uint32_t*)rcp_tree_node_data(node);
		if (*p != dat)
			rcp_error("it's not mine...");
		rcp_tree_verify(tree);
		//rcp_info("ok");
	}
	rcp_tree_delete(tree);

	//more itr
	tree = rcp_tree_new((cmp)rcp_uint32_compare,NULL);
	{
		uint32_t dat = 1;
		rcp_tree_node_ref node = rcp_tree_node_new(sizeof (uint32_t));
		uint32_t *p = (uint32_t*)rcp_tree_node_data(node);
		*p = dat;
		rcp_tree_add(tree, node);
	}
	rcp_tree_node_next(rcp_tree_begin(tree));

	//iterate _r
	{
		tree = rcp_tree_new((cmp)rcp_uint32_compare,NULL);
		for (int i = 0; i<200; i++){
			uint32_t dat = 199-i;
			rcp_tree_node_ref node = rcp_tree_node_new(sizeof (uint32_t));
			uint32_t *p = (uint32_t*)rcp_tree_node_data(node);
			*p = dat;
			rcp_tree_add(tree, node);
		}
		rcp_tree_node_ref itr = rcp_tree_begin(tree);
		bool frag[200];
		for (int i = 0; i<200; i++){
			frag[i] = false;
		}
		while (itr){
			uint32_t *p = (uint32_t*)rcp_tree_node_data(itr);
			frag[*p] = true;
			itr = rcp_tree_node_next(itr);
		}
		rcp_tree_verify(tree);
		for (int i = 0; i<200; i++){
			if (not frag[i])
				rcp_error("map:itr_r");
		}
	}

	//rm node
	for (int i = 0; i<50; i++){
		tree = rcp_tree_new((cmp)rcp_uint32_compare,NULL);
		for (int i = 0; i<200; i++){
			uint32_t dat = 199-i;
			rcp_tree_node_ref node = rcp_tree_node_new(sizeof (uint32_t));
			uint32_t *p = (uint32_t*)rcp_tree_node_data(node);
			*p = dat;
			rcp_tree_add(tree, node);
		}

		for (int i = 0; i<50; i++){

			int count_b = 0;
			rcp_tree_node_ref itr = rcp_tree_begin(tree);
			while (itr){
				count_b++;
				itr = rcp_tree_node_next(itr);
			}

			uint32_t dat = dist(eng);
			rcp_tree_node_ref node = rcp_tree_find(tree, &dat);
			if (!node)
				continue;
			rcp_tree_remove(tree, node);
			rcp_tree_verify(tree);
			int count_a = 0;
			itr = rcp_tree_begin(tree);
			while (itr){
				count_a++;
				itr = rcp_tree_node_next(itr);
			}
			node = rcp_tree_find(tree, &dat);
			//printf("%i,\n",count_a-count_b);
			if (node){
				rcp_error("why are you there");
				//printf("%i\n",dat);
			}
		}
	}
	rcp_info("tree done");
	return 0;
}
