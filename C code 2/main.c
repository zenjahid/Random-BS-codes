#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 9
#define MXLNE 100
#define MX_LN 100
char DAT[MXLNE][MX_LN];
int c=0;
struct elm
{
char nm[MX_LN];
};
struct elm st[999],tmp;
//struct for node
struct node { 
    char *value;// all void* types replaced by char*
    struct node *p_left;
    struct node *p_right;
};

//use typedef to make calling the compare function easier
typedef int (*Compare)(const char *, const char *);

//inserts elements into the tree
void insert(char* key, struct node** leaf, Compare cmp)
{
    int res;
    if( *leaf == NULL ) {
        *leaf = (struct node*) malloc( sizeof( struct node ) );
        (*leaf)->value = malloc( strlen (key) +1 );     // memory for key
        strcpy ((*leaf)->value, key);                   // copy the key
        (*leaf)->p_left = NULL;
        (*leaf)->p_right = NULL;
        //printf(  "\nnew node for %s" , key);
 
      } else {
        res = cmp (key, (*leaf)->value);
        if( res < 0)
        {
          insert( key, &(*leaf)->p_left, cmp);
          printf ("Key '%s' is left in the tree\n", key);
        }
        else if( res > 0)
        {
          insert( key, &(*leaf)->p_right, cmp);
          printf ("Key '%s is right in the tree\n", key);
        }
        else                                            // key already exists
            printf ("Key '%s' already in tree\n", key);
    }
}

//compares value of the new node against the previous node
int CmpStr(const char *a, const char *b)
{
    return (strcmp (a, b));     // string comparison instead of pointer comparison
}

//
char *input(void)
{
  fgetc(stdin);
    char line[MAXLEN+1];       // where to place key    
    printf("Please enter a string : ");
    fgets(line,MAXLEN+1,stdin);
    //line[strcspn(line,"\n")] = 0;
     return ( strtok(line, "\n" ));    // remove trailing newline
}



void in_order(struct node *root)
{
     
    if( root != NULL )
    {
      
        in_order(root->p_left);
        printf("   %s\n", root->value);
        in_order(root->p_right);
  
    }
  
}

void write()
{
  for(int i=0;i<c;i++)
    {
        for(int j=0;j<c-1;j++)
        {
            if(strcmp(st[j].nm,st[j+1].nm)>0)
            {
                tmp=st[j];
                st[j]=st[j+1];
                st[j+1]=tmp;
            }
        }
    }
  
  FILE*fp;


fp=fopen("sorted_out.txt","w");
  
if (fp == NULL)
    {
        printf("Could not open file");
        return 0;
    }
   
    for (int i = 0; i < c; i++)
    {
       
        fprintf(fp,"%s\n",st[i].nm);
    }
    fclose(fp);


  
  
  

};









//searches elements in the tree
void search(char* key, struct node* leaf, Compare cmp)  // no need for **
{
    int res;
    if( leaf != NULL ) {
        res = cmp(key, leaf->value);
        if( res < 0)
            search( key, leaf->p_left, cmp);
        else if( res > 0)
            search( key, leaf->p_right, cmp);
        else
            printf("\n'%s' found!\n", key);     // string type
    }
    else printf("\nNot in tree\n");
    return;
}

void delete_tree(struct node** leaf)
{
    if( *leaf != NULL ) {
        delete_tree(&(*leaf)->p_left);
        delete_tree(&(*leaf)->p_right);
        free( (*leaf)->value );         // free the key
        free( (*leaf) );
    }
}


//displays menu for user
void menu()
{
  //system("clear");

  
    printf("\nPress 'i' to insert elements from inputs.txt\n");
    printf("Press 's' to search for an element\n");
    printf("Press 'p' to print the tree inorder in sorted_output.txt file\n");
    //printf("Press 'f' to destroy current tree\n");
    printf("Press 'q' to quit\n\nEnter your choice : ");
}

int main()
{
   
    struct node *p_root = NULL;
    char *value;
    char option = 'x';
  char data[MXLNE][MX_LN];
  char t[MX_LN];
  


    while( option != 'q' ) {
        //displays menu for program
        menu();

        //gets the char input to drive menu
    
        option = getchar(); // instead of two getchar() calls

        if( option == 'i')
        {        
    FILE *fp;
    fp = fopen("inputs.txt", "r");

    if(fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    
    while(!feof(fp)&&!ferror(fp))
    if(fgets(st[c].nm,MX_LN,fp) !=NULL) c++;
  

    fclose(fp);

          for(int i=0;i<c;i++)
            {
            printf ("Inserting %s\n", st[i].nm);
            st[i].nm[strcspn(st[i].nm, "\n")] = 0;
              
            insert(st[i].nm,  &p_root, (Compare)CmpStr);
            }
  
          }
        else if( option == 's' ) {
            value = input();
            search(value, p_root, (Compare)CmpStr);     // no need for **
        }
          else if( option == 'g' ) {
           write();
        }
        else if( option == 'p' ) {
            in_order(p_root);
          write();
        }
      
        else if( option == 'f' ) {
            delete_tree(&p_root);
           
            printf("Tree destroyed");
            p_root = NULL;
        }
      
        else if( option == 'q' ) {
            printf("Quitting");
        }
    }
return 0;
}
