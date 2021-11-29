#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "scr/raygui.c"  
#include <strings.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// global decleration of the varuable so we can use it in all functions without passing it 
const int  arg_len=100;
int values_to_sort[256]; 
int nbr_rect;
int changed_values[1000][3] ;//first & second boxes to heighlight then 0 or 1 to swap or not ; and the forth is a special box that we want to bring attention to
pthread_t Sorting_Thread;
//----------------------------------------------------------------------------------------------------

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
void swap(int changes[]){
    if (changes[0]){
    int aux=values_to_sort[changes[1]];
    values_to_sort[changes[1]]=values_to_sort[changes[2]];
    values_to_sort[changes[2]]=aux;
    }
}
void *Bogosort(){
        int r=0;
        int change_counter=0;
        while (!(is_sorted_asscending(values_to_sort,nbr_rect)) && change_counter<1000){
        for ( int i = 0 ; i< nbr_rect ; i++){
            r=rand()%nbr_rect;
            while (r==i) r=rand()%nbr_rect;
            changed_values[change_counter][0]=1;
            changed_values[change_counter][1]=i;
            changed_values[change_counter][2]=r;  
            change_counter++;
        }
        }
    
}
void Bubble(){
}
void Selection(){
}

bool sort(int sorting_algo){
    if(!is_sorted_asscending(values_to_sort,nbr_rect)){ // 1 setp of any giving sorting algorithem
        
        switch (sorting_algo)
        {
        case 0: pthread_create(&Sorting_Thread, NULL, Bogosort, NULL);
                pthread_join(Sorting_Thread, NULL);
                break;
        case 1: Bubble();
            break;
        case 2: Selection();
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
    nbr_rect = parse_input(input_values,&values_to_sort,256);
    bool sorting = false;
    int delay=1000*(CLOCKS_PER_SEC/1000);
    clock_t time=clock()-delay;
    Color rect_colour = MAROON;
    bool havent_swaped=false;
    bool Done_sorting=false;
    int itr_nbr=0;

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
        if (GuiButton((Rectangle){screenWidth-110, 200, 50, 30 },"SORT")) {nbr_steps = -1;sorting=true;sort(sorting_type_selector);}
        if (GuiButton((Rectangle){screenWidth-110, 300, 50, 30 },"STOP SORT")) {nbr_steps = 0;sorting=false;} 

        //draw the rectangels 
        //----------------------------------------------------------------------------------
        if (!sorting){
            nbr_rect=parse_input(input_values,&values_to_sort,256);// keep parsing the input as long as the text box is foucesed and not currently sorting
            rescale_input(&values_to_sort, nbr_rect);
            
            
        }else if(clock()>time+delay) {
            itr_nbr++;
            time=clock();
            havent_swaped=true;
            Done_sorting=is_sorted_asscending(values_to_sort,nbr_rect);
            }
        if (Done_sorting){
            //whatere we want to go after we finish sorting goes here 
            havent_swaped=false;
        }
        if(havent_swaped && clock()>=time+(delay/2)) {swap(changed_values[itr_nbr]);havent_swaped=false;} // at hafe of the delay time switch the boxes so it's heighlited first then switched
        
        for (int i = 0 ; i<nbr_rect;i++){ //draw the inital rectangels   
            if (i==changed_values[itr_nbr][1] || i==changed_values[itr_nbr][2]) rect_colour = BLACK; // HEIGHTLIGHT THE CHANGEING BLOCKS

            DrawRectangle(200+i*((screenWidth-485)/nbr_rect), screenHeight-25-values_to_sort[i], (screenWidth-490)/nbr_rect, values_to_sort[i],rect_colour);//last rect width +5 white pixel seperator   485 490              
            rect_colour= MAROON;
            }
        
        EndDrawing();
    }
}
