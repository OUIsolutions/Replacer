#include "dependencies/cliinput.h"
#include "dependencies/doTheWorld.h"
#include "dependencies/CTextEngine.h"
CTextStackModule stack;
#include "src/user_data.c"
#include "src/search.c"
#include "src/replace.c"

int main(){
    stack = newCTextStackModule();
    UserData  *user_data = extract_user_informations();

    if(user_data->action == SEARCH){
        execute_the_search(user_data);
    }
    if(user_data->action == REPLACE){
        execute_the_replace(user_data);
    }

    user_data_free(user_data);



    return 0;
}