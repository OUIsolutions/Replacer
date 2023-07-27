//
// Created by mateusmoutinho on 27/07/23.
//

const int  SEARCH = 0;
const int  REPLACE = 1;

typedef struct UserData{
    int action;
    char *first_token;
    char *second_token;
    char *source;
    int type_of_source;
    bool ignore_strings;
}UserData;

UserData * extract_user_informations(){
    UserData *user = (UserData*)malloc(sizeof(UserData));
    //extracting informations
    CTextStackModule stack = newCTextStackModule();
    CliInterface  interface = newCliInterface();

    user->action = interface.ask_option( &interface,"type the action","search | replace");

    if(user->action == SEARCH){
        user->first_token = interface.ask_string(&interface,"type the element to search",CLI_TRIM);
    }
    if(user->action == REPLACE){
        user->first_token = interface.ask_string(&interface,"type the element that will be replaced",CLI_TRIM);
        user->second_token = interface.ask_string(&interface,"type the element to replace",CLI_TRIM);
    }
    user->ignore_strings = interface.ask_option( &interface,"do you want to ignore strings?","yes | no");


    while(true){
        if(user->action == REPLACE){
            user->source = interface.ask_string(&interface,"type the folder/file to replace",CLI_TRIM);
        }
        else{
            user->source = interface.ask_string(&interface,"type the folder/file to search",CLI_TRIM);
        }
        user->type_of_source =dtw_entity_type(user->source);
        if(user->type_of_source == DTW_NOT_FOUND){
            CTextStack *s = newCTextStack("","");
            stack.format(s,"%s is not an valid source\n",user->source);
            interface.warning(&interface,s->rendered_text);
            free(user->source);
            stack.free(s);
            continue;
        }
        break;
    }
    return user;
}

void user_data_free(UserData *user){
    free(user->source);
    free(user->first_token);
    if(user->action == REPLACE){
        free(user->second_token);
    }
    free(user);

}