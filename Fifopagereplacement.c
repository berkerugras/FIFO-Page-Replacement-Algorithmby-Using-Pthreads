#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h> 
int counter=0;
void *FIFO(void *param);
void *print();
void *FIFO(void *arg)
{
    
int i;
int j;
int k=0;
int g;
int framelength;
int reflength;

printf("Please enter the frame number");
scanf("%d",&framelength);
int frame[framelength]; 
for(i=0;i<framelength;i++){
    frame[i]=-1; 
}
int *arr = (int *) arg;
bool flag=true;
reflength=arr[0];

for(j=1;j<reflength+1;j++,k++){
    if(flag==false){ //flag becomes false if  and only if there is page hit
        k=k-1;
    }
    if(k>framelength-1){ //this is for the frame array, it prevents the extension 
        k=0;
    }
    for(g=0;g<framelength;g++){ 
        if(frame[g]==arr[j]){ //With this loop we control that if there is page hit or not
            flag=false;  //If there is  a page hit flag becomes false
            break; 
        }
        flag=true; //If there is no page hit flag remains true
    }
    
    if(flag==true){ //flag true means page fault so we need to set a frame to corresponding reference string value.
        frame[k]=arr[j];
        counter++; //counter is to store the number of page faults
    }
        
    }
    

pthread_exit(0); 
}

void *print(){
    printf("Number of page fault is ");  
    printf("%d",counter); //We are going to observe same counter value with other thread(Fifo) 
    
    pthread_exit(0);

}


int main(int argc, char argv[]){
    pthread_t tid;
    pthread_t tid2;
    pthread_attr_t attr;
    pthread_attr_t attr2;
    int count;
    printf("Enter the length of reference string:  \n"); //Here I get the length of the reference string from the user
    scanf("%d",&count);
    
    int arr[count+1]; //I defined an array that length is plus 1 greater than user wants. This is because I stored the length of the array in the index 0
    int i;
    arr[0]=count; 
    for(i=1;i<count+1;i++){ //Here I get the reference string from the user. It starts at 1 because index 0 is stores length of the array
        printf("Enter the elements of reference string%d: " ,i);
        scanf("%d",&arr[i]);

    }
    pthread_attr_init(&attr);
    pthread_attr_init(&attr2);
    pthread_create(&tid,&attr,FIFO,(void*)arr); //I created a thread and passed our array as a parameter.
    pthread_join(tid,NULL); //It blocks other thread until first thread  finishes (terminates) its job
    pthread_create(&tid2,&attr2,print,NULL); //When first thread is finished it creates another thread to print number  of page faults.
    pthread_join(tid2,NULL); //It blocks the other thread (main) until second thread  finishes (terminates) its job
  
 
    return 0;

}