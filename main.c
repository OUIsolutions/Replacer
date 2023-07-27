#include "dependencies/cliinput.h"
#include "dependencies/doTheWorld.h"
#include "dependencies/CTextEngine.h"
CTextStackModule stack;
CliInterface  interface;
#include "src/user_data.c"
#include "src/search.c"
#include "src/replace.c"
#include "src/restore.c"

int main(){

    stack = newCTextStackModule();
    interface = newCliInterface();
    /*
    UserData *user = malloc(sizeof(UserData));
    *user = (UserData){0};
    user->action = SEARCH;
    user->first_token = strdup("void");
    user->first_token_size = strlen("void");

    user->case_sensitive = true;
    user->ignore_strings= true;
    user->source = strdup("src");
    execute_the_search(user);
    user_data_free(user);
    return 0;
    */
     UserData  *user_data = extract_user_informations();


    if(user_data->action == SEARCH){
        execute_the_search(user_data);
    }
    if(user_data->action == REPLACE){
        execute_the_replace(user_data);
    }
    if(user_data->action == RESTORE){
        restore_data(user_data);
    }

    user_data_free(user_data);



    return 0;
}