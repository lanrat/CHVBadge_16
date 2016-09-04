/******************************************************************************
 * Copyright 2014-2016 Specialized Solutions LLC
 *
 * Title to the Materials (contents of this file) remain with Specialized
 * Solutions LLC.  The Materials are copyrighted and are protected by United
 * States copyright laws.  Copyright notices cannot be removed from the
 * Materials.
 *
 * See the file titled "Specialized Solutions LLC License Agreement.txt"
 * that has been distributed with this file for further licensing details.
 *
 * QCM Binary Tree Example
 *
 * Shows use of data structures and recursion
 *
 *****************************************************************************/

#define NODE[.used, .id, .left, .right]

new tree[20][NODE]
new root = -1;

tree_init()
{
    new i
    for (i = 0; i < 20; i++)
    {
        tree[i][.used] = 0
        tree[i][.id] = 0
        tree[i][.left] = 0
        tree[i][.right] = 0
    }
}

node_allocate(id)
{
    new i

    for (i = 0; i < 20; i++)
    {
        if (tree[i][.used] == 0)
        {
            tree[i][.used] = 1
            tree[i][.id] = id
            tree[i][.left] = 0
            tree[i][.right] = 0
            return i
        }
    }

    return -1
}

add_node(value, current_index)
{
    if (root == -1)    
    {
        root = node_allocate(value)
        printf("Allocating root node %d with id of %d\n",  root, value)
    }
    else if (value < tree[current_index][.id])
    {
        if (tree[current_index][.left] == 0)
        {
            /* allocate a new node */
            tree[current_index][.left] = node_allocate(value)
            printf("Allocating left node %d with id of %d\n",  tree[current_index][.left], value)
            
        }
        else
        {
            printf("Searching to add to left subtree of node %d with id %d\n", current_index, tree[current_index][.id])
            add_node(value, tree[current_index][.left])
        }
        
    }
    else if (value > tree[current_index][.id])
    {
        if (tree[current_index][.right] == 0)
        {
            /* allocate a new node */
            tree[current_index][.right] = node_allocate(value)
            printf("Allocating right node %d with id of %d\n",  tree[current_index][.right], value)
        }
        else
        {
            printf("Searching to add to right subtree of node %d with id %d\n", current_index, tree[current_index][.id])
            add_node(value, tree[current_index][.right])
        }
    }

}

find_node(value, current_index)
{
    if (tree[current_index][.id] == value)
    {
        printf("Found node %d with id %d\n", current_index, value);
    }
    else if (value < tree[current_index][.id])
    {
        printf("Searching left subtree of node %d with id %d\n", current_index, tree[current_index][.id])

        if (tree[current_index][.left] == 0)
        {
            printf("Node value of %d not found\n", value)
        }
        else
        {
            find_node(value, tree[current_index][.left])
        }
        
    }
    else if (value > tree[current_index][.id])
    {
        printf("Searching right subtree of node %d with id %d\n", current_index, tree[current_index][.id])

        if (tree[current_index][.right] == 0)
        {
            printf("Node value of %d not found\n", value)
        }
        else
        {
            find_node(value, tree[current_index][.right])
        }
        
    }
}

main()
{
    sleep 1000

    printf("Starting tree test\n");

    tree_init()

    add_node(5,root)
    add_node(1,root)
    add_node(6,root)
    add_node(9,root)
    add_node(4,root)
    add_node(2,root)
    add_node(7,root)
    add_node(3,root)
    add_node(8,root)

    find_node(5,root)
    find_node(1,root)
    find_node(6,root)
    find_node(9,root)
    find_node(4,root)
    find_node(2,root)
    find_node(7,root)
    find_node(3,root)
    find_node(8,root)

    find_node(11,root)

}


