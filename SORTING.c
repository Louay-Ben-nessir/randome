#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "scr/raygui.c"  
#include <strings.h>
#include <stdlib.h>
#include <time.h>




void sorting_types_formating(char *string, char **sorting_list, int nbr_sorting_types){
    char temp[15];
    int size=0;
    for(int i = 0 ; i < nbr_sorting_types ; i++){
        snprintf(temp,30,"#%d#%s\n",23,sorting_list[i]);
        strcat(string,temp);
    }
    while( string[size] != '\0') size++;
    string[size-1]='\0'; // remove the last \n to prevent an extra btn from appearing 
 

}

void empty_array(char input_array[],int size){memset(input_array, 0, size);}

int parse_input(char input[], int *output,int size){
    int current_num=0,j=0,i=0;
    while (input[i]!='\0'){
        if (input[i]!=',') {
            current_num=current_num*10+(int)input[i]-48;//quick conversion from char to int 
        }
        else{
            output[j]=current_num;
            current_num=0;
            j++;
        }
        i++;
    }
    output[j]=current_num;//adds the last item without the need to add ',' to the end 
    return j+1;//the size of the parsed output 
}
int fill_rand(char input[],char text_box[]){
    int num;
    parse_input(input , &num , 1);
    empty_array(text_box,256);
    sprintf(text_box,"%d",rand()%50);
    for(int i = 0; i<num-1;i++) sprintf(text_box,"%s,%d",text_box,rand()%50);
    return num;
}

bool is_sorted_asscending(int input[],int size){  
    for(int i = 0 ;i<size-1 ; i++) if (input[i]>input[i+1]) return false;
    return true;
}
void swap(int input[],int changes[],int sorting_algo){
    if (changes[2]){
    int aux=input[changes[0]];
    input[changes[0]]=input[changes[1]];
    input[changes[1]]=aux;
    }
}
void Bogosort(int input[],int size,int *itr_nbr,int changes[]){
    int current_position=*itr_nbr%size;
    int r=rand()%size, swap = input[current_position];
    while (r==current_position) r=rand()%size;
    changes[0]=current_position;changes[1]=r;changes[2]=true;
}
void Bubble(int input[],int size,int *itr_nbr,int changes[]){
    int current_position=*itr_nbr+1,nbr_cycles=1;
    while(current_position>=size-nbr_cycles) {current_position-=size-nbr_cycles;nbr_cycles++;};   
    changes[0]=current_position-1;changes[1]=current_position;changes[2]=input[changes[0]]>input[changes[1]];
}
void Selection(int input[],int size,int *itr_nbr,int changes[]){
    if(changes[3]==-1) changes[3]=0; // the min tracking box should start at 0 if it dosent have any value befor hand 
    int current_position=*itr_nbr+1,nbr_cycles=0;
    while(current_position>=size-nbr_cycles) {current_position-=size-nbr_cycles-1;nbr_cycles++;};  
    current_position+=nbr_cycles;
    changes[0]=nbr_cycles;//of the first unsorted element
    changes[1]=current_position;//normal counter going thorough every position
    changes[2]=false;//swap at the start of every cycel 
    changes[3]=(input[changes[3]]>input[changes[1]])?changes[1]:changes[3];
    if(current_position-nbr_cycles==0){//finesed a loop and about to start another
        changes[1]=changes[3];// take the smallest elemnt 
        changes[2]=true;
        changes[3]=nbr_cycles;
        *itr_nbr-=1;//set back the itterations by one so the swap dosent count as a step
    }
   
     
    
    printf("%d\n",current_position-nbr_cycles);
     
    
}

bool sort(int input[],int size , int sorting_algo, int *itr_nmb,int changes[2]){
    if(!is_sorted_asscending(input,size)){ // 1 setp of any giving sorting algorithem
        switch (sorting_algo)
        {
        case 0: Bogosort(input,size,itr_nmb,changes);
            break;
        case 1: Bubble(input,size,itr_nmb,changes);
            break;
        case 2: Selection(input,size,itr_nmb,changes);
            break;
        default:
            break;
        }
        return false;
    }
    return true;

}

void rescale_input(int input[],int size){
    int max=input[0],min=input[0],meadian=0;
    for (int i = 0; i<size; i++){max=(input[i]>max) ? input[i] : max; min=(input[i]<min) ? input[i] : min ;}
    for (int i = 0; i<size; i++) input[i]=(int)(680*((float)(input[i]-min)/(max-min)))+10;
}
int main(){
    // Initilaisation
    // ---------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;
    bool exitWindow = false;
    InitWindow(screenWidth, screenHeight, "sorting");


    // input array config and vars
    // ---------------------------------------------------------------------------
    bool input_values_f = false;//foucues on inpt values text box
    char input_values[256] = "1,2,3,4";//"Input the values you want sorted seperated by a ',' ";
    bool nmb_of_vals_f = false;//foucues on nmb value text box values 
    char nmb_of_vals[5] = "10";
    int values_to_sort[256];


    // SORTING TYPE BTNS SELECT CONFIG AND VARS
    // ---------------------------------------------------------------------------
    int sorting_type_selector = 0; // indicates the selected sorting type 
    int nbr_sorting_types = 3 ; // keep track of the number of implemented sorting types
    char *sorting_types[20] = {"Bogosort","Bubble","Selection"}; // A list of pointers to sorting names.
    char formated_sorting_types[100] = ""; //allocationg 10 chars for every sorting algo  name 
    sorting_types_formating(&formated_sorting_types,sorting_types,nbr_sorting_types); // format the string to use as argument for buttons


    //NON-GUI VARIABLES
    // ---------------------------------------------------------------------------
    int nbr_steps = -1; // the number of stepes to take in any giving sorting algo 
    int nbr_rect = parse_input(input_values,&values_to_sort,256);
    bool sorting = false;
    int itr_nbr = 0;
    int delay=1000*(CLOCKS_PER_SEC/1000);
    clock_t time=clock()-delay;
    int changed_values[3] = {-1, -1, 0, -1};//first & second boxes to heighlight then 0 or 1 to swap or not ; and the forth is a special box that we want to bring attention to
    Color rect_colour = MAROON;
    bool havent_swaped=false;
    bool Done_sorting=false;

    // Main loop
    // ---------------------------------------------------------------------------
    while (!exitWindow)
    {

        //Draw
        // ---------------------------------------------------------------------------
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        sorting_type_selector = GuiToggleGroup((Rectangle){ 25, 25, 125, 100 }, formated_sorting_types, sorting_type_selector);//the sorting type selector 
        
        if (GuiTextBoxMulti((Rectangle){ screenWidth-250, 25, 225, 100 }, input_values, 256, input_values_f)){input_values_f=!input_values_f;}//the input box for the diffrent values in the array   
        if (GuiButton((Rectangle){screenWidth-250, 135, 50, 30 },"clear")) empty_array(input_values,256);//empty the array box 
        if (GuiTextBox((Rectangle){screenWidth-190,135,75,30},nmb_of_vals,5,nmb_of_vals_f)) nmb_of_vals_f=!nmb_of_vals_f;//
        if (GuiButton((Rectangle){screenWidth-110, 135, 50, 30 },"random")) nbr_rect=fill_rand(nmb_of_vals,input_values);//fill the array box with n = num_array random values after clearing it
        //place holder btns
        if (GuiButton((Rectangle){screenWidth-110, 200, 50, 30 },"SORT")) {nbr_steps = -1;sorting=true;}
        if (GuiButton((Rectangle){screenWidth-110, 300, 50, 30 },"STOP SORT")) {nbr_steps = 0;sorting=false;} 

        //draw the rectangels 
        //----------------------------------------------------------------------------------
        if (!sorting){
            nbr_rect=parse_input(input_values,&values_to_sort,256);// keep parsing the input as long as the text box is foucesed and not currently sorting
            rescale_input(&values_to_sort, nbr_rect);
            itr_nbr=0;
            changed_values[0]=-1;changed_values[1]=-1;changed_values[2]=false,changed_values[3]=-1;
        }else if(clock()>time+delay) {
            Done_sorting=sort(values_to_sort,nbr_rect,sorting_type_selector,&itr_nbr,changed_values);
            time=clock();
            itr_nbr+=1;//position cant be bigger than nbr rect
            havent_swaped=true;
            }
        if (Done_sorting){
            //whatere we want to go after we finish sorting goes here 
            havent_swaped=false;
        }
        if(havent_swaped && clock()>=time+(delay/2)) {swap(values_to_sort,changed_values,sorting_type_selector);havent_swaped=false;} // at hafe of the delay time switch the boxes so it's heighlited first then switched
        for (int i = 0 ; i<nbr_rect;i++){ //draw the inital rectangels   
            if (i==changed_values[0] || i==changed_values[1]) rect_colour = BLACK; // HEIGHTLIGHT THE CHANGEING BLOCKS
            if (i==changed_values[3]) rect_colour = YELLOW;
            DrawRectangle(200+i*((screenWidth-485)/nbr_rect), screenHeight-25-values_to_sort[i], (screenWidth-490)/nbr_rect, values_to_sort[i],rect_colour);//last rect width +5 white pixel seperator   485 490              
            rect_colour= MAROON;
            }
        
        EndDrawing();
    }
}
