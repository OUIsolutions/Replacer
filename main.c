#include "dependencies/cliinput.h"
#include "dependencies/doTheWorld.h"
#include "dependencies/CTextEngine.h"
CTextStackModule stack;
#include "src/user_data.c"
#include "src/search.c"

int main(){
    stack = newCTextStackModule();
    UserData  *user_data = extract_user_informations();
    execute_the_search(user_data);
    user_data_free(user_data);



    return 0;
}