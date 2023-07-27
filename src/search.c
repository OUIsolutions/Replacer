//
// Created by mateusmoutinho on 27/07/23.
//

void execute_search_for_file(UserData *user_data,char *filename){

    char *content = dtw_load_string_file_content(filename);
    if(!content){
        return;
    }
    CTextStack * element = newCTextStack_string(content);

    CTextStack *mensage = newCTextStack_string_empty();
    stack.format(mensage,"file: \"%s\" lines:[",filename);


    long current_line = 0;
    bool found = false;
    bool inside_string = false;
    char string_breaker_char = '\0';

    for(int i =0; i < element->size; i++){
        char current_char = element->rendered_text[i];

        if(current_char =='\n'){
            current_line+=1;
        }

        if(user_data->ignore_strings){

            //means its inside and its an scape scape
            if(inside_string  == true && current_char =='\\'){
                i+=1;
                continue;
            }

            //means its inside string
            if(inside_string  == true && current_char != string_breaker_char){
                continue;
            }


            //means its an start of string
            if(inside_string == false && (current_char == '"' || current_char == '\'') ){
                string_breaker_char = current_char;
                inside_string = true;
                continue;
            }

            //means its an end of string
            if(inside_string == true && current_char == string_breaker_char){
                inside_string = false;
                string_breaker_char = '\0';
                continue;
            }


        }

        CTextStack *possible_element = stack.substr(element,i,i+user_data->first_token_size);

        if(!user_data->case_sensitive){
            stack.self_lower(possible_element);
        }

        if(strcmp(possible_element->rendered_text,user_data->first_token) == 0){
            found = true;
            stack.format(mensage,"%d,",current_line+1);
        }
        stack.free(possible_element);


    }

    if(!found){
        stack.free(mensage);
        stack.free(element);
        return;
    }

    stack.self_substr(mensage,0,-2);
    stack.format(mensage,"]\n");
    interface.print(&interface,"%s", mensage->rendered_text);
    free(content);
    stack.free(mensage);
    stack.free(element);


}

void execute_the_search(UserData *user_data){
    interface.print(&interface,"-------------------Occurrences------------------\n");
    if(user_data->type_of_source == DTW_FILE_TYPE){
        execute_search_for_file(user_data,user_data->source);
        return;
    }
    //VOID
    DtwStringArray *content = dtw_list_files_recursively(user_data->source,DTW_CONCAT_PATH);
    for(int i = 0; i < content->size; i++){
        execute_search_for_file(user_data,content->strings[i]);
    }
    content->free(content);


}