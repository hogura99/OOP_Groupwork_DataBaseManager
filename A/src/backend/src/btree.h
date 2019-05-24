#pragma once

#include <vector>
#include <iostream>
#include <queue>
#include "bitset.h"
#include "vector.h"
#include "print.h"
#include "utils.h"

#define BTNodePosi(T) BTNode<T> *
typedef long long Rank;

/**
 * B-tree node class
 */
template <typename T>
struct BTNode
{
    BTNodePosi(T) parent = nullptr; // parent pointer
    Vector<T> key;                  // key vector
    Vector<BTNodePosi(T)> child;    // children vector (child.size = key.size + 1)

    BTNode() { child.insert(child.begin(), nullptr); }

    BTNode(T e, BTNodePosi(T) lc = nullptr, BTNodePosi(T) rc = nullptr)
    {
        key.insert(key.begin(), e);
        child.push_back(lc);
        child.push_back(rc);
        if (lc)
            lc->parent = this;
        if (rc)
            rc->parent = this;
    }

    friend std::ostream &operator<<(std::ostream &out, const BTNode &btn)
    {
        return out << "BTNode(" << btn.key << ',' << btn.child << ')';
    }
};

/**
 * BTree class based on memory
 *
 * Provides methods to insert / delete / search in the b-tree
 */
template <typename T>
class BTree
{
  protected:
    int _size = 0;  // key size
    int _order = 3; // order of B tree
    BTNodePosi(T) _root;
    BTNodePosi(T) _hot; // the non-empty node BTree::search() last visits
    void solveOverflow(BTNodePosi(T));
    void solveUnderflow(BTNodePosi(T));

  public:
    BTree() { _root = new BTNode<T>(); }

    ~BTree() = default;
    int order() const { return _order; }
    int size() const { return _size; }
    BTNodePosi(T) & root() { return _root; }
    bool empty() const { return !_root; }
    BTNodePosi(T) search(const T &e);
    bool insert(const T &e);
    bool remove(const T &e);
    void release(BTNodePosi(T) v);

    friend std::ostream &operator<<(std::ostream &out, const BTree &bt)
    {
        Bitset leftmosts;
        Bitset rightmosts;
        bt.printBTree(out, bt._root, 0, true, true, leftmosts, rightmosts);
        return out;
    }

    void printBTree(std::ostream &out, BTNodePosi(T) bt, int depth, bool isLeftmost, bool isRightmost,
                    Bitset &leftmosts, Bitset &rightmosts) const
    {
        if (!bt)
            return;
        isLeftmost ? leftmosts.set(depth) : leftmosts.unset(depth);
        isRightmost ? rightmosts.set(depth) : rightmosts.unset(depth);
        int k = bt->child.size() - 1;
        printBTree(out, bt->child[k], depth + 1, false, true, leftmosts, rightmosts);
        bool parentOK = false;
        BTNodePosi(T) p = bt->parent;
        if (!p)
            parentOK = true;
        else
            for (int i = 0; i < p->child.size(); i++)
                if (p->child[i] == bt)
                    parentOK = true;
        while (0 < k--)
        {
            out << (parentOK ? " " : "X") << bt->key[k] << " *>";
            for (int i = 0; i < depth; i++)
                out << ((leftmosts.test(i) && leftmosts.test(i + 1) || rightmosts.test(i) && rightmosts.test(i + 1)) ? "      " : "│    ");
            if (((0 == depth && 1 < bt->key.size()) || !isLeftmost && isRightmost) && bt->key.size() - 1 == k)
                out << "┌—";
            else if (((0 == depth && 1 < bt->key.size()) || isLeftmost && !isRightmost) && 0 == k)
                out << "└—";
            else
                out << "├—";
            out << (bt->key[k]) << std::endl;
            printBTree(out, bt->child[k], depth + 1, 0 == k, false, leftmosts, rightmosts);
        }
    }
}; //BTree

template <typename T>
void BTree<T>::release(BTNodePosi(T) v)
{
    std::queue<BTNodePosi(T)> q;
    q.push(v);
    while (!q.empty())
    {
        v = q.front();
        q.pop();
        for (auto &nb : v->child)
            if (nb)
                q.push(nb);
        delete (v);
    }
}

template <typename T>
bool BTree<T>::insert(const T &e)
{
    BTNodePosi(T) v = search(e);
    if (v)
        return false;
    Rank r = _hot->key.search(e);
    _hot->key.insert(_hot->key.begin() + r + 1, e);
    _hot->child.insert(_hot->child.begin() + r + 2, nullptr);
    _size++;
    solveOverflow(_hot);
    return true;
}

template <typename T>
bool BTree<T>::remove(const T &e)
{
    BTNodePosi(T) v = search(e);
    if (!v)
        return false;
    Rank r = v->key.search(e);
    if (v->child[0])
    {
        BTNodePosi(T) u = v->child[r + 1];
        while (u->child[0])
            u = u->child[0];
        v->key[r] = u->key[0];
        v = u;
        r = 0;
    }
    v->key.erase(v->key.begin() + r);
    v->child.erase(v->child.begin() + r + 1);
    _size--;
    solveUnderflow(v);
    return true;
}

template <typename T>
BTNodePosi(T) BTree<T>::search(const T &e)
{
    BTNodePosi(T) v = _root;
    _hot = nullptr;
    while (v)
    {
        Rank r = v->key.search(e);
        if ((0 <= r) && (e == v->key[r]))
            return v;
        _hot = v;
        v = v->child[r + 1];
    }
    return nullptr;
}

template <typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v)
{
    if (_order >= v->child.size())
        return;
    Rank s = _order / 2;
    BTNodePosi(T) u = new BTNode<T>();
    for (Rank j = 0; j < _order - s - 1; j++)
    {
        auto backupChild = v->child[s + 1];
        v->child.erase(v->child.begin() + s + 1);
        u->child.insert(u->child.begin() + j, backupChild);
        auto backupKey = v->key[s + 1];
        v->key.erase(v->key.begin() + s + 1);
        u->key.insert(u->key.begin() + j, backupKey);
    }
    u->child[_order - s - 1] = v->child[s + 1];
    v->child.erase(v->child.begin() + s + 1);
    if (u->child[0])
        for (Rank j = 0; j < _order - s; j++)
            u->child[j]->parent = u;
    BTNodePosi(T) p = v->parent;
    if (!p)
    {
        _root = p = new BTNode<T>();
        p->child[0] = v;
        v->parent = p;
    }
    Rank r = 1 + p->key.search(v->key[0]);

    p->key.insert(p->key.begin() + r, v->key.remove(s));
    p->child.insert(p->child.begin() + r + 1, u);

    u->parent = p;
    solveOverflow(p);
}

template <typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v)
{
    if ((_order + 1) / 2 <= v->child.size())
        return;
    BTNodePosi(T) p = v->parent;
    if (!p)
    {
        if (!v->key.size() && v->child[0])
        {
            _root = v->child[0];
            _root->parent = NULL;
            v->child[0] = NULL;
            release(v);
        }
        return;
    }
    Rank r = 0;
    while (p->child[r] != v)
        r++;
    if (0 < r)
    {
        BTNodePosi(T) ls = p->child[r - 1];
        if ((_order + 1) / 2 < ls->child.size())
        {
            v->key.insert(0, p->key[r - 1]);
            p->key[r - 1] = ls->key.remove(ls->key.size() - 1);
            v->child.insert(0, ls->child.remove(ls->child.size() - 1));
            if (v->child[0])
                v->child[0]->parent = v;
            return;
        }
    }
    if (p->child.size() - 1 > r)
    {
        BTNodePosi(T) rs = p->child[r + 1];
        if ((_order + 1) / 2 < rs->child.size())
        {
            v->key.insert(v->key.size(), p->key[r]);
            p->key[r] = rs->key.remove(0);
            v->child.insert(v->child.size(), rs->child.remove(0));
            if (v->child[v->child.size() - 1])
                v->child[v->child.size() - 1]->parent = v;
            return;
        }
    }
    if (0 < r)
    {
        BTNodePosi(T) ls = p->child[r - 1];
        ls->key.insert(ls->key.size(), p->key.remove(r - 1));
        p->child.remove(r);
        ls->child.insert(ls->child.size(), v->child.remove(0));
        if (ls->child[ls->child.size() - 1])
            ls->child[ls->child.size() - 1]->parent = ls;
        while (!v->key.empty())
        {
            ls->key.insert(ls->key.size(), v->key.remove(0));
            ls->child.insert(ls->child.size(), v->child.remove(0));
            if (ls->child[ls->child.size() - 1])
                ls->child[ls->child.size() - 1]->parent = ls;
        }
        release(v);
    }
    else
    {
        BTNodePosi(T) rs = p->child[r + 1];
        rs->key.insert(0, p->key.remove(r));
        p->child.remove(r);
        rs->child.insert(0, v->child.remove(v->child.size() - 1));
        if (rs->child[0])
            rs->child[0]->parent = rs;
        while (!v->key.empty())
        {
            rs->key.insert(0, v->key.remove(v->key.size() - 1));
            rs->child.insert(0, v->child.remove(v->child.size() - 1));
            if (rs->child[0])
                rs->child[0]->parent = rs;
        }
        release(v);
    }
    solveUnderflow(p);
}