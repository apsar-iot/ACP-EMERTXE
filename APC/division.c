/*******************************************************************************************************************************************************************
*Title                  : Division
*Description            : This function performs division of two given large numbers and stores the result
*                         in the resultant doubly linked list.
*Prototype              : int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR);
*Input Parameters       : head1: Pointer to the first node of the first doubly linked list.
*                         tail1: Pointer to the last node of the first doubly linked list.
*                         head2: Pointer to the first node of the second doubly linked list.
*                         tail2: Pointer to the last node of the second doubly linked list.
*                         headR: Pointer to the head pointer of the resultant doubly linked list.
*Output                 : Status (SUCCESS / FAILURE).
*Algorithm              : 1. Check whether the divisor is zero.
*                         2. Read each digit of the dividend and build the partial dividend.
*                         3. Compare the partial dividend with the divisor.
*                         4. Repeatedly subtract the divisor from the partial dividend.
*                         5. Count the subtractions to obtain each quotient digit.
*                         6. Append each quotient digit to the quotient list.
*                         7. Remove leading zeros from the quotient.
*                         8. Store the quotient in the resultant list.
*                         9. Free allocated memory and return the operation status.
*Return Value           : SUCCESS if division is completed successfully.
*                         FAILURE if the divisor is zero or memory allocation or subtraction fails.
*Author                 : APSAR
*Date                   : 24-10-2026
*******************************************************************************************************************************************************************/
#include "apc.h"
#include <stdlib.h>

/* Insert a digit at the end of a list */
static int insert_last(Dlist **head, Dlist **tail, int data)
{
    Dlist *newNode = malloc(sizeof(Dlist));

    if (newNode == NULL)
        return FAILURE;

    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = *tail;

    if (*head == NULL)
        *head = newNode;
    else
        (*tail)->next = newNode;

    *tail = newNode;
    return SUCCESS;
}

/* Copy one list into another */
static int copy_list(Dlist *src, Dlist **head, Dlist **tail)
{
    while (src != NULL)
    {
        if (insert_last(head, tail, src->data) == FAILURE)
            return FAILURE;

        src = src->next;
    }

    return SUCCESS;
}

/* Find the tail of a list */
static Dlist *find_tail(Dlist *head)
{
    if (head == NULL)
        return NULL;

    while (head->next != NULL)
        head = head->next;

    return head;
}

/* Remove all nodes from a list */
static void free_list(Dlist **head, Dlist **tail)
{
    Dlist *temp = *head;

    while (temp != NULL)
    {
        Dlist *next = temp->next;
        free(temp);
        temp = next;
    }

    *head = NULL;
    *tail = NULL;
}

/* Remove leading zeros */
static void remove_zeros(Dlist **head, Dlist **tail)
{
    while (*head != NULL && (*head)->data == 0 &&
           (*head)->next != NULL)
    {
        Dlist *temp = *head;
        *head = temp->next;
        (*head)->prev = NULL;
        free(temp);
    }

    *tail = find_tail(*head);
}

/* Compare two non-negative numbers.
   Returns 1 if a>b, 0 if equal, -1 if a<b. */
static int compare_lists(Dlist *a, Dlist *b)
{
    int lenA = 0, lenB = 0;

    while (a != NULL)
    {
        lenA++;
        a = a->next;
    }

    while (b != NULL)
    {
        lenB++;
        b = b->next;
    }

    if (lenA > lenB)
        return 1;

    if (lenA < lenB)
        return -1;

    /* Lists have equal length */
    /* Caller supplies the heads again below */
    return 0;
}

/* Full comparison */
static int compare_numbers(Dlist *a, Dlist *b)
{
    Dlist *p = a;
    Dlist *q = b;
    int result = compare_lists(a, b);

    if (result != 0)
        return result;

    while (p != NULL && q != NULL)
    {
        if (p->data > q->data)
            return 1;

        if (p->data < q->data)
            return -1;

        p = p->next;
        q = q->next;
    }

    return 0;
}

/* Division using repeated subtraction */
int division(Dlist **head1, Dlist **tail1,
             Dlist **head2, Dlist **tail2,
             Dlist **headR)
{
    Dlist *res1 = NULL, *tailRes1 = NULL;
    Dlist *subResult = NULL, *tailSub = NULL;
    Dlist *quotient = NULL, *tailQuotient = NULL;
    Dlist *digitNode;
    Dlist *input;
    int count;

    *headR = NULL;

    if (*head2 == NULL)
        return FAILURE;

    /* Divisor must not be zero */
    input = *head2;
    while (input != NULL && input->data == 0)
        input = input->next;

    if (input == NULL)
        return FAILURE;

    /*
     * Bring down each digit from OP1 into res1.
     * For each partial dividend, repeatedly subtract OP2.
     */
    input = *head1;

    while (input != NULL)
    {
        if (insert_last(&res1, &tailRes1, input->data) == FAILURE)
            goto error;

        remove_zeros(&res1, &tailRes1);

        count = 0;

        while (compare_numbers(res1, *head2) >= 0)
        {
            free_list(&subResult, &tailSub);

            if (subtraction(&res1, &tailRes1,
                            head2, tail2, &subResult) == FAILURE)
                goto error;

            /* Replace res1 with the subtraction result */
            free_list(&res1, &tailRes1);

            res1 = subResult;
            tailRes1 = find_tail(res1);

            subResult = NULL;
            tailSub = NULL;

            remove_zeros(&res1, &tailRes1);
            count++;
        }

        /* Each quotient digit is between 0 and 9 */
        if (insert_last(&quotient, &tailQuotient, count) == FAILURE)
            goto error;

        input = input->next;
    }

    remove_zeros(&quotient, &tailQuotient);

    /* Transfer quotient list to output */
    *headR = quotient;
    quotient = NULL;
    tailQuotient = NULL;

    free_list(&res1, &tailRes1);
    free_list(&subResult, &tailSub);

    return SUCCESS;

error:
    free_list(&res1, &tailRes1);
    free_list(&subResult, &tailSub);
    free_list(&quotient, &tailQuotient);

    return FAILURE;
}

