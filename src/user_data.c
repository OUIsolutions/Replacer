//
// Created by mateusmoutinho on 27/07/23.
//

const int  SEARCH = 0;
const int  REPLACE = 1;

typedef struct UserData{
    int action;
    char *first_token;
    int first_token_size;
    char *second_token;
    int second_token_size;
    char *source;
    int type_of_source;
    bool ignore_strings;
    bool case_sensitive;
}UserData;

UserData * extract_user_informations(){
    UserData *user = (UserData*)malloc(sizeof(UserData));
    //extracting informations

    CliInterface  interface = newCliInterface();

    user->action = interface.ask_option( &interface,"type the action","search | replace");

    if(user->action == SEARCH){
        user->first_token = interface.ask_string(&interface,"type the element to search",CLI_NOT_TRIM);
    }
    if(user->action == REPLACE){
        user->first_token = interface.ask_string(&interface,"type the element that will be replaced",CLI_NOT_TRIM);
        user->second_token = interface.ask_string(&interface,"type the element to replace",CLI_NOT_TRIM);
        user->second_token_size = (int)strlen(user->second_token);
    }
    user->first_token_size  = (int) strlen(user->first_token);

    user->ignore_strings = interface.ask_option( &interface,"do you want to ignore strings?","no | yes");
    user->case_sensitive = interface.ask_option( &interface,"case sensitive?","no | yes");

    if(!user->case_sensitive){
        CTextStack *first_token = newCTextStack_string(user->first_token);
        free(user->first_token);
        stack.self_lower(first_token);
        user->first_token = strdup(first_token->rendered_text);
        stack.free(first_token);
    }

    while(true){
        if(user->action == REPLACE){
            user->source = interface.ask_string(&interface,"type the folder/file to replace",CLI_TRIM);
        }
        else{
            user->source = interface.ask_string(&interface,"type the folder/file to search",CLI_TRIM);
        }

        //means its the current dir
        if(strcmp(user->source,".") == 0){
            free(user->source);
            user->source = dtw_get_current_dir();
        }

        user->type_of_source =dtw_entity_type(user->source);
        if(user->type_of_source == DTW_NOT_FOUND){
            CTextStack *s = newCTextStack_string_empty();
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