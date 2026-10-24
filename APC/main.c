/*******************************************************************************************************************************************************************
* Title             : Main Function
*
* Description       : This file performs arithmetic operations on two large signed numbers using doubly
*                     linked lists. It validates the command-line arguments, creates linked lists for
*                     the input numbers, calls the appropriate arithmetic function, displays the result,
*                     and frees the allocated memory.
*
* Prototype         : int main(int argc, char *argv[]);
*
* Input Parameters  : argc : Number of command-line arguments.
*                     argv : Array containing the first number, operator, and second number.
*
* Operations        : Addition       (+)
*                     Subtraction    (-)
*                     Multiplication (*)
*                     Division       (/)
*
* Output            : Displays the calculated result or an appropriate error message.
*
* Return Value      : SUCCESS - If the operation completes successfully.
*                     FAILURE - If invalid input, memory allocation failure, or an arithmetic
*                               operation failure occurs.
*
* Algorithm         : 1. Validate the number of command-line arguments.
*                     2. Validate the arithmetic operator.
*                     3. Extract the signs of both input numbers.
*                     4. Validate the numeric strings.
*                     5. Create doubly linked lists for both numbers.
*                     6. Perform the selected arithmetic operation.
*                     7. Determine the sign of the result.
*                     8. Display the result.
*                     9. Free the allocated memory.
*
*  Author             : APSAR
*  Date               : 24-10-2026
*******************************************************************************************************************************************************************/
#include "apc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Print the resultant linked list */
void print_result(Dlist *headR, int resultSign)
{
    if (headR == NULL)
    {
        printf("Result list is empty!\n");
        return;
    }

    /* Check whether the result is zero */
    Dlist *temp = headR;
    int isZero = 1;

    while (temp != NULL)
    {
        if (temp->data != 0)
        {
            isZero = 0;
            break;
        }
        temp = temp->next;
    }

    printf("Result: ");

    if (resultSign == -1 && !isZero)
        printf("-");

    while (headR != NULL)
    {
        printf("%d", headR->data);
        headR = headR->next;
    }

    printf("\n");
}

/*
   Returns:
   0  -> first number is greater
  -1  -> first number is smaller
   1  -> both numbers are equal
*/
int compare_numbers(Dlist *head1, Dlist *head2)
{
    int len1 = 0, len2 = 0;
    Dlist *temp;

    for (temp = head1; temp != NULL; temp = temp->next)
        len1++;

    for (temp = head2; temp != NULL; temp = temp->next)
        len2++;

    if (len1 > len2)
        return 0;

    if (len1 < len2)
        return -1;

    while (head1 != NULL && head2 != NULL)
    {
        if (head1->data > head2->data)
            return 0;

        if (head1->data < head2->data)
            return -1;

        head1 = head1->next;
        head2 = head2->next;
    }

    return 1;
}

/* Free a doubly linked list */
void free_list(Dlist **head)
{
    Dlist *temp;

    while (*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

/* Check whether a number contains only zeros */
int is_zero(Dlist *head)
{
    while (head != NULL)
    {
        if (head->data != 0)
            return 0;

        head = head->next;
    }

    return 1;
}

/* Validate an unsigned digit string */
int valid_number(char *num)
{
    if (*num == '\0')
        return 0;

    for (int i = 0; num[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)num[i]))
            return 0;
    }

    return 1;
}

/* Create a doubly linked list from a digit string */
int create_list(char *num, Dlist **head, Dlist **tail)
{
    for (int i = 0; num[i] != '\0'; i++)
    {
        Dlist *newNode = malloc(sizeof(Dlist));

        if (newNode == NULL)
            return FAILURE;

        newNode->data = num[i] - '0';
        newNode->prev = *tail;
        newNode->next = NULL;

        if (*tail != NULL)
            (*tail)->next = newNode;
        else
            *head = newNode;

        *tail = newNode;
    }

    return SUCCESS;
}

int main(int argc, char *argv[])
{
    Dlist *head1 = NULL, *tail1 = NULL;
    Dlist *head2 = NULL, *tail2 = NULL;
    Dlist *headR = NULL;

    char operator;
    char *num1, *num2;
    int sign1 = 1, sign2 = 1;
    int resultSign = 1;
    int status = SUCCESS;
    int cmp;

    if (argc != 4)
    {
        printf("Usage: ./Slist.exe <number1> <operator> <number2>\n");
        return FAILURE;
    }

    /* Validate operator */
    if (strlen(argv[2]) != 1 ||
        (argv[2][0] != '+' && argv[2][0] != '-' &&
         argv[2][0] != '*' && argv[2][0] != '/'))
    {
        printf("Invalid operator!\n");
        return FAILURE;
    }

    operator = argv[2][0];

    /*
       STEP 1: Extract signs.
       The linked lists will contain digits only.
    */
    num1 = argv[1];
    num2 = argv[3];

    if (*num1 == '-')
    {
        sign1 = -1;
        num1++;
    }
    else if (*num1 == '+')
    {
        num1++;
    }

    if (*num2 == '-')
    {
        sign2 = -1;
        num2++;
    }
    else if (*num2 == '+')
    {
        num2++;
    }

    /* STEP 2: Validate numbers after removing signs */
    if (!valid_number(num1))
    {
        printf("Invalid first number!\n");
        return FAILURE;
    }

    if (!valid_number(num2))
    {
        printf("Invalid second number!\n");
        return FAILURE;
    }

    /* STEP 3: Create digit-only linked lists */
    if (create_list(num1, &head1, &tail1) == FAILURE ||
        create_list(num2, &head2, &tail2) == FAILURE)
    {
        printf("Memory allocation failed!\n");
        free_list(&head1);
        free_list(&head2);
        return FAILURE;
    }

    /* STEP 4: Perform arithmetic */
    switch (operator)
    {
        case '+':
        case '-':
        {
            /*
               For subtraction, flip the sign of the
               second operand.

               Example:
               12 - (-13) becomes 12 + 13.
            */
            if (operator == '-')
                sign2 = -sign2;

            if (sign1 == sign2)
            {
                /* Same signs: add magnitudes */
                status = addition(
                    &head1, &tail1,
                    &head2, &tail2, &headR);

                resultSign = sign1;
            }
            else
            {
                /* Different signs: subtract magnitudes */
                cmp = compare_numbers(head1, head2);

                if (cmp == 1)
                {
                    /* Equal magnitudes -> zero */
                    status = subtraction(
                        &head1, &tail1,
                        &head2, &tail2, &headR);

                    resultSign = 1;
                }
                else if (cmp == 0)
                {
                    /* First magnitude is greater */
                    status = subtraction(
                        &head1, &tail1,
                        &head2, &tail2, &headR);

                    resultSign = sign1;
                }
                else
                {
                    /* Second magnitude is greater */
                    status = subtraction(
                        &head2, &tail2,
                        &head1, &tail1, &headR);

                    resultSign = sign2;
                }
            }

            break;
        }

        case '*':
        {
            status = multiplication(
                &head1, &tail1,
                &head2, &tail2, &headR);

            resultSign = sign1 * sign2;
            break;
        }

        case '/':
        {
            if (is_zero(head2))
            {
                printf("Error: Division by zero!\n");
                free_list(&head1);
                free_list(&head2);
                return FAILURE;
            }

            status = division(
                &head1, &tail1,
                &head2, &tail2, &headR);

            resultSign = sign1 * sign2;
            break;
        }
    }

    /* STEP 5: Check operation status and print result */
    if (status == FAILURE)
    {
        printf("Arithmetic operation failed!\n");
        free_list(&head1);
        free_list(&head2);
        free_list(&headR);
        return FAILURE;
    }

    print_result(headR, resultSign);

    /* STEP 6: Free memory */
    free_list(&head1);
    free_list(&head2);
    free_list(&headR);

    return SUCCESS;
}
