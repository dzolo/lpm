/*
 * FIT VUT - PDS PROJECT 2012/2013 - LONGEST PREFIX MATCH
 * (c) Ondrej Fibich <xfibic01@stud.fit.vutbr.cz>
 */

#ifndef PREFIX_TREE_MATCH_H
#define	PREFIX_TREE_MATCH_H

#include <stdlib.h>

#ifndef uint8_t
#define uint8_t unsigned char
#endif

/*
 * Tree for storing prefixes and searching for longest match on stored prefixes.
 */

#ifdef	__cplusplus
extern "C"
{
#endif
    
    /**
     * Node of tree
     */
    typedef struct _node_t
    {
        /** Left children */
        struct _node_t *l;
        /** Right children */
        struct _node_t *r;
        /** Network ID (value) */
        int as;
        /** Indicator: is as loaded? (not empty) */
        uint8_t as_in;
    } node_t;
    
    #define TRIE_MEMORY_CELL_SIZE 32768
    
    /**
     * Memory stack for nodes
     */
    typedef struct _trie_memory_t
    {
        /** Count of used memory cells */
        int ln;
        /** Next memory cell */
        struct _trie_memory_t *next;
        /** Memory cells */
        node_t cells[TRIE_MEMORY_CELL_SIZE];
    } trie_memory_t;

    /**
     * Trie tree
     */
    typedef struct _trie_t
    {
        /** Root node */
        node_t *root;
        /** First memory cell */
        trie_memory_t *first;
        /** Current memory cell */
        trie_memory_t *last;
    } trie_t;
    
    /**
     * Initializes the trie tree
     * 
     * @param t
     */
    void trie_init(trie_t *t);
    
    /**
     * Destroys allocated memory
     * 
     * @param t
     */
    void trie_destroy(trie_t *t);
    
    /**
     * Puts the given network address to prefix tree
     * 
     * @param t trie tree
     * @param ip IP of subnet
     * @param cidr subnet length
     * @param as value
     */
    void trie_node_put(trie_t *t, uint8_t *ip, uint8_t cidr, int as);
    
    /**
     * Searches for network for the given IP address
     * 
     * @param t trie tree
     * @param ip IP of subnet
     * @param ln length of IP (32/128)
     * @param as store value to
     * @return if success than >0 else 0
     */
    uint8_t trie_node_search(trie_t *t, uint8_t *ip, uint8_t ln, int *as);


#ifdef	__cplusplus
}
#endif

#endif	/* PREFIX_TREE_MATCH_H */

