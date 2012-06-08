#include "rcp_pch.h"
#include "rcp_defines.h"
#include "rcp_utility.h"

#define RCP_INTERNAL_STRUCTURE

#include "rcp_tree.h"

#define RCP_TREE_BRACK 0
#define RCP_TREE_RED 1

rcp_extern rcp_tree_node_ref rcp_tree_node_new(size_t size)
{
	struct rcp_tree_node_core *node=malloc(size + sizeof *node);
	node->l = NULL;
	node->r = NULL;
	node->p = NULL;
	node->color = RCP_TREE_RED;
	return node;
}

rcp_extern void rcp_tree_node_delete(rcp_tree_node_ref node){
	free(node);
}

rcp_extern void *rcp_tree_node_data(rcp_tree_node_ref node)
{
	struct rcp_tree_node_core *core= node;
	if (core)
		return core + 1;
	return NULL;
}

rcp_tree_node_ref rcp_tree_node_next(rcp_tree_node_ref node)
{

	if (node->r){
		node = node->r;
		while (node->l)
			node = node->l;
		return node;
	}
	else {
		while (node->p && node->p->l != node)
			node = node->p;
		return node->p;
	}

	return NULL;
}

void rcp_tree_node_replace(rcp_tree_node_ref src, rcp_tree_node_ref dst){
	dst->l = src->l;
	dst->r = src->r;
	dst->p = src->p;
	if (dst->l)
		dst->l->p = dst;
	if (dst->r)
		dst->r->p = dst;
	if (dst->p){
		if (dst->p->r == src)
			dst->p->r = dst;
		else
			dst->p->l = dst;
	}
}

rcp_extern void rcp_tree_init(
		rcp_tree_ref tree, 
		int(*const compare)(void const*,void*,void*), void* extra_data)
{
	tree->root = NULL;
	tree->compare = compare;
	tree->extra_data = extra_data;
}
rcp_extern rcp_tree_ref rcp_tree_new(
		int(*compare)(const void*,void*,void*),void *extra_data)
{
	rcp_tree_ref tree = malloc(sizeof *tree);
	rcp_tree_init(tree, compare, extra_data);
	return tree;
}

void rcp_tree_delete_node(rcp_tree_node_ref node){
	if (node->l)
		rcp_tree_delete_node(node->l);
	if (node->r)
		rcp_tree_delete_node(node->r);
	rcp_tree_node_delete(node);
}

rcp_extern void rcp_tree_deinit(rcp_tree_ref tree)
{
	if (tree->root)
		rcp_tree_delete_node(tree->root);
}

rcp_extern void rcp_tree_delete(rcp_tree_ref tree)
{
	rcp_tree_deinit(tree);
	free(tree);
}

rcp_extern rcp_tree_node_ref rcp_tree_begin(rcp_tree_ref tree)
{
	rcp_tree_node_ref node = tree->root;
	while (node && node->l)
		node = node->l;
	return node;
	return tree->root;
}

rcp_extern rcp_tree_node_ref rcp_tree_find(rcp_tree_ref tree, void *key)
{
	if (!tree)
		return NULL;
	struct rcp_tree_node_core *cur = tree->root;
	while (1){
		if (cur == NULL)
			return NULL;
		int cmp = tree->compare(tree->extra_data,
				key, rcp_tree_node_data(cur));
		if (cmp < 0){
			cur = cur->l;
		}
		else if (cmp > 0){
			cur = cur->r;
		}
		else{
			return cur;
		}
	}
}

rcp_extern void rcp_tree_node_verify(rcp_tree_ref tree,
		rcp_tree_node_ref node, int b_depth, int c_b_depth)
{
	if (node->color == RCP_TREE_RED){
		if (node->l && node->l->color == RCP_TREE_RED)
			rcp_error("tree:red have red l");
		if (node->r && node->r->color == RCP_TREE_RED)
			rcp_error("tree:red have red r");
	}
	void *ex = tree->extra_data;
	void *dat = rcp_tree_node_data(node);
	//	
	void *ldat;
	if (node->l)
		ldat = rcp_tree_node_data(node->l);
	if (node->l && ! (tree->compare(ex,ldat,dat)<0))
		rcp_error("tree:comp l");

	void *rdat;
	if (node->r)
		rdat = rcp_tree_node_data(node->r);
	if (node->r && ! (tree->compare(ex,dat,rdat)<0))
		rcp_error("tree:comp r");
	//
	if (node->l)
		if (node->l->p != node)
			rcp_error("tree:p l");
	if (node->r)
		if (node->r->p != node)
			rcp_error("tree:p r");
	//
	int black_depth = c_b_depth;
	if (node->color == RCP_TREE_BRACK)
		black_depth ++;

	if (! node->l)
		if (black_depth + 1 != b_depth)
			rcp_error("tree:b_depth l");

	if (! node->r)
		if (black_depth + 1 != b_depth)
			rcp_error("tree:b_depth r");
	//

	if (node->l)
		rcp_tree_node_verify(tree, node->l, b_depth, black_depth);

	if (node->r)
		rcp_tree_node_verify(tree, node->r, b_depth, black_depth);
}

rcp_extern void rcp_tree_verify(rcp_tree_ref tree)
{
	struct rcp_tree_node_core *cur = tree->root;
	if (cur->color == RCP_TREE_RED)
		rcp_error("tree:red root");

	struct rcp_tree_node_core *l= tree->root;
	int black_depth = 1;//count root node

	while (l->l){
		l = l->l;
		if (l->color == RCP_TREE_BRACK)
			black_depth ++;
	}
	//count last NULL node
	black_depth ++;

	rcp_tree_node_verify(tree, cur, black_depth, 0);
}

rcp_extern rcp_tree_node_ref rcp_tree_put(
		rcp_tree_ref tree, rcp_tree_node_ref node, int replace)
{
	if (!tree){
		return node;
	}
	struct rcp_tree_node_core *cur = tree->root;
	struct rcp_tree_node_core *new = node;

#ifdef RCP_SELF_TEST
	if (new->l != NULL || new->r != NULL ||
			new->p != NULL || new->color != RCP_TREE_RED){
		rcp_error("tree_push");
	}
#endif

	if (tree->root == NULL){
		new->color = RCP_TREE_BRACK;
		tree->root = new;
		return NULL;
	}
	new->color = RCP_TREE_RED;

	//add to tree
	void *key = rcp_tree_node_data(node);
	while (1){
		int cmp = tree->compare(tree->extra_data,
				key, rcp_tree_node_data(cur));
		if (cmp < 0){
			if (cur->l == NULL){
				cur->l = new; 
				new->p = cur;
				break;
			}
			cur = cur->l;
		}
		else if (cmp > 0){
			if (cur->r == NULL){
				cur->r = new; 
				new->p = cur;
				break;
			}
			cur = cur->r;
		}
		else{
			if (replace){
				rcp_tree_node_replace(cur, node);
				return cur;
			}
			else{
				rcp_error("tree:key");
				return node;
			}
		}
	}

	//repaint
	struct rcp_tree_node_core *t = new;
	while (1){
		struct rcp_tree_node_core *p = t->p; 
		if (p == NULL){
			t->color = RCP_TREE_BRACK;
			return NULL;
		}
		if (p->color == RCP_TREE_BRACK){
			return NULL;
		}
		//p is red, g->l or g->r is red
		struct rcp_tree_node_core *g = p->p; 
		if ((g->l && g->r) && g->l->color == g->r->color){
			g->color = RCP_TREE_RED;
			g->l->color = RCP_TREE_BRACK;
			g->r->color = RCP_TREE_BRACK;
			t = g;
			continue;
		}
		
		struct rcp_tree_node_core *mp = g->p; 
		struct rcp_tree_node_core *m; 
		struct rcp_tree_node_core *l; 
		struct rcp_tree_node_core *r; 

		if(g->l == p){
			if (p->l == t){
				m = p;
				l = t;
				r = g;

				g->l = p->r;
			}
			else{
				m = t;
				l = p;
				r = g;

				g->l = t->r;
				p->r = t->l;
			}
		}
		else{
			if (p->r == t){
				m = p;
				l = g;
				r = t;

				g->r = p->l;
			}
			else{
				m = t;
				l = g;
				r = p;

				g->r = t->l;
				p->l = t->r;
			}
		}
		l->p = m;
		r->p = m;
		if (l->l)
			l->l->p = l;
		if (l->r)
			l->r->p = l;
		if (r->l)
			r->l->p = r;
		if (r->r)
			r->r->p = r;
		m->l = l;
		m->r = r;
		m->p = mp;
		m->color = RCP_TREE_BRACK;
		l->color = RCP_TREE_RED;
		r->color = RCP_TREE_RED;
		if (mp == NULL)
			tree->root = m;
		else if (mp->l == g)
			mp->l = m;
		else 
			mp->r = m;
		return NULL;
	}
}

rcp_extern int rcp_tree_remove(rcp_tree_ref tree, rcp_tree_node_ref node)
{
	if (node == NULL){
		rcp_error("tree: no key");
		return -1;
	}

	//find replace target
	struct rcp_tree_node_core *rep = node->l;
	while (1)
		if (rep->r)
			rep = rep->r;

	struct rcp_tree_node_core *t = node;

	if (t->color == RCP_TREE_RED){
#ifdef RCP_SELF_TEST
		if (t->l || t->r){
			rcp_error("tree:rem");
			return -1;
		}
#endif
		if (t->p->l == t)
			t->p->l = NULL;
		if (t->p->r == t)
			t->p->r = NULL;
	}
	else while (1){
		//t is black
		struct rcp_tree_node_core *p = t->p; 
		if (p == NULL){
			return 0;
		}
		struct rcp_tree_node_core *s = p->r; 
		if (s == t)
			s = p->l;
		int slb = ! s->l || s->l->color == RCP_TREE_BRACK;
		int srb = ! s->r || s->r->color == RCP_TREE_BRACK;

		if (slb && srb){
			if (p->color == RCP_TREE_BRACK){
				s->color = RCP_TREE_RED;
				t = p;
				continue;
			}
			else{
				p->color = RCP_TREE_BRACK;
				s->color = RCP_TREE_RED;
				break;
			}
		}

		struct rcp_tree_node_core *g = p->p; 
		if (p->color == RCP_TREE_BRACK){
			if (s->color == RCP_TREE_RED){
				p->color = RCP_TREE_RED;
				s->color = RCP_TREE_BRACK;

				if (p->r == t){
					p->l = s->r;
					p->l->p = p;

					s->l = p;
					s->l->p = s;

				}
				else{
					p->r = s->l;
					p->r->p = p;

					s->r = p;
					s->r->p = s;

				}

				s->p = g;
				if (g){
					if (g->r = p)
						g->r = s;
					else
						g->l = s;
				}
				break;
			}
		}
	}

	{
		rcp_tree_node_replace(node, rep);
	}
}


