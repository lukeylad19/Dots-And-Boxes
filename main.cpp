
//Program Name: Dots&Boxes
//Creator: Lukas Mallory
//Date: 2/23/19
//Class: CS 490 (The Fun Class)

// main.cpp
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <array>
#include <algorithm>

extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
struct Boxes {
    bool completed = false;
    bool my_sqr = false;
    bool top = false;
    bool bot = false;
    bool left = false;
    bool right = false;
    int num_edges = 0;
};

//Creates two lua states, one for each player
lua_State *L1 = luaL_newstate();
lua_State *L2 = luaL_newstate();
//---------------------------------------------------------
//   Board Creation
//   single dimension array of the created structure for easy referencing
    std::array<Boxes,4>board;
//    std::array<std::array<Boxes,10>,10>board2;
//---------------------------------------------------------
std::string get_name(lua_State* player){
    lua_getglobal(player, "get_name");
    lua_call(player, 0, 1);
    std::string name = lua_tostring(player, -1);
    lua_pop(player, 1);
    return name;
}
int sqr_info(lua_State* player){
  int index = lua_tonumber(player, -1);
  lua_pop(player, 1);
  lua_pushboolean(player, board[index].completed);
  lua_pushboolean(player, board[index].my_sqr);
  lua_pushnumber(player, board[index].num_edges);
  lua_pushboolean(player, board[index].top);
  lua_pushboolean(player, board[index].bot);
  lua_pushboolean(player, board[index].left);
  lua_pushboolean(player, board[index].right);

  return 7;
}

int get_opponent_name(lua_State* player){
  std::string opponent = "";

  if(player == L1){
    lua_getglobal(L2, "get_name");
    lua_call(L2, 0, 1);
    opponent = lua_tostring(player, -1);
    lua_pop(player, 1);
  }
  else{
    lua_getglobal(L1, "get_name");
    lua_call(L1, 0, 1);
    opponent = lua_tostring(player, -1);
    lua_pop(player, 1);
  }
  return 1;
}
int main(int argc, char **argv) {
//    Variable declarations
      bool gameEnd = false;
      int turn = 1;
      char *player_1;
      char *player_2;
      int player_1_score = 0;
      int player_2_score = 0;
      int display_array[100] = {0};
      int display_array2[10][10] = {0};
//---------------------------------------------------------
//    Gets player files
      player_1 = argv[1];
      player_2 = argv[2];
//      std::cout << '\n' << player_1 << '\n';
//      std::cout << '\n' << player_2 << '\n';
      std::cout << '\n';


//Open the libs

  luaL_openlibs(L1);
  luaL_openlibs(L2);

//---------------------------------------------------------
// Randomly chooses a player
  int turn_count = 0;
  srand(time(NULL));
  turn = rand()%100 + 1;
//  std::cout << turn;
  if (turn >= 50){
//    std::cout <<"higher" << "\n";
    turn = 2;
  }
  else{
//    std::cout <<"lower" << "\n";
    turn = 1;
  }
//---------------------------------------------------------
//Does each player file
  luaL_dofile(L1, player_1);
  luaL_dofile(L2, player_2);

//---------------------------------------------------------
//Gets names/string sizes/addresses
  std::string player_1_name = get_name(L1);
  std::string player_2_name = get_name(L2);
  std::cout << "Player 1: "<< player_1_name;
  std::cout << "Player 2: "<< player_2_name;

//---------------------------------------------------------
//Registers functions for use in Lua
  lua_register(L1, "sqr_info", sqr_info);
  lua_register(L1, "get_opponent_name", get_opponent_name);
  lua_register(L2, "sqr_info", sqr_info);
  lua_register(L2, "get_opponent_name", get_opponent_name);


//---------------------------------------------------------
//Game Loop
  while(gameEnd == false){
    bool claim1 = false;
    bool claim2 = false;
    std::cout <<"GAMELOOP" << '\n';
    if(turn == 1){

        while(claim1 == false){
          std::cout<<"Turns: "<< turn_count<<"\n\n";
        //claim2 = false;
        turn_count++;
//    Calls the turn function
        lua_getglobal(L1, "turn");
        lua_call(L1, 0, 2); // 0 arg, 1 return
        long long int edgeP1 = lua_tointeger(L1, -1);
        long long int sqr_numP1 = lua_tointeger(L1, -2);
        lua_pop(L1, -1);
        lua_pop(L1, -2);
        //std::cout<<"Edge: "<<edgeP1<<"\n\n";
        //std::cout<<"Sqr_Num: "<<sqr_numP1<<"\n\n";
        //std::cout<<"Top: "<< board[sqr_numP1].top<<"\n\n";
        //std::cout<<"Bot: "<< board[sqr_numP1].bot<<"\n\n";
        //std::cout<<"Left: "<< board[sqr_numP1].left<<"\n\n";
        //std::cout<<"Right: "<< board[sqr_numP1].right<<"\n\n";
  //    Takes the Turn function result and determines if it's valid
        if(edgeP1 == 0 && board[sqr_numP1].top == false){
          if(sqr_numP1 <= 9){
            board[sqr_numP1].top = true;
          }
          else{
            board[sqr_numP1].top = true;
            board[sqr_numP1+10].bot = true;
          }
          //std::cout<<"Top: "<<board[sqr_numP1].top<<"\n\n";
          claim1 = true;
          turn = 2;
        }
        else if(edgeP1 == 1 && board[sqr_numP1].bot == false){
          if(sqr_numP1 > 90){
            board[sqr_numP1].bot = true;
            board[sqr_numP1-10].top = true;
          }
          else{
            board[sqr_numP1].bot = true;
          }
        //  std::cout<<"Bot: "<<board[sqr_numP1].bot<<"\n\n";
          claim1 = true;
          turn = 2;
        }
        else if(edgeP1 == 2 && board[sqr_numP1].left == false){
          if(sqr_numP1 == 0 ||sqr_numP1 == 10 ||sqr_numP1 == 20 ||sqr_numP1 == 30 ||sqr_numP1 == 40 ||sqr_numP1 == 50 ||sqr_numP1 == 60 ||sqr_numP1 == 70 || sqr_numP1 == 80 ||sqr_numP1 == 90){
            board[sqr_numP1].left = true;
          }
          else{
            board[sqr_numP1].left = true;
            board[sqr_numP1-1].right = true;
          }
        //  std::cout<<"Left: "<<board[sqr_numP1].left<<"\n\n";
          claim1 = true;
          turn = 2;
        }
        else if(edgeP1 == 3 && board[sqr_numP1].right == false){
          if(sqr_numP1 == 9 ||sqr_numP1 == 19 ||sqr_numP1 == 29 ||sqr_numP1 == 39 ||sqr_numP1 == 49 ||sqr_numP1 == 59 ||sqr_numP1 == 69 ||sqr_numP1 == 79 || sqr_numP1 == 89 ||sqr_numP1 == 99){
              board[sqr_numP1].right = true;
          }
          else{
            board[sqr_numP1].right = true;
            board[sqr_numP1].left = true;
          }
        //std::cout<<"right: "<<board[sqr_numP1].right<<"\n\n";
          claim1 = true;
          turn = 2;
        }
        if(board[sqr_numP1].top == true && board[sqr_numP1].bot == true && board[sqr_numP1].left == true && board[sqr_numP1].right == true){
          board[sqr_numP1].completed = true;
          board[sqr_numP1].my_sqr = true;
          display_array[sqr_numP1] = 1;
          player_1_score++;
        }
      }
    }


//  INFINITE LOOP
      if(turn ==2){
        while(claim2 == false){
          std::cout<<"Turns: "<< turn_count<<"\n\n";
          claim1 = false;
          turn_count++;
    //    Calls the turn function
          lua_getglobal(L2, "turn");
          lua_call(L2, 0, 2); // 0 arg, 1 return
          long long int edgeP2 = lua_tointeger(L2, -1);
          long long int sqr_numP2 = lua_tointeger(L2, -2);
          lua_pop(L2, -1);
          lua_pop(L2, -2);
          std::cout<<"Edge: "<<edgeP2<<"\n\n";
          std::cout<<"Sqr_Num: "<<sqr_numP2<<"\n\n";
          std::cout<<"Top: "<< board[sqr_numP2].top<<"\n\n";
          std::cout<<"Bot: "<< board[sqr_numP2].bot<<"\n\n";
          std::cout<<"Left: "<< board[sqr_numP2].left<<"\n\n";
          std::cout<<"Right: "<< board[sqr_numP2].right<<"\n\n";
    //    Takes the Turn function result and determines if it's valid
          if(edgeP2 == 0 && board[sqr_numP2].top == false){
            if(sqr_numP2 <= 9){
              board[sqr_numP2].top = true;
            }
            else{
              board[sqr_numP2].top = true;
              board[sqr_numP2+10].bot = true;
            }
            //std::cout<<"Top: "<<board[sqr_numP2].top<<"\n\n";
            claim1 = true;
            turn = 1;
          }
          else if(edgeP2 == 1 && board[sqr_numP2].bot == false){
            if(sqr_numP2 > 90){
              board[sqr_numP2].bot = true;
              board[sqr_numP2-10].top = true;
            }
            else{
              board[sqr_numP2].bot = true;
            }
            //std::cout<<"Bot: "<<board[sqr_numP2].bot<<"\n\n";
            claim1 = true;
            turn = 1;
          }
          else if(edgeP2 == 2 && board[sqr_numP2].left == false){
            if(sqr_numP2 == 0 ||sqr_numP2 == 10 ||sqr_numP2 == 20 ||sqr_numP2 == 30 ||sqr_numP2 == 40 ||sqr_numP2 == 50 ||sqr_numP2 == 60 ||sqr_numP2 == 70 || sqr_numP2 == 80 ||sqr_numP2 == 90){
              board[sqr_numP2].left = true;
            }
            else{
              board[sqr_numP2].left = true;
              board[sqr_numP2-1].right = true;
            }
          //  std::cout<<"Left: "<<board[sqr_numP2].bot<<"\n\n";
            claim2 = true;
            turn = 1;
          }
          else if(edgeP2 == 3 && board[sqr_numP2].right == false){
            if(sqr_numP2 == 9 ||sqr_numP2 == 19 ||sqr_numP2 == 29 ||sqr_numP2 == 39 ||sqr_numP2 == 49 ||sqr_numP2 == 59 ||sqr_numP2 == 69 ||sqr_numP2 == 79 || sqr_numP2 == 89 ||sqr_numP2 == 99){
                board[sqr_numP2].right = true;
            }
            else{
              board[sqr_numP2].right = true;
              board[sqr_numP2 + 1].left = true;
            }
            //std::cout<<"right: "<<board[sqr_numP2].bot<<"\n\n";
            claim2 = true;
            turn = 1;
          }
          if(board[sqr_numP2].top == true && board[sqr_numP2].bot == true && board[sqr_numP2].left == true && board[sqr_numP2].right == true){
            board[sqr_numP2].completed = true;
            board[sqr_numP2].my_sqr = true;
            display_array[sqr_numP2] = 2;
            player_2_score++;
        }
      }
    }
    if( std::all_of(display_array, display_array+100, [](int i){return i!=0;})){
      gameEnd = true;
    }
  }



//---------------------------------------------------------
//Board Display
//    previously mentioned fancy displaying

      for(int x=0; x<10; ++x){
        for(int y=0; y<10; ++y){
          display_array2[x][y] = display_array[10*x+y];
        }
      }
      for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
          std::cout<<display_array2[i][j] <<"\t";
        }
        std::cout<<"\n\n";
      }
//    End of board creation/display
//---------------------------------------------------------
//Displays player win/lose messages
  if(player_1_score > player_2_score){
    lua_getglobal(L1, "i_win");
    lua_call(L1, 0, 0);

    lua_getglobal(L2, "i_lose");
    lua_call(L2, 0, 0);
  }
  else if(player_1_score == player_2_score){
    std::cout<< "ITS A TIEEEEE GAME!!" << "\n\n";
  }
  else{
    lua_getglobal(L2, "i_win");
    lua_call(L2, 0, 0);

    lua_getglobal(L1, "i_lose");
    lua_call(L1, 0, 0);
  }
//
//---------------------------------------------------------
//  Closes Lua files/states
    lua_close(L1);
    lua_close(L2);

    return EXIT_SUCCESS;
}
