#include<iostream>
#include<raylib.h>
#include<raymath.h>

using namespace std;

// Creating a Grid

#define cellsize 30
#define cellcount 25
#define offset 75

// Colors

Color green = { 0, 220, 48, 255 };
Color red = { 230, 41, 55, 255 };
Color darkblue = { 0, 82, 172, 255 };
Color white = { 255, 255, 255, 255};

class Food{
public:
 
    Vector2 food_coordinates;

   Food() : food_coordinates(generate_coordinates()) {}
   //Food() : food_coordinates({6,9}) {}

    void drawfood(){
        //void DrawCircle(int centerX, int centerY, float radius, Color color);

        DrawCircle(food_coordinates.x*cellsize+offset, food_coordinates.y*cellsize+offset, 20/2, red);  
    }

    Vector2 generate_coordinates(){

        float x = GetRandomValue(0, cellcount-1);
        float y = GetRandomValue(0, cellcount-1);
        
        return {x,y};
    }

};

class Snake{
public:
    Vector2 cell;
    Snake *next;

    Snake(Vector2 coordinates) : cell(coordinates), next(nullptr) {}

    void push_front(Snake *&head, Vector2 direction){

        // Adding direction so the snakes moves
        Vector2 coord = Vector2Add(head->cell,direction);
        Snake *temp = new Snake(coord);
        
        //cool way to insert temp infront of the head node and makeing temp the headnode
        temp->next = head;
        head = temp;
    }

    void pop_back(){
        
        Vector2 tempcord;
        Snake *temp = this;

        while(temp->next->next != NULL)temp = temp->next;
        delete(temp->next);
        temp->next = NULL;
    }

    void drawsnake(){
        
        Snake* current = this;

        while(current != NULL){

            float x = current->cell.x;
            float y = current->cell.y;

            Rectangle segment = {offset+x*cellsize,offset+y*cellsize,cellsize,cellsize};

            //void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color); 
            DrawRectangleRounded(segment,0.5,6,darkblue);

            current = current->next;
        }
    }
};

class gamelogic{
public:

    void movesnake(Snake* &head, Vector2 direction){
        head->pop_back();
        head->push_front(head,direction);
    }

    bool checkcoliwithfood(Vector2 food_coordinates, Vector2 snake_coordinates){
        // Vector2Equals check if they are equal or not
        if(Vector2Equals(food_coordinates,snake_coordinates))return true;
        return false;
    }

    bool checkcoliwithedge(Snake* &head){

        Vector2 headcoordinates = head->cell;

        if(headcoordinates.x >= cellcount || headcoordinates.x < 0 || headcoordinates.y >= cellcount || headcoordinates.y < 0){
            return true;
        }
        return false;
    }

    bool checkcoliwithbody(Snake *&head){

        Snake* temp = head;
        while(temp->next != NULL){
            temp = temp->next;
            if(Vector2Equals(temp->cell,head->cell)) return true;
        }
        return false;
    }

    void restartgame(Snake* &head){

        Vector2 temp = {6,9};
        head->cell = temp;
        Snake *body1 = new Snake(Vector2Add(temp,{0,1}));
        Snake *body2 = new Snake(Vector2Add(temp,{0,2}));

        head->next = body1;
        body1->next = body2;
    }

    //Cool bit to limit the snake moment speed

    /*you get the current time with GetTime function in the lib 
    you then make a var (lastupdatetime) this track the time escape 
    until now (ignore the last line if you don't get you will now) 
    So you compare if the current time has esceed the interval time given
    if it is you just update the "lastupdatetime" simple math thing going
    on here*/

    double lastupdatetime = 0;
    bool eventTriggered(double interval){

        double current_time = GetTime();
        if(current_time - lastupdatetime >= interval){
            lastupdatetime = current_time;
            return true;
        }
        return false;
    }

};

// lraylib

int main(){

    InitWindow(2*offset + cellcount*cellsize,2*offset + cellcount*cellsize,"🐍 Your Snake");
    SetTargetFPS(60);

    Food f1; // food obj creation

    //Snake head obj creation
    Vector2 temp = {6,9};
    Snake *head = new Snake(temp);
    Snake *body1 = new Snake(Vector2Add(temp,{0,1}));
    Snake *body2 = new Snake(Vector2Add(temp,{0,2}));
    head->next = body1;
    body1->next = body2;

    gamelogic g1; // gamelogic creation


    bool addsegment = false;
    Vector2 direction = {0,-1};
    bool isrunning = true;
    int score = 0;

    while(WindowShouldClose() == false){

        BeginDrawing();

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{offset-5, offset-5, cellcount*cellsize+10, cellcount*cellsize+10},5, white);
        DrawText(TextFormat("Score: %i",score), offset-5, offset+cellsize*cellcount+10, 40, white);
        DrawText(TextFormat("Your Snake"), 5*cellcount, 1*cellcount-15, 60, white); 
        f1.drawfood();
        head->drawsnake();

        /// remember to move this bit of code to game logic and for pop back and push front add them in the same function update
        if(g1.eventTriggered(0.2)){
            if(isrunning){
                if(addsegment){
                    head->push_front(head,direction);
                    addsegment = false;
                }
                else g1.movesnake(head,direction);
            }
        }

        //head->cell will give me the cor of head of the snake cuz first node
        if(g1.checkcoliwithfood(f1.food_coordinates, head->cell)){
            f1.food_coordinates = f1.generate_coordinates(); 
            addsegment = true;   
            score++;
        }       

        if(g1.checkcoliwithedge(head) || g1.checkcoliwithbody(head)){
            g1.restartgame(head);
            f1.food_coordinates = f1.generate_coordinates();
            isrunning = false;
            score = 0;
        }

        // Controls
        
        if(IsKeyPressed(KEY_UP) && direction.y != 1 ){
            direction = {0,-1};
            isrunning = true;
        }
        if(IsKeyPressed(KEY_DOWN) && direction.y != -1){
            direction = {0,1};
            isrunning = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && direction.x != -1){
            direction = {1,0};
            isrunning = true;
        }
        if(IsKeyPressed(KEY_LEFT) && direction.x != 1){
            direction = {-1,0};
            isrunning = true;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
