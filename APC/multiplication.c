/*******************************************************************************************************************************************************************
*Title                  : Multiplication
*Description            : This function performs multiplication of two given large numbers and stores the result
*                         in the resultant doubly linked list.
*Prototype              : int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR);
*Input Parameters       : head1: Pointer to the head of the first doubly linked list.
*                         tail1: Pointer to the tail of the first doubly linked list.
*                         head2: Pointer to the head of the second doubly linked list.
*                         tail2: Pointer to the tail of the second doubly linked list.
*                         headR: Pointer to the head pointer of the resultant doubly linked list.
*Output                 : Status (SUCCESS / FAILURE).
*Algorithm              : 1. Initialize the accumulated result to zero.
*                         2. Traverse each digit of the second number from right to left.
*                         3. Multiply the current digit with each digit of the first number.
*                         4. Calculate the product digit and carry.
*                         5. Append zeros to shift the partial product according to its decimal position.
*                         6. Add the partial product to the accumulated result.
*                         7. Repeat until all digits of the second number are processed.
*                         8. Remove leading zeros from the final result.
*                         9. Store the final result in the resultant list.
*Return Value           : SUCCESS if multiplication is completed successfully.
*                         FAILURE if memory allocation or an intermediate operation fails.
*Author                 : APSAR
*Date                   : 24-10-2026
*******************************************************************************************************************************************************************/
#include "apc.h"
#include <stdlib.h>

/* Insert a digit at the beginning */
static int insert_front(Dlist **head, Dlist **tail, int digit)
{
    Dlist *newNode = malloc(sizeof(Dlist));

    if (newNode == NULL)
        return FAILURE;

    newNode->data = digit;
    newNode->prev = NULL;
    newNode->next = *head;

    if (*head != NULL)
        (*head)->prev = newNode;
    else
        *tail = newNode;

    *head = newNode;
    return SUCCESS;
}

/* Free a linked list */
static void free_list(Dlist **head, Dlist **tail)
{
    Dlist *temp;

    while (*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    *tail = NULL;
}

/* Add two numbers and store the result in result3 */
static int add_lists(Dlist *head1, Dlist *tail1,
                     Dlist *head2, Dlist *tail2,
                     Dlist **head3, Dlist **tail3)
{
    int sum, carry = 0;
    Dlist *p = tail1;
    Dlist *q = tail2;

    *head3 = NULL;
    *tail3 = NULL;

    while (p != NULL || q != NULL || carry != 0)
    {
        sum = carry;

        if (p != NULL)
        {
            sum += p->data;
            p = p->prev;
        }

        if (q != NULL)
        {
            sum += q->data;
            q = q->prev;
        }

        if (insert_front(head3, tail3, sum % 10) == FAILURE)
        {
            free_list(head3, tail3);
            return FAILURE;
        }

        carry = sum / 10;
    }

    if (*head3 == NULL)
        return insert_front(head3, tail3, 0);

    return SUCCESS;
}

/* Multiply two large non-negative integers */
int multiplication(Dlist **head1, Dlist **tail1,
                   Dlist **head2, Dlist **tail2,
                   Dlist **headR)
{
    Dlist *result1 = NULL, *tailR1 = NULL;
    Dlist *result2 = NULL, *tailR2 = NULL;
    Dlist *result3 = NULL, *tailR3 = NULL;
    Dlist *temp1, *temp2;
    int product, carry, zeros = 0;

    *headR = NULL;

    /* Zero is the initial accumulated result */
    if (insert_front(&result3, &tailR3, 0) == FAILURE)
        return FAILURE;

    temp2 = *tail2;

    /* Process each digit of the second number from right to left */
    while (temp2 != NULL)
    {
        temp1 = *tail1;
        carry = 0;

        result1 = NULL;
        tailR1 = NULL;

        /* Multiply every digit of the first number */
        while (temp1 != NULL || carry != 0)
        {
            product = carry;

            if (temp1 != NULL)
            {
                product += temp1->data * temp2->data;
                temp1 = temp1->prev;
            }

            if (insert_front(&result1, &tailR1,
                             product % 10) == FAILURE)
            {
                free_list(&result1, &tailR1);
                free_list(&result2, &tailR2);
                free_list(&result3, &tailR3);
                return FAILURE;
            }

            carry = product / 10;
        }

        /* Add zeros at the end for decimal place shifting */
        for (int i = 0; i < zeros; i++)
        {
            Dlist *newNode = malloc(sizeof(Dlist));

            if (newNode == NULL)
            {
                free_list(&result1, &tailR1);
                free_list(&result2, &tailR2);
                free_list(&result3, &tailR3);
                return FAILURE;
            }

            newNode->data = 0;
            newNode->next = NULL;
            newNode->prev = tailR1;

            if (tailR1 != NULL)
                tailR1->next = newNode;
            else
                result1 = newNode;

            tailR1 = newNode;
        }

        /* result2 = result3 + result1 */
        if (add_lists(result3, tailR3, result1, tailR1,
                      &result2, &tailR2) == FAILURE)
        {
            free_list(&result1, &tailR1);
            free_list(&result3, &tailR3);
            return FAILURE;
        }

        /* Replace old accumulated result with result2 */
        free_list(&result1, &tailR1);
        free_list(&result3, &tailR3);

        result3 = result2;
        tailR3 = tailR2;

        result2 = NULL;
        tailR2 = NULL;

        temp2 = temp2->prev;
        zeros++;
    }

    /* Transfer the final result to headR */
    *headR = result3;

    /* Remove leading zeros, retaining one zero if necessary */
    while (*headR != NULL &&
           (*headR)->data == 0 &&
           (*headR)->next != NULL)
    {
        Dlist *temp = *headR;
        *headR = (*headR)->next;
        (*headR)->prev = NULL;
        free(temp);
    }

    return SUCCESS;
}
