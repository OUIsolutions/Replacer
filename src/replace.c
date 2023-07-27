//
// Created by mateusmoutinho on 27/07/23.
//

CTextStack * execute_replace_for_file(UserData *user_data,char *filename){

    char *content = dtw_load_string_file_content(filename);

    if(!content){
        return NULL;
    }

    CTextStack * element = newCTextStack_string(content);
    CTextStack *new_element = newCTextStack_string_empty();

    free(content);

    bool found = false;
    bool inside_string = false;
    char string_breaker_char = '\0';

    for(int i =0; i < element->size; i++){
        char current_char = element->rendered_text[i];


        if(user_data->ignore_strings){

            //means its inside and its an scape scape
            if(inside_string  == true && current_char =='\\'){
                i+=1;
                stack.format(new_element,"%c",current_char);
                continue;
            }

            //means its inside string
            if(inside_string  == true && current_char != string_breaker_char){
                stack.format(new_element,"%c",current_char);
                continue;
            }


            //means its an start of string
            if(inside_string == false && (current_char == '"' || current_char == '\'') ){
                string_breaker_char = current_char;
                inside_string = true;
                stack.format(new_element,"%c",current_char);
                continue;
            }

            //means its an end of string
            if(inside_string == true && current_char == string_breaker_char){
                inside_string = false;
                string_breaker_char = '\0';
                stack.format(new_element,"%c",current_char);
                continue;
            }


        }

        CTextStack *possible_element = stack.substr(element,i,i+user_data->first_token_size);

        if(!user_data->case_sensitive){
            stack.self_lower(possible_element);
        }

        if(strcmp(possible_element->rendered_text,user_data->first_token) == 0){
            found = true;
            stack.format(new_element,"%s",user_data->second_token);
            i+=user_data->first_token_size -1;

        }

        else{
            stack.format(new_element,"%c",current_char);
        }
        stack.free(possible_element);


    }
    if(!found){
        stack.free(element);
        stack.free(new_element);
        return NULL;
    }


    stack.free(element);
    return  new_element;

}
void generated_transaction_backup(UserData *user_data,DtwTransaction *transaction){
    if(!user_data->backup_file_path){
        return;
    }


    DtwTransaction *backup_transaction = newDtwTransaction();
    for(int i = 0; i < transaction->size; i++) {
        char *current_file_path = transaction->actions[i]->source;
        char *content = dtw_load_string_file_content(current_file_path);
        backup_transaction->write_string(backup_transaction,current_file_path,content);
        free(content);
    }

    backup_transaction->dumps_transaction_to_json_file(backup_transaction,user_data->backup_file_path);
    backup_transaction->free(backup_transaction);

}

void execute_the_replace(UserData *user_data){

    DtwTransaction *transaction = newDtwTransaction();

    if(user_data->type_of_source == DTW_FILE_TYPE){
        CTextStack *replaced_text = execute_replace_for_file(user_data,user_data->source);
        if(replaced_text){
            transaction->write_string(transaction,user_data->source,replaced_text->rendered_text);
            stack.free(replaced_text);
        }
    }
    else{
        DtwStringArray *content = dtw_list_files_recursively(user_data->source,DTW_CONCAT_PATH);
        for(int i = 0; i < content->size; i++){
            char *filename = content->strings[i];
            if(dtw_ends_with(filename,".replacer")){
                continue;
            }
            CTextStack *replaced_text =execute_replace_for_file(user_data,filename);
            if(replaced_text){
                transaction->write_string(transaction,filename,replaced_text->rendered_text);
                stack.free(replaced_text);
            }
        }
        content->free(content);

    }

    if(!transaction->size){
        interface.warning(&interface,"nothing to replace\n");
        transaction->free(transaction);
        return;
    }
    generated_transaction_backup(user_data,transaction);

    interface.warning(&interface,"The Following files will be modified\n");
    for(int i = 0; i < transaction->size; i++){
        CTextStack  *file = newCTextStack_string_empty();
        stack.format(file,"file: %s\n",transaction->actions[i]->source);
        interface.warning(&interface,"%s", file->rendered_text);
        stack.free(file);
    }

    bool execute = interface.ask_option(&interface,"continue?","no | yes");
    if(execute){
        transaction->commit(transaction,NULL);
        interface.print(&interface,"data modified\n");
    }
    transaction->free(transaction);





}