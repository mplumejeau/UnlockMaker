//
// Created by miniya on 16/04/23.
//

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

#include "hpdf.h"

#include "algo.h"
#include "../Model/VertexList.h"
#include "../Model/Card.h"
#include "../Model/EdgeList.h"

/**
 * Run an algorithm to check that there is no loops in the graph structure of a project
 * @param p the project to check
 * @return 1 if there are loops, 0 if there isn't
 */


int iterateLoops(Card* targetCard, Card* currentCard) {
    // Vérification si la carte courante est la carte cible

    // Parcours des liens sortants de la carte courante
    setOnFirstEdge(&currentCard->children);
    while (!isOutOfListEdge(&currentCard->children)) {
        Card* linkedCard = currentCard->children.current->link->child;
        printf("Target : %d ; Pointed : %d\n", targetCard->id, linkedCard->id);
        if (targetCard == linkedCard) {
            return 1; // Boucle détectée
        }
        // Appel récursif pour tester chaque carte enfant
        if (iterateLoops(targetCard, linkedCard) != 0) {
            return 1; // Boucle détectée dans la carte enfant
        }

        // Passage au lien suivant
        setOnNextEdge(&currentCard->children);
    }

    // Aucune boucle trouvée, retourne 0
    return 0;
}

/**
 * Run an algorithm to check that there is no loops in the graph structure of a project
 * @param p the project to check
 * @return 1 if there are loops, 0 if there aren't, -1 if there are errors
 */

int checkLoops(Project* p) {

    if (p != NULL) {

        Card *currentCard = NULL;

        setOnFirstVertex(&p->cardList);
        while (!isOutOfListVertex(&p->cardList)) {
            currentCard = p->cardList.current->card;
            printf("New Target : %d\n", currentCard->id);
            if (iterateLoops(currentCard, currentCard) != 0) {
                return 1;
            }
            setOnNextVertex(&p->cardList);
        }
        return 0;

    } else {
        fprintf(stderr, "Erreur : mauvaise allocation du projet.\n");
        return -1;
    }
}

/**
 * Run an algorithm to automatically assign a number between 1 and 99 to every card that hasn't a number yet
 * Must take in account that several specific numbers may have been already assigned by the user
 * @param p the project
 * @return 0 if it's a success, -1 if not
 */

int assignNumbers(Project* p) {
    int assignedNumbers[MAXCARD] = {0}; // Tableau pour stocker les numéros déjà attribués
    int assignedNumbersIndex = 0; // Indice du tableau assignedNumbers

    // Parcours des liens pour attribuer les numéros aux parents
    setOnFirstEdge(&(p->linkList));
    while (!isOutOfListEdge(&(p->linkList))) {
        Link* currentLink = p->linkList.current->link;
        if (currentLink->type == COMBINE) {
            Card* childCard = currentLink->child;

            // Vérification des parents
            printf("Child Card ID: %d\n", childCard->id);
            setOnFirstEdge(&(childCard->parents));
            while (!isOutOfListEdge(&(childCard->parents))) {
                Link* parentLink = childCard->parents.current->link;
                printf("Parent Link ID: %d\n", parentLink->id);
                if (parentLink->type == COMBINE) {
                    assignedNumbers[assignedNumbersIndex] = parentLink->parent->number;
                    assignedNumbersIndex++;
                }
                setOnNextEdge(&(childCard->parents));
            }

            // Attribution du numéro au child
            int sum = 0;
            for (int i = 0; i < assignedNumbersIndex; i++) {
                sum += assignedNumbers[i];
            }
            childCard->number = sum;

            // Réinitialisation des numéros attribués pour le prochain child
            assignedNumbersIndex = 0;
        }

        setOnNextEdge(&(p->linkList));
    }

    // Parcours de la liste des cartes pour attribuer les numéros restants
    setOnFirstVertex(&(p->cardList));
    int currentNumber = 1;
    while (!isOutOfListVertex(&(p->cardList))) {
        Card* currentCard = p->cardList.current->card;
        if (currentCard->number == -1) {
            while (assignedNumbers[assignedNumbersIndex] != 0) {
                assignedNumbersIndex++;
            }
            currentCard->number = currentNumber + assignedNumbersIndex;
            assignedNumbersIndex++;
        }
        currentNumber++;
        setOnNextVertex(&(p->cardList));
    }
    return 0;
}

/**
 * Create a printable file containing all the formatted cards of a completed project
 * whose name is [projectName].pdf in the /[projectPath] directory
 * If the .pdf file doesn't exist, create it and if it already exists, delete it and create a new one
 * @param p the project
 * @return 0 if it's a success, -1 if not
 */
int createPrintable(Project* p){

    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Image backImage;
    HPDF_Image topImage;
    HPDF_Image bottomImage;
    HPDF_Image cardImageBL;
    HPDF_Image cardImageBR;
    HPDF_Image cardImageTL;
    HPDF_Image cardImageTR;
    HPDF_Font font;

    char mainDirectoryPath[MAXPATH+1+MAXNAME+1];
    char projectDirectoryPath[MAXPATH+1+MAXNAME+1+7+1];
    char cardsDirectoryPathWithoutId[MAXPATH+1+MAXNAME+1+5+1+4+1];
    char pdfFilePath[MAXPATH+1+MAXNAME+1+MAXNAME+4+1];
    char backImagePath[MAXPATH+1+MAXNAME+1+7+1+13+1];
    char topImagePath[MAXPATH+1+MAXNAME+1+7+1+12+1];
    char bottomImagePath[MAXPATH+1+MAXNAME+1+7+1+15+1];
    char cardImagePath[MAXPATH+1+MAXNAME+1+5+1+4+TAILLEMAXID+1+9+1];

    int i;
    int nbPackagesOf4Complete, nbCardsOutOfPackages;
    int nbPagesTotal, currentPageNb;
    char nbPagesString[10];
    Card* currentCardBL = NULL;
    Card* currentCardBR = NULL;
    Card* currentCardTL = NULL;
    Card* currentCardTR = NULL;
    char idString[TAILLEMAXID];
    int nbCardBL, nbCardBR, nbCardTL, nbCardTR;
    char nbCardString[3];
    float correctionXCardNumber;

    // resolution of the pdf

    const float RESOLUTION_IN_DPI = 72;
    const float SIZE_OF_AN_INCH_IN_CM = 2.54;
    const float RESOLUTION_IN_CM = RESOLUTION_IN_DPI/SIZE_OF_AN_INCH_IN_CM;

    // size of an A4 page in pixels

    const float A4_PAGE_WIDTH = RESOLUTION_IN_CM*21;
    const float A4_PAGE_HEIGHT = RESOLUTION_IN_CM*29.7;

    // font type and sizes

    const char* FONT_TYPE = "Times-Bold";
    const int SIZE_FONT_CARD_NUMBER = 60;
    const int SIZE_FONT_CARD_NUMBER_SMALL = 30;
    const int SIZE_FONT_PAGE_NUMBER = 20;

    // size of margins in pixels

    const float HORIZONTAL_SIDE_MARGIN = RESOLUTION_IN_CM*3;
    const float HORIZONTAL_CENTER_MARGIN = RESOLUTION_IN_CM*2.8;
    const float VERTICAL_SIDE_MARGIN = RESOLUTION_IN_CM*2.5;
    const float VERTICAL_CENTER_MARGIN = RESOLUTION_IN_CM*2.3;
    const float HORIZONTAL_FONT_PAGE_NUMBER_MARGIN = RESOLUTION_IN_CM*2;
    const float VERTICAL_FONT_PAGE_NUMBER_MARGIN = RESOLUTION_IN_CM*1;
    const float HORIZONTAL_FONT_CARD_NUMBER_SMALL_MARGIN = RESOLUTION_IN_CM*0.5;
    const float VERTICAL_FONT_CARD_NUMBER_SMALL_MARGIN = RESOLUTION_IN_CM*0.5;

    // size of images in pixels

    const float BACK_IMAGE_WIDTH = RESOLUTION_IN_CM*6.1;
    const float BACK_IMAGE_HEIGHT = RESOLUTION_IN_CM*11.2;
    const float TOP_IMAGE_WIDTH = BACK_IMAGE_WIDTH;
    const float TOP_IMAGE_HEIGHT = RESOLUTION_IN_CM*1.7;
    const float BOTTOM_IMAGE_WIDTH = BACK_IMAGE_WIDTH;
    const float BOTTOM_IMAGE_HEIGHT = RESOLUTION_IN_CM*2.2;
    const float CARD_IMAGE_WIDTH = BACK_IMAGE_WIDTH;
    const float CARD_IMAGE_HEIGHT = BACK_IMAGE_HEIGHT - TOP_IMAGE_HEIGHT - BOTTOM_IMAGE_HEIGHT;

    // position of images in pixels (BL is for Bottom-Left, BR for Bottom-Right, etc)

    // Bottom-Left image

    const float X_BL_BOTTOM_IMAGE = HORIZONTAL_SIDE_MARGIN;
    const float Y_BL_BOTTOM_IMAGE = VERTICAL_SIDE_MARGIN;
    const float X_BL_CARD_IMAGE = X_BL_BOTTOM_IMAGE;
    const float Y_BL_CARD_IMAGE = Y_BL_BOTTOM_IMAGE + BOTTOM_IMAGE_HEIGHT;
    const float X_BL_TOP_IMAGE = X_BL_BOTTOM_IMAGE;
    const float Y_BL_TOP_IMAGE = Y_BL_CARD_IMAGE + CARD_IMAGE_HEIGHT;
    const float X_BL_BACK_IMAGE = X_BL_BOTTOM_IMAGE;
    const float Y_BL_BACK_IMAGE = Y_BL_BOTTOM_IMAGE;
    const float X_BL_FONT_NUMBER_INIT = X_BL_BACK_IMAGE + BACK_IMAGE_WIDTH/2;
    const float Y_BL_FONT_NUMBER_INIT = Y_BL_BACK_IMAGE + BACK_IMAGE_HEIGHT/2 - SIZE_FONT_CARD_NUMBER/3;
    const float X_BL_FONT_NUMBER_SMALL_INIT = X_BL_TOP_IMAGE + SIZE_FONT_CARD_NUMBER_SMALL/2;
    const float Y_BL_FONT_NUMBER_SMALL_INIT = Y_BL_TOP_IMAGE + TOP_IMAGE_HEIGHT/2 - SIZE_FONT_CARD_NUMBER_SMALL/3;

    // Bottom-Right image

    const float X_BR_BOTTOM_IMAGE = A4_PAGE_WIDTH - HORIZONTAL_SIDE_MARGIN - BOTTOM_IMAGE_WIDTH;
    const float Y_BR_BOTTOM_IMAGE = Y_BL_BOTTOM_IMAGE;
    const float X_BR_CARD_IMAGE = X_BR_BOTTOM_IMAGE;
    const float Y_BR_CARD_IMAGE = Y_BR_BOTTOM_IMAGE + BOTTOM_IMAGE_HEIGHT;
    const float X_BR_TOP_IMAGE = X_BR_BOTTOM_IMAGE;
    const float Y_BR_TOP_IMAGE = Y_BR_CARD_IMAGE + CARD_IMAGE_HEIGHT;
    const float X_BR_BACK_IMAGE = X_BR_BOTTOM_IMAGE;
    const float Y_BR_BACK_IMAGE = Y_BR_BOTTOM_IMAGE;
    const float X_BR_FONT_NUMBER_INIT = X_BR_BACK_IMAGE + BACK_IMAGE_WIDTH/2;
    const float Y_BR_FONT_NUMBER_INIT = Y_BR_BACK_IMAGE + BACK_IMAGE_HEIGHT/2 - SIZE_FONT_CARD_NUMBER/3;
    const float X_BR_FONT_NUMBER_SMALL_INIT = X_BR_TOP_IMAGE + SIZE_FONT_CARD_NUMBER_SMALL/2;
    const float Y_BR_FONT_NUMBER_SMALL_INIT = Y_BR_TOP_IMAGE + TOP_IMAGE_HEIGHT/2 - SIZE_FONT_CARD_NUMBER_SMALL/3;

    // Top-Left image

    const float X_TL_TOP_IMAGE = X_BL_BOTTOM_IMAGE;
    const float Y_TL_TOP_IMAGE = A4_PAGE_HEIGHT - VERTICAL_SIDE_MARGIN - TOP_IMAGE_HEIGHT;
    const float X_TL_CARD_IMAGE = X_TL_TOP_IMAGE;
    const float Y_TL_CARD_IMAGE = Y_TL_TOP_IMAGE - CARD_IMAGE_HEIGHT;
    const float X_TL_BOTTOM_IMAGE = X_TL_TOP_IMAGE;
    const float Y_TL_BOTTOM_IMAGE = Y_TL_CARD_IMAGE - BOTTOM_IMAGE_HEIGHT;
    const float X_TL_BACK_IMAGE = X_TL_BOTTOM_IMAGE;
    const float Y_TL_BACK_IMAGE = Y_TL_BOTTOM_IMAGE;
    const float X_TL_FONT_NUMBER_INIT = X_TL_BACK_IMAGE + BACK_IMAGE_WIDTH/2;
    const float Y_TL_FONT_NUMBER_INIT = Y_TL_BACK_IMAGE + BACK_IMAGE_HEIGHT/2 - SIZE_FONT_CARD_NUMBER/3;
    const float X_TL_FONT_NUMBER_SMALL_INIT = X_TL_TOP_IMAGE + SIZE_FONT_CARD_NUMBER_SMALL/2;
    const float Y_TL_FONT_NUMBER_SMALL_INIT = Y_TL_TOP_IMAGE + TOP_IMAGE_HEIGHT/2 - SIZE_FONT_CARD_NUMBER_SMALL/3;

    // Top-Right image

    const float X_TR_TOP_IMAGE = X_BR_TOP_IMAGE;
    const float Y_TR_TOP_IMAGE = Y_TL_TOP_IMAGE;
    const float X_TR_CARD_IMAGE = X_TR_TOP_IMAGE;
    const float Y_TR_CARD_IMAGE = Y_TR_TOP_IMAGE - CARD_IMAGE_HEIGHT;
    const float X_TR_BOTTOM_IMAGE = X_TR_TOP_IMAGE;
    const float Y_TR_BOTTOM_IMAGE = Y_TR_CARD_IMAGE - BOTTOM_IMAGE_HEIGHT;
    const float X_TR_BACK_IMAGE = X_TR_BOTTOM_IMAGE;
    const float Y_TR_BACK_IMAGE = Y_TR_BOTTOM_IMAGE;
    const float X_TR_FONT_NUMBER_INIT = X_TR_BACK_IMAGE + BACK_IMAGE_WIDTH/2;
    const float Y_TR_FONT_NUMBER_INIT = Y_TR_BACK_IMAGE + BACK_IMAGE_HEIGHT/2 - SIZE_FONT_CARD_NUMBER/3;
    const float X_TR_FONT_NUMBER_SMALL_INIT = X_TR_TOP_IMAGE + SIZE_FONT_CARD_NUMBER_SMALL/2;
    const float Y_TR_FONT_NUMBER_SMALL_INIT = Y_TR_TOP_IMAGE + TOP_IMAGE_HEIGHT/2 - SIZE_FONT_CARD_NUMBER_SMALL/3;

    // position of page number in pixels

    const float X_FONT_PAGE_NUMBER = A4_PAGE_WIDTH - HORIZONTAL_FONT_PAGE_NUMBER_MARGIN;
    const float Y_FONT_PAGE_NUMBER = VERTICAL_FONT_PAGE_NUMBER_MARGIN;

    // testing the correct allocation of the project

    if(p == NULL){
        fprintf(stderr, "error : project bad allocation\n");
        return -1;
    }

    // testing the presence of images for the project

    if(p->backImage != 1){
        fprintf(stderr, "error : project has no back image\n");
        return -1;
    }
    if(p->topImage != 1){
        fprintf(stderr, "error : project has no top image\n");
        return -1;
    }
    if(p->bottomImage != 1){
        fprintf(stderr, "error : project has no bottom image\n");
        return -1;
    }

    // testing the presence of images for each card

    setOnFirstVertex(&p->cardList);
    while(!isOutOfListVertex(&p->cardList)){
        if(p->cardList.current->card->image != 1){
            fprintf(stderr, "error : card with id : %d has no image\n", p->cardList.current->card->id);
            return -1;
        }
        setOnNextVertex(&p->cardList);
    }

    // creation of the directories and files paths

    //first directory : [projectPath]/[projectName]

    strncpy(mainDirectoryPath, p->path, MAXPATH);
    strcat(mainDirectoryPath,"/");
    strncat(mainDirectoryPath, p->name, MAXNAME);

    //second directory : [projectPath]/[projectName]/Project

    strncpy(projectDirectoryPath, mainDirectoryPath, MAXPATH+1+MAXNAME+1);
    strcat(projectDirectoryPath, "/Project");

    //third directory : [projectPath]/[projectName]/Cards

    strncpy(cardsDirectoryPathWithoutId, mainDirectoryPath, MAXPATH+1+MAXNAME+1);
    strcat(cardsDirectoryPathWithoutId, "/Cards/Card");

    //project printable file

    strncpy(pdfFilePath, mainDirectoryPath, MAXPATH+1+MAXNAME+1);
    strcat(pdfFilePath,"/");
    strncat(pdfFilePath, p->name, MAXNAME);
    strcat(pdfFilePath,".pdf");

    //project images

    strncpy(backImagePath, projectDirectoryPath, MAXPATH+1+MAXNAME+1+7+1);
    strcat(backImagePath, "/BackImage.jpg");

    strncpy(topImagePath, projectDirectoryPath, MAXPATH+1+MAXNAME+1+7+1);
    strcat(topImagePath, "/TopImage.jpg");

    strncpy(bottomImagePath, projectDirectoryPath, MAXPATH+1+MAXNAME+1+7+1);
    strcat(bottomImagePath, "/BottomImage.jpg");

    // calculation of important variables for the algorithm

    nbPackagesOf4Complete = p->nbCards/4;
    nbCardsOutOfPackages = p->nbCards%4;

    if(nbCardsOutOfPackages == 0){
        nbPagesTotal = nbPackagesOf4Complete*2;
    } else {
        nbPagesTotal = nbPackagesOf4Complete*2 + 2;
    }

    currentPageNb = 0;

    // creation of the pdf

    pdf = HPDF_New(NULL,NULL);
    if(!pdf){
        fprintf(stderr, "error : impossible to create the pdf file\n");
        HPDF_Free(pdf);
        return -1;
    }

    // setting attributes of the pdf

    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
    font = HPDF_GetFont(pdf, FONT_TYPE, NULL);

    // filling the pdf with complete pages

    setOnFirstVertex(&p->cardList);

    for(i=0;i<nbPackagesOf4Complete;i++){

        // page with the front of the cards

        // creation of a page

        page = HPDF_AddPage(pdf);
        if(!page){
            fprintf(stderr, "error : impossible to add a new page to the pdf file\n");
            HPDF_Free(pdf);
            return -1;
        }
        currentPageNb ++;

        // setting attributes of the page

        HPDF_Page_SetWidth(page, A4_PAGE_WIDTH);
        HPDF_Page_SetHeight(page, A4_PAGE_HEIGHT);

        // addition of top images of the 4 cards in the page

        topImage = HPDF_LoadJpegImageFromFile(pdf, topImagePath);
        if(topImage == NULL){
            fprintf(stderr, "error : impossible to load the top image file\n");
            HPDF_Free(pdf);
            return -1;
        }
        HPDF_Page_DrawImage(page,  topImage, X_BL_TOP_IMAGE, Y_BL_TOP_IMAGE,
                            TOP_IMAGE_WIDTH, TOP_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  topImage, X_BR_TOP_IMAGE, Y_BR_TOP_IMAGE,
                            TOP_IMAGE_WIDTH, TOP_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  topImage, X_TL_TOP_IMAGE, Y_TL_TOP_IMAGE,
                            TOP_IMAGE_WIDTH, TOP_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  topImage, X_TR_TOP_IMAGE, Y_TR_TOP_IMAGE,
                            TOP_IMAGE_WIDTH, TOP_IMAGE_HEIGHT);

        // addition of bottom images of the 4 cards in the page

        bottomImage = HPDF_LoadJpegImageFromFile(pdf, bottomImagePath);
        if(bottomImage == NULL){
            fprintf(stderr, "error : impossible to load the bottom image file\n");
            HPDF_Free(pdf);
            return -1;
        }
        HPDF_Page_DrawImage(page,  bottomImage, X_BL_BOTTOM_IMAGE, Y_BL_BOTTOM_IMAGE,
                            BOTTOM_IMAGE_WIDTH, BOTTOM_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  bottomImage, X_BR_BOTTOM_IMAGE, Y_BR_BOTTOM_IMAGE,
                            BOTTOM_IMAGE_WIDTH, BOTTOM_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  bottomImage, X_TL_BOTTOM_IMAGE, Y_TL_BOTTOM_IMAGE,
                            BOTTOM_IMAGE_WIDTH, BOTTOM_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  bottomImage, X_TR_BOTTOM_IMAGE, Y_TR_BOTTOM_IMAGE,
                            BOTTOM_IMAGE_WIDTH, BOTTOM_IMAGE_HEIGHT);

        // addition of card image of the BL card in the page

        currentCardBL = p->cardList.current->card;
        nbCardBL = currentCardBL->number;

        // creation of the path of the current card

        strncpy(cardImagePath, cardsDirectoryPathWithoutId, MAXPATH+1+MAXNAME+1+5+1+4+1);
        sprintf(idString, "%d", currentCardBL->id);
        strncat(cardImagePath, idString, TAILLEMAXID);
        strcat(cardImagePath, "/Image.jpg");

        cardImageBL = HPDF_LoadJpegImageFromFile(pdf, cardImagePath);
        if(cardImageBL == NULL){
            fprintf(stderr, "error : impossible to load the image file for the card with id : %s\n", idString);
            HPDF_Free(pdf);
            return -1;
        }
        HPDF_Page_DrawImage(page, cardImageBL, X_BL_CARD_IMAGE, Y_BL_CARD_IMAGE,
                            CARD_IMAGE_WIDTH, CARD_IMAGE_HEIGHT);

        // addition of small card number of the BL card

        sprintf(nbCardString, "%d", nbCardBL);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER_SMALL);
        HPDF_Page_MoveTextPos(page, X_BL_FONT_NUMBER_SMALL_INIT, Y_BL_FONT_NUMBER_SMALL_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        setOnNextVertex(&p->cardList);

        // addition of card image of the BR card in the page

        currentCardBR = p->cardList.current->card;
        nbCardBR = currentCardBR->number;

        // creation of the path of the current card

        strncpy(cardImagePath, cardsDirectoryPathWithoutId, MAXPATH+1+MAXNAME+1+5+1+4+1);
        sprintf(idString, "%d", currentCardBR->id);
        strncat(cardImagePath, idString, TAILLEMAXID);
        strcat(cardImagePath, "/Image.jpg");

        cardImageBR = HPDF_LoadJpegImageFromFile(pdf, cardImagePath);
        if(cardImageBR == NULL){
            fprintf(stderr, "error : impossible to load the image file for the card with id : %s\n", idString);
            HPDF_Free(pdf);
            return -1;
        }
        HPDF_Page_DrawImage(page, cardImageBR, X_BR_CARD_IMAGE, Y_BR_CARD_IMAGE,
                            CARD_IMAGE_WIDTH, CARD_IMAGE_HEIGHT);

        // addition of small card number of the BR card

        sprintf(nbCardString, "%d", nbCardBR);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER_SMALL);
        HPDF_Page_MoveTextPos(page, X_BR_FONT_NUMBER_SMALL_INIT, Y_BR_FONT_NUMBER_SMALL_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        setOnNextVertex(&p->cardList);

        // addition of card image of the TL card in the page

        currentCardTL = p->cardList.current->card;
        nbCardTL = currentCardTL->number;

        // creation of the path of the current card

        strncpy(cardImagePath, cardsDirectoryPathWithoutId, MAXPATH+1+MAXNAME+1+5+1+4+1);
        sprintf(idString, "%d", currentCardTL->id);
        strncat(cardImagePath, idString, TAILLEMAXID);
        strcat(cardImagePath, "/Image.jpg");

        cardImageTL = HPDF_LoadJpegImageFromFile(pdf, cardImagePath);
        if(cardImageTL == NULL){
            fprintf(stderr, "error : impossible to load the image file for the card with id : %s\n", idString);
            HPDF_Free(pdf);
            return -1;
        }
        HPDF_Page_DrawImage(page, cardImageTL, X_TL_CARD_IMAGE, Y_TL_CARD_IMAGE,
                            CARD_IMAGE_WIDTH, CARD_IMAGE_HEIGHT);

        // addition of small card number of the TL card

        sprintf(nbCardString, "%d", nbCardTL);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER_SMALL);
        HPDF_Page_MoveTextPos(page, X_TL_FONT_NUMBER_SMALL_INIT, Y_TL_FONT_NUMBER_SMALL_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        setOnNextVertex(&p->cardList);

        // addition of card image of the TR card in the page

        currentCardTR = p->cardList.current->card;
        nbCardTR = currentCardTR->number;

        // creation of the path of the current card

        strncpy(cardImagePath, cardsDirectoryPathWithoutId, MAXPATH+1+MAXNAME+1+5+1+4+1);
        sprintf(idString, "%d", currentCardTR->id);
        strncat(cardImagePath, idString, TAILLEMAXID);
        strcat(cardImagePath, "/Image.jpg");

        cardImageTR = HPDF_LoadJpegImageFromFile(pdf, cardImagePath);
        if(cardImageTR == NULL){
            fprintf(stderr, "error : impossible to load the image file for the card with id : %s\n", idString);
            HPDF_Free(pdf);
            return -1;
        }
        HPDF_Page_DrawImage(page, cardImageTR, X_TR_CARD_IMAGE, Y_TR_CARD_IMAGE,
                            CARD_IMAGE_WIDTH, CARD_IMAGE_HEIGHT);

        // addition of small card number of the TR card

        sprintf(nbCardString, "%d", nbCardTR);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER_SMALL);
        HPDF_Page_MoveTextPos(page, X_TR_FONT_NUMBER_SMALL_INIT, Y_TR_FONT_NUMBER_SMALL_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        setOnNextVertex(&p->cardList);

        // addition of the page number

        sprintf(nbPagesString, "%d/%d", currentPageNb, nbPagesTotal);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_PAGE_NUMBER);
        HPDF_Page_MoveTextPos(page, X_FONT_PAGE_NUMBER, Y_FONT_PAGE_NUMBER);
        HPDF_Page_ShowText(page, nbPagesString);
        HPDF_Page_EndText(page);

        // page with the back of the cards

        // creation of a page

        page = HPDF_AddPage(pdf);
        if(!page){
            fprintf(stderr, "error : impossible to add a new page to the pdf file\n");
            HPDF_Free(pdf);
            return -1;
        }
        currentPageNb ++;

        // setting attributes of the page

        HPDF_Page_SetWidth(page, A4_PAGE_WIDTH);
        HPDF_Page_SetHeight(page, A4_PAGE_HEIGHT);

        // addition of back images of the 4 cards in the page

        backImage = HPDF_LoadJpegImageFromFile(pdf, backImagePath);
        if(backImage == NULL){
            fprintf(stderr, "error : impossible to load the back image file\n");
            HPDF_Free(pdf);
            return -1;
        }
        HPDF_Page_DrawImage(page,  backImage, X_BL_BACK_IMAGE, Y_BL_BACK_IMAGE,
                            BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  backImage, X_BR_BACK_IMAGE, Y_BR_BACK_IMAGE,
                            BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  backImage, X_TL_BACK_IMAGE, Y_TL_BACK_IMAGE,
                            BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT);
        HPDF_Page_DrawImage(page,  backImage, X_TR_BACK_IMAGE, Y_TR_BACK_IMAGE,
                            BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT);

        // addition of the cards number (inversion in right and left in order to fit front and back when printing in recto-verso mode)

        // BL card number

        sprintf(nbCardString, "%d", nbCardBL);
        if(nbCardBL>=0 && nbCardBL<10){
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/4;
        } else {
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/2;
        }

        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER);
        // inversion in right and left in order to fit front and back when printing in recto-verso mode
        HPDF_Page_MoveTextPos(page, X_BR_FONT_NUMBER_INIT - correctionXCardNumber, Y_BR_FONT_NUMBER_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        // BR card number

        sprintf(nbCardString, "%d", nbCardBR);
        if(nbCardBR>=0 && nbCardBR<10){
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/4;
        } else {
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/2;
        }

        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER);
        // inversion in right and left in order to fit front and back when printing in recto-verso mode
        HPDF_Page_MoveTextPos(page, X_BL_FONT_NUMBER_INIT - correctionXCardNumber, Y_BL_FONT_NUMBER_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        // TL card number

        sprintf(nbCardString, "%d", nbCardTL);
        if(nbCardTL>=0 && nbCardTL<10){
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/4;
        } else {
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/2;
        }

        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER);
        // inversion in right and left in order to fit front and back when printing in recto-verso mode
        HPDF_Page_MoveTextPos(page, X_TR_FONT_NUMBER_INIT - correctionXCardNumber, Y_TR_FONT_NUMBER_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        // TR card number

        sprintf(nbCardString, "%d", nbCardTR);
        if(nbCardTR>=0 && nbCardTR<10){
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/4;
        } else {
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/2;
        }

        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER);
        // inversion in right and left in order to fit front and back when printing in recto-verso mode
        HPDF_Page_MoveTextPos(page, X_TL_FONT_NUMBER_INIT - correctionXCardNumber, Y_TL_FONT_NUMBER_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        // addition of the page number

        sprintf(nbPagesString, "%d/%d", currentPageNb, nbPagesTotal);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_PAGE_NUMBER);
        HPDF_Page_MoveTextPos(page, X_FONT_PAGE_NUMBER, Y_FONT_PAGE_NUMBER);
        HPDF_Page_ShowText(page, nbPagesString);
        HPDF_Page_EndText(page);

    }

    // filling the pdf with the non-complete page

    if(nbCardsOutOfPackages >= 1){

        // page with the front of the cards

        // creation of a page

        page = HPDF_AddPage(pdf);
        if(!page){
            fprintf(stderr, "error : impossible to add a new page to the pdf file\n");
            HPDF_Free(pdf);
            return -1;
        }
        currentPageNb ++;

        // setting attributes of the page

        HPDF_Page_SetWidth(page, A4_PAGE_WIDTH);
        HPDF_Page_SetHeight(page, A4_PAGE_HEIGHT);

        // addition of top image of the TL card in the page

        HPDF_Page_DrawImage(page, topImage, X_TL_TOP_IMAGE, Y_TL_TOP_IMAGE,
                            TOP_IMAGE_WIDTH, TOP_IMAGE_HEIGHT);

        // addition of bottom image of the TL card in the page

        HPDF_Page_DrawImage(page, bottomImage, X_TL_BOTTOM_IMAGE, Y_TL_BOTTOM_IMAGE,
                            BOTTOM_IMAGE_WIDTH, BOTTOM_IMAGE_HEIGHT);

        // addition of card image of the TL card in the page

        currentCardTL = p->cardList.current->card;
        nbCardTL = currentCardTL->number;

        // creation of the path of the current card

        strncpy(cardImagePath, cardsDirectoryPathWithoutId, MAXPATH+1+MAXNAME+1+5+1+4+1);
        sprintf(idString, "%d", currentCardTL->id);
        strncat(cardImagePath, idString, TAILLEMAXID);
        strcat(cardImagePath, "/Image.jpg");

        cardImageTL = HPDF_LoadJpegImageFromFile(pdf, cardImagePath);
        if(cardImageTL == NULL){
            fprintf(stderr, "error : impossible to load the image file for the card with id : %s\n", idString);
            HPDF_Free(pdf);
            return -1;
        }
        HPDF_Page_DrawImage(page, cardImageTL, X_TL_CARD_IMAGE, Y_TL_CARD_IMAGE,
                            CARD_IMAGE_WIDTH, CARD_IMAGE_HEIGHT);

        // addition of small card number of the TL card

        sprintf(nbCardString, "%d", nbCardTL);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER_SMALL);
        HPDF_Page_MoveTextPos(page, X_TL_FONT_NUMBER_SMALL_INIT, Y_TL_FONT_NUMBER_SMALL_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        setOnNextVertex(&p->cardList);

        if(nbCardsOutOfPackages >=2){

            // addition of top image of the TR card in the page

            HPDF_Page_DrawImage(page,  topImage, X_TR_TOP_IMAGE, Y_TR_TOP_IMAGE,
                                TOP_IMAGE_WIDTH, TOP_IMAGE_HEIGHT);

            // addition of bottom image of the TR card in the page

            HPDF_Page_DrawImage(page,  bottomImage, X_TR_BOTTOM_IMAGE, Y_TR_BOTTOM_IMAGE,
                                BOTTOM_IMAGE_WIDTH, BOTTOM_IMAGE_HEIGHT);

            // addition of card image of the TR card in the page

            currentCardTR = p->cardList.current->card;
            nbCardTR = currentCardTR->number;

            // creation of the path of the current card

            strncpy(cardImagePath, cardsDirectoryPathWithoutId, MAXPATH+1+MAXNAME+1+5+1+4+1);
            sprintf(idString, "%d", currentCardTR->id);
            strncat(cardImagePath, idString, TAILLEMAXID);
            strcat(cardImagePath, "/Image.jpg");

            cardImageTR = HPDF_LoadJpegImageFromFile(pdf, cardImagePath);
            if(cardImageTR == NULL){
                fprintf(stderr, "error : impossible to load the image file for the card with id : %s\n", idString);
                HPDF_Free(pdf);
                return -1;
            }
            HPDF_Page_DrawImage(page, cardImageTR, X_TR_CARD_IMAGE, Y_TR_CARD_IMAGE,
                                CARD_IMAGE_WIDTH, CARD_IMAGE_HEIGHT);

            // addition of small card number of the TR card

            sprintf(nbCardString, "%d", nbCardTR);
            HPDF_Page_BeginText(page);
            HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER_SMALL);
            HPDF_Page_MoveTextPos(page, X_TR_FONT_NUMBER_SMALL_INIT, Y_TR_FONT_NUMBER_SMALL_INIT);
            HPDF_Page_ShowText(page, nbCardString);
            HPDF_Page_EndText(page);

            setOnNextVertex(&p->cardList);

        }

        if(nbCardsOutOfPackages >= 3){

            // addition of top image of the BL card in the page

            HPDF_Page_DrawImage(page, topImage, X_BL_TOP_IMAGE, Y_BL_TOP_IMAGE,
                                TOP_IMAGE_WIDTH, TOP_IMAGE_HEIGHT);

            // addition of bottom image of the BL card in the page

            HPDF_Page_DrawImage(page, bottomImage, X_BL_BOTTOM_IMAGE, Y_BL_BOTTOM_IMAGE,
                                BOTTOM_IMAGE_WIDTH, BOTTOM_IMAGE_HEIGHT);

            // addition of card image of the BL card in the page

            currentCardBL = p->cardList.current->card;
            nbCardBL = currentCardBL->number;

            // creation of the path of the current card

            strncpy(cardImagePath, cardsDirectoryPathWithoutId, MAXPATH+1+MAXNAME+1+5+1+4+1);
            sprintf(idString, "%d", currentCardBL->id);
            strncat(cardImagePath, idString, TAILLEMAXID);
            strcat(cardImagePath, "/Image.jpg");

            cardImageBL = HPDF_LoadJpegImageFromFile(pdf, cardImagePath);
            if(cardImageBL == NULL){
                fprintf(stderr, "error : impossible to load the image file for the card with id : %s\n", idString);
                HPDF_Free(pdf);
                return -1;
            }
            HPDF_Page_DrawImage(page, cardImageBL, X_BL_CARD_IMAGE, Y_BL_CARD_IMAGE,
                                CARD_IMAGE_WIDTH, CARD_IMAGE_HEIGHT);

            // addition of small card number of the BL card

            sprintf(nbCardString, "%d", nbCardBL);
            HPDF_Page_BeginText(page);
            HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER_SMALL);
            HPDF_Page_MoveTextPos(page, X_BL_FONT_NUMBER_SMALL_INIT, Y_BL_FONT_NUMBER_SMALL_INIT);
            HPDF_Page_ShowText(page, nbCardString);
            HPDF_Page_EndText(page);

            setOnNextVertex(&p->cardList);

        }

        // addition of the page number

        sprintf(nbPagesString, "%d/%d", currentPageNb, nbPagesTotal);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_PAGE_NUMBER);
        HPDF_Page_MoveTextPos(page, X_FONT_PAGE_NUMBER, Y_FONT_PAGE_NUMBER);
        HPDF_Page_ShowText(page, nbPagesString);
        HPDF_Page_EndText(page);

        // page with the back of the cards

        // creation of a page

        page = HPDF_AddPage(pdf);
        if(!page){
            fprintf(stderr, "error : impossible to add a new page to the pdf file\n");
            HPDF_Free(pdf);
            return -1;
        }
        currentPageNb ++;

        // setting attributes of the page

        HPDF_Page_SetWidth(page, A4_PAGE_WIDTH);
        HPDF_Page_SetHeight(page, A4_PAGE_HEIGHT);

        // addition of back images of the TL card in the page
        // inversion in right and left in order to fit front and back when printing in recto-verso mode

        HPDF_Page_DrawImage(page,  backImage, X_TR_BACK_IMAGE, Y_TR_BACK_IMAGE,
                            BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT);

        // addition of the TL card number (inversion in right and left in order to fit front and back when printing in recto-verso mode)

        sprintf(nbCardString, "%d", nbCardTL);
        if(nbCardTL>=0 && nbCardTL<10){
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/4;
        } else {
            correctionXCardNumber = SIZE_FONT_CARD_NUMBER/2;
        }

        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER);
        // inversion in right and left in order to fit front and back when printing in recto-verso mode
        HPDF_Page_MoveTextPos(page, X_TR_FONT_NUMBER_INIT - correctionXCardNumber, Y_TR_FONT_NUMBER_INIT);
        HPDF_Page_ShowText(page, nbCardString);
        HPDF_Page_EndText(page);

        if(nbCardsOutOfPackages >= 2){

            // addition of back images of the TR card in the page
            // inversion in right and left in order to fit front and back when printing in recto-verso mode

            HPDF_Page_DrawImage(page,  backImage, X_TL_BACK_IMAGE, Y_TL_BACK_IMAGE,
                                BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT);

            // addition of the TR card number (inversion in right and left in order to fit front and back when printing in recto-verso mode)

            sprintf(nbCardString, "%d", nbCardTR);
            if(nbCardTR>=0 && nbCardTR<10){
                correctionXCardNumber = SIZE_FONT_CARD_NUMBER/4;
            } else {
                correctionXCardNumber = SIZE_FONT_CARD_NUMBER/2;
            }

            HPDF_Page_BeginText(page);
            HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER);
            // inversion in right and left in order to fit front and back when printing in recto-verso mode
            HPDF_Page_MoveTextPos(page, X_TL_FONT_NUMBER_INIT - correctionXCardNumber, Y_TL_FONT_NUMBER_INIT);
            HPDF_Page_ShowText(page, nbCardString);
            HPDF_Page_EndText(page);

        }

        if(nbCardsOutOfPackages >= 3){

            // addition of back images of the BL card in the page
            // inversion in right and left in order to fit front and back when printing in recto-verso mode

            HPDF_Page_DrawImage(page,  backImage, X_BR_BACK_IMAGE, Y_BR_BACK_IMAGE,
                                BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT);

            // addition of the TL card number (inversion in right and left in order to fit front and back when printing in recto-verso mode)

            sprintf(nbCardString, "%d", nbCardBL);
            if(nbCardBL>=0 && nbCardBL<10){
                correctionXCardNumber = SIZE_FONT_CARD_NUMBER/4;
            } else {
                correctionXCardNumber = SIZE_FONT_CARD_NUMBER/2;
            }

            HPDF_Page_BeginText(page);
            HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_CARD_NUMBER);
            // inversion in right and left in order to fit front and back when printing in recto-verso mode
            HPDF_Page_MoveTextPos(page, X_BR_FONT_NUMBER_INIT - correctionXCardNumber, Y_BR_FONT_NUMBER_INIT);
            HPDF_Page_ShowText(page, nbCardString);
            HPDF_Page_EndText(page);

        }

        // addition of the page number

        sprintf(nbPagesString, "%d/%d", currentPageNb, nbPagesTotal);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, SIZE_FONT_PAGE_NUMBER);
        HPDF_Page_MoveTextPos(page, X_FONT_PAGE_NUMBER, Y_FONT_PAGE_NUMBER);
        HPDF_Page_ShowText(page, nbPagesString);
        HPDF_Page_EndText(page);

    }

    // remove precedent pdf file if it existed

    if(remove(pdfFilePath) != 0){
        fprintf( stderr, "error : deletion of the file %s is impossible\n", pdfFilePath);
        fprintf(stderr,"%d\n", errno);
    }

    // saving pdf file

    if(HPDF_SaveToFile(pdf, pdfFilePath) != HPDF_OK){
        fprintf( stderr, "error : creation of the file %s is impossible\n", pdfFilePath);
        fprintf(stderr,"%d\n", errno);
        HPDF_Free(pdf);
        return -1;
    }

    // freeing the resources of the pdf

    HPDF_Free(pdf);

    return 0;
}
