// C program to implement a student database system
// Rowan M. Shah
// 17082571

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


# define LEN sizeof(struct Student)

typedef struct Student {
    char num[10]; // Student Number
    char name[30]; // Student Name
    float score[5];// Student Results
    struct Student *next;
}*PNODE;


// ****** STARTING OF BINARY TREE PART ******


//The left subtree is higher than the right subtree
#define LH 1
//The left subtree is as high as the right subtree
#define EH 0
// The left subtree is lower than the right subtree
#define RH -1
#define EQ(a,b) ((a) == (b))
#define LT(a,b) ((a) < (b))
#define LQ(a,b)((a) <= (b))

int nDataMode = 1; //1--struct;2--balanceed binary trees

// Declaring a struct for binary tree implementation
typedef struct BSTNode
{
    PNODE data;
    int bf;
    struct BSTNode * lchild;
    struct BSTNode * rchild;
}BSTNode;
typedef BSTNode * BSTree;

PNODE head;
BSTree* root;
char filename[30]={"studentinfo.txt"}; //Global variable, used to save the name of the file to open

// Data save (wb only)
void WriteData_wb(struct Student *head)
{
    FILE *fp;
    struct Student *p;
    if((fp = fopen(filename, "wb"))==NULL)
        printf("\a Error! The file cannot be opened!");

    p = head;
    while(p!=NULL)
    {
        if(fwrite(p,LEN,1,fp)!=1)
        {
            printf("Error writing data\n");
            fclose(fp);
            return;
        }
        p=p->next;
    }
    fclose(fp);
}

// Read data file saved to the list, return pointer to this list header
struct Student *ReadData(void)
{
    struct Student *head = NULL;
    struct Student *p1, *p2;//s = p1;p = p2;
    int taller;

    FILE *fp;
    if((fp=fopen(filename,"rb+"))==NULL)
    {
        printf("Error opening file\\n");
        exit(0);
    }

    while(!feof(fp))
    //Allocating memory
    {
        if((p1=(struct Student*)malloc(LEN))==NULL)
        {
            printf("Memory Request Error\n");
            fclose(fp);
            exit(0);
        }
        if(fread(p1,LEN,1,fp)!=1){
            free(p1);
            break;
        }

        if (nDataMode==2)
        {
            insert(root, p1, taller);
        }
        else
        {
            if(head==NULL)
                head=p2=p1;
            else{
                p2->next=p1;
                p2=p1;
            }
        }
    }
    fclose(fp);

    system("cls");
    printf("Retrieve data from file ok!\n");
    return (head);
}

// Left-handed
void leftRotate(BSTree* root)
{
    BSTree rc = (*root)->rchild;
    (*root)->rchild = rc->lchild;
    rc->lchild = *root;
    *root = rc;
}
// Right-handed
void rightRotate(BSTree* root)
{
    BSTree lc = (*root)->lchild;
    (*root)->lchild = lc->rchild;
    lc->rchild = *root;
    *root = lc;
}
// Left balance the root of the binary tree (LL type and LR type)
void leftBalance(BSTree* root)
{
    BSTree rc;
    BSTree lc = (*root)->lchild;
    switch (lc->bf)
    {
        //LL only requires right-handed operation
    case LH:
        //The balance between the root and the left subtree after the right rotation
        (*root)->bf = EH;
        lc->bf = EH;
        // Right-handed operation
        rightRotate(root);
        break;
        //LR type needs left-handed operation, then right-handed operation
    case RH:
        rc = lc->rchild;
        switch (rc->bf)
        {
        case LH:
            (*root)->bf = RH;
            lc->bf = EH;
            break;
        case EH:
            (*root)->bf = EH;
            lc->bf = EH;
            break;
        case RH:
            (*root)->bf = EH;
            lc->bf = LH;
            break;
        }
        rc->bf = EH;
        leftRotate(&(*root)->lchild);
        rightRotate(root);
        break;
    }
}
// Function: left balance the root of the binary tree (RR type and RL type)
void rightBalance(BSTree* root)
{
    BSTree lc;
    BSTree rc = (*root)->rchild;
    switch (rc->bf)
    {
        //RR only needs to do left-handed operation
    case RH:
        (*root)->bf = EH;
        rc->bf = EH;
        // Left-handed operation
        leftRotate(root);
        break;
        // RL type need to do a right-hand operation, and then do a left-hand operation
    case LH:
        lc = rc->lchild;
        switch (lc->bf)
        {
        case LH:
            (*root)->bf = EH;
            rc->bf = RH;
            break;
        case EH:
            (*root)->bf = EH;
            rc->bf = EH;
            break;
        case RH:
            (*root)->bf = LH;
            rc->bf = EH;
            break;
        }
        lc->bf = EH;
        rightRotate(&(*root)->rchild);
        leftRotate(root);
        break;
    }
}
// Function: Insert the element data into the balanced binary tree root
int insert(BSTree* root, PNODE data, int taller)
{
    if (NULL == (*root))
    {
        *root = (BSTree)malloc(sizeof(BSTNode));
        (*root)->rchild = NULL;
        (*root)->lchild = NULL;
        (*root)->data = data;
        (*root)->bf = EH;
        taller = 1;
    }
    else
    {
        //This element already exists in the balanced binary tree
        if (strcmp(data->name, (*root)->data->name)==0)
        {
            taller = 0;
            return taller;
        }
        // Insert the left subtree
        else if (strcmp(data->name, (*root)->data->name)<0)
        {
            if (!insert(&(*root)->lchild, data, taller))
            {
                return 0;
            }
            //The insert was successful and the tree became taller
            if (taller)
            {
                switch ((*root)->bf)
                {
                case LH:
                    leftBalance(root);
                    // Balanced binary tree after left balance operation
                    // Tree height does not change, so taller = false
                    taller = 0;
                    break;
                case EH:
                    (*root)->bf = LH;
                    // originally it was balanced so after inserting an element
                    //The tree height must increase
                    taller = 1;
                    break;
                case RH:
                    (*root)->bf = EH;
                    // It turns out that the right subtree is taller than the left subtree, but when it is in the left subtree
                    // When the element is inserted, the tree becomes balanced, so taller = false
                    taller = 0;
                    break;
                default:
                    break;
                }
            }
        }
        // Insert the right subtree
        else
        {
            if (!insert(&(*root)->rchild, data, taller))
            {
                return 0;
            }
            if (taller)
            {
                switch ((*root)->bf)
                {
                case LH:
                    (*root)->bf = EH;
                    taller = 0;
                    break;
                case EH:
                    (*root)->bf = RH;
                    taller = 1;
                    break;
                case RH:
                    rightBalance(root);
                    taller = 0;
                    break;
                }
            }
        }
    }
    return taller;
}

int DeleteAVL(BSTree* T, PNODE key, int shorter)
{
    if (!T)
    {
        //No such key
        shorter = 0;
        return shorter;
    }
    else
    {
        if (strcmp((*T)->data->name, key->name)==0) //find the need to delete the node
        {
            BSTree q = (*T);
            if (!(*T)->lchild) // if the node lchild is NULL
            {
                q = (*T);
                (*T) = (*T)->rchild;
                free(q);
                shorter = 1;
                return 1;
            }
            else if (!(*T)->rchild){//if the node rchild is NULL
                q = (*T);
                (*T) = (*T)->lchild;
                free(q);
                shorter = 1;
                return 1;
            }
            else {
                //delete a child about non empty nodes
                //The direct precursor node P data to replace the data node
                //Then change key=p.data
                BSTree s = (*T)->lchild;
                while (s->rchild)
                    s = s->rchild;
                (*T)->data = s->data;
                key = s->data;//Now, delete the vertice whose data was the new key
            }
        }
        if (strcmp(key->name, (*T)->data->name)<0)
        // here is different from InsertAVL
        {
            if (!DeleteAVL(&(*T)->lchild, key, shorter)) return 0;
            if (shorter){
                switch((*T)->bf){
                case LH:(*T)->bf = EH; shorter = 1;break;
                case EH:(*T)->bf = RH;shorter = 0;break;
                case RH:rightBalance(T);
                    if ((*T)->rchild->bf == EH)
                        shorter = 0;
                    else
                        shorter = 1;break;
                }
            }
        }
        else
        {
            if (!DeleteAVL(&(*T)->rchild, key, shorter)) return 0;
            if (shorter){
                switch((*T)->bf)
                {
                case LH:leftBalance(T);
                    if ((*T)->lchild->bf == EH)
                        shorter = 0;
                    else
                        shorter = 1;break;
                case EH:(*T)->bf = LH; shorter = 0;break;
                case RH:(*T)->bf = EH;shorter = 1;break;
                }
            }
        }
    }
    return shorter;
} //Delete

// insertNode: insert the node into the list in ascending order
// pHead is the head of the list pointer.
PNODE insertNode(PNODE pHead, PNODE node)
{
    // pointer to the new node.
    PNODE previous; // previous pointer to the current pointer
    PNODE current; // current pointer

    current = pHead; // initialize
    previous = NULL;

    // Look into the position
    while (current != NULL && strcmp(current->name, node->name)<0)
    {
        previous = current;
        current = current->next;
    }

    // Precursor and successor node
    node->next = current;
    if (previous == NULL) // insert node is the first node in the linked list, modify the head pointer
        pHead = node;
    else
        previous->next = node;

    return pHead;
}

// ****** STARTING OF LINKED LIST PART ******

// Function for introducing a new student into the linked list in alphabetical order
PNODE IntroduceStudent(PNODE pHead, int n)
{
    PNODE p1;
    int i;
    int taller;

    for (i=1; i < n+1; i++)
    {
        p1 = (struct Student*)malloc(LEN);

        printf("Please enter the student number %d :", i);
        scanf("%s", p1->num);
        printf("Please enter the name of student %d :", i);
        scanf("%s", p1->name);
        p1->next = NULL;
        p1->score[0]=0;
        p1->score[1]=0;

        if (nDataMode==2)
        {
            insert(root, p1, taller);
        }
        else
            head = insertNode(head, p1);
    }
    
    system("cls");
    printf("Introduction of student is successful!");
    return head;
}

// Function for deleting a student
PNODE RemoveStudent(PNODE pHead)
{
    PNODE pt1, pt2, head;
    char str_num[20];
    int shorter;
    printf("Please enter the student number:");
    scanf("%s", str_num);
    if (pHead==NULL)
    {
        pt1 = ReadData();
        head = pt1;
    }
    else
        head=pt1=pHead;
    while(pt2!=NULL)
    {
        if(strcmp(pt1->num,str_num)==0)
        {
            WriteData_wb(pt2);
            
            if (nDataMode==2)
            {
                DeleteAVL(root, pt1, shorter);
            }
            else
                free(pt1);

            break;
        }
        else if(strcmp(pt2->num,str_num)==0)
        {
            pt1->next = pt2->next;
            WriteData_wb(head);
            if (nDataMode==2)
            {
             DeleteAVL(root, pt2, shorter);
            }
            else
                free(pt2);
            
            break;
        }
        pt2 = pt2->next;
        pt1 = pt1->next;
    }

    if(pt2!=NULL)
        printf("The database does not store the students you want to delete!\n");

    printf("\n\n");
    printf("Student Removed successfully!\n");
    return 0;
}

// Function for introducing marks for a specific student
PNODE IntroduceMarks(PNODE pHead)
{
    PNODE pt1, pt2, head;
    char str_num[20];
    int nflag=0;
    printf("Please enter the student number:");
    scanf("%s", str_num);
    if (pHead==NULL)
    {
        pt1 = ReadData();
        head = pt1;
    }
    else
        head=pt1=pHead;

    while(pt1!=NULL)
    {
        if(strcmp(pt1->num,str_num)==0)
        {
            printf("Please enter first course grades:");
            scanf("%f", &pt1->score[0]);
            printf("Please enter second course grades:");
            scanf("%f", &pt1->score[1]);
            printf("Please enter third course grades:");
            scanf("%f", &pt1->score[2]);
            printf("Please enter fourth course grades:");
            scanf("%f", &pt1->score[3]);
            printf("Please enter fifth course grades:");
            scanf("%f", &pt1->score[4]);
            WriteData_wb(head);
            nflag=1;
            break;
        }
        pt1 = pt1->next;
    }

    if(nflag==0)
        printf("The database does not store the students you want to add!\n");

    printf("\n\n");
    system("cls");
    printf("Marks introduced successfully!\n");
    return head;
}
// Function for searching a student and print information
void PrintStudent(PNODE pHead)
{
    PNODE pt1, pt2, head;
    char str_num[20];
    int nflag=0;
    system("cls");
    printf("Please enter the student number that you want to print:");
    scanf("%s", str_num);
    if (pHead==NULL)
    {
        pt1 = ReadData();
        head = pt1;
    }
    else
        head=pt1=pHead;
    
    while(pt1!=NULL)
    {
        if(strcmp(pt1->num,str_num)==0)
        {
            printf("Student Number: %s\n", pt1->num);
            printf("Student Name: %s\n", pt1->name);
            printf("subject 1: %0.2f\n", pt1->score[0]);
            printf("subject 2: %0.2f\n", pt1->score[1]);
            printf("subject 3: %0.2f\n", pt1->score[2]);
            printf("subject 4: %0.2f\n", pt1->score[3]);
            printf("subject 5: %0.2f\n", pt1->score[4]);
            nflag=1;
            break;
        }
        pt1 = pt1->next;
    }

    if(nflag==0)
        printf("The database does not store the student information you want to print!\n");

    printf("\n\n");
    printf("print student ok!\n");
}

//Function to print out all information of every student
void PrintAllStudent(PNODE pHead)
{
    PNODE pt1, head;


    if (pHead==NULL)
    {
        pt1 = ReadData();
        head = pt1;
    }
    else
        head=pt1=pHead;

    system("cls");
    printf("S.No\tName\tSubject 1\tSubject 2\tSubject 3\tSubject 4\tSubject 5\n");
    printf("----\t----\t---------\t---------\t---------\t---------\t---------\n");
    

    while(pt1!=NULL)
    {
        printf("%s\t%s\t%0.2f\t\t%0.2f\t\t%0.2f\t\t%0.2f\t\t%0.2f\n", pt1->num, pt1->name, pt1->score[0], pt1->score[1], pt1->score[2], pt1->score[3], pt1->score[4]);
        pt1 = pt1->next;
    }

    printf("\n\n");
    printf("The print for the student report for all students is okay!\n");
}
//Function to save data of all students to a file
void Save(PNODE head)
{
    WriteData_wb(head);
    system("cls");
    printf("Save to file successful!\n");
}

// The number of input data write
int Creat_num(void)
{
    int n;
    printf("Please enter the number of students you want to introduce:");
    
    if(scanf("%d", &n)!=1)
    {
        printf("\a Error!");
    }
    return n;
}

// Select the file to open
int File_name()
{
    printf("\nPlease enter the file you want to open:");
    if(scanf("%s", filename)!=1)
        printf("\a Error!");
    return 0;
}

void menu()
{
    printf("\n");
    printf("Welcome to the student manager system! \n");
    printf("\nMenu\n");
    printf("----------------------------\n");
    printf("1. Introduce student \n");
    printf("2. Remove student \n");
    printf("3. Introduce marks\n");
    printf("4. Print student report \n");
    printf("5. Print report for all students \n");
    printf("6. Save to file \n");
    printf("7. Retrieve data from file \n");
    printf("8. Exit\n");
    printf("----------------------------\n");
    printf("Please enter your choice:");
}

int main(int argc, char** argv)
{
    int nChoice=0;
    PNODE pCur, pEnd;
    pCur=pEnd=NULL;

    if (argc>1)
    {
        // program, enter./main.exe 2
        if (strcmp(argv[1], "2")==0)
        {
            nDataMode=2;
        }
    }

    while(1)
    {
        fflush(stdin);
        menu();
        scanf("%d", &nChoice);
        switch(nChoice)
        {
        case 1:
            head = IntroduceStudent(head, Creat_num());
            break;//introduce a student
        case 2:
            head = RemoveStudent(head);
            break; //remove a student
        case 3:
            head = IntroduceMarks(head);
            break; //introduce marks for a student
        case 4:
            PrintStudent(head);
            break; //print a report for a student
        case 5:
            PrintAllStudent(head);
            break; //print a report for all the students
        case 6:
            Save(head);
            break;//appendtotable
        case 7:
            head = ReadData();
            break;//delete results
        case 8:
         printf("Program ended!\n");
            exit(0);//exit the program
        default:
            printf("Input error\n");
        }
    }
    
}
