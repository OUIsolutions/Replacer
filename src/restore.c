



void restore_data(UserData *user_data){

    DtwTransaction *backup_transaction = newDtwTransaction_from_json_file(user_data->backup_file_path);

    if(!backup_transaction){
        DtwJsonTransactionError *generated_error = dtw_validate_json_transaction_file(user_data->backup_file_path);
        interface.warning(&interface,"%s",generated_error->mensage);
        generated_error->free(generated_error);
        return;
    }

    interface.warning(&interface,"the following files will be modified:\n");
    for(int i = 0; i < backup_transaction->size; i++){
        CTextStack *file = newCTextStack_string_empty();
        stack.format(file,"file: \"%s\"\n",backup_transaction->actions[i]->source);
        interface.warning(&interface,"%s",file->rendered_text);
        stack.free(file);
    }
    bool execute = interface.ask_option(&interface,"execute the restoration?","no | yes");

    if(execute){
        backup_transaction->commit(backup_transaction,NULL);
        interface.print(&interface,"data restored");
    }

    backup_transaction->free(backup_transaction);


}