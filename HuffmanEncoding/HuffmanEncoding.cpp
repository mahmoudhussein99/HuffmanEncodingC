
#include <conio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct ListNode {
    char theCharachter;
    int freq;
    ListNode* next;
    ListNode* previous;
    // required for huffman tree
    ListNode* right;
    ListNode* left;
    ListNode* parent;
    int* code;
};

struct CodeNode {
    char theCharachter;
    char* code;
    CodeNode* next;
    int length;

};

struct CodeNode* codesHead = NULL;
int longestCodeLength=0;


struct ListNode* firstChar;
int linkedListSize = 0;
bool startedScanning = false;



struct ListNode* headOfQueue;
struct ListNode* myAllocator;
int queueSize = 0;
bool huffmanCreation = false;



void addOrIncrement(char c , int &sizeLinkedList) {

    if (!startedScanning) {
        // if it's the first charachter instantiate a new node 
        *firstChar = { c,1,NULL,NULL };
        linkedListSize++;
        startedScanning = true;
    }
    else {
        struct ListNode* currentNode = firstChar;
        struct ListNode previousNode;
        bool addedYet = false;
        //loop until u find the node to increment
        while (currentNode->next != NULL && !addedYet) {

            if (currentNode->theCharachter == c) {
                currentNode->freq++;
                addedYet = true;
            }
            currentNode = currentNode->next;
        }

        if (!addedYet) {
            if (currentNode->theCharachter == c) {
                currentNode->freq++;
                addedYet = true;
            }
            else {
                struct ListNode* newNode = currentNode;
                // goto the next free location and adjust the inputs accordingly
                newNode++;
                newNode->theCharachter = c;
                newNode->freq = 1;
                newNode->next = NULL;
                newNode->previous = currentNode;
                currentNode->next = newNode;
                linkedListSize++;
                addedYet = true;
            }
        }
    }
}

void printList() {
    if (false ){//queueSize==0) {
        return;
    }
    else {
        struct CodeNode* currentNode = codesHead;
        while (currentNode != NULL) {
            printf("char %c & code is:  ", currentNode->theCharachter);
            for (int i = 0; i < currentNode->length;i++) {
            char c = currentNode->code[i];
            printf("%c", currentNode->code[i]);
            }
            printf("\n");
            currentNode = currentNode->next;
        }
        //printf("Number of occurences of %c is %d", currentNode->theCharachter, currentNode->freq);

    }

}

void scanTheFile(char theFile[], int size ) {

    firstChar =new ListNode[size];
    
    for (int i = 0; i < size;i++) {
        char c = theFile[i];
        addOrIncrement( theFile[i], linkedListSize);
        
    }

    
}

void pushToQueue(char c , int f, ListNode* first, ListNode* second) {
    // first Node to push
    if (queueSize == 0) {
        myAllocator->freq = f;
        myAllocator->theCharachter = c;
        myAllocator->next = NULL;
        myAllocator->previous = NULL;
        // get the Allocator to the next position
        headOfQueue = myAllocator;
        queueSize++;
    }
    else {
        struct ListNode* currentNode = headOfQueue;
        struct ListNode* previousNode = NULL;
        bool done = false;
        // find insertion point to keep queue sorted
        while (currentNode != NULL && !done) {
            if (currentNode->freq >f) {
                myAllocator->freq = f;
                myAllocator->theCharachter = c;
                myAllocator->previous = previousNode;
                myAllocator->next = currentNode;

                //let the current Node point to this Node 
                currentNode->previous = myAllocator;
                // if previous node isnt null let it point to the new added node 
                if(previousNode!=NULL)
                    previousNode->next = myAllocator;
                else { // we have just added a node to the beginning of the queue
                    headOfQueue = myAllocator;
                }

                queueSize++;
                done = true;
                break;
            }
            previousNode = currentNode;
            currentNode = currentNode->next;
        }
        // it means that we went out because we reached basically the last node 
        if (!done) {
            myAllocator->freq = f;
            myAllocator->theCharachter = c;
            myAllocator->previous = previousNode;
            myAllocator->next = NULL; // the currently added node is the last one 
            
            if(previousNode!=NULL)
            previousNode->next = myAllocator; // let the previous node point to this node
            queueSize++;

        }

    }

    if (!huffmanCreation) {
        myAllocator->left = NULL;
        myAllocator->right = NULL;
    }
    else {
        myAllocator->left = first;
        myAllocator->right = second;
        first->parent = myAllocator;
        second->parent = myAllocator;
    }

    // push my allocator forward
    myAllocator ++; 

}

void initializeQueue() {
    // at max we will have 2* linked list size tree nodes 
    //struct ListNode ex = { 'c',0,NULL,NULL,NULL,NULL,NULL };
    //myAllocator = &ex;
    myAllocator = new ListNode[2 * linkedListSize];

    struct ListNode* currentNode = firstChar;
    while (currentNode != NULL) {
        pushToQueue(currentNode->theCharachter, currentNode->freq,NULL,NULL);
        currentNode = currentNode->next;
    }
    delete firstChar;
}

ListNode* popFromQueue() {
    if (queueSize == 0) {
        exit(-1);
    }
    struct ListNode* top = headOfQueue; //get the top node 
    headOfQueue = top->next; // move the head pointer forward 
    if(headOfQueue)
    headOfQueue->previous = NULL; // set the previous pointer of the next node to null
    //decrement queue size
    queueSize--;

    // return the top node 
    return top;
}

ListNode* createHuffmanTree() {

    huffmanCreation = true;
    while (queueSize > 1) {
        struct ListNode* first = popFromQueue();
        struct ListNode* second = popFromQueue();


        // set the parent of the current poped nodes to the new node that will be allocated soon
        
        // set the new node left and right to the 2 nodes that are popped right now

        pushToQueue('x', first->freq + second->freq, first,second);
    }


    return headOfQueue;
}

bool isLeaf(struct ListNode* node) {
    return node->left == NULL && node->right == NULL;
}



void addCodeToList(char c, char* str, int size) {
    
    
    if (codesHead == NULL) {
        codesHead = new CodeNode[linkedListSize];
        codesHead->theCharachter = c;
        codesHead->next = NULL;
        codesHead->code = new char[size];
        codesHead->length = size;
        for(int i=0;i<size;i++){
            codesHead->code[i] = str[i];
        }
    }
    else {
        // begin from the start of the list
        struct CodeNode* currentNode = codesHead;
        //navigate to last node
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        //add the node
        struct CodeNode* newNode = currentNode;
        newNode++;
        newNode->theCharachter = c;
        newNode->length = size;
        newNode->next = NULL;
        newNode->code = new char[size];
        for (int i = 0;i < size;i++) {
            newNode->code[i] = str[i];
        }
        currentNode->next = newNode;
    }
}
void calcCodes(struct ListNode* node, char codeArray[], int currentIndx)

{

    // Assign 0 to left edge and call the function again 
    if (node->left) {

        codeArray[currentIndx] = '0';
        calcCodes(node->left, codeArray, currentIndx + 1);
    }

    // Assign 1 to right edge and call urself recursively 
    if (node->right) {

        codeArray[currentIndx] = '1';
        calcCodes(node->right, codeArray, currentIndx + 1);
    }


    if (isLeaf(node)) {
        if (currentIndx > longestCodeLength) {
            longestCodeLength = currentIndx;
        }
        addCodeToList(node->theCharachter, codeArray, currentIndx);
    }
}

struct CodeNode* getCharacterNode(char c) {
    //Start from the beginning
    struct CodeNode* currentNode = codesHead;
    
    //find the specific charcter and return it's code
    while (currentNode->theCharachter != c) {
        currentNode = currentNode->next;
    }
    
    return currentNode;
}



char* getBitStream(char* str, int size) {
    char* stream = new char[size * longestCodeLength]; // at max we will have size*longestCodeLength bits
    stream[0] = '\0'; // empty string
    int j = 0;          //index to null pointer
    for (int i = 0;i < size; i++) {
        struct CodeNode* currentNode = getCharacterNode(str[i]);
        // concatenate stream and the currentCode
        while (stream[j] != '\0') {
            j++;
        }
        int k = 0;
        for(;k<currentNode->length;k++){
            stream[j + k] = currentNode->code[k];
        }
        //set the null pointer
        stream[j + k] = '\0';
        j += k;
    }

    return stream;
}

char* getDecodingStream(char* str, int size) {
    char* stream = new char[size * 8+1]; // at max we will have size*8 bits
    stream[0] = '\0'; // empty string
    int j = 0;       // indx of last null pointer
    for (int i = 0;i < size; i++) {
        int c = str[i]; //get the ascii value of the current character

        // we need to check if it's positive or negative 
        // if it's coded in negative we add to it 256
        if (c < 0) {
            c += 256;
        }
        
        //change the ascii decimal to binary 
        char* currentByte = new char[8];
        for (int x = 7; x >= 0;x--) {
            
            currentByte[x] = (char)('0' + c%2);
            c /= 2;
        }


        // concatenate stream and the currentByte
        while (stream[j] != '\0') {
            j++;
        }
        int k = 0;
        for (;k < 8;k++) {
            stream[j + k] = currentByte[k];
        }
        //set the null pointer
        stream[j + k] = '\0';
        j += k;
    }

    return stream;
}


void printString(char* bitStream) {
    int i = 0;
    while (bitStream[i] != '\0') {
        printf("%c", bitStream[i]);
        i++;
    }
    printf("\n");

}

char encodeChar(char* bitStream, int strt, int end) {
    int multiplicity = 128;
    int value = 0;
    for (int i = strt;i < end;i++) {
        value += (bitStream[i] - '0') * multiplicity;
        multiplicity /= 2;
    }
    return (char)value;
}

char* encode(char* bitStream,int size) {
    char* encodedString = new char[(int)ceil(1.0*size / 8)+1];
    int i = 0;
    for (;i < size;i+=8) { //go 8 by 8
        if (i + 8 < size) {
            char c = encodeChar(bitStream, i, i + 8);
            encodedString[(int)i / 8] = c;
        }
        else {
            char c = encodeChar(bitStream, i, size);
            encodedString[(int)i / 8] = c;

        }
    }
    //add null character to the string to print it for test
    encodedString[(int)i / 8] = '\0';
    return encodedString;

}

char checkForCurrentCode(char* currentCode, bool &found, int length) {
    char c = 'x';
    //Start from the beginning
    struct CodeNode* currentNode = codesHead;

    //find the specific Code and return it's character
    while (currentNode != NULL) {
        //assume that's the one
        found = true;
        for (int i = 0;i < length;i++) {
            
            if (currentCode[i] != currentNode->code[i]) {
                // if !found ste found to false and break
                found = false;
                break;
            }
        }
        if(length!=currentNode->length){
            found = false;
        }
        //if the currentNode has the code of the currentCode
        if (found) {
            c = currentNode->theCharachter;
            break;
        }
        currentNode = currentNode->next;
    }

    return c;
}


char* decode(char* bitStream, int fileSize) {
    char* decodedString = new char[fileSize];
    int charsDecodedYet = 0;
    int indx = 0;
    while (charsDecodedYet < fileSize) {
        char* currentCode = new char[longestCodeLength];
        
        for (int i=0; i < longestCodeLength;i++) {
            currentCode[i] = bitStream[indx];
            indx++;
            bool found = false;
            char c = checkForCurrentCode(currentCode,found,i+1);
            if (found == true) {
                decodedString[charsDecodedYet] = c;
                charsDecodedYet++;
                break;
            }
        }
    }
    //set the null pointer
    decodedString[charsDecodedYet] = '\0';
    return decodedString;
}



int main()
{
    char* str= new char[1000];

#pragma region read from file
    char* reader = str;
    FILE* fptr;
    errno_t err;
    err=fopen_s(&fptr, "testIn.txt", "r");
    if (err!= 0) {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // reads text until EOF is encountered
    int fileSize = 0;
    while (fscanf_s(fptr, "%c", reader)!=EOF) {
        reader++;
        fileSize++;
    }
    //set null pointer
    reader[0] = '\0';
    fclose(fptr);
#pragma endregion


    scanTheFile(str, fileSize);

    initializeQueue();

    createHuffmanTree();

    int indx = 0;
    int  arraySize =  linkedListSize; // maximum we will have  linked_list_size bits each entry
    char* codes= new char[arraySize];
    calcCodes(headOfQueue, codes, indx);
    char* bitStream = getBitStream(str, fileSize);

    //We encode here then write it to the file
    char* encodedString = encode(bitStream,strlen(bitStream));


#pragma region Writing to File


    char* writer = encodedString;
    FILE* wptr;
    err = fopen_s(&wptr, "testOut.txt", "w");
    if (err != 0) {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // write text until '\0' is encountered
    while (*writer!='\0') {
        fprintf_s(fptr, "%c", *writer);
        writer++;
    }
    
    // printf("Data from the file:\n%s", str);
    fclose(wptr);
#pragma endregion


    // we have finished encoding have written everything to the file and we start decoding again
    //+1 to enocde length to encode also the null pointer
    char* decodingStream = getDecodingStream(encodedString,strlen(encodedString)+1);
   

    char* decodedString= decode(decodingStream,fileSize);

    //test after we finish to see if the beginning string as the final decoded one 
    printString(str);
    printString(decodedString);
    
    return 0;
}
