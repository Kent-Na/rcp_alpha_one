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

void rcp_tree_node_replace(rcp_tree_ref tree, 
		rcp_tree_node_ref src, rcp_tree_node_ref dst){
	if (src->p == NULL)
		tree->root = dst;
	dst->l = src->l;
	dst->r = src->r;
	dst->p = src->p;
	dst->color = src->color;
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
	if (tree == NULL)
		return;
	if (tree->root == NULL)
		return;

	struct rcp_tree_node_core *cur = tree->root;
	if (cur->color == RCP_TREE_RED)
		rcp_error("tree:red root");

	if (tree->root->p != NULL)
		rcp_error("tree:root p");

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
				rcp_tree_node_replace(tree, cur, node);
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

rcp_extern void rcp_tree_remove(
		rcp_tree_ref tree, rcp_tree_node_ref node)
{
	if (node == NULL){
		rcp_error("tree: no key");
		return;
	}

	{
		int cond0 = tree->root == node;
		int cond1 = node->l == NULL;
		int cond2 = node->r == NULL;
		if (cond0 && cond1 && cond2){
			tree->root = NULL;
			return;
		}
	}

	rcp_tree_node_ref t = node;
	if (t->l)
		t = t->l;
	while (t->r)
		t = t->r;

	if (t != node){
		struct rcp_tree_node_core tmp;
		memcpy(&tmp, t, sizeof tmp);
		memcpy(t, node, sizeof tmp);
		memcpy(node, &tmp, sizeof tmp);

		if (t->l == t){
			t->l = node;
			node->p = t;
		}
		if (t->r == t){
			t->r = node;
			node->p = t;
		}

		if (t->l)
			t->l->p = t;
		if (t->r)
			t->r->p = t;
		if (t->p){
			if (t->p->l == node)
				t->p->l = t;
			else if (t->p->r == node)
				t->p->r = t;
		}
		else{
			tree->root = t;
		}

		if (node->l)
			node->l->p = node;
		if (node->r)
			node->r->p = node;
		if (node->p){
			if (node->p->l == t)
				node->p->l = node;
			else if (node->p->r == t)
				node->p->r = node;
		}
		else{
			tree->root = node;
		}

		t = node;
	}

	if (t->color == RCP_TREE_RED){
		if (t->p->r == t)
			t->p->r = NULL;
		else if (t->p->l == t)
			t->p->l = NULL;
		return;
	}

	while (1){
		rcp_tree_node_ref p = t->p;
		rcp_tree_node_ref s = p->l;
		if (s == t)
			s = p->r;
		rcp_tree_node_ref sl = s->l;
		rcp_tree_node_ref sr = s->r;

		//black sib case
		{
			int cond = p->color == RCP_TREE_BRACK;
			cond = cond && s->color == RCP_TREE_BRACK;
			cond = cond && (! sl || sl->color == RCP_TREE_BRACK);
			cond = cond && (! sr || sr->color == RCP_TREE_BRACK);
			if (cond){
				s->color = RCP_TREE_RED;
				t = p;
				if (!p->p)
					break;
				continue;
			}
		}
		{
			int cond = p->color == RCP_TREE_RED;
			cond = cond && s->color == RCP_TREE_BRACK;
			cond = cond && (! sl || sl->color == RCP_TREE_BRACK);
			cond = cond && (! sr || sr->color == RCP_TREE_BRACK);
			if (cond){
				s->color = RCP_TREE_RED;
				p->color = RCP_TREE_BRACK;
				break;
			}
		}
		if (t == p->l){
			int cond = s->color == RCP_TREE_BRACK;
			cond = cond && sr && sr->color == RCP_TREE_RED;
			rcp_tree_node_ref g = p->p;
			if (cond){
				p->r = sl;
				if (sl)
					sl->p = p;
				s->l = p;
				p->p = s;
				s->color = p->color;
				if (sr)
					sr->color = RCP_TREE_BRACK;
				p->color = RCP_TREE_BRACK;
				s->p = g;
				if (g && g->l == p)
					g->l = s;
				if (g && g->r == p)
					g->r = s;
				if (g == NULL)
					tree->root = s;
				break;
			}
		}
		else{
			int cond = s->color == RCP_TREE_BRACK;
			cond = cond && sl && sl->color == RCP_TREE_RED;
			rcp_tree_node_ref g = p->p;
			if (cond){
				p->l = sr;
				if (sr)
					sr->p = p;
				s->r = p;
				p->p = s;
				s->color = p->color;
				if (sl)
					sl->color = RCP_TREE_BRACK;
				p->color = RCP_TREE_BRACK;
				s->p = g;
				if (g && g->l == p)
					g->l = s;
				if (g && g->r == p)
					g->r = s;
				if (g == NULL)
					tree->root = s;
				break;
			}
		}
		if (t == p->l){
			int cond = s->color == RCP_TREE_BRACK;
			cond = cond && (!sr || sr->color == RCP_TREE_BRACK);
			cond = cond && sl && sl->color == RCP_TREE_RED;
			rcp_tree_node_ref g = p->p;
			if (cond){
				s->l = sl->r;
				if (s->l)
					s->l->p = s;
				p->r = sl->l;
				if (p->r)
					p->r->p = p;
				sl->r = s;
				s->p = sl;
				sl->l = p;
				p->p = sl;
				
				sl->color = p->color;
				p->color = RCP_TREE_BRACK;
				sl->p = g;
				if (g && g->l == p)
					g->l = sl;
				if (g && g->r == p)
					g->r = sl;
				if (g == NULL)
					tree->root = sl;
				break;
			}
		}
		else{
			int cond = s->color == RCP_TREE_BRACK;
			cond = cond && (!sl || sl->color == RCP_TREE_BRACK);
			cond = cond && sr && sr->color == RCP_TREE_RED;
			rcp_tree_node_ref g = p->p;
			if (cond){
				s->r = sr->l;
				if (s->r)
					s->r->p = s;
				p->l = sr->r;
				if (p->l)
					p->l->p = p;
				sr->l = s;
				s->p = sr;
				sr->r = p;
				p->p = sr;
				
				sr->color = p->color;
				p->color = RCP_TREE_BRACK;
				sr->p = g;
				if (g && g->l == p)
					g->l = sr;
				if (g && g->r == p)
					g->r = sr;
				if (g == NULL)
					tree->root = sr;
				break;
			}
		}
		//red sib case
		if (t == p->l){
			int cond = p->color == RCP_TREE_BRACK;
			cond = cond && s->color == RCP_TREE_RED;
			cond = cond && (! sl || sl->color == RCP_TREE_BRACK);
			cond = cond && (! sr || sr->color == RCP_TREE_BRACK);
			cond = cond && (!sl||!sl->l||sl->l->color == RCP_TREE_BRACK);
			cond = cond && (!sl||!sl->r||sl->r->color == RCP_TREE_BRACK);
			rcp_tree_node_ref g = p->p;
			if (cond){
				p->r = sl;
				if (sl)
					sl->p = p;
				s->l = p;
				p->p = s;
				s->color = RCP_TREE_BRACK;
				s->p = g;
				if (g && g->l == p)
					g->l = s;
				if (g && g->r == p)
					g->r = s;
				if (sl)
					sl->color = RCP_TREE_RED;
				if (g == NULL)
					tree->root = s;
				break;
			}
		}
		else{
			int cond = p->color == RCP_TREE_BRACK;
			cond = cond && s->color == RCP_TREE_RED;
			cond = cond && (! sl || sl->color == RCP_TREE_BRACK);
			cond = cond && (! sr || sr->color == RCP_TREE_BRACK);
			cond = cond && (!sr||!sr->l||sr->l->color == RCP_TREE_BRACK);
			cond = cond && (!sr||!sr->r||sr->r->color == RCP_TREE_BRACK);
			rcp_tree_node_ref g = p->p;
			if (cond){
				p->l = sr;
				if (sr)
					sr->p = p;
				s->r = p;
				p->p = s;
				s->color = RCP_TREE_BRACK;
				s->p = g;
				if (g && g->l == p)
					g->l = s;
				if (g && g->r == p)
					g->r = s;
				if (sr)
					sr->color = RCP_TREE_RED;
				if (g == NULL)
					tree->root = s;
				break;
			}
		}
		if (t == p->l){
			int cond = p->color == RCP_TREE_BRACK;
			cond = cond && s->color == RCP_TREE_RED;
			cond = cond && sl && sl->color == RCP_TREE_BRACK;
			cond = cond && (! sr || sr->color == RCP_TREE_BRACK);
			cond = cond && sl->r && sl->r->color == RCP_TREE_RED;
			rcp_tree_node_ref g = p->p;
			rcp_tree_node_ref slr = sl->r;
			if (cond){
				p->r = sl->l;
				if (p->r)
					p->r->p = p;
				s->l = slr;
				slr->p = s;

				sl->l = p;
				p->p = sl;
				sl->r = s;
				s->p = sl;

				slr->color = RCP_TREE_BRACK;

				sl->p = g;
				if (g && g->l == p)
					g->l = sl;
				if (g && g->r == p)
					g->r = sl;
				if (g == NULL)
					tree->root = sl;
				break;
			}
		}
		else{
			int cond = p->color == RCP_TREE_BRACK;
			cond = cond && s->color == RCP_TREE_RED;
			cond = cond && sr && sr->color == RCP_TREE_BRACK;
			cond = cond && (! sl || sl->color == RCP_TREE_BRACK);
			cond = cond && sr->l && sr->l->color == RCP_TREE_RED;
			rcp_tree_node_ref g = p->p;
			rcp_tree_node_ref srl = sr->l;
			if (cond){
				p->l = sr->r;
				if (p->l)
					p->l->p = p;
				s->r = srl;
				srl->p = s;

				sr->r = p;
				p->p = sr;
				sr->l = s;
				s->p = sr;

				srl->color = RCP_TREE_BRACK;

				sr->p = g;
				if (g && g->l == p)
					g->l = sr;
				if (g && g->r == p)
					g->r = sr;
				if (g == NULL)
					tree->root = sr;
				break;
			}
		}
		if (t == p->l){
			int cond = p->color == RCP_TREE_BRACK;
			cond = cond && s->color == RCP_TREE_RED;
			cond = cond && sl && sl->color == RCP_TREE_BRACK;
			cond = cond && (! sr || sr->color == RCP_TREE_BRACK);
			cond = cond && sl->l && sl->l->color == RCP_TREE_RED;
			rcp_tree_node_ref sll = sl->l;
			if (cond){
				s->l = sll;
				sll->p = s;
				sl->l = sll->r;
				if (sl->l)
					sl->l->p = sl;
				sll->r = sl;
				sl->p = sll;
				sl->color = RCP_TREE_RED;
				sll->color = RCP_TREE_BRACK;
				continue;
			}
		}
		else{
			int cond = p->color == RCP_TREE_BRACK;
			cond = cond && s->color == RCP_TREE_RED;
			cond = cond && sr && sr->color == RCP_TREE_BRACK;
			cond = cond && (! sl || sl->color == RCP_TREE_BRACK);
			cond = cond && sr->r && sr->r->color == RCP_TREE_RED;
			rcp_tree_node_ref srr = sr->r;
			if (cond){
				s->r = srr;
				srr->p = s;
				sr->r = srr->l;
				if (sr->r)
					sr->r->p = sr;
				srr->l = sr;
				sr->p = srr;
				sr->color = RCP_TREE_RED;
				srr->color = RCP_TREE_BRACK;
				continue;
			}
		}
		rcp_caution("go to inf");
		return;
	}

	if (node->p->r == node)
		node->p->r = NULL;
	else if (node->p->l == node)
		node->p->l = NULL;
	else
		rcp_error("errrrrerrererrererer");
}


