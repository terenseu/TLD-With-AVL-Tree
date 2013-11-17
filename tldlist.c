//
//  LOW SING WEE, TERENCE
//  2110003L
//  AP3 Excercise 1
//  This is my own work except that the tree iterator and AVL implementation are referenced from two websites:
//  https://gist.github.com/Knifa/1318375
//  http://anoopsmohan.blogspot.sg/2011/11/avl-tree-implementation-in-c.html
//
//  tldlist.c

#include <stdio.h>
#include "tldlist.h"
#include <stdlib.h>
#include "date.h"
#include <string.h>
#include <ctype.h>

static TLDNode *addtldnoder(TLDList *tld, char *temptld, TLDNode *node);
static TLDNode *newtldnode(char *temptld);
static void iteraddr(TLDIterator *iter, TLDNode *node, int *i);

static int getheight(TLDNode *node);
static int balfact(TLDNode *node);
static TLDNode *rr(TLDNode *node);
static TLDNode *rl(TLDNode *node);
static TLDNode *ll(TLDNode *node);
static TLDNode *lr(TLDNode *node);
static TLDNode *balance(TLDNode *node);



struct tldlist {
    TLDNode *root;
    Date *begin;
    Date *end;
    
    long count;
    long size;
};


struct tldnode {
    TLDNode *leftchild;
    TLDNode *rightchild;
    
    char *temptld;
    long count;
};


struct tlditerator {
    TLDList *tld;
    
    int i;
    long size;
    TLDNode **next;
};

TLDList *tldlist_create(Date *begin, Date *end)
{
    TLDList *tldlist = malloc(sizeof(TLDList));
    if (tldlist != NULL)
    {
        tldlist->count = 0;
        tldlist->size = 0;
        tldlist->root = NULL;
        tldlist->begin = begin;
        tldlist->end = end;
    }
    
    return tldlist;
    
}

int tldlist_add(TLDList *tld, char *hostname, Date *d) {
    // check if it's within the tld dates
    if (date_compare(tld->begin, d) > 0 ||
        date_compare(tld->end, d) < 0)
        return 0;
    
    char *dot = strrchr(hostname, '.') + 1;
    int i = 0;
    while (i < sizeof(dot))
    {
        dot[i] = tolower(dot[i]);
        i++;
    }
    char *temptld = (char *)malloc(sizeof(dot));
    strcpy(temptld, dot);
    
    tld->root = addtldnoder(tld, temptld, tld->root);
    tld->count++;
    return 1;
}

static TLDNode *addtldnoder(TLDList *tld, char *temptld, TLDNode *node) {
    
    if (node == NULL)
    {
        node = newtldnode(temptld);
        tld->root = node;
        tld->size++;
        return node;
    }
    else if (strcmp(temptld, node->temptld) > 0)
    {
        node->rightchild = addtldnoder(tld, temptld, node->rightchild);
        node = balance(node);
    }
    else if (strcmp(temptld, node->temptld) < 0)
    {
        node->leftchild = addtldnoder(tld, temptld, node->leftchild);
        node = balance(node);
    }
    else
    {
        free(temptld);
        node->count++;
    }
        
    return node;
}

long tldlist_count(TLDList *tld)
{
    return tld->count;
}

TLDIterator *tldlist_iter_create(TLDList *tld) {
    TLDIterator *iter = (TLDIterator *)malloc(sizeof(TLDIterator));
    
    if (iter == NULL)
    {
        free(iter);
        return NULL;
    }
    else {
        iter->tld = tld;
        iter->size = tld->size;
        iter->i = 0;
        
        iter->next = (TLDNode **)malloc(sizeof(TLDNode *) * iter->size);
        if (iter->next == NULL)
        {
            tldlist_iter_destroy(iter);
            return NULL;
        }
        
        int i = 0;
        iteraddr(iter, iter->tld->root, &i);
        return iter;
    }
}

static void iteraddr(TLDIterator *iter, TLDNode *node, int *i)
{
    
    if (node->leftchild)
        iteraddr(iter, node->leftchild, i);
    
    *(iter->next + (*i)++) = node;
    
    if (node->rightchild)
        iteraddr(iter, node->rightchild, i);
}

TLDNode *tldlist_iter_next(TLDIterator *iter)
{
    if (iter->i == iter->size)
        return NULL;
    
    return *(iter->next + iter->i++);
}

void tldlist_iter_destroy(TLDIterator *iter) {

int i = 0;

while (i < iter->size)
{   
    free(iter->next[i]->temptld);
    free(iter->next[i]);
    i++;	
}
    free(iter->next);
    free(iter);
}

char *tldnode_tldname(TLDNode *node)
{
    return node->temptld;
}

long tldnode_count(TLDNode *node)
{
    return node->count;
}

static TLDNode *newtldnode(char *temptld)
{
    TLDNode *node = (TLDNode *)malloc(sizeof(TLDNode));
    if (node == NULL)
	{
	  free(node);
	  return NULL;
	}
        
    else
    {
    node->temptld = temptld;
    node->leftchild = NULL;
    node->rightchild = NULL;
    node->count = 1;
    	
    return node;
	}
}

void tldlist_destroy(TLDList *tld)
{
    free(tld);   
}

static int getheight(TLDNode *node)
{
    int ht = 0;
    int leftht;
    int rightht;
    int last;
    
    if (node != NULL)
    {
        leftht = getheight(node->leftchild);
        rightht = getheight(node->rightchild);
        if (leftht > rightht)
        {
            last = leftht;
        }
        else
        {
            last = rightht;
        }
        ht= 1+last;
    }
    return ht;
}

static int balfact(TLDNode *node)
{
    int bfact;
    int leftht;
    int rightht;
    
    leftht = getheight(node->leftchild);
    rightht = getheight(node->rightchild);
    bfact = leftht - rightht;
    
    return bfact;
}

static TLDNode *rr(TLDNode *node)
{
    TLDNode *temp;
    temp = node->rightchild;
    node->rightchild = temp->leftchild;
    temp->leftchild = node;
    return temp;
}

static TLDNode *rl(TLDNode *node)
{
    TLDNode *temp;
    temp =node->rightchild;
    node->rightchild = ll(temp);
    return rr(node);
}

static TLDNode *ll(TLDNode *node)
{
    TLDNode *temp;
    temp = node->leftchild;
    node->leftchild = temp->rightchild;
    temp->rightchild = node;
    return temp;
}

static TLDNode *lr(TLDNode *node)
{
    TLDNode *temp;
    temp = node->leftchild;
    node->leftchild = rr(temp);
    return ll(node);
}

static TLDNode *balance(TLDNode *node)
{
    int balf;
    balf = balfact(node);
    if (balf > 1)
    {
        if (balfact(node->leftchild) > 0)
        {
            node = ll(node);
        }
        else
        {
            node = lr(node);
        }
    }
    else if (balf < -1)
        {
            if (balfact(node->rightchild) > 0)
            {
                node = rl(node);
            }
            else
            {
                node = rr(node);
            }
        }
    return node;
}
