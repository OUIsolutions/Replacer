



void restore_data(UserData *user_data){

    DtwTransaction *backup_transaction = newDtwTransaction_from_json_file(user_data->backup_file_path);

    if(!backup_transaction){
        DtwJsonTransactionError *generated_error = dtw_validate_json_transaction_file(user_data->backup_file_path);
        cli_interface.warning(&cli_interface,"%s",generated_error->mensage);
        generated_error->free(generated_error);
        return;
    }

    cli_interface.warning(&cli_interface,"the following files will be modified:\n");
    for(int i = 0; i < backup_transaction->size; i++){
        CTextStack *file = newCTextStack_string_empty();
        stack.format(file,"file: \"%s\"\n",backup_transaction->actions[i]->source);
        cli_interface.warning(&cli_interface,"%s",file->rendered_text);
        stack.free(file);
    }
    bool execute = cli_interface.ask_option(&cli_interface,"execute the restoration?","no | yes");

    if(execute){
        backup_transaction->commit(backup_transaction,NULL);
        cli_interface.print(&cli_interface,"data restored");
    }

    backup_transaction->free(backup_transaction);


}