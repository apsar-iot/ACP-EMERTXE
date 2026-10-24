/*******************************************************************************************************************************************************************
*Title                  : Subtraction
*Description            : This function performs subtraction of two given large numbers and stores the result
*                         in the resultant doubly linked list.
*Prototype              : int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR);
*Input Parameters       : head1: Pointer to the first node of the first doubly linked list.
*                         tail1: Pointer to the last node of the first doubly linked list.
*                         head2: Pointer to the first node of the second doubly linked list.
*                         tail2: Pointer to the last node of the second doubly linked list.
*                         headR: Pointer to the head pointer of the resultant doubly linked list.
*Output                 : Status (SUCCESS / FAILURE).
*Algorithm              : 1. Initialize pointers to the last nodes of both input lists.
*                         2. Initialize borrow to zero and the result list to NULL.
*                         3. Subtract the corresponding digits from right to left.
*                         4. If the first digit is smaller, borrow 1 from the next position.
*                         5. Calculate the difference between the digits.
*                         6. Allocate a new node for each difference digit.
*                         7. Insert each new node at the beginning of the result list.
*                         8. Remove leading zeros from the resultant list.
*                         9. Return SUCCESS after completing the subtraction.
*Return Value           : SUCCESS if subtraction is completed successfully.
*                         FAILURE if memory allocation fails.
*Author                 : APSAR
*Date                   : 24-10-2026
*******************************************************************************************************************************************************************/
#include "apc.h"
#include <stdlib.h>
int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR)
{
	/* Definition goes here */
	 Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;
    Dlist *newNode;
    int digit1, digit2, diff, borrow = 0;

    *headR = NULL;

    while (temp1 != NULL)
    {
        digit1 = temp1->data - borrow;
        digit2 = (temp2 != NULL) ? temp2->data : 0;

        if (digit1 < digit2)
        {
            digit1 += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        diff = digit1 - digit2;

        newNode = malloc(sizeof(Dlist));
        if (newNode == NULL)
            return FAILURE;

        newNode->data = diff;
        newNode->prev = NULL;
        newNode->next = *headR;

        if (*headR != NULL)
            (*headR)->prev = newNode;

        *headR = newNode;

        temp1 = temp1->prev;

        if (temp2 != NULL)
            temp2 = temp2->prev;
    }

    /* Remove leading zeros */
    while (*headR != NULL && (*headR)->data == 0 &&
           (*headR)->next != NULL)
    {
        Dlist *temp = *headR;
        *headR = (*headR)->next;
        (*headR)->prev = NULL;
        free(temp);
    }

    return SUCCESS;
}
