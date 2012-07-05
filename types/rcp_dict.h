#include "../def/rcp_dict.h"
#include "../def/rcp_dict_node.h"
#include "../def/rcp_type.h"
#include "../def/rcp_data.h"


///
//type map
//

rcp_type_ref rcp_dict_type_new(
		rcp_type_ref key_type, rcp_type_ref data_type);
void rcp_dict_type_delete(
		rcp_type_ref type);

rcp_type_ref rcp_dict_type_key_type(rcp_type_ref type);
rcp_type_ref rcp_dict_type_data_type(rcp_type_ref type);

///
//map itself
//

void rcp_dict_init(rcp_type_ref type, rcp_data_ref data);
void rcp_dict_deinit(rcp_type_ref type, rcp_data_ref data);

rcp_data_ref rcp_dict_at(rcp_type_ref type, rcp_data_ref data,
		rcp_data_ref key);

rcp_data_ref rcp_dict_set(rcp_type_ref type, rcp_data_ref data,
		rcp_record_ref rec);
rcp_data_ref rcp_dict_unset(rcp_type_ref type, rcp_data_ref data,
		rcp_record_ref rec);

rcp_extern 
rcp_dict_node_ref rcp_dict_find(
		rcp_dict_ref map, rcp_data_ref key);

rcp_extern 
rcp_dict_node_ref rcp_dict_set_node(
		rcp_dict_ref map, rcp_dict_node_ref node);

rcp_extern 
void rcp_dict_unset_node(rcp_dict_ref map, rcp_dict_node_ref node);

///
//map node
//
rcp_extern 
rcp_dict_node_ref rcp_dict_node_alloc(rcp_type_ref type);

rcp_extern 
void rcp_dict_node_deallec(rcp_dict_node_ref node);

rcp_extern 
void rcp_dict_node_init(rcp_type_ref type, rcp_dict_node_ref node);

rcp_extern 
void rcp_dict_node_deinit(rcp_type_ref type, rcp_dict_node_ref node);

rcp_extern 
rcp_dict_node_ref rcp_dict_node_new(rcp_type_ref type);

rcp_extern 
void rcp_dict_node_delete(rcp_type_ref type, rcp_dict_node_ref node);

rcp_extern 
rcp_data_ref rcp_dict_node_key(
		rcp_type_ref type, rcp_dict_node_ref node);

rcp_extern 
rcp_data_ref rcp_dict_node_data(
		rcp_type_ref type, rcp_dict_node_ref node);

///
//iterate
rcp_extern 
rcp_dict_node_ref rcp_dict_begin(rcp_dict_ref map);

rcp_extern 
rcp_dict_node_ref rcp_dict_node_next(rcp_dict_node_ref node);

