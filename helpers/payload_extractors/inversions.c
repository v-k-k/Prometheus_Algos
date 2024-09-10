#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include "../simple_dynamic_string/sds.h"


void prepare_inversions_samples(sds content){
    sds *tokens;
    int count, j, new_count, fin_count, users, films;

    tokens = sdssplitlen(content, sdslen(content),"############################\n\n", 1, &count);

    for (j = 0; j < count; j++){
        if(sdslen(tokens[j])){
            //printf("!!!   %d\n", sdslen(tokens[j]));
            sds *new_tokens = sdssplitlen(tokens[j], sdslen(tokens[j]),"\n", 1, &new_count);
            for (int i = 0; i < new_count; i++){
                if(sdslen(new_tokens[i])){
                    if(strstr(new_tokens[i], "Кількість користувачів") != NULL){
                        sds *fin_tokens = sdssplitlen(new_tokens[i], sdslen(new_tokens[i])," ", 1, &fin_count);
                        users = atoi(fin_tokens[fin_count-1]);
                    }
                    else if(strstr(new_tokens[i], "Кількість фільмів") != NULL){
                        sds *fin_tokens = sdssplitlen(new_tokens[i], sdslen(new_tokens[i])," ", 1, &fin_count);
                        films = atoi(fin_tokens[fin_count-1]);
                    }

                    if(isdigit(new_tokens[i][0])){
                        sds_to_int_array(new_tokens[i]);
                    }
                }
                
            }
            
        }
    }
        
    sdsfreesplitres(tokens,count);
}