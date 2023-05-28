//
// Created by louenn on 08/05/23.
//
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <graphviz/gvc.h>

#include "GraphHandler.h"
#include "ViewApplication.h"

/* The project's graph files paths */
char graphDirPath[MAXPATH+6] = "";
char graphDataPath[MAXPATH+20] = "";
char graphPNGPath[MAXPATH+16] = "";

void initGraphFiles(Project* p) {
    strcat(graphDirPath, p->path);
    strcat(graphDirPath, "/");
    strcat(graphDirPath, p->name);
    strcat(graphDirPath, "/Graph");
    strcpy(graphDataPath, graphDirPath);
    strcat(graphDataPath, "/graphData.txt");
    strcpy(graphPNGPath, graphDirPath);
    strcat(graphPNGPath, "/graph.png");
    if (mkdir(graphDirPath, 0755) != 0 ) {
        fprintf( stderr, "error : creation of the directory %s is impossible\n", graphDirPath);
        fprintf(stderr,"%d\n", errno);
    }
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "w");
    if(fp == NULL) {
        fprintf(stderr, "error : creation of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        fclose(fp);
        return;
    }
    fprintf(fp, "digraph G{bgcolor=lightgrey;node[style=filled;shape=circle];\n}");
    fclose(fp);
    exportGraphPNGFromFile(graphDataPath, graphPNGPath);
    setCenterImagePath(graphPNGPath);
}

int deleteGraphFiles() {
    // remove the graph files

    if(remove(graphDataPath) != 0){
        fprintf(stderr, "error : deletion of the file %s is impossible\n",graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
    }
    if(remove(graphPNGPath) != 0){
        fprintf(stderr, "error : deletion of the file %s is impossible\n", graphPNGPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
    }

    // remove the graph directory

    if(rmdir(graphDirPath) != 0){
        fprintf( stderr, "error : deletion of the directory %s is impossible\n", graphDirPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
    }
    return 0;
}

int exportGraphPNGFromFile(char* dataInput, char* PNGOutput) {
    Agraph_t *g;
    GVC_t *gvc;
    gvc = gvContext();
    FILE *fp = NULL;
    fp = fopen(dataInput, "r");
    if(fp == NULL) return -1;
    g = agread(fp, NULL);
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", PNGOutput);
    gvFreeLayout(gvc,g);
    agclose(g);
    gvFreeContext(gvc);
    fclose(fp);
    return 0;
}

int refreshGraphPNG() {
    int result = exportGraphPNGFromFile(graphDataPath, graphPNGPath);
    reloadCenterImage();
    return result;
}

int addCardGraphData(const char* id, int type) {
    char cardType[4][7] = {"grey52","blue3 ","red2  ","green3"};
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
    }
    fseek(fp, -3, SEEK_END);
    fprintf(fp,"\nnode[fillcolor=%s];%s;\n\n}",cardType[type],id);
    fclose(fp);
    return 0;
}

int modifyCardTypeGraphData(const char* id, int newType) {
    char cardType[4][7] = {"grey52","blue3 ","red2  ","green3"};
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
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
    return 0;
}

int setCardAsRootGraphData(const char* id) {
    char cardId[5] = "";
    strcpy(cardId, id);
    if(strlen(cardId)==1) strcat(cardId, " ");
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
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
            return -1;
        }
    } while(buffer[0]!='}'&&buffer[0]!='\0');
    fseek(fp, -3, SEEK_END);
    fprintf(fp,"\n%s[shape=Msquare];\n\n}",cardId);
    fclose(fp);
    return 0;
}

int removeCardsAsRootGraphData() {
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
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
        return -1;
    }
    fprintf(fp, "%s", cpy);
    fclose(fp);
    return 0;
}

int removeCardGraphData(const char* id) {
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
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
        return -1;
    }
    fprintf(fp, "%s", cpy);
    fclose(fp);
    return 0;
}

int addLinkGraphData(char* idParent, char* idChild, int type) {
    char linkType[4][7] = {"black ","orchid","orange","cyan3 "};
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
    }
    fseek(fp, -3, SEEK_END);
    fprintf(fp,"\nedge[color=%s];%s->%s;\n\n}",linkType[type],idParent,idChild);
    fclose(fp);
    return 0;
}

int modifyLinkTypeGraphData(const char* idParent, const char* idChild, int newType) {
    char linkType[4][7] = {"black ","orchid","orange","cyan3 "};
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r+");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
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
    return 0;
}

int removeLinkGraphData(const char* idParent, const char* idChild) {
    FILE* fp = NULL;
    fp = fopen(graphDataPath, "r");
    if(fp == NULL) {
        fprintf(stderr, "error : opening of the file %s is impossible\n", graphDataPath);
        fprintf(stderr,"%d\n", errno);
        return -1;
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
        else if(buffer[0] == 'e') {
            if(buffer[20]=='-') edgeChiffre1 = 1;
            else                edgeChiffre1 = 2;

            if(!((buffer[19]==idParent[0]&&(buffer[20]==idParent[1]||(buffer[20]=='-'&&idParent[1]=='\0')))&&(buffer[21+edgeChiffre1]==idChild[0]&&(buffer[22+edgeChiffre1]==idChild[1]||(buffer[22+edgeChiffre1]==';'&&idChild[1]=='\0'))))) {
                strcat(cpy, buffer);
                strcat(cpy, "\n");
            }
        } else if(buffer[0]!='}'&&buffer[0]!='\0') {
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
        return -1;
    }
    fprintf(fp, "%s", cpy);
    fclose(fp);
    return 0;
}
