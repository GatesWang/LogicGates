#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

typedef struct numst {
    char name[80];
    int value;
    struct numst * next;
}

numst;
typedef struct gatest {
  char operator[15];
  char operand1[20];
  char operand2[20];
  char operand3[100];
  char operands[100];
  struct gatest * next;
}

gatest;
numst * addNum(numst * head, char * name) {
	numst * new;
  if (head == NULL) {
  	new = malloc(sizeof(numst));
    strcpy(new->name, name);
    new->next = NULL;
    return new;
  }

  numst * currentNum = head;
  while (currentNum->next != NULL) {
  	currentNum = currentNum->next;
  }
  new = malloc(sizeof(numst));
  strcpy(new->name, name);
  new->next = NULL;
  currentNum->next = new;
  return head;
}
gatest * addGate(gatest * head, char * operator, char * operand1, char * operand2, char * operand3, char * operands) {
	gatest * new;
	if (head == NULL) {
		new = malloc(sizeof(gatest));
    strcpy(new->operator, operator);
    strcpy(new->operand1, operand1);
    strcpy(new->operand2, operand2);
    strcpy(new->operand3, operand3);
    strcpy(new->operands, operands);
    new->next = NULL;
    return new;
  }

  gatest * currentGate = head;
  while (currentGate->next != NULL) {
  	currentGate = currentGate->next;
  }

  new = malloc(sizeof(gatest));
  strcpy(new->operator, operator);
  strcpy(new->operand1, operand1);
  strcpy(new->operand2, operand2);
  strcpy(new->operand3, operand3);
  strcpy(new->operands, operands);
	currentGate->next = new;
  return head;
}
int getNum(char * name, numst * head1) {
  numst * ptr = head1;
  while (ptr != NULL) {
    if (strcmp(ptr->name, name) == 0) {
    	return ptr->value;
    }
    ptr = ptr->next;
  }
  return -1;
}
int setNum(char * name, int n, numst * head1) {
  numst * ptr = head1;
  while (ptr != NULL) {
    if (strcmp(ptr->name, name) == 0) {
    	ptr->value = n;
    	return 0;
		}
    ptr = ptr->next;
  }
  return -1;
}
int not(int a) {
  if (a == 1) {
    return 0;
  }
  else{
    return 1;
  }
}
int and(int a, int b) {
  if (a == 1 && b == 1) {
    return 1;
  }
  else{
  	return 0;
  }
}
int or(int a, int b) {
  if (a == 1 || b == 1) {
    return 1;
  }
  else{
  	return 0;
  }
}
int nand(int a, int b) {
  if (a == 1 && b == 1) {
    return 0;
  }
  else{
    return 1;
  }
}
int nor(int a, int b) {
  if (a == 1 || b == 1) {
    return 0;
  }
  else{
    return 1;
  }
}
int xor(int a, int b) {
  if (a == 1 && b == 0) {
    return 1;
  }
  else if (a == 0 && b == 1) {
    return 1;
  }
  else {
    return 0;
  }
}
void printArray(int ** array, int row, int separator, int col) {
  int i, j;
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++) {
			if(j==separator){
		  	printf("|%d ", array[i][j]);
			}
			else{
				printf("%d ", array[i][j]);
			}
    }
		printf("\n");
  }
}
int main(int argc, char ** argv) {
  FILE * fp;
  int m;
  int n;
  int numRows;
  int num1;
  int num2;
  int num3;
  int num4;
  int i;
  int j;
  int k;
  numst * inputHead = NULL; //a linked list containing the current input values
  numst * outputHead = NULL;//a linked list containing the current output values
  numst * tempHead = NULL;//a linked list containing the current temp values
  char * value = malloc(15 * sizeof(char));

  //get m and n
  fp = fopen(argv[1], "r");
  fscanf(fp, "INPUT %d", &m);
  for (i = 0; i < m; i++) {
    fscanf(fp, " %s", value);
    inputHead = addNum(inputHead, value);
  }

  fscanf(fp, "\n");

  fscanf(fp, "OUTPUT %d", &n);
  for (i = 0; i < n; i++) {
    fscanf(fp, " %s", value);
    outputHead = addNum(outputHead, value);
  }
  fscanf(fp, "\n");

  //create the array
  numRows = 1<<(m);
  int** array=malloc(numRows*sizeof(int*));
  for(i=0;i<numRows;i++){
    array[i]=(int*)malloc((m+n+1)*sizeof(int));
  }
  for(i=0; i<numRows; i++){
		unsigned mask = 1<<(m-1);
		char * strI = (char*) malloc((m+1)*sizeof(char));
		while (mask)
		{
			sprintf(strI, "%s%d", strI, ((mask & i) != 0));
			mask >>= 1;
		}
		for(j=0; j<m; j++){
			array[i][j] = strI[j] - '0';
		}
  }

  char * operator = malloc(30 * sizeof(char));
  char * operand1 = malloc(30 * sizeof(char));
  char * operand2 = malloc(30 * sizeof(char));
  char * operand3 = malloc(30 * sizeof(char));
  char * operands = malloc(100 * sizeof(char));

  //create gate structures
  gatest * gate = NULL;
  while (fscanf(fp, " %s", operator) == 1) {
    if (strcmp("NOT", operator)==0) {
      fscanf(fp, " %s", operand1);
      if(getNum(operand1, inputHead)==-1 && getNum(operand1, outputHead)==-1){//if not an input or output variable
      	if(getNum(operand1, tempHead) == -1){//not already stored
        	tempHead = addNum(tempHead, operand1);
      	}
      }
      fscanf(fp, " %s", operand2);
      if(getNum(operand2, inputHead)==-1 && getNum(operand2, outputHead)==-1){//if not an input or output variable
      	if(getNum(operand2, tempHead) == -1){
        	tempHead = addNum(tempHead, operand2);
      	}
      }
      gate = addGate(gate, operator, operand1, operand2, operand2, operands);
    }

    else if (operator[0] == 'D') {
      fscanf(fp, "%[^\n]", operands);
      gate = addGate(gate, operator, operand1, operand2, operand3, operands);
    }
    else if (operator[0] == 'M') {
      fscanf(fp, "%[^\n]", operands);
      gate = addGate(gate, operator, operand1, operand2, operand3, operands);
    }
    else{
      fscanf(fp, " %s", operand1);
      if(getNum(operand1, inputHead)==-1 && getNum(operand1, outputHead)==-1){//if not an input or output variable
      	if(getNum(operand1, tempHead) == -1){//not already stored
        	tempHead = addNum(tempHead, operand1);
      	}
      }
      fscanf(fp, " %s", operand2);
      if(getNum(operand2, inputHead)==-1 && getNum(operand2, outputHead)==-1){//if not an input or output variable
      	if(getNum(operand2, tempHead) == -1){//not already stored
        	tempHead = addNum(tempHead, operand2);
      	}
      }
      fscanf(fp, " %s", operand3);
      if(getNum(operand3, inputHead)==-1 && getNum(operand3, outputHead)==-1){//if not an input or output variable
      	if(getNum(operand3, tempHead) == -1){//not already stored
        	tempHead = addNum(tempHead, operand3);
      	}
      }
      gate = addGate(gate, operator, operand1, operand2, operand3, operands);
    }
    fscanf(fp, "\n");

  }
  for (i = 0; i < numRows; i++) {
    numst * inputPtr = inputHead; //a pointer in the input list
  	//set inputs
    for (j = 0; j < m; j++) {
    	inputPtr->value = array[i][j];
    	inputPtr = inputPtr->next;
    }
    gatest * currentGate = gate;
		while (currentGate != NULL) {
			//not gate
			if (currentGate->operator[0] == 'N' && currentGate->operator[2] == 'T') {
				//get
				num1 = getNum(currentGate->operand1, inputHead);
				if(num1==-1){//it is a temp variable
					num1 = getNum(currentGate->operand1, tempHead);
				}
				//operate
				num2 = not(num1);
				//set
				num1 = setNum(currentGate->operand2, num2, outputHead);
				if(num1==-1){//it is a temp variable
					setNum(currentGate->operand2, num2, tempHead);
				}
			}
      //multiplexer
      else if (currentGate->operator[0] == 'M') {

      }
      //decoder
      else if (currentGate->operator[0] == 'D') {
      }
      else{
				//get
				num1 = getNum(currentGate->operand1, inputHead);
				if(num1==-1){//it is a temp variable
					num1 = getNum(currentGate->operand1, tempHead);
				}
				num2 = getNum(currentGate->operand2, inputHead);
				if(num2==-1){//it is a temp variable
					num2 = getNum(currentGate->operand2, tempHead);
				}
				//operate
				if (currentGate->operator[0] == 'A') {//and gate
					num3 = and(num1, num2);
				}
				else if (currentGate->operator[0] == 'O') {//or gate
					num3 = or(num1, num2);
				}
				else if (currentGate->operator[0] == 'N' && currentGate->operator[1] == 'A') {//nand gate
					num3 = nand(num1, num2);
				}
				else if (currentGate->operator[0] == 'N' && currentGate->operator[2] == 'R') {//nor gate
					num3 = nor(num1, num2);
				}
				else if (currentGate->operator[0] == 'X') {//xor gate
					num3 = xor(num1, num2);
				}
				//set
				num4 = setNum(currentGate->operand3, num3, outputHead);
				printf("%d\n", num4);
				if(num4==-1){//it is a temp variable
					setNum(currentGate->operand3, num3, tempHead);
				}
      }
			currentGate = currentGate->next;
		}
	 	//put outputs into array
	  numst * outputPtr = outputHead; //a pointer in the input list
		for (k = m; k < (m + n); k++) {
			array[i][k] = outputPtr->value;
			outputPtr = outputPtr->next;
		}
  }
	printArray(array, numRows, m, (m + n));
	/*
	numst * ptr = outputHead;
  while (ptr != NULL) {
  	printf("%s", ptr->name);
  	ptr = ptr->next;
  }
*/
	return 0;

}
