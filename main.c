#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dirent.h>

typedef struct baby* AVL;
typedef struct numbers* num;

struct numbers{
int year;
int frequency;
num next;
};
struct baby{
char name[30];
char gender;
num frequencies;
int height;
AVL right;
AVL left;
};

AVL Search(AVL, char[], char);
AVL insert(AVL, char[], char, num);
int max(int a, int b);
int height(AVL);
AVL rightRotate(AVL);
AVL leftRotate(AVL);
void insertNew(AVL);
AVL newNode(char[],char,num);
int getBalance(AVL);
AVL readData(AVL);
void updateFrequencies(num, num);
void inOrder(AVL);
void getSearch(AVL);
void AvgFrequencies(AVL);
void getMaxFrequency(AVL);
void FindMax(AVL, AVL*, int*);
int SumFrequencies(AVL);
void FindTotalOfYear(AVL);
int getTotal(AVL, int);
int chceckFrequency(num, int);
void saveTreeToFile(AVL, FILE*);

int main()
{
    char filename[20];
    AVL tree = NULL;
    tree = readData(tree);
    int choice;
    do{
    printf("Wellcome\n");
    printf("Choose an option from the table:\n\n");
    printf("1-Search for a specific baby.\n");
    printf("2-Display average frequencies of a name.\n");
    printf("3-Display name with max frequencies.\n");
    printf("4-Display total number of babies in a specific year.\n");
    printf("5-Export AVL tree data to a file.\n");
    printf("0-Exit.\n");

    scanf("%d",&choice);
    system("cls");
    switch(choice)
    {
    case 1:
        getSearch(tree);
        getch();
        system("cls");
        break;
    case 2:
        AvgFrequencies(tree);
        getch();
        system("cls");
        break;
    case 3:
        getMaxFrequency(tree);
        getch();
        system("cls");
        break;
    case 4:
        FindTotalOfYear(tree);
        getch();
        system("cls");
        break;
    case 5:
        printf("Output file name: ");
        fflush(stdin);
        FILE *out = fopen(gets(filename), "w");
        fflush(stdin);
        saveTreeToFile(tree, out);
        fclose(out);
        printf("Done\n");
        getch();
        system("cls");
        break;
    }
    }while(choice!= 0);
    return 0;
}
AVL readData(AVL tree)
{
    DIR *d;
    FILE *input;
    struct dirent *dir;
    d = opendir("./input");
    dir = readdir(d);
    dir = readdir(d);
    int year;
    char data[100];
    char path[20];
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            sprintf(path,"./input/%s", dir->d_name);
            input = fopen(path, "r");
            year = atoi(strtok(dir->d_name,"."));
            if(input == NULL)
            {
                printf("null");
            }
            while(fgets(data, 100, input))
            {
                char *name = strtok(data, ",");
                char *gender = strtok(NULL, ",");
                int frequencies = atoi(strtok(NULL,"\n"));
                num newNode = (num)malloc(sizeof(struct numbers));
                newNode->next = NULL;
                newNode->frequency = frequencies;
                newNode->year = year;
                if(gender[0] == 'f' || gender[0] == 'm')
                    gender[0]-=32;
                AVL temp = Search(tree, name, gender[0]);
                if(temp)
                {
                    updateFrequencies(temp->frequencies, newNode);
                }
                else{
                    tree = insert(tree, name, gender[0], newNode);
                }
            }
            fclose(input);
        }
        closedir(d);
    }
    return tree;
}
int height(AVL N)
{
    if (N == NULL)
        return 0;

    return N->height;
}
int max(int a, int b)
{
    return (a > b)? a : b;
}
AVL newNode(char name[30], char gender, num frequencies)
{
    AVL node = (AVL)malloc(sizeof(struct baby));
    node->gender = gender;
    strcpy(node->name,name);
    node->frequencies = frequencies;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return(node);
}
AVL rightRotate(AVL y)
{
    AVL x = y->left;
    AVL T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    return x;
}
AVL leftRotate(AVL x)
{
     AVL y = x->right;
     AVL T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    return y;
}
int getBalance(AVL N)
{
    if (N == NULL){
        return 0;
    }
    return height(N->left) - height(N->right);
}
AVL insert(AVL node, char name[30], char gender, num frequencies)
{
    if (node == NULL)
        return(newNode(name, gender, frequencies));

    if (strcmp(name, node->name) <= 0){
        node->left  = insert(node->left, name, gender, frequencies);
    }
    else if (strcmp(name, node->name) > 0){
        node->right = insert(node->right, name, gender, frequencies);
    }
    else
        return node;

    node->height = 1 + max(height(node->left),
                           height(node->right));


    int balance = getBalance(node);


    if (balance > 1 && strcmp(name,node->left->name) < 0){
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(name, node->right->name) > 0){
        return leftRotate(node);
    }
    if (balance > 1 && strcmp(name, node->name) > 0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(name, node->name) < 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}
void updateFrequencies(num list, num frequencies)
{
    num temp = list;
    if(temp == NULL)
    {
        temp = frequencies;
        return;
    }
    else
    {
        while(temp->next != NULL)
        {
            if(temp->year == frequencies->year)
            {
                temp->frequency += frequencies->frequency;
                return;
            }
            temp = temp->next;
        }
    }
     if(temp->year == frequencies->year)
        {
            temp->frequency += frequencies->frequency;
            return;
        }
    temp->next = frequencies;
    return;
}
void inOrder(AVL root)
{
    if(root != NULL)
    {
        inOrder(root->left);

        printf("\nNAME: %s - GENDER: %c\n",root->name,root->gender);

        inOrder(root->right);
    }
}
AVL minValueNode(AVL node)
{
    AVL current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}
void deleteAllTree(AVL root)
{
    if (root == NULL)
    {
           return;
    }
            deleteAllTree(root -> left);
            deleteAllTree(root -> right);
            free(root);
}
AVL Search(AVL root, char name[30], char gender)
{
    while(root != NULL)
    {
        if(strcmp(name,root ->name) < 0)
            return Search(root->left, name, gender);
        else if(strcmp(name, root ->name) > 0)
            return Search(root->right, name, gender);
        else if(root->gender != gender)
            return Search(root->left, name, gender);
        else
            return root;
    }
    return NULL;
}
void getSearch(AVL root)
{
    char name[30];
    int gender;
    printf("Name: ");
    fflush(stdin);
    gets(name);
    fflush(stdin);
    printf("gender\n1- Male\n2- Female\n3- Both\n");
    scanf("%d", &gender);
    while(gender != 1 && gender != 2 && gender != 3)
    {
        printf("\nChoose a valid option\n");
        scanf("%d",&gender);
    }
    if(gender == 1)
    {
        AVL temp = Search(root, name, 'M');
        if(temp)
        {
            printf("\nFound\n\n");
            printf("Name: %s, Gender: Male\n", temp->name);
            num pos = temp->frequencies;
            printf("Year: ");
            while(pos != NULL)
            {
                printf("%d\t", pos->year);
                pos = pos -> next;
            }
            printf("\nFreq: ");
            pos = root -> frequencies;
            while(pos != NULL)
            {
                printf("%d\t", pos->frequency);
                pos = pos -> next;
            }
            return;
        }
        else
        {
            printf("Not Found\n");
            return;
        }
    }
    else if(gender == 2)
    {
        AVL temp = Search(root, name, 'F');
        if(temp)
        {
            printf("\nFound\n\n");
            printf("%s Female\n", temp->name);
            num pos = temp->frequencies;
            printf("Year: ");
            while(pos != NULL)
            {
                printf("%d\t", pos->year);
                pos = pos -> next;
            }
            printf("\nFreq: ");
            pos = temp -> frequencies;
            while(pos != NULL)
            {
                printf("%d\t", pos->frequency);
                pos = pos -> next;
            }
            return;
        }
        else
        {
            printf("Not Found\n");
            return;
        }
    }
    else if(gender == 3)
    {
        AVL temp = Search(root, name, 'M');
        if(temp)
        {
            printf("\nMale Found\n\n");
            printf("Name: %s, Gender: Male\n", temp->name);
            num pos = temp->frequencies;
            printf("Year: ");
            while(pos != NULL)
            {
                printf("%d\t", pos->year);
                pos = pos -> next;
            }
            printf("\nFreq: ");
            pos = temp -> frequencies;
            while(pos != NULL)
            {
                printf("%d\t", pos->frequency);
                pos = pos -> next;
            }
        }
        else
        {
            printf("\nMale Not Found\n");
        }
        temp = Search(root, name, 'F');
        if(temp)
        {
            printf("\n\nFemale Found\n\n");
            printf("Name: %s Female\n", temp->name);
            num pos = temp->frequencies;
            printf("Year: ");
            while(pos != NULL)
            {
                printf("%d\t", pos->year);
                pos = pos -> next;
            }
            printf("\nFreq: ");
            pos = temp -> frequencies;
            while(pos != NULL)
            {
                printf("%d\t", pos->frequency);
                pos = pos -> next;
            }
        }
        else
        {
            printf("Female Not Found\n");
        }
    }
    printf("\n");
}
void AvgFrequencies(AVL root)
{
    char name[30];
    int gender;
    int sum = 0;
    int number = 0;
    printf("Name: ");
    fflush(stdin);
    gets(name);
    fflush(stdin);
    printf("Gender\n1- Male\n2- Female\n");
    scanf("%d", &gender);
    while(gender != 1 && gender != 2)
    {
        printf("Choose a valid option\n");
        scanf("%d", &gender);
    }
    if (gender == 1)
    {
        AVL temp = Search(root, name, 'M');
        if(temp)
        {
            num pos = temp->frequencies;
            while(pos != NULL)
            {
                sum += pos->frequency;
                number++;
                pos = pos->next;
            }
            printf("Average frequencies is: %d\n", (sum/number));
        }
        else
            printf("Not found\n");
    }
    else if(gender == 2)
    {
            AVL temp = Search(root, name, 'F');
        if(temp)
        {
            num pos = temp->frequencies;
            while(pos != NULL)
            {
                sum += pos->frequency;
                number++;
                pos = pos->next;
            }
            printf("Average frequencies is: %d\n", (sum/number));
        }
        else
            printf("Not found\n");
    }
}
void getMaxFrequency(AVL root)
{
    int max = 0;
    AVL MaxFreq = NULL;
    FindMax(root, &MaxFreq, &max);
    if(MaxFreq == NULL)
    {
        printf("Tree is empty \n");
        return;
    }
    else
    {
        printf("Name with maximum frequency is: %s, Gender: %c, frequency: %d\n", MaxFreq->name, MaxFreq->gender, max);
    }
}
void FindMax(AVL root, AVL* pos, int *max)
{
    if (root == NULL)
        return;

    FindMax(root->left, pos, max);
    int sum = SumFrequencies(root);
    if(sum > *max)
    {
        *max = sum;
        *pos = root;
    }
    FindMax(root->right, pos, max);
}
int SumFrequencies(AVL root)
{
    num temp = root->frequencies;
    int sum = 0;
    while(temp != NULL)
    {
        sum+=temp->frequency;
        temp = temp->next;
    }
    return sum;
}
void FindTotalOfYear(AVL root)
{
    int year;
    printf("Year to check: ");
    scanf("%d", &year);
    int total = getTotal(root, year);
    printf("\nTotal Babies in %d, Are: %d\n", year, total);

}
int getTotal(AVL root, int year)
{
    if(root == NULL)
        return 0;
    return (chceckFrequency(root->frequencies, year) + getTotal(root->left, year) + getTotal(root->right, year));
}
int chceckFrequency(num freq, int year)
{
    num temp = freq;
    while(temp != NULL)
    {
        if(temp ->year == year)
            return temp->frequency;
        temp = temp->next;
    }
    return 0;
}
void saveTreeToFile(AVL root, FILE* out)
{
    if(root != NULL)
    {
    saveTreeToFile(root->left,out);
    fprintf(out,"%s, %c, ", root->name, root->gender);
    num temp = root->frequencies;
    int sum = 0;
    while(temp != NULL)
    {
        sum += temp->frequency;
        temp = temp->next;
    }
    fprintf(out, "%d\n", sum);
    saveTreeToFile(root->right,out);
    }
}
