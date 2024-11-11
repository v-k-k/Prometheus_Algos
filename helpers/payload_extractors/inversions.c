#include "payload_extractors.h"


void prepare_inversions_samples(sds fname, sds content){
    sds *tokens;
    int tokens_count, j, data_count, fin_count, users, films, _;

    tokens = sdssplitlen(content, sdslen(content),"\n\n\n", 3, &tokens_count);
    sds *data_lines = sdssplitlen(tokens[0], sdslen(tokens[0]),"\n", 1, &data_count);
    sds *users_films = sdssplitlen(data_lines[0], sdslen(data_lines[0])," ", 1, &_);
    users = atoi(users_films[0]);
    films = atoi(users_films[1]);

    // TODO: loop through data_lines and create appropriate struct

    for (j = 0; j < tokens_count; j++){
        sds sss = tokens[j];
        if(sdslen(tokens[j])){
            //printf("!!!   %d\n", sdslen(tokens[j]));
            sds *new_tokens = sdssplitlen(tokens[j], sdslen(tokens[j]),"\n", 1, &data_count);
            for (int i = 0; i < data_count; i++){
                if(sdslen(new_tokens[i])){
                    if(strstr(new_tokens[i], "Кількість користувачів") != NULL){
                        sds *fin_tokens = sdssplitlen(new_tokens[i], sdslen(new_tokens[i])," ", 1, &fin_count);
                        users = atoi(fin_tokens[fin_count-1]);
                    }
                    else if(strstr(new_tokens[i], "Кількість фільмів") != NULL){
                        sds *fin_tokens = sdssplitlen(new_tokens[i], sdslen(new_tokens[i])," ", 1, &fin_count);
                        films = atoi(fin_tokens[fin_count-1]);
                    }

                    //if(isdigit(new_tokens[i][0])){
                      //  sds_to_int_array(new_tokens[i]);
                    //}
                }
                
            }
            
        }
    }
        
    sdsfreesplitres(tokens, tokens_count);
}