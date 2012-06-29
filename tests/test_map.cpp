#include "../rcp_pch.h"
#include "../rcp_utility.h"
#include "../rcp_types.h"

#include <random>

int test_map(void){

	std::mt19937 eng(time(NULL));
	std::uniform_int_distribution<int> dist(0,500);
	
	rcp_map_ref map = rcp_map_new(rcp_string_type, rcp_uint32_type);

	{
		rcp_map_node_ref node = rcp_map_node_new(map);
		rcp_string_init_with_c_str(
				rcp_map_node_key(map, node),"val");
		uint32_t *val = (uint32_t*)rcp_map_node_value(map, node);
		*val = 3200;

		rcp_map_set(map, node);
	}

	{
		rcp_string_ref key = rcp_string_new("test");
		rcp_map_node_ref node = rcp_map_find(map, key);
		rcp_delete(rcp_string_type, key);
		if (node)
			rcp_error("found sometihng strange");
	}

	{
		rcp_map_node_ref node = rcp_map_node_new(map);
		uint32_t *val = (uint32_t*)rcp_map_node_value(map, node);
		rcp_string_init_with_c_str(
				rcp_map_node_key(map, node),"test");
		*val = 100;
		rcp_map_set(map, node);
	}

	{
		rcp_string_ref key = rcp_string_new("test");
		rcp_map_node_ref node = rcp_map_find(map, key);
		uint32_t *val = (uint32_t*)rcp_map_node_value(map, node);
		if (!val)
			rcp_error("lost?");
		if (*val != 100)
			rcp_error("isn't this");
		//rcp_string_ref *rkey = (rcp_string_ref*)rcp_map_node_key(map, node);
		//if (rcp_string_type.compare(rkey,&key) !=0)
		//	rcp_error("key mismatch");
	}
	rcp_info("map done");

	return 0;
}
