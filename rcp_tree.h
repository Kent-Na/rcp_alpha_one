//key:str value:any tree


typedef struct rcp_tree *rcp_tree_ref;
typedef struct rcp_tree_node_core *rcp_tree_node_ref;

rcp_extern rcp_tree_node_ref rcp_tree_node_new(size_t size);
rcp_extern void rcp_tree_node_delete(rcp_tree_node_ref node);
rcp_extern void *rcp_tree_node_data(rcp_tree_node_ref node);
rcp_extern rcp_tree_node_ref rcp_tree_node_next(rcp_tree_node_ref node);

rcp_extern rcp_tree_ref rcp_tree_new(int(*compare)(void*,void*));
rcp_extern void rcp_tree_verify(rcp_tree_ref tree);
rcp_extern void rcp_tree_delete(rcp_tree_ref tree);
rcp_extern rcp_tree_node_ref rcp_tree_find(rcp_tree_ref tree, void *key);
rcp_extern rcp_tree_node_ref rcp_tree_root(rcp_tree_ref tree);
rcp_extern int rcp_tree_add(rcp_tree_ref tree, rcp_tree_node_ref node);
rcp_extern int rcp_tree_remove(rcp_tree_ref tree, rcp_tree_node_ref node);
