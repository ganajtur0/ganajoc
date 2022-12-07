#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef
struct dir {
    struct dir *parent;
    struct dir *children[100];
    int size;
    int count;
    char name[20];
} dir;

dir *
dir_add(dir *parent, char *name){

    dir *d = malloc(sizeof(dir));
    d->parent = parent;

    parent->children[parent->count] = d;
    parent->count++;

    strncpy(d->name, name, 20);
    return d;
}

dir *
cd(dir *d, char *name){
    if (strncmp("..", name, 2)==0)
        return d->parent;
    for (int i = 0; i<d->count; i++){
        if(strncmp(d->children[i]->name, name, 20)==0)
            return d->children[i];
    }
    return NULL;
}

void
dir_print(dir *d, int level){
    for (int i=0;i<d->count;i++){
        for (int j=0;j<level;j++)
            putc(' ', stdout);
        printf("|-%s (%d)\n",d->children[i]->name, d->children[i]->size);
        dir_print(d->children[i],level+1);
    }
    return;
}

void
exec(char *l, dir **current){
    char cmd[3], name[20];
    if (l[0]=='$'){
        if(sscanf(l, "$ %s %s", cmd, name)==1)
            return;
        *current = cd(*current, name);
        return;
    }
    int size=0;
    if (sscanf(l, "%d %s", &size, name)==0){
        sscanf(l, "dir %s", name);
        dir_add(*current, name);
        return;
    }
    ((*current)->size)+=size;
}

int
total_size(dir *d){
    for (int i=0;i<d->count;i++)
        d->size+=total_size(d->children[i]);
    return d->size;
}

int
answer(dir *d){
    int ans = (d->size<=100000 ? d->size : 0);
    for (int i=0;i<d->count;i++)
        ans += answer(d->children[i]);
    return ans;
}

void
part_two(dir *d, int req, int *smallest){
    *smallest=(d->size>=req && d->size<(*smallest)) ? d->size : *smallest;
    for (int i = 0; i<d->count;i++)
        part_two(d->children[i], req, smallest);
    return;
}

int main(){
    //create root
    dir root;
    strcpy(root.name, "/");
    root.parent = NULL;
    root.size=0;
    root.count=0;

#ifdef DEBUG
    dir *iter = dir_add(&root, "a");
    iter = dir_add(&root, "b");
    iter = cd(&root,"a");
    iter = dir_add(iter, "c");
    dir_add(iter, "d");
    dir_add(iter, "e");

    /*
    /
    |-a
    | |-c
    | | |-d
    | | |-e
    |-b
    */

    printf("/\n");
    dir_print(&root, 0);
#endif //DEBUG
#ifdef TEST
    FILE *f = fopen("test", "r");
#else
    FILE *f = fopen("i", "r");
#endif //TEST

    dir *d=&root;
    char l[100];
    //skip first line
    fgets(l,100,f);
    while (fgets(l, 100, f)!=NULL)
        exec(l, &d);
    total_size(&root);
#ifdef DEBUG
    dir_print(&root,0);
#endif //DEBUG
    int smallest=70000000;
    printf("%d ",answer(&root));
    part_two(&root, root.size-40000000, &smallest);
    printf("%d\n",smallest);
    return 0;
}
