#include "dependencies/cliinput.h"
#include "dependencies/doTheWorld.h"
#include "dependencies/CTextEngine.h"

int main(){

    CTextStackModule stack = newCTextStackModule();
    CliInterface  interface = newCliInterface();

    const int SEARCH = 0;
    const int REPLACE = 2;
    int action = interface.ask_option( &interface,"type the action","search | replace");

    char *first_token;
    char *second_token;

    if(action == SEARCH){
        first_token = interface.ask_string(&interface,"type the element to search",CLI_TRIM);
    }
    if(action == REPLACE){
        first_token = interface.ask_string(&interface,"type the element that will be replaced",CLI_TRIM);
        second_token = interface.ask_string(&interface,"type the element to replace",CLI_TRIM);
    }
    bool ignore_strings = interface.ask_option( &interface,"do you want to ignore strings?","yes | no");

    char *source;
    int type_source;
    while(true){
        source = interface.ask_string(&interface,"type the source to act",CLI_TRIM);
        type_source =dtw_entity_type(source);
        if(type_source == DTW_NOT_FOUND){
            CTextStack *s = newCTextStack("","");
            stack.format(s,"%s is not an valid source\n",source);
            interface.warning(&interface,s->rendered_text);
            free(source);
            stack.free(s);
            continue;
        }
        break;
    }



    
    return 0;
}