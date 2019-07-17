#include<stdio.h>
#include<stdlib.h>
#define hsize 100
//address returned will be -1 to denote null


typedef struct node
{
	short int start,end;
	struct node * next;
}node;


node * allocl,*freel;
int heap[hsize];

void initialize(){
	allocl=NULL;
	freel=(node*)malloc(sizeof(node));
	freel->next=NULL;
	freel->start=0;
	freel->end=hsize-1;
}

short int firstfit(int size)
{
	short int a=-1;
	if(freel!=NULL)
	{
		
		if(size<2)						//if size is taken then size should be equated with 8 (2*4) this is for no.of blocks
		{
			size=2;
		}
		node *t=freel,*prev=NULL;
		/*if(size%4!=0){
			size+=4;
		}
		size=size/4;*/
		while ((t!=NULL) && (((t->end)-(t->start)+1)<size)){			//automatically takes care of heap full condn
			prev=t;
			t=t->next;
		}
		if(t!=NULL){
			node * temp=(node*)malloc(sizeof(node));
			temp->next=NULL;
			temp->start=t->start;
			temp->end=t->start+size-1;
			t->start+=size;
			temp->next=allocl;
			allocl=temp;
			a=temp->start;
			if((t->start)>(t->end) ){								//for heap full condn
				if(prev==NULL){
					freel=freel->next;
					
				}
				else{
					prev->next=t->next;

				}
				free(t);
			}
		}

	}
	return a;
} 


short int bestfit(int size)
{
	short int a=-1;
	node * ptr=NULL,*prev=NULL;
	node * t=freel,*tprev=NULL;
	if(freel!=NULL){
		
		if(size<2)						//if size is taken then size should be equated with 8 (2*4) this is for no.of blocks
		{
			size=2;
		}
		
		/*if(size%4!=0){
			size+=4;
		}
		size=size/4;*/
		while ((t!=NULL) && (((t->end)-(t->start)+1)<size)){			//automatically takes care of heap full condn
			tprev=t;
			t=t->next;
		}
		if(t!=NULL){
			int s1=0,s2=0;
			ptr=t;
			prev=tprev;
			while(t!=NULL){
				s1=(t->end)-(t->start)+1;
				s2=(ptr->end)-(ptr->start)+1;
				if((s1 >= size) && (s1 < s2)) {
					ptr=t;
					prev=tprev;
				}
				tprev=t;
				t=t->next;
			}
			node * temp=(node*)malloc(sizeof(node));
			temp->next=NULL;
			temp->start=ptr->start;
			temp->end=ptr->start+size-1;
			ptr->start+=size;
			temp->next=allocl;
			allocl=temp;
			a=temp->start;
			if((ptr->start)>(ptr->end) ){								//for heap full condn
				if(prev==NULL){
					freel=freel->next;
					
				}
				else{
					prev->next=ptr->next;

				}
				free(ptr);
			}
		}

	}
	return a;
}


short int nextfit(int size)
{
	short int a=-1;
	node* lnext=NULL,*prev=NULL,*t=freel;
	if(freel!=NULL)
	{
		if(size<2)						//if size is taken then size should be equated with 8 (2*4) this is for no.of blocks
		{
			size=2;
		}
		
		/*if(size%4!=0){
			size+=4;
		}
		size=size/4;*/
		if(allocl!=NULL)
		{
			int x=allocl->end;
			while(t!=NULL && t->start < x)
			{
				prev=t;
				t=t->next;
			}
			if(t!=NULL && t->start==x+1){
				t=t->next;
			}
			lnext=t;
		}
		if(lnext==NULL)
		{															//essentially else of above
			prev=NULL;
			lnext=freel;
		}
		while ((lnext!=NULL) && (((lnext->end)-(lnext->start)+1) < size))
		{																			//automatically takes care of heap full condn
			prev=lnext;
			lnext=lnext->next;
		}
		if(lnext==NULL)
		{															//if next fit starts searching from betn and ends so it should be started again from start 
			prev=NULL;
			lnext=freel;
		}
		while ((lnext!=t) && (((lnext->end)-(lnext->start)+1)<size))
		{																			//automatically takes care of heap full condn
			prev=lnext;
			lnext=lnext->next;
		}
		if (lnext==t && (((lnext->end)-(lnext->start)+1)<size))					//condn where the space is less
		{
			lnext=NULL;
			prev=NULL;
		}
		if(lnext!=NULL)
		{
			node * temp=(node*)malloc(sizeof(node));
			temp->next=NULL;
			temp->start=lnext->start;
			temp->end=lnext->start+size-1;
			lnext->start+=size;
			temp->next=allocl;
			allocl=temp;
			a=temp->start;
			if((lnext->start)>(lnext->end))
			{																	//for heap full condn
				if(prev==NULL)
				{
					freel=freel->next;
					
				}
				else
				{
					prev->next=lnext->next;

				}
				free(lnext);
			}
		}
	}
	return a;
}


void freeblock(short int a){
	node * prev=NULL, * ptr=allocl;
	while(ptr!=NULL && ptr->start!=a){
		prev=ptr;
		ptr=ptr->next;
	}
	if(ptr!=NULL){									//this ensures only if the given address is present
		if(prev==NULL){								//taking out the adress node from allocated list
			allocl=allocl->next;
		}
		else{
			prev->next=ptr->next;
		}
		node * search=freel;
		prev=NULL;
		while(search!=NULL && search->start < ptr->end){			//searching for the right spot to give back the address
			prev=search;
			search=search->next;
		}
		if(freel==NULL){
			freel=ptr;
			ptr->next=NULL;
		}
		else if(prev==NULL){
			if(search->start==ptr->end+1){
				search->start=ptr->start;
				free(ptr);
			}
			else{
				ptr->next=search;
				freel=ptr;
			}
		}
		else{
			if (search!=NULL)
			{
				if(search->start==ptr->end+1){
					search->start=ptr->start;
					free(ptr);
					ptr=search;
				}
				else{
					ptr->next=search;
					prev->next=ptr;
				}
				if(prev->end==ptr->start-1){
					prev->end=ptr->end;
					prev->next=ptr->next;
					free(ptr);
					ptr=NULL;
				}
			}
			else{
				prev->next=ptr;
				ptr->next=NULL;
				if(prev->end==ptr->start-1){
					prev->end=ptr->end;
					prev->next=ptr->next;
					free(ptr);
					ptr=NULL;
				}
			}
		}
	}
}

void showlists(){
	node * t=freel;
	printf("free list is:\n");
	while(t!=NULL){
		printf("\t start address:%d \t end address:%d\n",t->start,t->end);
		t=t->next;
	}
	t=allocl;
	printf("allocated list is:\n");
	while(t!=NULL){
		printf("\t start address:%d \t end address:%d\n",t->start,t->end);
		t=t->next;
	}
}

main(){
	initialize();
	int size=0,choice=1,add=-1;
	while(choice){
		printf("give choice\n");
		printf("1.firstfit\n2.bestfit\n3.nextfit\n4.free memory\n5.show lists\n");
		scanf("%d",&choice);
		switch(choice){
			case 1:
			{
				printf("enter the no. of the block to allocate\n");
				scanf("%d",&size);
			 	add=firstfit(size);
			 	if(add!=-1){
			 		int *p=&heap[add];
			 		printf("%p is actual address\n",p );
			 		printf("your address is:%d\n",add );
			 	}
			 	else{
			 		printf("sorry less space on heap\n");
			 	}
			}
			break;
			case 2:
			{
				printf("enter the no. of the block to allocate\n");
				scanf("%d",&size);
				add=bestfit(size);
				if(add!=-1){
			 		int *p=&heap[add];
			 		printf("%p is actual address\n",p );
			 		printf("your address is:%d\n",add );
			 	}
			 	else{
			 		printf("sorry less space on heap\n");
			 	}
			}
			break;
			case 3:
			{
				printf("enter the no. of the block to allocate\n");
				scanf("%d",&size);
				add=nextfit(size);
				if(add!=-1){
			 		int *p=&heap[add];
			 		printf("%p is actual address\n",p );
			 		printf("your address is:%d\n",add );
			 	}
			 	else{
			 		printf("sorry less space on heap\n");
			 	}
			}
			break;
			case 4:
			{
				printf("give the start address\n");
				scanf("%d",&add);
				freeblock(add);
				showlists();
			}
			break;
			case 5:showlists();
			break;

		}
		
	}
}