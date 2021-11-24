/**
 * @file parser.c
 * @author Martin Zmitko (xzmitk01@stud.fit.vutbr.cz)
 * @brief Implementace semantickeho a syntaktickeho parseru
 */

#include <stdlib.h>
#include "scanner.h"
#include "parser.h"

int err;

int prog(token_t *token){
    char *id;
    switch(token->type){
        case T_KW:
            switch(token->keyword){
                case KW_GLOBAL: //<prog> -> global ID : function ( <fdec_args> <f_types> <prog>
                    NEXT_CHECK_TYPE(token, T_ID);
                    id = token->data; 
                    //CREATE NEW FUNCTION
                    //ADD FUNCTION TO GLOBAL SYMTABLE
                    NEXT_CHECK_TYPE(token, T_COLON);
                    NEXT_CHECK_KW(token, KW_FUNCTION);
                    NEXT_CHECK_TYPE(token, T_PAR_L);
                    NEXT_TOKEN(token);
                    CALL_RULE(fdec_args, token);
                    CALL_RULE(f_types, token);
                    return prog(token);
                case KW_FUNCTION: //<prog> -> function ID ( <fdef_args> <f_types> <stat> <prog>
                    NEXT_CHECK_TYPE(token, T_ID);
                    id = token->data; 
                    //CREATE NEW FUNCTION
                    //ADD FUNCTION TO GLOBAL SYMTABLE
                    NEXT_CHECK_TYPE(token, T_PAR_L);
                    NEXT_TOKEN(token);
                    CALL_RULE(fdef_args, token);
                    CALL_RULE(f_types, token);
                    CALL_RULE(stat, token);
                    return prog(token);
                default:
                    return ERR_PARSE;
            }
            break;
        case T_ID: //<prog> -> ID ( <args> <prog>
            id = token->data;
            NEXT_CHECK_TYPE(token, T_PAR_L);
            CALL_RULE(args, token);
            //GENERATE CODE FOR FUNC CALL
            return prog(token);
        case T_EOF: //<prog> -> EOF
            return ERR_OK;
        default:
            return ERR_PARSE;
    }
}

int fdec_args(token_t *token){
    if(token->type == T_PAR_R)
        return ERR_OK;
    else{
        CALL_RULE(type, token);
        return fdec_args_n(token);
    }
}

int fdec_args_n(token_t *token){
    if(token->type == T_PAR_R)
        return ERR_OK;
    else if(token->type == T_COMMA){
        NEXT_TOKEN(token);
        CALL_RULE(type, token);
        return fdec_args_n(token);
    }
    else
        return ERR_PARSE;
}

int fdef_args(token_t *token){
    if(token->type == T_PAR_R)
        return ERR_OK;
    else if(token->type == T_ID){
        NEXT_CHECK_TYPE(token, T_COLON);
        NEXT_TOKEN(token);
        CALL_RULE(type, token);
        return fdef_args_n(token);
    }
    else
        return ERR_PARSE;
}

int fdef_args_n(token_t *token){
    if(token->type == T_PAR_R)
        return ERR_OK;
    else if(token->type == T_COMMA){
        NEXT_CHECK_TYPE(token, T_ID);
        NEXT_CHECK_TYPE(token, T_COLON);
        NEXT_TOKEN(token);
        CALL_RULE(type, token);
        return fdef_args_n(token);
    }
    else
        return ERR_PARSE;
}

int f_types(token_t *token){
    if(token->type == T_COLON){
        NEXT_TOKEN(token);
        return types(token);
    }
    else
        return ERR_OK; //TODO handle returning to previous token
}

int types(token_t *token){
    CALL_RULE(type, token);
    return types_n(token);
}

int types_n(token_t *token){
    if(token->type == T_COMMA){
        NEXT_TOKEN(token);
        CALL_RULE(type, token);
        return types_n(token);
    }
    else
        return ERR_OK; //TODO handle returning to previous token
}

int args(token_t *token){
    if(token->type == T_PAR_R)
        return ERR_OK;
    else{
        CALL_RULE(term, token);
        return args_n(token);
    }
}

int args_n(token_t *token){
    if(token->type == T_PAR_R)
        return ERR_OK;
    else if (token->type == T_COMMA){
        NEXT_TOKEN(token);
        CALL_RULE(term, token);
        return args_n(token);
    }
    else
        return ERR_PARSE;
}

int stat(token_t *token){
    return 0;
}

int IDs(token_t *token){
    return 0;
}

int EXPRs(token_t *token){
    return 0;
}

int type(token_t *token){
    return 0;
}

int term(token_t *token){
    switch(token->type){ //TODO
        case T_ID:
        case T_NUMBER:
        case T_INTEGER:
        case T_STRING:
            return ERR_OK;
        case T_KW:
            if(token->keyword == KW_NIL)
                return ERR_OK;
            else
                return ERR_PARSE;
        default:
            return ERR_PARSE;
    }
}
