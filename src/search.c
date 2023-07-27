//
// Created by mateusmoutinho on 27/07/23.
//

void execute_search_for_file(UserData *user_data,char *filename){

    char *content = dtw_load_string_file_content(filename);
    if(!content){
        return;
    }
    CTextStack * element = newCTextStack_string(content);
    printf("element size %ld\n",element->size);
    printf("content size %ld\n", strlen(content));
    return;

    CTextStack *mensage = newCTextStack_string_empty();
    stack.format(mensage,"file: %s lines:[",filename);


    long current_line = 0;
    bool inside_string = false;

    for(int i =0; i < strlen(content); i++){
        char current_char = content[i];

        if(current_char =='\n'){
            current_line+=1;
        }
        /*
        if(user_data->ignore_strings){
            bool is_string_breaker = current_char == '"' || current_char == '\'';

            if(inside_string && is_string_breaker == false){
                continue;
            }
            if(inside_string == false && is_string_breaker){
                inside_string = true;
                continue;
            }
            inside_string = false;
        }
        CTextStack *possible_element = stack.substr(element,i,i+user_data->first_token_size);
        if(strcmp(possible_element->rendered_text,user_data->first_token) == 0){
            stack.format(mensage,"%d,",current_line);
        }
        stack.free(possible_element);

        */
    }

    printf("total de linhas %ld\n",current_line);
    stack.self_substr(mensage,0,-1);
    stack.format(mensage,"]\n");
    CliInterface  interfacce = newCliInterface();
    interfacce.print(&interfacce,mensage->rendered_text);
    free(content);

    stack.free(mensage);
    stack.free(element);


}

void execute_the_search(UserData *user_data){
    if(user_data->type_of_source == DTW_FILE_TYPE){
        execute_search_for_file(user_data,user_data->source);
        return;
    }

    DtwStringArray *content = dtw_list_files_recursively(user_data->source,DTW_CONCAT_PATH);
    for(int i = 0; i < content->size; i++){
        execute_search_for_file(user_data,content->strings[i]);
    }
    content->free(content);


}