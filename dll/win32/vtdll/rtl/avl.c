 /*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */
 
#define NDEBUG

#include <main.h>

PVOID 
NTAPI 
RtlInsertElementGenericTableFull( 	
	IN PRTL_GENERIC_TABLE  	Table,
	IN PVOID  	Buffer,
	IN ULONG  	BufferSize,
	OUT PBOOLEAN NewElement  	OPTIONAL,
	IN PVOID  	NodeOrParent,
	IN TABLE_SEARCH_RESULT  	SearchResult 
) 
{
    PRTL_SPLAY_LINKS NewNode;

    /* Check if the entry wasn't already found */
    if (SearchResult != TableFoundNode)
    {
        /* We're doing an allocation, sanity check */
        ASSERT(Table->NumberGenericTableElements != (MAXULONG - 1));

        /* Allocate a node */
        NewNode = Table->AllocateRoutine(Table,
                                         BufferSize +
                                         FIELD_OFFSET(TABLE_ENTRY_HEADER,
                                                      UserData));
        if (!NewNode)
        {
            /* No memory or other allocation error, fail */
            if (NewElement) *NewElement = FALSE;
            return NULL;
        }

        /* Initialize the new inserted element */
        RtlInitializeSplayLinks(NewNode);
        InsertTailList(&Table->InsertOrderList,
                       &((PTABLE_ENTRY_HEADER)NewNode)->ListEntry);

        /* Increase element count */
        Table->NumberGenericTableElements++;

        /* Check where we should insert the entry */
        if (SearchResult == TableEmptyTree)
        {
            /* This is the new root node */
            Table->TableRoot = NewNode;
        }
        else if (SearchResult == TableInsertAsLeft)
        {
            /* Insert it left */
            RtlInsertAsLeftChild(NodeOrParent, NewNode);
        }
        else
        {
            /* Right node */
            RtlInsertAsRightChild(NodeOrParent, NewNode);
        }

        /* Copy user buffer */
        RtlCopyMemory(&((PTABLE_ENTRY_HEADER)NewNode)->UserData,
                      Buffer,
                      BufferSize);
    }
    else
    {
        /* Return the node we already found */
        NewNode = NodeOrParent;
    }

    /* Splay the tree */
    Table->TableRoot = RtlSplay(NewNode);

    /* Return status */
    if (NewElement) *NewElement = (SearchResult != TableFoundNode);

    /* Return pointer to user data */
    return &((PTABLE_ENTRY_HEADER)NewNode)->UserData;
}

FORCEINLINE
VOID
RtlpPromoteAvlTreeNode(
	IN PRTL_BALANCED_LINKS Node
)
{
    PRTL_BALANCED_LINKS ParentNode, SuperParentNode;
    PRTL_BALANCED_LINKS *SwapNode1, *SwapNode2;

    /* Grab parents up to 2 levels high */
    ParentNode = RtlParentAvl(Node);
    SuperParentNode = RtlParentAvl(ParentNode);

    /* Pick which nodes will be rotated */
    SwapNode1 = RtlIsLeftChildAvl(Node) ? &ParentNode->LeftChild : &ParentNode->RightChild;
    SwapNode2 = RtlIsLeftChildAvl(Node) ? &Node->RightChild : &Node->LeftChild;

    /* Do the rotate, and update the parent and super-parent as needed */
    *SwapNode1 = *SwapNode2;
    if (*SwapNode1) RtlSetParent(*SwapNode1, ParentNode);
    *SwapNode2 = ParentNode;
    RtlSetParent(ParentNode, Node);

    /* Now update the super-parent child link, and make it parent of the node*/
    SwapNode1 = (RtlLeftChildAvl(SuperParentNode) == ParentNode) ?
                 &SuperParentNode->LeftChild: &SuperParentNode->RightChild;
    *SwapNode1 = Node;
    RtlSetParent(Node, SuperParentNode);
}


FORCEINLINE
BOOLEAN
RtlpRebalanceAvlTreeNode(
	IN PRTL_BALANCED_LINKS Node
)
{
    PRTL_BALANCED_LINKS ChildNode, SubChildNode;
    CHAR Balance;
    ASSERT(RtlParentAvl(Node) != Node);

    /* Get the balance, and figure out which child node to go down on */
    Balance = RtlBalance(Node);
    ChildNode = (Balance == RtlRightHeavyAvlTree) ?
                 RtlRightChildAvl(Node) : RtlLeftChildAvl(Node);

    /* The child and node have the same balance, promote the child upwards */
    if (RtlBalance(ChildNode) == Balance)
    {
        /* This performs the rotation described in Knuth A8-A10 for Case 1 */
        RtlpPromoteAvlTreeNode(ChildNode);

        /* The nodes are now balanced */
        RtlSetBalance(ChildNode, RtlBalancedAvlTree);
        RtlSetBalance(Node, RtlBalancedAvlTree);
        return FALSE;
    }

    /* The child has the opposite balance, a double promotion of the child's child must happen */
    if (RtlBalance(ChildNode) == -Balance)
    {
        /* Pick which sub-child to use based on the balance */
        SubChildNode = (Balance == RtlRightHeavyAvlTree) ?
                        RtlLeftChildAvl(ChildNode) : RtlRightChildAvl(ChildNode);

        /* Do the double-rotation described in Knuth A8-A10 for Case 2 */
        RtlpPromoteAvlTreeNode(SubChildNode);
        RtlpPromoteAvlTreeNode(SubChildNode);

        /* Was the sub-child sharing the same balance as the node? */
        if (RtlBalance(SubChildNode) == Balance)
        {
            /* Then the subchild is now balanced, and the node's weight is inversed */
            RtlSetBalance(ChildNode, RtlBalancedAvlTree);
            RtlSetBalance(Node, -Balance);
        }
        else if (RtlBalance(SubChildNode) == -Balance)
        {
            /*
             * In this case, the sub-child weight was the inverse of the node, so
             * the child now shares the node's balance original weight, while the
             * node becomes balanced.
             */
            RtlSetBalance(ChildNode, Balance);
            RtlSetBalance(Node, RtlBalancedAvlTree);
        }
        else
        {
            /*
             * Otherwise, the sub-child was unbalanced, so both the child and node
             * now become balanced.
             */
            RtlSetBalance(ChildNode, RtlBalancedAvlTree);
            RtlSetBalance(Node, RtlBalancedAvlTree);
        }

        /* In all cases, the sub-child is now balanced */
        RtlSetBalance(SubChildNode, RtlBalancedAvlTree);
        return FALSE;
    }

    /*
     * The case that remains is that the child was already balanced, so this is
     * This is the rotation required for Case 3 in Knuth A8-A10
     */
    RtlpPromoteAvlTreeNode(ChildNode);

    /* Now the child has the opposite weight of the node */
    RtlSetBalance(ChildNode, -Balance);

    /* This only happens on deletion, so we return TRUE to terminate the delete */
    return TRUE;
}


FORCEINLINE
VOID
RtlpInsertAvlTreeNode(
	IN PRTL_AVL_TABLE Table,
    IN PRTL_BALANCED_LINKS NewNode,
    IN OUT PVOID NodeOrParent,
    IN OUT TABLE_SEARCH_RESULT SearchResult)
{
    CHAR Balance;

    /* Initialize the new inserted element */
    MI_ASSERT(SearchResult != TableFoundNode);
    NewNode->LeftChild = NewNode->RightChild = NULL;
    RtlSetBalance(NewNode, RtlBalancedAvlTree);

    /* Increase element count */
    Table->NumberGenericTableElements++;

    /* Check where we should insert the entry */
    if (SearchResult == TableEmptyTree)
    {
        /* This is the new root node */
        RtlInsertAsRightChildAvl(&Table->BalancedRoot, NewNode);

        /* On AVL trees, we also update the depth */
        ASSERT(Table->DepthOfTree == 0);
        Table->DepthOfTree = 1;
        return;
    }
    else if (SearchResult == TableInsertAsLeft)
    {
        /* Insert it left */
        RtlInsertAsLeftChildAvl(NodeOrParent, NewNode);
    }
    else
    {
        /* Right node */
        RtlInsertAsRightChildAvl(NodeOrParent, NewNode);
    }

    /* Little cheat to save on loop processing, taken from Timo */
    RtlSetBalance(&Table->BalancedRoot, RtlLeftHeavyAvlTree);

    /*
     * This implements A6-A7 from Knuth based on http://coding.derkeiler.com
     * /pdf/Archive/C_CPP/comp.lang.c/2004-01/1812.pdf, however the algorithm
     * is slightly modified to follow the tree based on the Parent Node such
     * as the Windows algorithm does it, instead of following the nodes down.
     */
    while (TRUE)
    {
        /* Calculate which side to balance on */
        Balance = RtlIsLeftChildAvl(NewNode) ? RtlLeftHeavyAvlTree : RtlRightHeavyAvlTree;

        /* Check if the parent node was balanced */
        if (RtlBalance(NodeOrParent) == RtlBalancedAvlTree)
        {
            /* It's not balanced anymore (heavy on one side) */
            RtlSetBalance(NodeOrParent, Balance);

            /* Move up */
            NewNode = NodeOrParent;
            NodeOrParent = RtlParentAvl(NodeOrParent);
        }
        else if (RtlBalance(NodeOrParent) != Balance)
        {
            /* The parent's balance is opposite, so the tree is balanced now */
            RtlSetBalance(NodeOrParent, RtlBalancedAvlTree);

            /* Check if this is the root (the cheat applied earlier gets us here) */
            if (RtlBalance(&Table->BalancedRoot) == RtlBalancedAvlTree)
            {
                /* The depth has thus increased */
                Table->DepthOfTree++;
            }

            /* We reached the root or a balanced node, so we're done */
            break;
        }
        else
        {
            /* The tree is now unbalanced, so AVL rebalancing must happen */
            RtlpRebalanceAvlTreeNode(NodeOrParent);
            break;
        }
    }
}


/*
 * @implemented
 */
PVOID
NTAPI
RtlInsertElementGenericTableFullAvl(
	IN PRTL_AVL_TABLE Table,
	IN PVOID Buffer,
	IN ULONG BufferSize,
	OUT PBOOLEAN NewElement OPTIONAL,
	IN OUT PVOID NodeOrParent,
	IN OUT TABLE_SEARCH_RESULT SearchResult
)
{
    PRTL_BALANCED_LINKS NewNode;
    PVOID UserData;

    /* Check if the entry wasn't already found */
    if (SearchResult != TableFoundNode)
    {
        /* We're doing an allocation, sanity check */
        ASSERT(Table->NumberGenericTableElements != (MAXULONG - 1));

        /* Allocate a node */
        NewNode = Table->AllocateRoutine(Table,
                                         BufferSize +
                                         FIELD_OFFSET(TABLE_ENTRY_HEADER,
                                                      UserData));
        if (!NewNode)
        {
            /* No memory or other allocation error, fail */
            if (NewElement) *NewElement = FALSE;
            return NULL;
        }

        /* Data to return to user */
        UserData = &((PTABLE_ENTRY_HEADER)NewNode)->UserData;

        /* Insert the node in the tree */
        RtlZeroMemory(NewNode, sizeof(RTL_BALANCED_LINKS));
        RtlpInsertAvlTreeNode(Table, NewNode, NodeOrParent, SearchResult);

        /* Copy user buffer */
        RtlCopyMemory(UserData, Buffer, BufferSize);
    }
    else
    {
        /* Return the node we already found */
        NewNode = NodeOrParent;
        UserData = &((PTABLE_ENTRY_HEADER)NewNode)->UserData;
    }

    /* Return status */
    if (NewElement) *NewElement = (SearchResult != TableFoundNode);

    /* Return pointer to user data */
    return UserData;
}

TABLE_SEARCH_RESULT NTAPI RtlpFindGenericTableNodeOrParent( 	
	IN PRTL_GENERIC_TABLE  	Table,
	IN PVOID  	Buffer,
	OUT PRTL_SPLAY_LINKS *  	NodeOrParent 
) 		
{
    PRTL_SPLAY_LINKS CurrentNode, ChildNode;
    RTL_GENERIC_COMPARE_RESULTS Result;

    /* Quick check to see if the table is empty */
    if (RtlIsGenericTableEmpty(Table))
    {
        *NodeOrParent = NULL;
        return TableEmptyTree;
    }

    /* Set the current node */
    CurrentNode = Table->TableRoot;

    /* Start compare loop */
    while (TRUE)
    {
        /* Do the compare */
        Result = Table->CompareRoutine(Table,
                                       Buffer,
                                       &((PTABLE_ENTRY_HEADER)CurrentNode)->
                                       UserData);
        if (Result == GenericLessThan)
        {
            /* We're less, check if this is the left child */
            if ((ChildNode = RtlLeftChild(CurrentNode)))
            {
                /* Continue searching from this node */
                CurrentNode = ChildNode;
            }
            else
            {
                /* Otherwise, the element isn't in this tree */
                *NodeOrParent = CurrentNode;
                return TableInsertAsLeft;
            }
        }
        else if (Result == GenericGreaterThan)
        {
            /* We're more, check if this is the right child */
            if ((ChildNode = RtlRightChild(CurrentNode)))
            {
                /* Continue searching from this node */
                CurrentNode = ChildNode;
            }
            else
            {
                /* Otherwise, the element isn't in this tree */
                *NodeOrParent = CurrentNode;
                return TableInsertAsRight;
            }
        }
        else
        {
            /* We should've found the node */
            ASSERT(Result == GenericEqual);

            /* Return node found */
            *NodeOrParent = CurrentNode;
            return TableFoundNode;
        }
    }
}

PVOID 
NTAPI 
RtlLookupElementGenericTableFull( 	
	IN PRTL_GENERIC_TABLE  	Table,
	IN PVOID  	Buffer,
	OUT PVOID *  	NodeOrParent,
	OUT TABLE_SEARCH_RESULT *  	SearchResult 
) 		
{
    /* Do the initial lookup */
    *SearchResult = RtlpFindGenericTableNodeOrParent(Table,
                                                     Buffer,
                                                     (PRTL_SPLAY_LINKS *)
                                                     NodeOrParent);

    /* Check if we found anything */
    if ((*SearchResult == TableEmptyTree) || (*SearchResult != TableFoundNode))
    {
        /* Nothing found */
        return NULL;
    }

    /* Otherwise, splay the tree and return this entry */
    Table->TableRoot = RtlSplay(*NodeOrParent);
    return &((PTABLE_ENTRY_HEADER)*NodeOrParent)->UserData;
}

TABLE_SEARCH_RESULT FORCEINLINE RtlpFindAvlTableNodeOrParent( 	
	IN PRTL_AVL_TABLE  	Table,
	IN PVOID  	Buffer,
	OUT PRTL_BALANCED_LINKS *  	NodeOrParent 
) 		
{
    PRTL_BALANCED_LINKS CurrentNode, ChildNode;
    RTL_GENERIC_COMPARE_RESULTS Result;

    /* Quick check to see if the table is empty */
    if (!Table->NumberGenericTableElements) return TableEmptyTree;

    /* Set the current node */
    CurrentNode = RtlRightChildAvl(&Table->BalancedRoot);

    /* Start compare loop */
    while (TRUE)
    {
        /* Compare which side is greater */
        Result = RtlpAvlCompareRoutine(Table,
                                       Buffer,
                                       &((PTABLE_ENTRY_HEADER)CurrentNode)->
                                       UserData);
        if (Result == GenericLessThan)
        {
            /* We're less, check if this is the left child */
            ChildNode = RtlLeftChildAvl(CurrentNode);
            if (ChildNode)
            {
                /* Continue searching from this node */
                CurrentNode = ChildNode;
            }
            else
            {
                /* Otherwise, the element isn't in this tree */
                *NodeOrParent = CurrentNode;
                return TableInsertAsLeft;
            }
        }
        else if (Result == GenericGreaterThan)
        {
            /* We're more, check if this is the right child */
            ChildNode = RtlRightChildAvl(CurrentNode);
            if (ChildNode)
            {
                /* Continue searching from this node */
                CurrentNode = ChildNode;
            }
            else
            {
                /* Otherwise, the element isn't in this tree */
                *NodeOrParent = CurrentNode;
                return TableInsertAsRight;
            }
        }
        else
        {
            /* We should've found the node */
            ASSERT(Result == GenericEqual);

            /* Return node found */
            *NodeOrParent = CurrentNode;
            return TableFoundNode;
        }
    }
}

PVOID 
NTAPI 
RtlLookupElementGenericTableFullAvl( 	
	IN PRTL_AVL_TABLE  	Table,
	IN PVOID  	Buffer,
	IN OUT PVOID *  	NodeOrParent,
	IN OUT TABLE_SEARCH_RESULT *  	SearchResult 
) 		
{
    /* Find the node */
    *SearchResult = RtlpFindAvlTableNodeOrParent(Table,
                                                 Buffer,
                                                 (PRTL_BALANCED_LINKS*)NodeOrParent);
    if (*SearchResult != TableFoundNode) return NULL;

    /* Node found, return the user data */
    return &((PTABLE_ENTRY_HEADER)*NodeOrParent)->UserData;
}