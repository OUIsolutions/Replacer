//
// Created by mateusmoutinho on 27/07/23.
//

const int  SEARCH = 0;
const int  REPLACE = 1;
const int RESTORE = 2;

typedef struct UserData{
    int action;
    char *first_token;
    char *second_token;
    char *source;
    char *backup_file_path;
    int first_token_size;
    int second_token_size;
    int type_of_source;
    bool ignore_strings;
    bool case_sensitive;
}UserData;

UserData * extract_user_informations(){
    UserData *user = (UserData*)malloc(sizeof(UserData));
    *user = (UserData){0};
    //extracting informations


    user->action = interface.ask_option( &interface,"type the action","search | replace | restore");
    if(user->action == RESTORE){
        while(true){
            char *not_formated_backup_file = interface.ask_string(&interface,"type the name of your backup",CLI_TRIM);
            CTextStack *backup_modifed = newCTextStack_string(not_formated_backup_file);
            stack.self_replace(backup_modifed,".replacer","");
            stack.text(backup_modifed, ".replacer");
            free(not_formated_backup_file);

            if(dtw_entity_type(backup_modifed->rendered_text) != DTW_FILE_TYPE){
                CTextStack *already_exist_mensage = newCTextStack_string_empty();
                stack.format(already_exist_mensage,"file: %s not exist\n",backup_modifed->rendered_text);
                interface.warning(&interface,already_exist_mensage->rendered_text);
                stack.free(already_exist_mensage);
                stack.free(backup_modifed);
                continue;
            }
            user->backup_file_path = strdup(backup_modifed->rendered_text);
            stack.free(backup_modifed);
            break;
        }
        return user;
    }

    if(user->action == SEARCH){
        user->first_token = interface.ask_string(&interface,"type the element to search",CLI_NOT_TRIM);
    }


    if(user->action == REPLACE){
        user->first_token = interface.ask_string(&interface,"type the element that will be replaced",CLI_NOT_TRIM);
        user->second_token = interface.ask_string(&interface,"type the element to replace",CLI_NOT_TRIM);
        user->second_token_size = (int)strlen(user->second_token);

        bool store_backup = interface.ask_option(&interface,"store backup?","no | yes");
        if(store_backup) {

            while(true){
                char *not_formated_backup_file = interface.ask_string(&interface,"type the name of your backup",CLI_TRIM);
                CTextStack *backup_modifed = newCTextStack_string(not_formated_backup_file);
                stack.self_replace(backup_modifed,".replacer","");
                stack.text(backup_modifed, ".replacer");
                free(not_formated_backup_file);

                if(dtw_entity_type(backup_modifed->rendered_text) != DTW_NOT_FOUND){
                    CTextStack *already_exist_mensage = newCTextStack_string_empty();
                    stack.format(already_exist_mensage,"file: %s already exist\n",backup_modifed->rendered_text);
                    interface.warning(&interface,already_exist_mensage->rendered_text);
                    stack.free(already_exist_mensage);
                    stack.free(backup_modifed);
                    continue;
                }
                user->backup_file_path = strdup(backup_modifed->rendered_text);
                stack.free(backup_modifed);
                break;
            }
        }

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
            user->source = interface.ask_string(&interface,"type the folder/file to replace (. for current folder)",CLI_TRIM);
        }
        else{
            user->source = interface.ask_string(&interface,"type the folder/file to search (. for current folder)",CLI_TRIM);
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
    if(user->source){
        free(user->source);
    }
    if(user->first_token){
        free(user->first_token);
    }
    if(user->second_token){
        free(user->second_token);
    }
    if(user->backup_file_path){
        free(user->backup_file_path);
    }

    free(user);

}