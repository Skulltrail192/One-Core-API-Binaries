/*
 * PROJECT:         ReactOS system libraries
 * LICENSE:         GNU GPL - See COPYING in the top level directory
 *                  BSD - See COPYING.ARM in the top level directory
 * FILE:            lib/rtl/bitmap.c
 * PURPOSE:         Bitmap functions
 * PROGRAMMER:      Timo Kreuzer (timo.kreuzer@reactos.org)
 */

/* INCLUDES *****************************************************************/

#include <rtl.h>
#include <rtlfuncs.h>

#define NDEBUG
#include <debug.h>

/* FULLY FROM WINDOWS 2000 SOURCE CODE*/

#define RTL_NTC_PREFIX_TABLE             ((CSHORT)0x0200)
#define RTL_NTC_ROOT                     ((CSHORT)0x0201)
#define RTL_NTC_INTERNAL                 ((CSHORT)0x0202)

typedef struct _PREFIX_TABLE_ENTRY
{
      CSHORT NodeTypeCode;
      CSHORT NameLength;
      struct _PREFIX_TABLE_ENTRY *NextPrefixTree;
      RTL_SPLAY_LINKS Links;
      PSTRING Prefix;
} PREFIX_TABLE_ENTRY, *PPREFIX_TABLE_ENTRY;

typedef struct _PREFIX_TABLE
{
      CSHORT NodeTypeCode;
      CSHORT NameLength;
      PPREFIX_TABLE_ENTRY NextPrefixTree;
} PREFIX_TABLE, *PPREFIX_TABLE;

typedef enum _COMPARISON {
    IsLessThan,
    IsPrefix,
    IsEqual,
    IsGreaterThan
} COMPARISON;

CLONG
ComputeNameLength(
    IN PSTRING Name
    )

/*++

Routine Description:

    This routine counts the number of names appearing in the input string.
    It does this by simply counting the number of backslashes in the string.
    To handle ill-formed names (i.e., names that do not contain a backslash)
    this routine really returns the number of backslashes plus 1.

Arguments:

    Name - Supplies the input name to examine

Returns Value:

    CLONG - the number of names in the input string

--*/

{
    ULONG NameLength;
    ULONG i;
    ULONG Count;

    extern PUSHORT NlsLeadByteInfo;  // Lead byte info. for ACP ( nlsxlat.c )
    extern BOOLEAN NlsMbCodePageTag;

    RTL_PAGED_CODE;

    //
    //  Save the name length, this should make the compiler be able to
    //  optimize not having to reload the length each time
    //

    NameLength = Name->Length - 1;

    //
    //  Now loop through the input string counting back slashes
    //

    if (NlsMbCodePageTag) {

        //
        // ComputeNameLength() skip DBCS character when counting '\'
        //

        for (i = 0, Count = 1; i < NameLength; ) {

            if (NlsLeadByteInfo[(UCHAR)Name->Buffer[i]]) {

                i += 2;

            } else {

                if (Name->Buffer[i] == '\\') {

                    Count += 1;
                }

                i += 1;
            }
        }

    } else {

        for (i = 0, Count = 1; i < NameLength; i += 1) {

            //
            //  check for a back slash
            //

            if (Name->Buffer[i] == '\\') {

                Count += 1;
            }
        }
    }

    //
    //  return the number of back slashes we found
    //

    //DbgPrint("ComputeNameLength(%s) = %x\n", Name->Buffer, Count);

    return Count;
}

COMPARISON
CompareNamesCaseSensitive (
    IN PSTRING Prefix,
    IN PSTRING Name
    )

/*++

Routine Description:

    This routine takes a prefix string and a full name string and determines
    if the prefix string is a proper prefix of the name string (case sensitive)

Arguments:

    Prefix - Supplies the input prefix string

    Name - Supplies the full name input string

Return Value:

    COMPARISON - returns

        IsLessThan    if Prefix < Name lexicalgraphically,
        IsPrefix      if Prefix is a proper prefix of Name
        IsEqual       if Prefix is equal to Name, and
        IsGreaterThan if Prefix > Name lexicalgraphically

--*/

{
    ULONG PrefixLength;
    ULONG NameLength;
    ULONG MinLength;
    ULONG i;

    UCHAR PrefixChar;
    UCHAR NameChar;

    extern PUSHORT NlsLeadByteInfo;  // Lead byte info. for ACP ( nlsxlat.c )
    extern BOOLEAN NlsMbCodePageTag;

    RTL_PAGED_CODE;

    //DbgPrint("CompareNamesCaseSensitive(\"%s\", \"%s\") = ", Prefix->Buffer, Name->Buffer);

    //
    //  Save the length of the prefix and name string, this should allow
    //  the compiler to not need to reload the length through a pointer every
    //  time we need their values
    //

    PrefixLength = Prefix->Length;
    NameLength = Name->Length;

    //
    //  Special case the situation where the prefix string is simply "\" and
    //  the name starts with an "\"
    //

    if ((Prefix->Length == 1) && (Prefix->Buffer[0] == '\\') &&
        (Name->Length > 1) && (Name->Buffer[0] == '\\')) {
        //DbgPrint("IsPrefix\n");
        return IsPrefix;
    }

    //
    //  Figure out the minimum of the two lengths
    //

    MinLength = (PrefixLength < NameLength ? PrefixLength : NameLength);

    //
    //  Loop through looking at all of the characters in both strings
    //  testing for equalilty, less than, and greater than
    //

    i = (ULONG) RtlCompareMemory( &Prefix->Buffer[0], &Name->Buffer[0], MinLength );

    if (i < MinLength) {

        UCHAR c;

        //
        //  Get both characters to examine and keep their case
        //

        PrefixChar = ((c = Prefix->Buffer[i]) == '\\' ? (CHAR)0 : c);
        NameChar   = ((c = Name->Buffer[i])   == '\\' ? (CHAR)0 : c);

        //
        //  Unfortunately life is not so easy in DBCS land.
        //

        if (NlsMbCodePageTag) {

            //
            // CompareNamesCaseSensitive(): check backslash in trailing bytes
            //

            if (Prefix->Buffer[i] == '\\') {

                ULONG j;
                extern PUSHORT   NlsLeadByteInfo;  // Lead byte info. for ACP ( nlsxlat.c )

                for (j = 0; j < i;) {

                    j += NlsLeadByteInfo[(UCHAR)Prefix->Buffer[j]] ? 2 : 1;
                }

                if (j != i) {

                    PrefixChar = '\\';
                    //DbgPrint("RTL:CompareNamesCaseSensitive encountered a fake backslash!\n");
                }
            }

            if (Name->Buffer[i] == '\\') {

                ULONG j;
                extern PUSHORT   NlsLeadByteInfo;  // Lead byte info. for ACP ( nlsxlat.c )

                for (j = 0; j < i;) {

                    j += NlsLeadByteInfo[(UCHAR)Name->Buffer[j]] ? 2 : 1;
                }

                if (j != i) {

                    NameChar = '\\';
                    //DbgPrint("RTL:CompareNamesCaseSensitive encountered a fake backslash!\n");
                }
            }
        }

        //
        //  Now compare the characters
        //

        if (PrefixChar < NameChar) {

            return IsLessThan;

        } else if (PrefixChar > NameChar) {

            return IsGreaterThan;
        }
    }

    //
    //  They match upto the minimum length so now figure out the largest string
    //  and see if one is a proper prefix of the other
    //

    if (PrefixLength < NameLength) {

        //
        //  The prefix string is shorter so if it is a proper prefix we
        //  return prefix otherwise we return less than (e.g., "\a" < "\ab")
        //

        if (Name->Buffer[PrefixLength] == '\\') {

            return IsPrefix;

        } else {

            return IsLessThan;
        }

    } else if (PrefixLength > NameLength) {

        //
        //  The Prefix string is longer so we say that the prefix is
        //  greater than the name (e.g., "\ab" > "\a")
        //

        return IsGreaterThan;

    } else {

        //
        //  They lengths are equal so the strings are equal
        //

        return IsEqual;
    }
}

PPREFIX_TABLE_ENTRY
NTAPI
PfxFindPrefix (
    IN PPREFIX_TABLE PrefixTable,
    IN PSTRING FullName
    )

/*++

Routine Description:

    This routine finds if a full name has a prefix in a prefix table.
    It returns a pointer to the largest proper prefix found if one exists.

Arguments:

    PrefixTable - Supplies the prefix table to search

    FullString - Supplies the name to search for

Return Value:

    PPREFIX_TABLE_ENTRY - a pointer to the longest prefix found if one
        exists, and NULL otherwise

--*/

{
    CLONG NameLength;

    PPREFIX_TABLE_ENTRY PreviousTree;
    PPREFIX_TABLE_ENTRY CurrentTree;
    PPREFIX_TABLE_ENTRY NextTree;

    PRTL_SPLAY_LINKS Links;

    PPREFIX_TABLE_ENTRY Node;

    COMPARISON Comparison;

    RTL_PAGED_CODE;

    //
    //  Determine the name length of the input string
    //

    NameLength = ComputeNameLength(FullName);

    //
    //  Locate the first tree that can contain a prefix
    //

    PreviousTree = (PPREFIX_TABLE_ENTRY)PrefixTable;
    CurrentTree = PreviousTree->NextPrefixTree;

    while (CurrentTree->NameLength > (CSHORT)NameLength) {

        PreviousTree = CurrentTree;
        CurrentTree = CurrentTree->NextPrefixTree;
    }

    //
    //  Now search for a prefix until we find one or until we exhaust
    //  the prefix trees
    //

    while (CurrentTree->NameLength > 0) {

        Links = &CurrentTree->Links;

        while (Links != NULL) {

            Node = CONTAINING_RECORD(Links, PREFIX_TABLE_ENTRY, Links);

            //
            //  Compare the prefix in the tree with the full name
            //

            Comparison = CompareNamesCaseSensitive(Node->Prefix, FullName);

            //
            //  See if they don't match
            //

            if (Comparison == IsGreaterThan) {

                //
                //  The prefix is greater than the full name
                //  so we go down the left child
                //

                Links = RtlLeftChild(Links);

                //
                //  And continue searching down this tree
                //

            } else if (Comparison == IsLessThan) {

                //
                //  The prefix is less than the full name
                //  so we go down the right child
                //

                Links = RtlRightChild(Links);

                //
                //  And continue searching down this tree
                //

            } else {

                //
                //  We found it.
                //
                //  Now that we've located the node we can splay the tree.
                //  To do this we need to remember how we find this tree in the root
                //  tree list, set the root to be an internal, splay, the tree, and
                //  then setup the new root node.
                //

                if (Node->NodeTypeCode == RTL_NTC_INTERNAL) {

                    //DbgPrint("PrefixTable  = %08lx\n", PrefixTable);
                    //DbgPrint("Node         = %08lx\n", Node);
                    //DbgPrint("CurrentTree  = %08lx\n", CurrentTree);
                    //DbgPrint("PreviousTree = %08lx\n", PreviousTree);
                    //DbgBreakPoint();

                    //
                    //  Save a pointer to the next tree, we already have the previous tree
                    //

                    NextTree = CurrentTree->NextPrefixTree;

                    //
                    //  Reset the current root to be an internal node
                    //

                    CurrentTree->NodeTypeCode = RTL_NTC_INTERNAL;
                    CurrentTree->NextPrefixTree = NULL;

                    //
                    //  Splay the tree and get the root
                    //

                    Node = CONTAINING_RECORD(RtlSplay(&Node->Links), PREFIX_TABLE_ENTRY, Links);

                    //
                    //  Set the new root's node type code and make it part of the
                    //  root tree list
                    //

                    Node->NodeTypeCode = RTL_NTC_ROOT;
                    PreviousTree->NextPrefixTree = Node;
                    Node->NextPrefixTree = NextTree;
                }

                return Node;
            }
        }

        //
        //  This tree is done so now find the next tree
        //

        PreviousTree = CurrentTree;
        CurrentTree = CurrentTree->NextPrefixTree;
    }

    //
    //  We sesarched everywhere and didn't find a prefix so tell the
    //  caller none was found
    //

    return NULL;
}

VOID
NTAPI
PfxInitialize (
    IN PPREFIX_TABLE PrefixTable
    )

/*++

Routine Description:

    This routine initializes a prefix table record to the empty state.

Arguments:

    PrefixTable - Supplies the prefix table being initialized

Return Value:

    None.

--*/

{
    RTL_PAGED_CODE;

    PrefixTable->NodeTypeCode = RTL_NTC_PREFIX_TABLE;

    PrefixTable->NameLength = 0;

    PrefixTable->NextPrefixTree = (PPREFIX_TABLE_ENTRY)PrefixTable;

    //
    //  return to our caller
    //

    return;
}

BOOLEAN
NTAPI
PfxInsertPrefix (
    IN PPREFIX_TABLE PrefixTable,
    IN PSTRING Prefix,
    IN PPREFIX_TABLE_ENTRY PrefixTableEntry
    )

/*++

Routine Description:

    This routine inserts a new prefix into the specified prefix table

Arguments:

    PrefixTable - Supplies the target prefix table

    Prefix - Supplies the string to be inserted in the prefix table

    PrefixTableEntry - Supplies the entry to use to insert the prefix

Return Value:

    BOOLEAN - TRUE if the Prefix is not already in the table, and FALSE
        otherwise

--*/

{
    ULONG PrefixNameLength;

    PPREFIX_TABLE_ENTRY PreviousTree;
    PPREFIX_TABLE_ENTRY CurrentTree;
    PPREFIX_TABLE_ENTRY NextTree;

    PPREFIX_TABLE_ENTRY Node;

    COMPARISON Comparison;

    RTL_PAGED_CODE;

    //
    //  Determine the name length of the input string
    //

    PrefixNameLength = ComputeNameLength(Prefix);

    //
    //  Setup parts of the prefix table entry that we will always need
    //

    PrefixTableEntry->NameLength = (CSHORT)PrefixNameLength;
    PrefixTableEntry->Prefix = Prefix;

    RtlInitializeSplayLinks(&PrefixTableEntry->Links);

    //
    //  find the corresponding tree, or find where the tree should go
    //

    PreviousTree = (PPREFIX_TABLE_ENTRY)PrefixTable;
    CurrentTree = PreviousTree->NextPrefixTree;

    while (CurrentTree->NameLength > (CSHORT)PrefixNameLength) {

        PreviousTree = CurrentTree;
        CurrentTree = CurrentTree->NextPrefixTree;

    }

    //
    //  If the name length of the current tree is not equal to the
    //  prefix name length then the tree does not exist and we need
    //  to make a new tree node.
    //

    if (CurrentTree->NameLength != (CSHORT)PrefixNameLength) {

        //
        //  Insert the new prefix entry to the list between
        //  previous and current tree
        //

        PreviousTree->NextPrefixTree = PrefixTableEntry;
        PrefixTableEntry->NextPrefixTree = CurrentTree;

        //
        //  And set the node type code
        //

        PrefixTableEntry->NodeTypeCode = RTL_NTC_ROOT;

        //
        //  And tell our caller everything worked fine
        //

        return TRUE;

    }

    //
    //  The tree does exist so now search the tree for our
    //  position in it.  We only exit the loop if we've inserted
    //  a new node, and node is left is left pointing to the
    //  tree position
    //

    Node = CurrentTree;

    while (TRUE) {

        //
        //  Compare the prefix in the tree with the prefix we want
        //  to insert
        //

        Comparison = CompareNamesCaseSensitive(Node->Prefix, Prefix);

        //
        //  If we do match case sensitive then we cannot add
        //  this prefix so we return false.  Note this is the
        //  only condition where we return false
        //

        if (Comparison == IsEqual) {

            return FALSE;
        }

        //
        //  If the tree prefix is greater than the new prefix then
        //  we go down the left subtree
        //

        if (Comparison == IsGreaterThan) {

            //
            //  We want to go down the left subtree, first check to see
            //  if we have a left subtree
            //

            if (RtlLeftChild(&Node->Links) == NULL) {

                //
                //  there isn't a left child so we insert ourselves as the
                //  new left child
                //

                PrefixTableEntry->NodeTypeCode = RTL_NTC_INTERNAL;
                PrefixTableEntry->NextPrefixTree = NULL;

                RtlInsertAsLeftChild(&Node->Links, &PrefixTableEntry->Links);

                //
                //  and exit the while loop
                //

                break;

            } else {

                //
                //  there is a left child so simply go down that path, and
                //  go back to the top of the loop
                //

                Node = CONTAINING_RECORD( RtlLeftChild(&Node->Links),
                                          PREFIX_TABLE_ENTRY,
                                          Links );

            }

        } else {

            //
            //  The tree prefix is either less than or a proper prefix
            //  of the new string.  We treat both cases a less than when
            //  we do insert.  So we want to go down the right subtree,
            //  first check to see if we have a right subtree
            //

            if (RtlRightChild(&Node->Links) == NULL) {

                //
                //  These isn't a right child so we insert ourselves as the
                //  new right child
                //

                PrefixTableEntry->NodeTypeCode = RTL_NTC_INTERNAL;
                PrefixTableEntry->NextPrefixTree = NULL;

                RtlInsertAsRightChild(&Node->Links, &PrefixTableEntry->Links);

                //
                //  and exit the while loop
                //

                break;

            } else {

                //
                //  there is a right child so simply go down that path, and
                //  go back to the top of the loop
                //

                Node = CONTAINING_RECORD( RtlRightChild(&Node->Links),
                                          PREFIX_TABLE_ENTRY,
                                          Links );
            }

        }

    }

    //
    //  Now that we've inserted the new node we can splay the tree.
    //  To do this we need to remember how we find this tree in the root
    //  tree list, set the root to be an internal, splay, the tree, and
    //  then setup the new root node.  Note: we cannot splay the prefix table
    //  entry because it might be a case match node so we only splay
    //  the Node variable, which for case match insertions is the
    //  internal node for the case match and for non-case match insertions
    //  the Node variable is the parent node.
    //

    //
    //  Save a pointer to the next tree, we already have the previous tree
    //

    NextTree = CurrentTree->NextPrefixTree;

    //
    //  Reset the current root to be an internal node
    //

    CurrentTree->NodeTypeCode = RTL_NTC_INTERNAL;
    CurrentTree->NextPrefixTree = NULL;

    //
    //  Splay the tree and get the root
    //

    Node = CONTAINING_RECORD(RtlSplay(&Node->Links), PREFIX_TABLE_ENTRY, Links);

    //
    //  Set the new root's node type code and make it part of the
    //  root tree list
    //

    Node->NodeTypeCode = RTL_NTC_ROOT;
    PreviousTree->NextPrefixTree = Node;
    Node->NextPrefixTree = NextTree;

    //
    //  tell our caller everything worked fine
    //

    return TRUE;
}

VOID
NTAPI
PfxRemovePrefix (
    IN PPREFIX_TABLE PrefixTable,
    IN PPREFIX_TABLE_ENTRY PrefixTableEntry
    )

/*++

Routine Description:

    This routine removes the indicated prefix table entry from
    the prefix table

Arguments:

    PrefixTable - Supplies the prefix table affected

    PrefixTableEntry - Supplies the prefix entry to remove

Return Value:

    None.

--*/

{
    PRTL_SPLAY_LINKS Links;

    PPREFIX_TABLE_ENTRY Root;
    PPREFIX_TABLE_ENTRY NewRoot;

    PPREFIX_TABLE_ENTRY PreviousTree;

    RTL_PAGED_CODE;

    //
    //  case on the type of node that we are trying to delete
    //

    switch (PrefixTableEntry->NodeTypeCode) {

    case RTL_NTC_INTERNAL:
    case RTL_NTC_ROOT:

        //
        //  The node is internal or root node so we need to delete it from
        //  the tree, but first find the root of the tree
        //

        Links = &PrefixTableEntry->Links;

        while (!RtlIsRoot(Links)) {

            Links = RtlParent(Links);
        }

        Root = CONTAINING_RECORD( Links, PREFIX_TABLE_ENTRY, Links );

        //
        //  Now delete the node
        //

        Links = RtlDelete(&PrefixTableEntry->Links);

        //
        //  Now see if the tree is deleted
        //

        if (Links == NULL) {

            //
            //  The tree is now empty so remove this tree from
            //  the tree list, by first finding the previous tree that
            //  references us
            //

            PreviousTree = Root->NextPrefixTree;

            while ( PreviousTree->NextPrefixTree != Root ) {

                PreviousTree = PreviousTree->NextPrefixTree;
            }

            //
            //  We've located the previous tree so now just have it
            //  point around the deleted node
            //

            PreviousTree->NextPrefixTree = Root->NextPrefixTree;

            //
            //  and return the our caller
            //

            return;
        }

        //
        //  The tree is not deleted but see if we changed roots
        //

        if (&Root->Links != Links) {

            //
            //  Get a pointer to the new root
            //

            NewRoot = CONTAINING_RECORD(Links, PREFIX_TABLE_ENTRY, Links);

            //
            //  We changed root so we better need to make the new
            //  root part of the prefix data structure, by
            //  first finding the previous tree that
            //  references us
            //

            PreviousTree = Root->NextPrefixTree;

            while ( PreviousTree->NextPrefixTree != Root ) {

                PreviousTree = PreviousTree->NextPrefixTree;
            }

            //
            //  Set the new root
            //

            NewRoot->NodeTypeCode = RTL_NTC_ROOT;

            PreviousTree->NextPrefixTree = NewRoot;
            NewRoot->NextPrefixTree = Root->NextPrefixTree;

            //
            //  Set the old root to be an internal node
            //

            Root->NodeTypeCode = RTL_NTC_INTERNAL;

            Root->NextPrefixTree = NULL;

            //
            //  And return to our caller
            //

            return;
        }

        //
        //  We didn't change roots so everything is fine and we can
        //  simply return to our caller
        //

        return;

    default:

        //
        //  If we get here then there was an error and the node type
        //  code is unknown
        //

        return;
    }
}
