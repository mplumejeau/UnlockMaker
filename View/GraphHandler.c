//
// Created by louenn on 08/05/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "GraphHandler.h"



void addCardGraphData(char* graphDataPath, const char* id, int type) {
    char cardType[4][7] = {"grey52","blue3 ","red2  ","green3"};
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fseek(fp, -3, SEEK_END);
    fprintf(fp,"\nnode[fillcolor=%s];%s;\n\n}",cardType[type],id);
    fclose(fp);
}
void addLinkGraphData(char* graphDataPath, char* idParent, char* idChild, int type) {
    char linkType[4][7] = {"black ","orchid","cyan3 ","orange"};
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fseek(fp, -3, SEEK_END);
    fprintf(fp,"\nedge[color=%s];%s->%s;\n\n}",linkType[type],idParent,idChild);
    fclose(fp);
}

void modifyCardTypeGraphData(char* graphDataPath, const char* id, int newType) {
    char cardType[4][7] = {"grey52","blue3 ","red2  ","green3"};
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    char buffer[60];
    char returns[5];
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%[^\n]%*[\n]", buffer);
    do {
        fscanf(fp, "%[^\n]%[\n]", buffer, returns);
        if(buffer[0] == 'n') {
            if(buffer[23]==id[0]&&(buffer[24]==id[1]||(buffer[24]==';'&&id[1]=='\0'))) {
                fseek(fp, -9-(int)strlen(id)-(int)strlen(returns), SEEK_CUR);
                fprintf(fp,"%s", cardType[newType]);
                fseek(fp, 3+(int)strlen(id)+(int)strlen(returns), SEEK_CUR);
            }
        }
    } while(buffer[0]!='}'&&buffer[0]!='\0');
    fclose(fp);
}

void modifyLinkTypeGraphData(char* graphDataPath, const char* idParent, const char* idChild, int newType) {
    char linkType[4][7] = {"black ","orchid","cyan3 ","orange"};
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    char buffer[60];
    char returns[5];
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%[^\n]%*[\n]", buffer);
    int edgeChiffre1;
    do {
        fscanf(fp, "%[^\n]%[\n]", buffer, returns);
        if(buffer[0] == 'e') {
            if(buffer[20]=='-') edgeChiffre1 = 1;
            else                edgeChiffre1 = 2;
            if((buffer[19]==idParent[0]&&(buffer[20]==idParent[1]||(buffer[20]=='-'&&idParent[1]=='\0')))&&(buffer[21+edgeChiffre1]==idChild[0]&&(buffer[22+edgeChiffre1]==idChild[1]||(buffer[22+edgeChiffre1]==';'&&idChild[1]=='\0')))) {
                fseek(fp, -11-(int)strlen(idParent)-(int)strlen(idChild)-(int)strlen(returns), SEEK_CUR);
                fprintf(fp,"%s", linkType[newType]);
                fseek(fp, 5+(int)strlen(idParent)+(int)strlen(idChild)+(int)strlen(returns), SEEK_CUR);
            }
        }
    } while(buffer[0]!='}'&&buffer[0]!='\0');
    fclose(fp);
}

void removeCardGraphData(char* graphDataPath, const char* id) {
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fseek(fp, 0, SEEK_END);
    char cpy[ftell(fp)];
    char buffer[30] = "";
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%[^\n]%*[\n]", cpy);
    strcat(cpy, "\n");
    int edgeChiffre1;
    do {
        fscanf(fp, "%[^\n]%*[\n]", buffer);
        if(buffer[0] == 'n') {
            if(!(buffer[23]==id[0]&&(buffer[24]==id[1]||(buffer[24]==';'&&id[1]=='\0')))) {
                strcat(cpy, buffer);
                strcat(cpy, "\n");
            }
        }
        else if(buffer[0] == 'e') {
            if(buffer[20]=='-') edgeChiffre1 = 1;
            else                edgeChiffre1 = 2;

            if(!((buffer[19]==id[0]&&(buffer[20]==id[1]||(buffer[20]=='-'&&id[1]=='\0')))||(buffer[21+edgeChiffre1]==id[0]&&(buffer[22+edgeChiffre1]==id[1]||(buffer[22+edgeChiffre1]==';'&&id[1]=='\0'))))) {
                strcat(cpy, buffer);
                strcat(cpy, "\n");
            }
        }
        else if(buffer[0]!='}'&&buffer[0]!='\0') {
            if(!(buffer[0]==id[0]&&(buffer[1]==id[1]||(buffer[1]==' '&&id[1]=='\0')))) {
                strcat(cpy, buffer);
                strcat(cpy, "\n");
            }
        }
    } while(buffer[0]!='}'&&buffer[0]!='\0');
    strcat(cpy, "\n}");
    fclose(fp);
    fp = fopen(graphDataPath, "w");
    if(fp == NULL) {
        fprintf(stderr, "error : opening/writing of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fprintf(fp, "%s", cpy);
    fclose(fp);
}

void removeLinkGraphData(char* graphDataPath, const char* idParent, const char* idChild) {
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fseek(fp, 0, SEEK_END);
    char cpy[ftell(fp)];
    char buffer[30] = "";
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%[^\n]%*[\n]", cpy);
    strcat(cpy, "\n");
    int edgeChiffre1;
    do {
        fscanf(fp, "%[^\n]%*[\n]", buffer);
        if(buffer[0] == 'n') {
            strcat(cpy, buffer);
            strcat(cpy, "\n");
        }
        if(buffer[0] == 'e') {
            if(buffer[20]=='-') edgeChiffre1 = 1;
            else                edgeChiffre1 = 2;

            if(!((buffer[19]==idParent[0]&&(buffer[20]==idParent[1]||(buffer[20]=='-'&&idParent[1]=='\0')))&&(buffer[21+edgeChiffre1]==idChild[0]&&(buffer[22+edgeChiffre1]==idChild[1]||(buffer[22+edgeChiffre1]==';'&&idChild[1]=='\0'))))) {
                strcat(cpy, buffer);
                strcat(cpy, "\n");
            }
        }
    } while(buffer[0]!='}'&&buffer[0]!='\0');
    strcat(cpy, "\n}");
    fclose(fp);
    fp = fopen(graphDataPath, "w");
    if(fp == NULL) {
        fprintf(stderr, "error : opening/writing of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fprintf(fp, "%s", cpy);
    fclose(fp);
}

void setCardAsRootGraphData(char* graphDataPath, const char* id) {
    char cardId[5] = "";
    strcpy(cardId, id);
    if(strlen(cardId)==1) strcat(cardId, " ");
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    char buffer[60];
    char returns[5];
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%[^\n]%*[\n]", buffer);
    do {
        fscanf(fp, "%[^\n]%[\n]", buffer, returns);
        if(buffer[0]!='e'&&buffer[0]!='n'&&buffer[0]!='}'&&buffer[0]!='\0') {
            fseek(fp, -18-(int)strlen(returns), SEEK_CUR);
            fprintf(fp,"%s",cardId);
            fclose(fp);
            return;
        }
    } while(buffer[0]!='}'&&buffer[0]!='\0');
    fseek(fp, -3, SEEK_END);
    fprintf(fp,"\n%s[shape=Msquare];\n\n}",cardId);
    fclose(fp);
}

void removeCardAsRootGraphData(char* graphDataPath) {
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fseek(fp, 0, SEEK_END);
    char cpy[ftell(fp)];
    char buffer[30] = "";
    fseek(fp, 0, SEEK_SET);
    fscanf(fp, "%[^\n]%*[\n]", cpy);
    strcat(cpy, "\n");
    do {
        fscanf(fp, "%[^\n]%*[\n]", buffer);
        if(buffer[0] == 'n' || buffer[0] == 'e') {
            strcat(cpy, buffer);
            strcat(cpy, "\n");
        }
    } while(buffer[0]!='}'&&buffer[0]!='\0');
    strcat(cpy, "\n}");
    fclose(fp);
    fp = fopen(graphDataPath, "w");
    if(fp == NULL) {
        fprintf(stderr, "error : opening/writing of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
    }
    fprintf(fp, "%s", cpy);
    fclose(fp);
}