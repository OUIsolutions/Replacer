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
            i+=user_data->second_token_size;
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

            CTextStack *replaced_text =execute_replace_for_file(user_data,content->strings[i]);
            if(replaced_text){
                transaction->write_string(transaction,user_data->source,replaced_text->rendered_text);
                stack.free(replaced_text);
            }
        }
        content->free(content);

    }

    CliInterface  anInterface = newCliInterface();
    if(!transaction->size){
        anInterface.warning(&anInterface,"nothing to replace\n");
        transaction->free(transaction);
        return;
    }
    
    anInterface.warning(&anInterface,"The Following files will be modified\n");
    for(int i = 0; i < transaction->size; i++){
        anInterface.warning(&anInterface,transaction->actions[i]->source);
    }



}