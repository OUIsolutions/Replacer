#include "dependencies/cliinput.h"
#include "dependencies/doTheWorld.h"
#include "dependencies/CTextEngine.h"
#include "user_data.c"

int main(){
    UserData  *user_data = extract_user_informations();
    user_data_free(user_data);



    return 0;
}