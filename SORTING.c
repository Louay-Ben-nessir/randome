#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "scr/raygui.c"  
#include <strings.h>
#include <stdlib.h>
#include <time.h>

void sorting_types_formating(char *string, char **sorting_list, int nbr_sorting_types){
    char temp[15];
    int size=0;
    for(int i = 0 ; i < nbr_sorting_types ; i++) 
    {
        snprintf(temp,30,"#%d#%s\n",23,sorting_list[i]);
        strcat(string,temp);
    }
    while( string[size] != '\0') size++;
    string[size-1]='\0'; // remove the last \n to prevent an extra btn from appearing 
 

}

void empty_array(char input_array[],int size){memset(input_array, 0, size);}
void fill_rand(){}

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
bool is_sorted_asscending(int input[],int size){  
    for(int i = 0 ;i<size-1 ; i++) if (input[i]>input[i+1]) return false;
    return true;
}
void Bogosort(int *input[],int size,int current_position){//1 setp of bogo sort
    int r=rand()%size, swap = input[current_position];
    input[current_position]=input[r];
    input[r]=swap;
}
void sort(int *input[],int size , int sorting_algo, int current_position){
    if(!is_sorted_asscending(input,size)){
        switch (sorting_algo)
        {
        case 0:Bogosort(input,size,current_position);
            break;
        default:
            break;
        }
    }

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
    char input_values[256] = "1,23,51,50";//"Input the values you want sorted seperated by a ',' ";
    bool nmb_of_vals_f = false;//foucues on nmb value text box values 
    char nmb_of_vals[5] = "0";
    int values_to_sort[256];


    // SORTING TYPE BTNS SELECT CONFIG AND VARS
    // ---------------------------------------------------------------------------
    int sorting_type_selector = 0; // indicates the selected sorting type 
    int nbr_sorting_types = 1 ; // keep track of the number of implemented sorting types
    char *sorting_types[20] = {"Bogosort"}; // A list of pointers to sorting names.
    char formated_sorting_types[100] = ""; //allocationg 10 chars for every sorting algo  name 
    sorting_types_formating(&formated_sorting_types,sorting_types,nbr_sorting_types); // format the string to use as argument for buttons

    //NON-GUI VARIABLES
    // ---------------------------------------------------------------------------
    int nbr_steps = -1; // the number of stepes to take in any giving sorting algo 
    int nbr_rect = parse_input(input_values,&values_to_sort,256);
    bool sorting = false;
    int current_position = 0;
    int delay=100*(CLOCKS_PER_SEC/1000);
    clock_t time;
    // Main loop
    // ---------------------------------------------------------------------------
    while (!exitWindow)
    {

        //Draw
        // ---------------------------------------------------------------------------
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        sorting_type_selector = GuiToggleGroup((Rectangle){ 25, 25, 125, 100*nbr_sorting_types }, formated_sorting_types, sorting_type_selector);//the sorting type selector 
        
        if (GuiTextBoxMulti((Rectangle){ screenWidth-250, 25, 225, 100 }, input_values, 256, input_values_f)){input_values_f=!input_values_f;}//the input box for the diffrent values in the array   
        if (GuiButton((Rectangle){screenWidth-250, 135, 50, 30 },"clear")) empty_array(input_values,256);//empty the array box 
        if (GuiTextBox((Rectangle){screenWidth-190,135,75,30},nmb_of_vals,5,nmb_of_vals_f)) nmb_of_vals_f=!nmb_of_vals_f;//
        if (GuiButton((Rectangle){screenWidth-110, 135, 50, 30 },"random")) fill_rand();//fill the array box with n = num_array random values after clearing it
        //place holder btns
        if (GuiButton((Rectangle){screenWidth-110, 200, 50, 30 },"SORT")) {nbr_steps = -1;sorting=true;}
        if (GuiButton((Rectangle){screenWidth-110, 300, 50, 30 },"STOP SORT")) {nbr_steps = 0;sorting=false;} 

        //draw the rectangels 
        //----------------------------------------------------------------------------------
        if (!sorting){
            if (input_values_f)  nbr_rect=parse_input(input_values,&values_to_sort,256);// keep parsing the input as long as the text box is foucesed and not currently sorting
            rescale_input(&values_to_sort, nbr_rect);
            current_position=0;
        }else if(clock()>time+delay) {
            sort(&values_to_sort,nbr_rect,sorting_type_selector,current_position);
            time=clock();
            
            current_position=(current_position+1)%nbr_rect;//position cant be bigger than nbr rect
            }
        for (int i = 0 ; i<nbr_rect;i++){ //draw the inital rectangels   
            DrawRectangle(200+i*((screenWidth-485)/nbr_rect), screenHeight-25-values_to_sort[i], (screenWidth-490)/nbr_rect, values_to_sort[i], MAROON);//last rect width +5 white pixel seperator   485 490              
            }
        
        EndDrawing();
    }
}