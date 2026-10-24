/*******************************************************************************************************************************************************************
 * Title             : Addition
 *
 * Description       : This function performs addition of two given large numbers
 *                     and stores the result in the resultant doubly linked list.
 *
 * Prototype         : int addition(Dlist **head1, Dlist **tail1,
 *                                  Dlist **head2, Dlist **tail2,
 *                                  Dlist **headR);
 *
 * Input Parameters : head1 : Pointer to the head of the first doubly linked list.
 *                    tail1 : Pointer to the tail of the first doubly linked list.
 *                    head2 : Pointer to the head of the second doubly linked list.
 *                    tail2 : Pointer to the tail of the second doubly linked list.
 *                    headR : Pointer to the head pointer of the resultant list.
 *
 * Output            : SUCCESS - If addition is performed successfully.
 *                     FAILURE - If memory allocation fails.
 *
 * Return Value      : Returns SUCCESS or FAILURE.
 *
 * Algorithm         : 1. Initialize pointers to the last nodes of both input lists.
 *                     2. Start carry with zero and initialize the result list to NULL.
 *                     3. Add digits from right to left along with the carry.
 *                     4. Calculate the new carry and resultant digit.
 *                     5. Allocate a new node for each resultant digit.
 *                     6. Insert each new node at the beginning of the result list.
 *                     7. Repeat until both lists and the carry are empty.
 *                     8. Return SUCCESS after completing the addition.
 * 
 * Author             : APSAR
 * Date               : 24-10-2026
 *
 *******************************************************************************************************************************************************************/
#include "apc.h"
#include<stdlib.h>
int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR)
{
	/* Definition goes here */
	  Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;
    Dlist *newNode;
    int sum, carry = 0;

    *headR = NULL;

    while (temp1 != NULL || temp2 != NULL || carry != 0)
    {
        sum = carry;

        if (temp1 != NULL)
        {
            sum += temp1->data;
            temp1 = temp1->prev;
        }

        if (temp2 != NULL)
        {
            sum += temp2->data;
            temp2 = temp2->prev;
        }

        carry = sum / 10;
        sum = sum % 10;

        newNode = malloc(sizeof(Dlist));
        if (newNode == NULL)
            return FAILURE;

        newNode->data = sum;
        newNode->prev = NULL;
        newNode->next = *headR;

        if (*headR != NULL)
            (*headR)->prev = newNode;

        *headR = newNode;
    }

    return SUCCESS;
	
	
}
