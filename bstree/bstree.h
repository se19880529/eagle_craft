#define MIN_CHILD_OF(node) node->lchild
#define MAX_CHILD_OF(node) node->rchild
#define MAX(x, y) ((x > y)? x: y)
#define HEIGHT(p) ((p == NULL)?-1:p->height)


template<typename elem_t, typename key_t>
class btree
{
public:
	struct node
	{
		node* lchild;
		node* rchild;
		key_t key;
		elem_t data;
		int height;
	};
	btree()
	{
		_root = NULL;
		_last_changed_root = NULL;
	}
public:
	key_t KeyOf(elem_t data)
	{
		return (key_t)data;
	}
	void Insert(elem_t data)
	{
		bool changed = false;
		_insert(_root, data, KeyOf(data), changed);
	}
	node* Find(key_t key)
	{
		node* res = _find(_root, key);
		return res;
	}
	node* Delete(key_t key)
	{
		bool changed = false;
		return _delete(_root, key, changed);
	}
protected:
	node* _root;
	node* _last_changed_root;
	void _insert(node*& root, elem_t data, key_t key, bool& changed)
	{
		if(root == NULL)
		{
			root = new node();
			MIN_CHILD_OF(root) = NULL;
			MAX_CHILD_OF(root) = NULL;
			root->key = key;
			root->data = data;
			root->height = 0;
			changed = true;
		}
		else
		{
			if(root->key > key)
			{
				_insert(MIN_CHILD_OF(root), data, key, changed);
			}
			else if(root->key < key)
			{
				_insert(MAX_CHILD_OF(root), data, key, changed);
			}
			else
			{
				//insert failed
			}
			
			if(changed)
			{
				int height = HEIGHT(root);
				_check_depth(root);
				if(HEIGHT(root) == height)
				{
					changed = false;
				}
			}
			
		}
	}

	void _balance(node*& root)
	{
		int height = HEIGHT(root);
		_check_depth(root);
		if(HEIGHT(root) != height)
		{
			//_balance(node
		}
	}

	void _check_depth(node*& root)
	{
		int maxHeight = -1, minHeight = -1;
		if(MAX_CHILD_OF(root) != NULL)
		{
			maxHeight = MAX_CHILD_OF(root)->height;
		}
		if(MIN_CHILD_OF(root) != NULL)
		{
			minHeight = MIN_CHILD_OF(root)->height;
		}
		if(minHeight > maxHeight + 1)
		{
			//min is to high
			node* child = MIN_CHILD_OF(root);
			if(MIN_CHILD_OF(child) != NULL && (MAX_CHILD_OF(child) == NULL || MAX_CHILD_OF(child)->height < MIN_CHILD_OF(child)->height))
			{
				//do one rotate is ok
				node* oldroot = root;
				root = child;
				MIN_CHILD_OF(oldroot) = MAX_CHILD_OF(child);
				MAX_CHILD_OF(child) = oldroot;
				oldroot->height = MAX(HEIGHT(MAX_CHILD_OF(oldroot)), HEIGHT(MIN_CHILD_OF(oldroot))) + 1;
				root->height = MAX(oldroot->height, HEIGHT(MIN_CHILD_OF(root))) + 1;
			}
			else
			{
				//need to do two rotate
				node* oldroot = root;
				root = MAX_CHILD_OF(child);
				MAX_CHILD_OF(child) = MIN_CHILD_OF(root);
				MIN_CHILD_OF(oldroot) = MAX_CHILD_OF(root);
				MAX_CHILD_OF(root) = oldroot;
				MIN_CHILD_OF(root) = child;
				oldroot->height = HEIGHT(MAX_CHILD_OF(oldroot)) + 1;
				child->height = HEIGHT(MAX_CHILD_OF(child)) + 1;
				root->height = HEIGHT(MAX_CHILD_OF(root)) + 1;
			}
		}
		else if(maxHeight > minHeight + 1)
		{
			//max is to high
			node* child = MAX_CHILD_OF(root);
			if(MAX_CHILD_OF(child) != NULL && (MIN_CHILD_OF(child) == NULL || MIN_CHILD_OF(child)->height < MAX_CHILD_OF(child)->height))
			{
				//do one rotate is ok
				node* oldroot = root;
				root = child;
				MAX_CHILD_OF(oldroot) = MIN_CHILD_OF(child);
				MIN_CHILD_OF(child) = oldroot;
				oldroot->height = MAX(HEIGHT(MAX_CHILD_OF(oldroot)), HEIGHT(MIN_CHILD_OF(oldroot))) + 1;
				root->height = MAX(oldroot->height, HEIGHT(MAX_CHILD_OF(root))) + 1;
			}
			else
			{
				//need to do two rotate
				node* oldroot = root;
				root = MIN_CHILD_OF(child);
				MIN_CHILD_OF(child) = MAX_CHILD_OF(root);
				MAX_CHILD_OF(oldroot) = MIN_CHILD_OF(root);
				MIN_CHILD_OF(root) = oldroot;
				MAX_CHILD_OF(root) = child;
				oldroot->height = HEIGHT(MIN_CHILD_OF(oldroot)) + 1;
				child->height = HEIGHT(MIN_CHILD_OF(child)) + 1;
				root->height = HEIGHT(MIN_CHILD_OF(root)) + 1;
			}
		}
		else
		{
			root->height = MAX(HEIGHT(MAX_CHILD_OF(root)), HEIGHT(MIN_CHILD_OF(root))) + 1;
		}
	}

	node* _find(node* root, key_t key)
	{
		if(root == NULL)
		{
			return NULL;
		}
		else if(root->key == key)
		{
			return root;
		}
		else if(root->key > key)
		{
			return _find(MIN_CHILD_OF(root), key);
		}
		else
		{
			return _find(MAX_CHILD_OF(root), key);
		}
	}
	node* _delete(node*& root, key_t key, bool& changed)
	{
		node* todelete = NULL;
		if(root == NULL)
		{
			return NULL;
		}
		else if(root->key == key)
		{	
			todelete = _delete(root, changed, root);
			return todelete;
		}
		else if(root->key > key)
		{
			todelete = _delete(MIN_CHILD_OF(root), key, changed);
		}
		else
		{
			todelete = _delete(MAX_CHILD_OF(root), key, changed);
		}

		if(changed)
		{
			int height = HEIGHT(root);
			_check_depth(root);
			if(HEIGHT(root) == height)
			{
				changed = false;
			}
		}
		return todelete;
	}

	node* _delete(node*& root, bool& changed, node* target)
	{
		if(MAX_CHILD_OF(root) == NULL)
		{
			node* todelete = root;
			root = MIN_CHILD_OF(root);
			changed = true;
			return todelete;
		}
		else if(MIN_CHILD_OF(root) == NULL)
		{
			node* todelete = root;
			root = MAX_CHILD_OF(root);
			changed = true;
			return todelete;
		}
		else
		{
			node* pre = NULL;
			if(root == target)
				pre = _delete(MIN_CHILD_OF(root), changed, target);
			else
				pre = _delete(MAX_CHILD_OF(root), changed, target);
			node* todelete = pre;
			if(root == target)
			{
				todelete = root;
				root = pre;
				MIN_CHILD_OF(root) = MIN_CHILD_OF(todelete);
				MAX_CHILD_OF(root) = MAX_CHILD_OF(todelete);
				root->height = todelete->height;
			}
			if(changed)
			{
				int height = HEIGHT(root);
				_check_depth(root);
				if(HEIGHT(root) == height)
				{
					changed = false;
				}
			}
			return todelete;
		}
	}

	node*& _max(node*& root)
	{
		if(MAX_CHILD_OF(root) == NULL)
		{
			return root;
		}
		else
		{
			node*& res = _max(MAX_CHILD_OF(root));
			return res;
		}
	}
	node*& _min(node*& root)
	{
		if(MIN_CHILD_OF(root) == NULL)
		{
			return root;
		}
		else
		{
			node*& res = _min(MIN_CHILD_OF(root));
			return res;
		}
	}


	
};