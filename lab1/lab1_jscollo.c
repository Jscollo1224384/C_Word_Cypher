#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// A simple tokenizer struct
typedef struct Tokenizer{
    char* string[100];  // original string.
    char* string2[100]; // extracted message string.
    int option;         // holds option number for switch statement.
}Tokenizer;
// A helper function that takes a decimal number and returns a binary equivalent
int* decimalToBinary(int dec){
    int decimal = dec;
    int remainder;
    int* result = malloc(8 * sizeof (int));
    for(int i = 7; i > 0; i--){
        remainder = decimal % 2;
        result[i] = remainder;
        decimal = decimal / 2;
    }
    return result;
}
// A helper function that takes a binary number and returns a decimal equivalent.
int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;
    int base = 1;
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }
    return dec_value;
}
/* This function takes 3 parameters
 * 1) a char pointer reference to a string
 * 2) the length of the string.
 * 3) k amount shift spaces fot the cypher.
 * The function takes the users input and performs a caesar cypher of k shifts and returns
 * and encrypted message to the user. */
void cypher(char* p,int size,int k){
    int e = 0;
    for(int i =0; i < size;i++){
        if((p[i] + k) > 126) //89
            e = ((p[i] + k) %125) +33;
        else
            e = ((p[i] + k) %125);
        p[i]=e;
        e = 0;
    }
}
/*
 * The following function takes the original string and tokenizes it.
 * Token 1 is the command and token2 is the message. This function
 * will assign a switch option number that corresponds with the entered command. It also passes
 * the message that will need to be encrypted, decrypted, encoded or decoded.
 */
Tokenizer strTokenizer(char* s,int l){
    Tokenizer tkn;
    char* token = malloc(l*sizeof (char));
    char* token2 = malloc(l*sizeof (char));
    strcpy(token,s);
    strcpy(token2,s);
    if(strcmp(&s[strlen(s) -1],")")!=0 && strcmp(&s[strlen(token)],"(")!=0) {
        if(strcmp(s,"exit")==0)
            tkn.option = 4;
        else
            tkn.option = 5;
    }
    else {
        strtok(token, "(");
        strcpy(token2, strtok(&token2[strlen(token) + 1], ")"));
        strcpy(tkn.string, token);
        strcpy(tkn.string2, token2);
    }

    if(strcmp(token,"encrypt")==0)
        tkn.option = 0;
    else if(strcmp(token,"decrypt")==0)
        tkn.option = 1;
    else if(strcmp(token,"encode")==0)
        tkn.option = 2;
    else if(strcmp(token,"decode")==0) {
        if(strlen(token2) % 8 != 0)
            tkn.option = 5;
        else {
            for (int i = 0; i < strlen(token2); i++) {
                if (!isdigit(token2[i])) {
                    tkn.option = 5;
                    break;
                }
                if (i == 7)
                    tkn.option = 3;
            }
        }
    }
    else if(strcmp(token,"exit")==0)
        tkn.option = 4;
    else
        tkn.option = 5;
    return tkn;

}
/*
* This function takes 3 parameters
* 1) a char pointer reference to a string
* 2) the length of the string.
* 3) k amount shift spaces to decypher.
* The function takes the users input and performs a caesar cypher of k shifts and returns
* and decrypted message to the user.
*/
void decypher(char* c,int size,int k){
    int d = 0;
    for(int i =0; i < size;i++){
        if((c[i] - k) < 33)
            d = ((c[i] - k) %125) +33;
        else
            d = ((c[i] - k) %125);
        c[i] = d;
        d = 0;
    }
}
/*
 * The following function takes the ascii code of each character in the user's input string.
 * It converts each ascii code into an 8 character long binary number and concatenates them
 * into an encoded message.
 */
char* encoder(char* s,int l){
    char* string = malloc(l*sizeof (char));
    strcpy(string,s);
    int ** ascii = malloc(l * sizeof (int*));
    int n;
    for(int i = 0;i < l;i++) {
        ascii[i] = malloc(9 * sizeof (int));
        n = string[i];
        ascii[i] = decimalToBinary(n);
    }
    printf("Encoded Result is: ");
    for(int i = 0; i < l; i++){
        for(int j = 0;j < 8; j++){
            printf("%d",ascii[i][j]);
        }
    }
    printf("\n");
    return 0;
}
/*
 * The following function takes the binary message entered by the user.
 * the message id broken up into individual ascii codes then converted to a decoded message.
 */
char* decoder(char* s,int l){
    int length = l / 8;
    char* string = malloc(l * sizeof(char));
    strcpy(string,s);
    char** substring = malloc(length* sizeof(char*));
    int* bin = malloc(l * sizeof(int));
    int i = 0;
    int count =0;
    for(int k = 0;k<length;k++){
        substring[k] = malloc(9 * sizeof (char));
    }
    while(i < length){
        for(int j = 0; j < l; j++){
            substring[i][count] = string[j];
            count++;
            if(count > 7) {

                bin[i] = binaryToDecimal(atoi(substring[i]));
                count = 0;
                i++;
            }
        }
    }
    char* ascii = malloc(length * sizeof (char));
    for(int m = 0; m < length;m++){
        ascii[m] = bin[m];
    }
    return ascii;
}
int main() {
    int option;
    int quit = 0;
    Tokenizer tkn;
    //program command loop
    while(quit == 0) {
        char* select = malloc(100 * sizeof (char ));
        char result[strlen(tkn.string2)];
        printf("\nEnter a command: ");
        scanf("%s",select);
        int varLength = strlen(select);

        tkn = strTokenizer(select,varLength);
        option = tkn.option;
        // switch statement of possible commands plus a default that throws error message.
        switch (option) {
            case 0:
                cypher(tkn.string2,strlen(tkn.string2),5);
                printf("The encrypted message is: %s\n",tkn.string2);
                break;
            case 1:
                decypher(tkn.string2, strlen(tkn.string2),5);
                printf("The decrypted message is: %s\n",tkn.string2);
                break;
            case 2:
                encoder(tkn.string2,strlen(tkn.string2));
                break;
            case 3:
                strcpy(result,decoder(tkn.string2,strlen(tkn.string2)));
                printf("Decoded message is: %s\n",result);
                break;
                // case 4 exits the program and frees allocated heap memory. memory that was allocated
                // within function are freed when the functions return.
            case 4:
                printf("You have exited the program.");
                quit = 1;
                free(select);
                break;
                //default is called whenever an invalid input is entered.
            default:
                printf("Error! Malformed Command. Try another command.\n");
                break;
        }
    }

    return 0;
}
/*
 char* select = malloc(100 * sizeof (char ));
        printf("Enter a command: ");
        scanf("%s",&*select);
        int varLength = strlen(select);

        if (strcmp(encrypt, select) == 0)
                option = 0;

        else if (strcmp(decrypt, select) == 0)
            option = 1;
        else if (strcmp(encode, select) == 0)
            option = 2;
        else if (strcmp(decode, select) == 0)
            option = 3;
        else if (strcmp(exit, select) == 0)
            option = 4;
        else
            option = 5;*/