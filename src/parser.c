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
    //char *id;
    switch(token->type){
        case T_KW:
            switch(token->keyword){
                case KW_GLOBAL: //<prog> -> global ID : function ( <fdec_args> <f_types> <prog>
                    NEXT_CHECK_TYPE(token, T_ID);
                    //id = token->data; 
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
                    //id = token->data; 
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
            //id = token->data;
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
    if(token->type == T_PAR_R) //<fdec_args> -> )
        return ERR_OK;
    else{ //<fdec_args> -> <type> <fdec_args_n>
        CALL_RULE(type, token);
        return fdec_args_n(token);
    }
}

int fdec_args_n(token_t *token){
    if(token->type == T_PAR_R) //<fdec_args_n> -> )
        return ERR_OK;
    else if(token->type == T_COMMA){ //<fdec_args_n> -> , <type> <fdec_args_n>
        NEXT_TOKEN(token);
        CALL_RULE(type, token);
        return fdec_args_n(token);
    }
    else
        return ERR_PARSE;
}

int fdef_args(token_t *token){
    if(token->type == T_PAR_R) //<fdef_args> -> )
        return ERR_OK;
    else if(token->type == T_ID){ //<fdef_args> -> ID : <type> <fdef_args_n>
        NEXT_CHECK_TYPE(token, T_COLON);
        NEXT_TOKEN(token);
        CALL_RULE(type, token);
        return fdef_args_n(token);
    }
    else
        return ERR_PARSE;
}

int fdef_args_n(token_t *token){
    if(token->type == T_PAR_R) //<fdef_args_n> -> )
        return ERR_OK;
    else if(token->type == T_COMMA){ //<fdef_args_n> -> , ID : <type> <fdef_args_n>
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
    if(token->type == T_COLON){ //<f_types> -> : <types>
        NEXT_TOKEN(token);
        return types(token);
    }
    else //<f_types> -> e
        return ERR_OK; //TODO handle returning to previous token
}

int types(token_t *token){ //<types> -> <type> <types_n>
    CALL_RULE(type, token);
    return types_n(token);
}

int types_n(token_t *token){
    if(token->type == T_COMMA){ //<types_n> -> , <type> <types_n>
        NEXT_TOKEN(token);
        CALL_RULE(type, token);
        return types_n(token);
    }
    else //<types_n> -> e
        return ERR_OK; //TODO handle returning to previous token
}

int args(token_t *token){
    if(token->type == T_PAR_R) //<args> -> )
        return ERR_OK;
    else{ //<args> -> <term> <args_n>
        CALL_RULE(term, token);
        return args_n(token);
    }
}

int args_n(token_t *token){
    if(token->type == T_PAR_R) //<args_n> -> )
        return ERR_OK;
    else if (token->type == T_COMMA){ //<args_n> -> , <term> <args_n>
        NEXT_TOKEN(token);
        CALL_RULE(term, token);
        return args_n(token);
    }
    else
        return ERR_PARSE;
}

int stat(token_t *token){
    if(token->type == T_ID){
        if(1/*TODO if type of id from symtable is function*/){ //<stat> -> ID ( <args> <stat>
            NEXT_CHECK_TYPE(token, T_PAR_L);
            NEXT_TOKEN(token);
            CALL_RULE(args, token);
            return stat(token);
        }
        else{ //<stat> -> <IDs> <EXPRs> <stat>
            CALL_RULE(IDs, token);
            CALL_RULE(EXPRs, token);
            return stat(token);
        }
    }
    else if(token->type == T_KW){
        switch(token->keyword){
            case KW_LOCAL:
                NEXT_CHECK_TYPE(token, T_ID);
                NEXT_CHECK_TYPE(token, T_COLON);
                NEXT_TOKEN(token);
                CALL_RULE(type, token);
                if(token->type == T_EQ){ //<stat> -> local ID : <type> = <EXPRs> <stat>
                    NEXT_TOKEN(token);
                    CALL_RULE(EXPRs, token);
                }
                else //<stat> -> local ID : <type> <stat>
                    NEXT_TOKEN(token);
                return stat(token);
            case KW_IF: //<stat> -> if EXPR then <stat> else <stat> <stat>
                NEXT_TOKEN(token);
                //PARSE EXPRESSION
                NEXT_CHECK_KW(token, KW_THEN);
                NEXT_TOKEN(token);
                CALL_RULE(stat, token);
                CHECK_KW(token, KW_ELSE);
                NEXT_TOKEN(token);
                CALL_RULE(stat, token);
                return stat(token);
            case KW_WHILE: //<stat> -> while EXPR do <stat> <stat>
                NEXT_TOKEN(token);
                //PARSE EXPRESSION
                NEXT_CHECK_KW(token, KW_DO);
                NEXT_TOKEN(token);
                CALL_RULE(stat, token);
                return stat(token);
            case KW_RETURN: //<stat> -> return <EXPRs> <stat>
                NEXT_TOKEN(token);
                CALL_RULE(EXPRs, token);
                return stat(token);
            case KW_END: //<stat> -> end
                return ERR_OK;
            default:
                return ERR_PARSE;
        }
    }
    else
        return ERR_PARSE;
}

int IDs(token_t *token){
    if(token->type == T_ID){ //<IDs> -> ID <IDs_n>
        NEXT_TOKEN(token);
        return IDs_n(token);
    }
    else
        return ERR_PARSE;
}

int IDs_n(token_t *token){
    if(token->type == T_EQ) //<IDs_n> -> =
        return ERR_OK;
    else if(token->type == T_COMMA){ //<IDs_n> -> , ID <IDs_n>
        NEXT_CHECK_TYPE(token, T_ID);
        NEXT_TOKEN(token);
        return IDs_n(token);
    }
    else
        return ERR_PARSE;
}

int EXPRs(token_t *token){
    if(token->type == T_ID){ //<EXPRs> -> ID ( <args>
        NEXT_CHECK_TYPE(token, T_PAR_L);
        NEXT_TOKEN(token);
        CALL_RULE(args, token);
        return ERR_OK;
    }
    else{ //<EXPRs> -> EXPR <EXPRs_n>
        //PARSE EXPRESSION
        NEXT_TOKEN(token);
        return EXPRs_n(token);
    }
}

int EXPRs_n(token_t *token){
    if(token->type == T_COMMA){ //<EXPRs_n> -> , EXPR <EXPRs_n>
        NEXT_TOKEN(token);
        //PARSE EXPRESSION
        NEXT_TOKEN(token);
        return EXPRs_n(token);
    }
    else //<EXPRs_n> -> e
        return ERR_OK; //TODO handle returning to previous token
}

int type(token_t *token){
    if(token->type != T_KW)
        return ERR_PARSE;
    switch(token->keyword){ //TODO
        case KW_NUMBER:
        case KW_INTEGER:
        case KW_STRING:
        case KW_NIL:
            return ERR_OK;
        default:
            return ERR_PARSE;
    }
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
